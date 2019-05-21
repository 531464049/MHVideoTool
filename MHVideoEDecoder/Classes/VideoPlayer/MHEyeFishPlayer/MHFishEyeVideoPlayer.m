//
//  MHFishEyeVideoPlayer.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/8.
//  Copyright © 2019 mh. All rights reserved.
//

#import "MHFishEyeVideoPlayer.h"
#import "MHFishEyeTool.h"
#import <sys/timeb.h>

UInt64 MHSystemTms()
{
    struct timeb t;
    ftime(&t);
    return (UInt64)t.time * 1000 + t.millitm;
}

@interface MHFishEyeVideoPlayer ()
{
    CAEAGLLayer * _mhEagLayer;
    //上下文对象
    EAGLContext *_context;
    //shader着色器属性
    MHShaderUniforms shaderUniforms;
    
    //layer的像素尺寸
    GLint _backingWidth;
    GLint _backingHeight;
    
    //buffer引用
    GLuint frameBuffer;
    GLuint renderBuffer;
    GLuint depthBuffer;
    GLuint shadervBuffer;
    
    mhModel_st modelst;
    mhTextureFragMode fragMode;
    
    GLuint TextureRGBd;
    GLuint FsTextureRgb;
    GLuint TextureY;
    GLuint TextureU;
    GLuint TextureV;
    
    BOOL IsUpdateTexture;
    
    CGSize imgSize;//要渲染的图片size
    
//    CVOpenGLESTextureRef YTexture; //亮度文理
//    CVOpenGLESTextureRef UVTexture;//色彩文理
    CVOpenGLESTextureCacheRef TextureCache;//纹理缓存引用
    
    CADisplayLink * updateLink;//刷新定时器
    UInt64 updateTimeOut;//定时器上一次响应时间
    
    
    mhColor_st    BgColor;     //背景RGBA
    mhColor_st    ModelColor;  //模型RGBA
//    lgCircular_st Circular;    //校准圆（x,y,r）
    CGFloat       lgZoomVlz;   //z压缩比
    CGFloat       lgWScale;    //x宽压缩比
    CGFloat       lgHScale;    //y高压缩比
    CGFloat       lgFocus;     //焦距
//    lgTextureFragMode_e FragMode;
    GLint         FsColorRGB;
    GLint         glwidth;     //渲染宽度
    GLint         glheight;    //渲染高度
    
    const GLfloat * conversion;
}
/** 着色器 */
@property (nonatomic, assign) GLuint program;

@end

@implementation MHFishEyeVideoPlayer

