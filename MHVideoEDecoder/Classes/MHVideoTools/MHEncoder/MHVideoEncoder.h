//
//  MHVideoEncoder.h
//  MHVideoCoder
//
//  Created by Junky on 2019/4/16.
//  Copyright © 2019年 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <VideoToolbox/VideoToolbox.h>

@class MHVideoEncoderParam;

typedef NS_ENUM(NSUInteger, MHVideoEncoderProfileLevel)
{
    MHVideoEncoderProfileLevelBP,//标清
    MHVideoEncoderProfileLevelMP,//半高清
    MHVideoEncoderProfileLevelHP//全高清
};


/**
 编码器配置参数
 */
@interface MHVideoEncoderParam : NSObject
/** ProfileLevel 默认为BP */
@property (nonatomic, assign) MHVideoEncoderProfileLevel profileLevel;
/** 编码内容的宽度  默认180*/
@property (nonatomic, assign) NSInteger encodeWidth;
/** 编码内容的高度  默认320*/
@property (nonatomic, assign) NSInteger encodeHeight;
/** 编码类型 默认kCMVideoCodecType_H264*/
@property (nonatomic, assign) CMVideoCodecType encodeType;
/** 码率 单位kbps 默认1024 * 1024*/
@property (nonatomic, assign) NSInteger bitRate;
/** 帧率 单位为fps，默认为15fps */
@property (nonatomic, assign) NSInteger frameRate;
/** 最大I帧间隔，单位为秒，默认为240秒一个I帧 */
@property (nonatomic, assign) NSInteger maxKeyFrameInterval;
/** 是否允许产生B帧 默认为NO */
@property (nonatomic, assign) BOOL allowFrameReordering;
@end



@protocol MHVideoEncoderDelegate <NSObject>

@optional
/**
 编码输出数据
 @param data 输出数据
 @param isKeyFrame 是否为关键帧
 */
- (void)mhVideoEncodeOutputDataCallback:(NSData *)data isKeyFrame:(BOOL)isKeyFrame;

@end

@interface MHVideoEncoder : NSObject

@property (nonatomic, weak) id<MHVideoEncoderDelegate> delegate;

/** 编码参数 */
@property (nonatomic,strong)MHVideoEncoderParam * param;

//初始化
-(instancetype)initWithParam:(MHVideoEncoderParam *)param;

/**
 开始编码
 @return 成功/失败
 */
- (BOOL)startVideoEncode;

/**
 停止编码
 @return 成功/失败
 */
- (BOOL)stopVideoEncode;

/**
 输入待编码数据
 @param sampleBuffer 待编码数据
 @param forceKeyFrame 是否强制I桢
 @return 成功/失败
 */
- (BOOL)videoEncodeInputData:(CVImageBufferRef)sampleBuffer forceKeyFrame:(BOOL)forceKeyFrame;
/**
 编码过程中设置码率
 @param bitRate 码率
 @return 成功/失败
 */
- (BOOL)updateBitRate:(NSInteger)bitRate;

@end




