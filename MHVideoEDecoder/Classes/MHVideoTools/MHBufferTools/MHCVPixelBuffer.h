//
//  MHCVPixelBuffer.h
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/19.
//  Copyright © 2019 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
#import "libyuv.h"

@interface MHCVPixelBuffer : NSObject

#pragma mark - 裁剪CVPixelBufferRef

/**
 裁剪CVPixelBufferRef
 @param sourcePixelBuffer 要裁剪的CVPixelBufferRef
 @param scaledSize 裁剪大小
 @return 裁剪后的CVPixelBufferRef
 */
+(CVPixelBufferRef)cropPixelBuffer:(CVPixelBufferRef)sourcePixelBuffer scaledSize:(CGSize)scaledSize;

/*
 * 注意旋转SampleBuffer 为argb或者bgra格式，其他格式可能不支持
 * rotationConstant:
 *  0 -- rotate 0 degrees (simply copy the data from src to dest)
 *  1 -- rotate 90 degrees counterclockwise
 *  2 -- rotate 180 degress
 *  3 -- rotate 270 degrees counterclockwise
 */
+ (CVPixelBufferRef)rotateBuffer:(CVImageBufferRef)sampleBuffer withConstant:(uint8_t)rotationConstant;

/**
 32BGRA 转 nv12(Y_UV)
 @param pixelBuffer rgbBuffer
 @return nv12buffer(Y_UV)
 */
+(CVPixelBufferRef)convertRGB_BuffertoYUV:(CVPixelBufferRef)pixelBuffer;

/**
 Y_U_V(i420) 转 Y_UV(nv12)
 @param pixelBuffer Y_U_V(i420) BUFFER
 @return Y_UV(nv12) BUFFER
 */
+(CVPixelBufferRef)convertY_U_V_BuffertoY_UV:(CVPixelBufferRef)pixelBuffer;

@end