+ (Class)layerClass {
    return [CAEAGLLayer class];
}
-(instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor blackColor];
        mhColor_st color = {0.0, 0.0, 0.0, 1.0};
        BgColor = color;//背景颜色
        ModelColor = color;//模型颜色
        
        //初始化CAEAGLLayer
        _mhEagLayer = (CAEAGLLayer*)self.layer;
        // 设置像素比例，也就是iOS的原图、@2x、@3x像素密度是按倍数渲染的
        _mhEagLayer.contentsScale = [[UIScreen mainScreen] scale];
        // 默认是透明的，透明的性能不好，设置为不透明
        _mhEagLayer.opaque = YES;
        // 设置描绘属性，设置不缓存渲染内容
        /*
         kEAGLDrawablePropertyRetainedBacking设置是否需要保留已经绘制到图层上面的内容，用NSNumber来包装，
         为FALSE，表示不想保持呈现的内容，因此在下一次呈现时，应用程序必须完全重绘一次。将该设置为 TRUE 对性能和资源影像较大，因此只有当renderbuffer需要保持其内容不变时，我们才设置 kEAGLDrawablePropertyRetainedBacking  为 TRUE。
         kEAGLDrawablePropertyColorFormat 设置绘制对象内部的颜色缓冲区的格式 32位的RGBA的形式
         包含的格式
         kEAGLColorFormatRGBA8; 32位RGBA的颜色 4x8=32
         kEAGLColorFormatRGB565; 16位的RGB的颜色
         kEAGLColorFormatSRGBA8 SRGB
         */
        _mhEagLayer.drawableProperties = @{ kEAGLDrawablePropertyRetainedBacking :@(FALSE), kEAGLDrawablePropertyColorFormat : kEAGLColorFormatRGBA8};
        
        //初始化上下文
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!_context || ![EAGLContext setCurrentContext:_context]) {
            return nil;
        }
        
        glEnable(GL_DEPTH_TEST);//这个不知道是干啥的
        
        //加载着色器
        [self loadShaders];
        //设置绘制窗口frame
        [self setupFrame];
        //初始化bufer
        [self setupBuffer];
        //初始化鱼眼数据
        [self setupFishEyeData];
        
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, _context, NULL, &TextureCache);
        if(err != noErr){
            NSLog(@"Error at CVOpenGLESTextureCacheCreate %d", err);
        }
    }
    return self;
}
#pragma mark - 初始化buffer
-(void)setupBuffer
{
    [EAGLContext setCurrentContext:_context];
    //清理缓存区
    [self destoryBuffer];
    
    //创建帧缓存
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //创建渲染缓存
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    // 将 _colorRenderBuffer 装配到 GL_COLOR_ATTACHMENT0 这个装配点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    //渲染缓存绑定到渲染图层
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_mhEagLayer];
    //创建深度缓存
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,  &_backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _backingWidth, _backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
        NSLog(@"Failed to make complete framebuffer object %x", status);
    }
    
    glGenTextures(1,&TextureRGBd);
    glGenTextures(1,&FsTextureRgb);
    glGenTextures(1,&TextureY);
    glGenTextures(1,&TextureU);
    glGenTextures(1,&TextureV);
    
    glGenBuffers(1, &shadervBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, shadervBuffer);
}
#pragma mark - 设置绘制窗口frame
-(void)setupFrame
{
    GLfloat scale = [[UIScreen mainScreen] scale]; //获取视图放大倍数，
    GLfloat width  = self.frame.size.width * scale;
    GLfloat height = self.frame.size.height * scale;
    
    [EAGLContext setCurrentContext:_context];
    glViewport(0, 0, width, height);
}
#pragma mark - 初始化着色器
- (BOOL)loadShaders
{
    GLuint vertShader = 0;
    GLuint fragShader = 0;
    
    //创建着色器程序
    self.program = glCreateProgram();
    
    NSURL * vertUrl = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"fishEyeShaderv" ofType:@"vsh"]];
    NSURL * fragUrl = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"fishEyeShaderf" ofType:@"fsh"]];
    
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER URL:vertUrl]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER URL:fragUrl]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }

    //绑定顶点着色器
    glAttachShader(self.program, vertShader);
    //绑定片段着色器
    glAttachShader(self.program, fragShader);
    //链接着色器程序
    glLinkProgram(self.program);
    //获取链接结果，失败则释放内存
    GLint status;
    glGetProgramiv(self.program, GL_LINK_STATUS, &status);
    if (status == 0) {
        NSLog(@"Failed to link program: %d", self.program);
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (self.program) {
            glDeleteProgram(self.program);
            self.program = 0;
        }
        return NO;
    }
    //启动着色器程序
    glUseProgram(self.program);
    
    //获取shader里面的变量，这里记得要在glLinkProgram后面，后面，后面！
    shaderUniforms.VshCoorAxes = glGetUniformLocation(self.program, "CoorAxes");
    shaderUniforms.VshCircular = glGetUniformLocation(self.program, "Circular");
    shaderUniforms.VshRotate   = glGetUniformLocation(self.program, "Rotate");
    shaderUniforms.VshhDegrees = glGetUniformLocation(self.program, "hDegrees");
    shaderUniforms.VshvDegrees = glGetUniformLocation(self.program, "vDegrees");
    shaderUniforms.VshZoomxy   = glGetUniformLocation(self.program, "Zoomxy");
    shaderUniforms.VshZoomz    = glGetUniformLocation(self.program, "Zoomz");
    shaderUniforms.VshWScale   = glGetUniformLocation(self.program, "WScale");
    shaderUniforms.VshHScale   = glGetUniformLocation(self.program, "HScale");
    shaderUniforms.VshImgSize  = glGetUniformLocation(self.program, "ImgSize");
    shaderUniforms.FsBgColor   = glGetUniformLocation(self.program, "BgColor");
    shaderUniforms.FsModelColor= glGetUniformLocation(self.program, "ModelColor");
    shaderUniforms.FsFragMode  = glGetUniformLocation(self.program, "FragMode");
    shaderUniforms.FsColorRGB  = glGetUniformLocation(self.program, "ColorRGB");
    shaderUniforms.FsMatrix    = glGetUniformLocation(self.program, "Matrix");
    shaderUniforms.FsSamplerY  = glGetUniformLocation(self.program, "SamplerY");
    shaderUniforms.FsSamplerUV = glGetUniformLocation(self.program, "SamplerUV");
    shaderUniforms.FsRGBd      = glGetUniformLocation(self.program, "RGBd");
    shaderUniforms.FsSaY       = glGetUniformLocation(self.program, "SaY");
    shaderUniforms.FsSaU       = glGetUniformLocation(self.program, "SaU");
    shaderUniforms.FsSaV       = glGetUniformLocation(self.program, "SaV");
    shaderUniforms.VshMode     = glGetUniformLocation(self.program, "Mode");
    shaderUniforms.VshScaleAspectFit =glGetUniformLocation(self.program, "ScaleAspectFit");
    shaderUniforms.VshFocus   = glGetUniformLocation(self.program, "Focus");
    shaderUniforms.VshPosition =  glGetAttribLocation(self.program, "position");
    shaderUniforms.VshTextCoor =  glGetAttribLocation(self.program, "textCoordinate");
    
    
    //两个着色器都已绑定到着色器程序上了，删除
    if (vertShader) {
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDeleteShader(fragShader);
    }

    return YES;
}
#pragma mark - 初始化鱼眼数据
-(void)setupFishEyeData
{
    //每组数据:[x,y,z,s,t][x,y,z,s,t][x,y,z,s,t].....
    //说明
    //参数1:VshPosition -> 对应 着色器里边的 position
    //参数2: 3          -> 说明每组数据里边有3个数据为顶点数据
    //参数3: GL_FLOAT   -> 3个数据都是GLfloat类型
    //参数4: GL_FALSE   ->
    //参数5: sizeof(GLfloat) * 5 -> 每组5个数据(x,y,z,s,t)占用的内存大小为: sizeof(GLfloat)*5
    //参数6: NULL       -> 告诉着色器在每组数据中取数据时从起始位置开始，也就是从起始位置开始取一3个作为顶点坐标数据
    glVertexAttribPointer(shaderUniforms.VshPosition,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*5,NULL);
    glEnableVertexAttribArray(shaderUniforms.VshPosition);
    
    glVertexAttribPointer(shaderUniforms.VshTextCoor,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*5,(GLfloat *)NULL + 3);
    glEnableVertexAttribArray(shaderUniforms.VshTextCoor);
    
    //设置鱼眼模式
    self.hAngle = 0.f;
    self.zDepth = mhEyeMinZdepth;
    self.vAngle = 0.0;
    
    modelst = [MHFishEyeTool fishEyeModel_st];
    
    [EAGLContext setCurrentContext:_context];
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*5*modelst.PtCount, (GLvoid *)modelst.PtArrays, GL_DYNAMIC_DRAW);
    
    //IsUpdateTexture = YES;
}
#pragma mark - 贴图接口-输入YUV数据源
-(void)inputYuvDataTexture:(char *)yuv width:(int)width height:(int)height
{
    if(!_context || yuv == NULL || width <= 0 || height <= 0){
        return;
    }
    fragMode = mhTextureFragModeYuv;
    //裁剪YUV
    char *desbuf = (char *)malloc(width * height * 3 / 2);
    [MHFishEyeTool cutxYUV420:yuv srcW:width srcH:height desBuffer:desbuf cutX:(width-height)/2 cutY:0 cutW:height cutH:height];
    if (desbuf == NULL) {
        NSLog(@"------- 裁剪YUV出错 ------");
        return;
    }
    //渲染裁剪后的YUV数据
    [self displayYuvBuffer:desbuf width:height height:height];
    free(desbuf);
    IsUpdateTexture = YES;
    
    //开启定时器
    [self startUpdateLink];
}
#pragma mark - 渲染YUV数据
- (void)displayYuvBuffer:(char *)yuv width:(int)width height:(int)height
{
    if (yuv == NULL || width == 0 || height == 0) {
        return;
    }
    
    imgSize = CGSizeMake(width, height);
    conversion = kColorConversion709;
    
    [EAGLContext setCurrentContext:_context];
    bindTexture(GL_TEXTURE0,TextureY, yuv, width, height);
    bindTexture(GL_TEXTURE1,TextureU, yuv + width * height, width/2, height/2);
    bindTexture(GL_TEXTURE2,TextureV, yuv + width * height * 5 / 4, width/2, height/2);
    
    glUniform1i(shaderUniforms.FsSaY, 0);
    glUniform1i(shaderUniforms.FsSaU, 1);
    glUniform1i(shaderUniforms.FsSaV, 2);
}
static GLuint bindTexture(GLenum lgt,GLuint texture, const char *buffer, GLuint w , GLuint h)
{
    //激活lgt->GL_TEXTURE0 1 2 这个纹理单元，用于之后的纹理采样
    glActiveTexture(lgt);
    //绑定纹理对象texture到纹理目标GL_TEXTURE_2D，接下来对纹理目标的操作都发生在此对象上
    glBindTexture( GL_TEXTURE_2D, texture);
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
    return texture;
}

