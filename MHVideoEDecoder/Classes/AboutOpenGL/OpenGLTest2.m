//
//  OpenGLTest2.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/5.
//  Copyright © 2019 mh. All rights reserved.
//

#import "OpenGLTest2.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <GLKit/GLKit.h>

typedef struct {
    GLKVector3 positionCoords;//顶点坐标 以屏幕中心点为圆点xyz
    GLKVector2 textureCoords;//纹理坐标  以左下角为顶点坐标xy
}SceneVertex;

//矩形6个点（2个三角形）顶点坐标-纹理坐标
static const SceneVertex vertices[] = {
    {{-1, 1, 0.f},{0.f, 1}},         //左上
    {{-1, -1, 0.f},{0.f, 0.f}},      //左下
    {{1, -1, 0.f},{1, 0.f}},         //右下
    
    {{-1, 1, 0.f},{0.f, 1}},         //左上
    {{1, 1, 0.f},{1, 1}},            //右上
    {{1, -1, 0.f},{1, 0.f}},         //右下
};


@interface OpenGLTest2 ()

@property(nonatomic,strong)GLKView * glView;
@property(nonatomic,strong)GLKBaseEffect * baseEffect;
//声明缓存ID属性
@property (nonatomic,assign)GLuint vertextBufferID;

@end

@implementation OpenGLTest2

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    //新建OpenGLES 上下文
    self.glView = (GLKView *)self.view;
    
    self.glView.context = [[EAGLContext alloc]initWithAPI: kEAGLRenderingAPIOpenGLES2];
    //设置当前上下文
    [EAGLContext setCurrentContext:self.glView.context];
    
    self.baseEffect = [[GLKBaseEffect alloc]init];
    //使用静态颜色绘制
    self.baseEffect.useConstantColor = GL_TRUE;
    //设置默认绘制颜色，参数分别是 RGBA
    self.baseEffect.constantColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
    
    // 设置顶点缓存buffer
    [self fillVertexArray];
    
    //生成纹理
    [self loadTexture];
}
// 设置顶点缓存buffer
-(void)fillVertexArray
{
    glGenBuffers(1, &_vertextBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertextBufferID); //绑定指定标识符的缓存为当前缓存
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(GLKVertexAttribPosition); //顶点数据缓存
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), NULL + offsetof(SceneVertex, positionCoords));
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0); //纹理
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), NULL + offsetof(SceneVertex, textureCoords));
}
//生成纹理
-(void)loadTexture
{
    //获取图片
    CGImageRef imageRef = [[UIImage imageNamed:@"test.jpeg"] CGImage];
    
    //通过图片数据产生纹理缓存
    //GLKTextureInfo封装了纹理缓存的信息，包括是否包含MIP贴图
    NSDictionary* options = [NSDictionary dictionaryWithObjectsAndKeys:@(1), GLKTextureLoaderOriginBottomLeft, nil];
    GLKTextureInfo *textureInfo = [GLKTextureLoader textureWithCGImage:imageRef options:options error:NULL];    self.baseEffect.texture2d0.name = textureInfo.name;
    
    self.baseEffect.texture2d0.target = textureInfo.target;
}
//绘制
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
    //清除背景色
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    [self.baseEffect prepareToDraw];
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
//释放
- (void)dealloc
{
    [EAGLContext setCurrentContext:self.glView.context];
    if ( 0 != _vertextBufferID) {
        glDeleteBuffers(1,&_vertextBufferID);
        _vertextBufferID = 0;
    }
    [EAGLContext setCurrentContext:nil];
}

@end
