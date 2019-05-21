//
//  OpenGLES_3_2.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/6.
//  Copyright © 2019 mh. All rights reserved.
//

#import "OpenGLES_3_2.h"

@interface GLKEffectPropertyTexture (AGLKAdditions)
- (void)aglkSetParameter:(GLenum)parameterID
                   value:(GLint)value;

@end

@implementation GLKEffectPropertyTexture(AGLKAdditions)

- (void)aglkSetParameter:(GLenum)parameterID value:(GLint)value{
    glBindTexture(self.target, self.name);
    glTexParameteri(self.target, parameterID, value);
}

@end

typedef struct {
    GLKVector3 positionCoords;
    GLKVector2 textureCoord;
}SceneVertex;

//顶点 三角形
static SceneVertex vertices[] =
{
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // lower left corner
    {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // lower right corner
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}, // upper left corner
};

//默认顶点 -- 用于关闭动画时候恢复默认顶点
static const SceneVertex defaultVertices[] =
{
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}},
};


//move结构体 用于动画，各个坐标的变换动画效果
static GLKVector3 movementVectors[3] = {
    {-0.02f,  -0.01f, 0.0f},
    {0.01f,  -0.005f, 0.0f},
    {-0.01f,   0.01f, 0.0f},
};

@interface OpenGLES_3_2 ()
{
    GLuint vertextBufferID;
}

@property (nonatomic,strong)GLKBaseEffect *baseEffect;
@property (nonatomic,assign)BOOL shouldUseLineFilter;//是否使用线性过滤器
@property (nonatomic,assign)BOOL shouldAnimate;//是否开启动画
@property (nonatomic,assign)BOOL shouldRepeatTexture;//是否重复纹理
@property (nonatomic,assign)GLfloat sCoordinateOffset;//顶点s坐标的offset

@end

@implementation OpenGLES_3_2

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    //初始化context和baseEffect以及load顶点缓存和纹理
    self.preferredFramesPerSecond = 60;
    self.shouldAnimate = YES;
    self.shouldRepeatTexture = YES;
    self.shouldUseLineFilter = NO;
    
    
    GLKView *view = (GLKView *)self.view;
    NSAssert([view isKindOfClass:[GLKView class]], @"View controller's is not a GLKView");
    
    view.context = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:view.context];
    
    self.baseEffect = [[GLKBaseEffect alloc]init];
    self.baseEffect.useConstantColor = GL_TRUE;
    self.baseEffect.constantColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    //顶点缓存和纹理
    [self loadVertexBuffer];
    [self loadTexture];
}
//顶点缓存
- (void)loadVertexBuffer
{
    glGenBuffers(1, &vertextBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertextBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}
//纹理
- (void)loadTexture
{
    //绑定图片纹理
    CGImageRef imageRef = [[UIImage imageNamed:@"test.jpeg"] CGImage];
    
    NSDictionary* options = [NSDictionary dictionaryWithObjectsAndKeys:@(1), GLKTextureLoaderOriginBottomLeft, nil];
    GLKTextureInfo *textureInfo = [GLKTextureLoader textureWithCGImage:imageRef options:options error:NULL];
    self.baseEffect.texture2d0.name = textureInfo.name;
    self.baseEffect.texture2d0.target = textureInfo.target;
}
//绘制
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClear(GL_COLOR_BUFFER_BIT);
    [self.baseEffect prepareToDraw];
    glBindBuffer(GL_ARRAY_BUFFER, vertextBufferID);
    
    //设置vertex偏移指针
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(SceneVertex),NULL + offsetof(SceneVertex, positionCoords));
    
    
    //设置textureCoords偏移指针
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(SceneVertex),NULL + offsetof(SceneVertex, textureCoord));
    
    //Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
//系统方法update 更新顶点坐标和纹理取样设置参数 调用频率和系统屏幕帧数一致
- (void)update
{
    //更新动画顶点位置
    [self updateAnimateVertexPositions];
    
    //更新纹理参数设置
    [self updateTextureParameters];
    
    //刷新vertexBuffer
    glBindBuffer(GL_ARRAY_BUFFER, vertextBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}
//更新纹理参数设置
- (void)updateTextureParameters
{
    glBindTexture(self.baseEffect.texture2d0.target, self.baseEffect.texture2d0.name);
    glTexParameterf(self.baseEffect.texture2d0.target, GL_TEXTURE_WRAP_S, (self.shouldRepeatTexture) ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameterf(self.baseEffect.texture2d0.target, GL_TEXTURE_MAG_FILTER, (self.shouldUseLineFilter) ? GL_LINEAR : GL_NEAREST);
}
//更新动画顶点位置
- (void)updateAnimateVertexPositions
{
    if (_shouldAnimate) {
        for (int i = 0; i < 3; i++) {
            
            vertices[i].positionCoords.x += movementVectors[i].x;
            if (vertices[i].positionCoords.x > 1.0f || vertices[i].positionCoords.x < -1.0f) {
                movementVectors[i].x = -movementVectors[i].x;
            }
            
            vertices[i].positionCoords.y += movementVectors[i].y;
            if(vertices[i].positionCoords.y >= 1.0f || vertices[i].positionCoords.y <= -1.0f) {
                movementVectors[i].y = -movementVectors[i].y;
            }
            
            vertices[i].positionCoords.z += movementVectors[i].z;
            if(vertices[i].positionCoords.z >= 1.0f || vertices[i].positionCoords.z <= -1.0f) {
                movementVectors[i].z = -movementVectors[i].z;
            }
        }
    } else {
        for(int i = 0; i < 3; i++) {
            vertices[i].positionCoords.x = defaultVertices[i].positionCoords.x;
            vertices[i].positionCoords.y = defaultVertices[i].positionCoords.y;
            vertices[i].positionCoords.z = defaultVertices[i].positionCoords.z;
        }
    }
    
    {  // Adjust the S texture coordinates to slide texture and
        // reveal effect of texture repeat vs. clamp behavior
        // 'i' is current vertex index
        for(int i = 0; i < 3; i++)
        {
            vertices[i].textureCoord.s = (defaultVertices[i].textureCoord.s + _sCoordinateOffset);
        }
    }
}
@end
