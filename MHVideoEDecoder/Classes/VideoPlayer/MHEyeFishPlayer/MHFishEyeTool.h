//
//  MHFishEyeTool.h
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/9.
//  Copyright © 2019 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import <GLKit/GLKit.h>

#import "MHStreamHeader.h"

static CGFloat const mhEyeMaxZdepth = 4.0;
static CGFloat const mhEyeMinZdepth = 1.0;

/** 鱼眼着色器属性 */
typedef struct {
    GLint  VshCoorAxes;  //坐标轴
    GLint  VshCircular;
    GLint  VshRotate;    //传给着色器 水平旋转量
    GLint  VshhDegrees;  //传给着色器 水平偏移
    GLint  VshvDegrees;  //传给着色器 垂直偏移
    GLint  VshZoomxy;    //传给着色器 xy缩放比
    GLint  VshZoomz;     //传给着色器 z压缩比
    GLint  VshWScale;    //传给着色器 x宽压缩比
    GLint  VshHScale;    //传给着色器 y高压缩比
    GLuint VshImgSize;   //传给着色器 图片尺寸
    GLuint FsBgColor;    //传给着色器 背景RGBA
    GLint  FsModelColor; //传给着色器 模型颜色
    GLuint FsFragMode;
    GLint FsColorRGB;
    GLuint FsMatrix;
    GLuint FsSamplerY;
    GLuint FsSamplerUV;
    GLuint FsRGBd;
    GLuint FsSaY;
    GLuint FsSaU;
    GLuint FsSaV;
    GLint  VshMode;      //传给着色器 模型模式
    GLint  VshScaleAspectFit;
    GLint  VshFocus;     //传给着色器 鱼眼模式下有效投影面夹角
    GLuint VshPosition;  //传给着色器 顶点
    GLuint VshTextCoor;  //传给着色器 贴图
}MHShaderUniforms;//着色器属性

typedef struct{
    GLfloat * PtArrays;//x,y,z,s,t 数组
    GLuint PtCount;     //数组元素个数
    GLuint Stacks;
    GLuint Slices;
}mhModel_st;

typedef struct{
    GLfloat R;
    GLfloat G;
    GLfloat B;
    GLfloat A;
}mhColor_st;

@interface MHFishEyeTool : NSObject

/**
 获取鱼眼模式model_st
 @return model_st
 */
+(mhModel_st)fishEyeModel_st;
+(mhModel_st)creatEyeMap:(GLint)stacks Slices:(GLint)slices;


/**
 裁剪YUV数据
 @param srcBuffer 原始数据
 @param srcW 原始宽
 @param srcH 原始高
 @param desBuffer 输出数据
 @param x 裁剪x
 @param y 裁剪y
 @param desW 裁剪宽
 @param desH 裁剪高
 */
+(void)cutxYUV420:(char *)srcBuffer srcW:(int)srcW srcH:(int)srcH desBuffer:(char *)desBuffer cutX:(int)x cutY:(int)y cutW:(int)desW cutH:(int)desH;

@end
