//
//  MHFishEyeTool.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/9.
//  Copyright © 2019 mh. All rights reserved.
//

#import "MHFishEyeTool.h"



@implementation MHFishEyeTool

#pragma mark - 计算鱼眼模式modelst
+(mhModel_st)fishEyeModel_st
{
    return [MHFishEyeTool creatEyeMap:270 Slices:180];
}
+(mhModel_st)creatEyeMap:(GLint)stacks Slices:(GLint)slices
{
    mhModel_st modelst = {NULL,0,0,0};
    
    CGFloat CIRCULAR_R = 1.0f;   //透视球体半径
    CGFloat CIRCULAR_T = 1.0f;   //透视球体半径
    
    //stacks: 一圈分数   slices: 层数
    GLint MallocCont = slices * (stacks*2+2);//总环数*(单环点数*2+2)
    size_t MallocSize = sizeof(GLfloat) * 5 * MallocCont;
    GLfloat * PtArray = (GLfloat *)malloc(MallocSize);
    if(PtArray == NULL){
        return modelst;
    }
    
    GLint Offset = 0;
    double jstmp = 360.0 / stacks;  //份跨度
    double rstmp =  90.0 / slices;  //每层厚度
    double x,y,z,s,t,u,v,or;
    for(double i = 0.0; i < 90.0-0.000001; i += rstmp){//每一环
        double R = CIRCULAR_R * cos(GLKMathDegreesToRadians(i));
        double r = CIRCULAR_R * cos(GLKMathDegreesToRadians(i+rstmp));
        //NSLog(@"第 %.0f 层 R0-R1 (%f  %f)  %f",i/rstmp+1,R,r,i+rstmp);
        for(double j = 0.0; j < 360.0+0.000001; j += jstmp){//绕一圈
            double ag = GLKMathDegreesToRadians(j);
            //球面
            x = R * cos(ag);
            y = R * sin(ag);
            z = sqrt( fabs( pow(CIRCULAR_R, 2)-( pow(x,2) + pow(y,2) ) ) );
            
            or = CIRCULAR_R * (1.0-i/90.0);
            u = or * cos(ag);//x1
            v = or * sin(ag);//y1
            //贴图
            s = (u + CIRCULAR_T) / (CIRCULAR_T*2.0);
            t = (CIRCULAR_T - v) / (CIRCULAR_T*2.0);
            
            *(PtArray+Offset++) = x;
            *(PtArray+Offset++) = y;
            *(PtArray+Offset++) = z;
            *(PtArray+Offset++) = s;
            *(PtArray+Offset++) = 1.0-t;
            
            x = r * cos(ag);
            y = r * sin(ag);
            z = sqrt( fabs( pow(CIRCULAR_R, 2)-( pow(x,2) + pow(y,2) ) ) );
            
            or = CIRCULAR_R * (1.0-(i+rstmp)/90.0);
            u = or * cos(ag);//x1
            v = or * sin(ag);//y1
            //贴图
            s = (u + CIRCULAR_T) / (CIRCULAR_T*2.0);
            t = (CIRCULAR_T - v) / (CIRCULAR_T*2.0);
            
            *(PtArray+Offset++) = x;
            *(PtArray+Offset++) = y;
            *(PtArray+Offset++) = z;
            *(PtArray+Offset++) = s;
            *(PtArray+Offset++) = 1.0-t;
        }
    }
    
    modelst.PtCount = Offset/5;
    modelst.Stacks = stacks;
    modelst.Slices = slices;
    modelst.PtArrays = PtArray;
    
    return modelst;
}

#pragma mark - 裁剪YUV数据
+(void)cutxYUV420:(char *)srcBuffer srcW:(int)srcW srcH:(int)srcH desBuffer:(char *)desBuffer cutX:(int)x cutY:(int)y cutW:(int)desW cutH:(int)desH
{
    int tmpRange;
    int bufferIndex;
    
    int yIndex = 0;
    bufferIndex = 0 + x + y * srcW;
    tmpRange = srcW * desH;
    for (int i = 0; i < tmpRange;) {
        memcpy(desBuffer + yIndex, srcBuffer + bufferIndex + i, desW);
        i += srcW;
        yIndex += desW;
    }
    
    int uIndex = desW * desH;
    int uIndexStep = srcW / 2;
    int uWidthCopy = desW / 2;
    bufferIndex = srcW * srcH + x / 2 + y / 2 * srcW / 2;
    tmpRange = srcW * desH / 4;
    for (int i = 0; i < tmpRange;) {
        memcpy(desBuffer + uIndex, srcBuffer + bufferIndex + i, uWidthCopy);
        i += uIndexStep;
        uIndex += uWidthCopy;
    }
    
    int vIndex = desW * desH + desW * desH / 4;
    int vIndexStep = srcW / 2;
    int vWidthCopy = desW / 2;
    bufferIndex = srcW * srcH + srcW * srcH / 4 + x / 2 + y / 2 * srcW / 2;
    tmpRange = srcW * desH / 4;
    for (int i = 0; i < tmpRange;) {
        memcpy(desBuffer + vIndex, srcBuffer + bufferIndex + i, vWidthCopy);
        i += vIndexStep;
        vIndex += vWidthCopy;
    }
}

@end
