//
//  MHBufferTools.h
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/20.
//  Copyright © 2019 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface MHBufferTools : NSObject

/** char转NSData */
+(NSData *)charToData:(char *)ar;
/** NSData转char */
+(char *)dataToChar:(NSData *)data;

/**
 CVImageBufferRef 转 YUV数据
 
 @param pixelBuffer CVImageBufferRef
 @param callBack yuvData frameWidth frameHeight
 */
+(void)imageBuffer:(CVImageBufferRef)pixelBuffer toYuvData:(void(^)(NSData * yuvData,int width,int height))callBack;
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

/**
 CVImageBufferRef转换为data数据
 @param imageBuffer CVImageBufferRef
 @return NSData数据
 */
+(NSData *)imageBufferToData:(CVImageBufferRef)imageBuffer;

/**
 CVImageBufferRef转换为CGImageRef
 @param imageBuffer CVImageBufferRef
 @return CGImageRef
 */
+(CGImageRef)imageRefFromimageBuffer:(CVImageBufferRef)imageBuffer;

/**
 CVImageBufferRef转换为UIImage
 @param imageBuffer CVImageBufferRef
 @return image
 */
+(UIImage *)imageFromimageBuffer:(CVImageBufferRef)imageBuffer;

@end

