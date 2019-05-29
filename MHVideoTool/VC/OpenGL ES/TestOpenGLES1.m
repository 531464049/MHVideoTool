//
//  TestOpenGLES1.m
//  MHVideoTool
//
//  Created by mahao on 2019/5/29.
//  Copyright © 2019 mh. All rights reserved.
//

#import "TestOpenGLES1.h"
#import <QuartzCore/QuartzCore.h>
#import <CoreVideo/CoreVideo.h>
#import <GLKit/GLKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>



@interface TestOpenGLES1 ()
{
    TestView * _testView;
}
@end

@implementation TestOpenGLES1

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    _testView = [[TestView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.width)];
    _testView.center = CGPointMake(self.view.frame.size.width/2, self.view.frame.size.height/2);
    _testView.backgroundColor = [UIColor grayColor];
    [self.view addSubview:_testView];

    
    [_testView drawImage];
}
@end


@interface TestView ()
{
    TestView * _testView;
    CAEAGLLayer * _mhEagLayer;
    // 上下文对象
    EAGLContext *_context;
    // 着色器
    GLuint _program;
    // buffer的引用
    GLuint _frameBuffer;
    GLuint _renderBuffer;
    
    GLint shColorRGB;//默认编号为0的纹理
    
    //顶点着色器属性
    GLuint shPosition;  //传给着色器 顶点
    GLuint shTextCoor;  //传给着色器 贴图
    
    // layer的像素尺寸
    GLint _backingWidth;
    GLint _backingHeight;
}
@end

@implementation TestView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}
-(instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setUpLayer];
        [self setUpContext];
        [self loadShader];
        [self setUpBuffer];
    }
    return self;
}
-(void)setUpLayer
{
    _mhEagLayer = (CAEAGLLayer *)self.layer;
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
}
-(void)setUpContext
{
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!_context || ![EAGLContext setCurrentContext:_context]) {
        NSLog(@"----初始化context上下文失败----");
    }
}
-(void)loadShader
{
    //读取文件路径
    NSString* vertFile = [[NSBundle mainBundle] pathForResource:@"Test1" ofType:@"vsh"];
    NSString* fragFile = [[NSBundle mainBundle] pathForResource:@"test1" ofType:@"fsh"];
    
    GLuint verShader, fragShader;
    GLint program = glCreateProgram();
    //编译
    BOOL comV = [self compileShader:&verShader  type:GL_VERTEX_SHADER   filePath:vertFile];
    if (!comV) {
        NSLog(@"Failed to compile vertex shader");
    }
    BOOL comF = [self compileShader:&fragShader type:GL_FRAGMENT_SHADER filePath:fragFile];
    if (!comF) {
        NSLog(@"Failed to compile fragment shader");
    }
    
    glAttachShader(program, verShader);
    glAttachShader(program, fragShader);
    
    //释放不需要的shader
    glDeleteShader(verShader);
    glDeleteShader(fragShader);
    
    _program = program;
    //链接着色器程序
    glLinkProgram(_program);
    
    //获取链接结果，失败则释放内存
    GLint status;
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(_program, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"Shader Program Error:%@", messageString);
    }
    
    //启动着色器程序
    glUseProgram(_program);
    
    //获取全局属性，一定要在链接完成后才行，否则拿不到
    shColorRGB  = glGetUniformLocation(_program, "ColorRGB");
    shPosition = glGetAttribLocation(_program, "position");
    shTextCoor = glGetAttribLocation(_program, "textCoordinate");
}
-(BOOL)compileShader:(GLuint *)shader type:(GLenum)type filePath:(NSString *)filePath
{
    GLchar* source = NULL;
    NSString* content = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
    source = (GLchar *)[content UTF8String];
    if(source == NULL){
        return NO;
    }
    const GLchar* Str = source;
    //创建着色器
    *shader = glCreateShader(type);
    //加载着色器源码
    glShaderSource(*shader, 1, &Str, NULL);
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
-(void)setUpBuffer
{
    glDeleteBuffers(1, &_frameBuffer);
    _frameBuffer = 0;
    glDeleteBuffers(1, &_renderBuffer);
    _renderBuffer = 0;
    
    //申请一个缓冲区
    glGenRenderbuffers(1, &_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
    //渲染缓存绑定到渲染图层
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_mhEagLayer];
    
    //获取渲染缓存对象的宽高属性，获取到的实际上就是当前layer尺寸 * 像素倍数 （@2x @3x）
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);
    
    //申请一个缓存区句柄
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    //将 renderbuffer 装配到 GL_COLOR_ATTACHMENT0 这个装配点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
}
-(void)drawImage
{
    UIImage * img = [UIImage imageNamed:@"charger_inmap"];
    //转换为CGImage，获取图片基本参数
    CGImageRef spriteImage = [img CGImage];
    GLuint width = (GLuint)CGImageGetWidth(spriteImage);
    GLuint height = (GLuint)CGImageGetHeight(spriteImage);
    
    //绘制图片
    GLubyte * spriteData = (GLubyte *)calloc(width*height*4, sizeof(GLubyte));
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4,CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    //3.在CGContextRef上绘图
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);//绑定到0号纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    glUniform1i(shColorRGB, 0);
    
    //释放内存
    CGContextRelease(spriteContext);
    free(spriteData);
    
    //顶点坐标 纹理坐标
    {
        // 确定顶点数据结构
        GLfloat quadVertexData[] = {
            -1.0, -1.0,
            1.0, -1.0,
            -1.0, 1.0,
            1.0, 1.0,
        };
        glVertexAttribPointer(shPosition, 2, GL_FLOAT, 0, 0, quadVertexData);
        glEnableVertexAttribArray(shPosition);
        
        // 确定纹理数据结构
        GLfloat quadTextureData[] =  {
            0.0, 0.0,
            1.0, 0.0,
            0.0, 1.0,
            1.0, 1.0,
        };
        glVertexAttribPointer(shTextCoor, 2, GL_FLOAT, 0, 0, quadTextureData);
        glEnableVertexAttribArray(shTextCoor);
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        // 设置绘制窗口
        glViewport(0, 0, _backingWidth, _backingHeight);
        // 把上下文的东西渲染到屏幕上
        [_context presentRenderbuffer:GL_RENDERBUFFER];
    }
}
@end
