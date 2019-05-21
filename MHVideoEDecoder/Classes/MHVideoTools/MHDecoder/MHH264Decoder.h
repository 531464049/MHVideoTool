//
//  MHH264Decoder.h
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/17.
//  Copyright © 2019 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <VideoToolbox/VideoToolbox.h>
#import "MHBufferTools.h"


@protocol MHH264DecoderDelegate <NSObject>

@optional
/** H264解码数据回调 */
-(void)mhVideoDecoderOuPutPixel:(CVPixelBufferRef)pixelBuffer;
-(void)mhVideoDecoderOutPutImageref:(CGImageRef)imageref;
-(void)mhVideoDecoderOuPutYUVData:(NSData *)yuvData frameWidth:(int)width height:(int)height;
@end

typedef enum : NSUInteger {
    MHH264DecoderOutPutType_y_uv          = 0,
    MHH264DecoderOutPutType_y_u_v         = 1,
    MHH264DecoderOutPutType_imageref      = 2,
}MHH264DecoderOutPutType;//输出视频格式

@interface MHH264Decoder : NSObject

@property(nonatomic,weak)id<MHH264DecoderDelegate> delegate;

@property (nonatomic,assign)MHH264DecoderOutPutType outPutType;    //解码后回调类型

/**
 H264视频流解码
 @param videoData 视频帧数据
 */
-(void)decodeH264VideoData:(NSData *)videoData;

/**
 释放解码器
 */
- (void)releaseH264Decoder;

@end