#pragma mark - 创建着色器
-(BOOL)compileShader:(GLuint *)shader type:(GLenum)type URL:(NSURL *)URL
{
    NSError * error;
    NSString * sourceString = [NSString stringWithContentsOfURL:URL encoding:NSUTF8StringEncoding error:&error];
    if (!sourceString) {
        NSLog(@"Failed to load vertex shader: %@", [error localizedDescription]);
        return NO;
    }
    
    const GLchar * source = (GLchar *)[sourceString UTF8String];
    return [self compileShaderString:shader type:type shaderString:source];
}
-(BOOL)compileShaderString:(GLuint *)shader type:(GLenum)type shaderString:(const GLchar*)shaderString
{
    //shader 着色器
    //type 类型 顶点着色器：GL_VERTEX_SHADER   片段着色器：GL_FRAGMENT_SHADER
    //shaderString 着色器源码
    
    //创建着色器
    *shader = glCreateShader(type);
    //加载着色器源码
    glShaderSource(*shader, 1, &shaderString, NULL);
    //编译着色器
    glCompileShader(*shader);
    // 获取结果，没获取到就释放内存
    GLint status = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(*shader);
        return NO;
    }
    return YES;
}
- (void)setEyeHangle:(float)Ha Vangle:(float)Va zDepth:(float)depth
{
    if(imgSize.width <= 0.0 || imgSize.height <= 0.0){
        return;
    }
    
    [self setWHScale];
    
    if(fabs(Ha) > 360.0){
        Ha = (Ha > 0.0) ? (Ha - 360.0) : (Ha + 360.0);
    }
    
    if(fabs(Va) > 360.0){
        Va = (Va > 0.0) ? (Va - 360.0) : (Va + 360.0);
    }
    
    if(depth > mhEyeMaxZdepth){
        depth = mhEyeMaxZdepth;
    }else if(depth < mhEyeMinZdepth){
        depth = mhEyeMinZdepth;
    }
    
    self.hAngle  = Ha;
    self.vAngle  = Va;
    self.zDepth    = depth;
    
    //CGFloat TmpZoom = 1.0;
    CGPoint center  = CGPointMake(0.0, 0.0);
    CGFloat radius  = 1.0;
    [EAGLContext setCurrentContext:_context];// 设置为当前上下文
    
    //CGFloat maxvd = atan(1.6)*180.0/acos(-1.0);//倾斜一定角度后上边界到达屏幕顶部边界
    if(self.vAngle < -57.5){
        self.vAngle = -57.5;
    }else if(self.vAngle > 57.5){
        self.vAngle = 57.5;
    }
    
    lgFocus = (self.zDepth-mhEyeMinZdepth)/(mhEyeMaxZdepth-mhEyeMinZdepth);
    
    glUniform3f(shaderUniforms.VshCoorAxes, GLKMathDegreesToRadians(0.f), GLKMathDegreesToRadians(0.f), GLKMathDegreesToRadians(0.f));
    glUniform1f(shaderUniforms.VshhDegrees, GLKMathDegreesToRadians(0));
    glUniform1f(shaderUniforms.VshRotate, GLKMathDegreesToRadians(360.0-self.hAngle));
    
    glUniform1f(shaderUniforms.VshvDegrees, GLKMathDegreesToRadians(self.vAngle));
    glUniform1f(shaderUniforms.VshZoomxy, self.zDepth);
    glUniform1f(shaderUniforms.VshFocus, 1.0-lgFocus);
    
    glUniform1i(shaderUniforms.VshScaleAspectFit , YES);
    glUniform1i(shaderUniforms.FsFragMode , fragMode);
    
    glUniform1i(shaderUniforms.VshMode, 1);
    glUniform1f(shaderUniforms.VshZoomz,0.5);
    
    glUniform1f(shaderUniforms.VshWScale  , lgWScale);
    glUniform1f(shaderUniforms.VshHScale  , lgHScale);
    
    glUniformMatrix3fv(shaderUniforms.FsMatrix, 1, GL_FALSE, conversion);
    
    glUniform2f(shaderUniforms.VshImgSize , imgSize.width, imgSize.height);
    glUniform4f(shaderUniforms.FsBgColor  , BgColor.R, BgColor.G, BgColor.B, BgColor.A);
    glUniform4f(shaderUniforms.FsModelColor,ModelColor.R,ModelColor.G,ModelColor.B,ModelColor.A);
    glUniform3f(shaderUniforms.VshCircular, center.x, center.y, radius);
    
    glClearColor(BgColor.R, BgColor.G, BgColor.B, BgColor.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, modelst.PtCount);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}
- (void)setWHScale
{
    GLfloat lgScale = [[UIScreen mainScreen] scale]; //获取视图放大倍数，可以把scale设置为1试试
    GLfloat width  = self.frame.size.width  * lgScale;
    GLfloat height = self.frame.size.height * lgScale;
    
    if(width > height){
        lgWScale = height/width;//宽压缩比
        lgHScale = 1.0;         //高压缩比
    }else{
        lgWScale = 1.0;
        lgHScale = width/height;
    }
}
#pragma mark - 定时器刷新
- (void)onUpdateLink
{
//    UInt64 curTiime = MHSystemTms();
//    if(updateTimeOut == 0){
//        updateTimeOut = curTiime;
//    }
//    CGFloat Dtime = 1.0/(curTiime - updateTimeOut);
//    updateTimeOut = curTiime;
    
    if(IsUpdateTexture){
        IsUpdateTexture = NO;
        [self setEyeHangle:self.hAngle Vangle:self.vAngle zDepth:self.zDepth];
    }
}
#pragma mark - 定时器
- (void)startUpdateLink
{
    if (!updateLink) {
        updateLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onUpdateLink)];
        updateLink.frameInterval = 2;
        [updateLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
    }
}
#pragma mark - 清空纹理
-(void)cleanUpTextures
{
//    //清空亮度引用
//    if (YTexture) {
//        CFRelease(YTexture);
//        YTexture = NULL;
//    }
//    //清空色度引用
//    if (UVTexture) {
//        CFRelease(UVTexture);
//        UVTexture = NULL;
//    }
//    //清空纹理缓存
//    CVOpenGLESTextureCacheFlush(TextureCache, 0);
//    if (TextureCache) {
//        CFRelease(TextureCache);
//    }
}
#pragma mark - 清理缓存区
-(void)destoryBuffer
{
    //清理缓存区
    if (frameBuffer) {
        glDeleteBuffers(1, &frameBuffer);
        frameBuffer = 0;
    }
    if (renderBuffer) {
        glDeleteBuffers(1, &renderBuffer);
        renderBuffer = 0;
    }
    if (depthBuffer) {
        glDeleteBuffers(1, &depthBuffer);
        depthBuffer = 0;
    }
    if (shadervBuffer) {
        glDeleteBuffers(1, &shadervBuffer);
        shadervBuffer = 0;
    }
}
-(void)dealloc
{
    if(modelst.PtArrays){
        free(modelst.PtArrays);
        modelst.PtArrays = NULL;
    }
    [EAGLContext setCurrentContext:_context];
    [self destoryBuffer];
    
    glDeleteTextures(1,&TextureRGBd);
    glDeleteTextures(1,&FsTextureRgb);
    glDeleteTextures(1,&TextureY);
    glDeleteTextures(1,&TextureU);
    glDeleteTextures(1,&TextureV);
    
    glDeleteProgram(self.program);
}
@end
