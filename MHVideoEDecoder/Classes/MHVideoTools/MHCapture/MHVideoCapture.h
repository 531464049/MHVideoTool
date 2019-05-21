//
//  MHVideoCapture.h
//  MHVideoCoder
//
//  Created by Junky on 2019/4/16.
//  Copyright © 2019年 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>


@protocol MHVideoCaptureDelegate;

/**
 视频采集
 */
@interface MHVideoCapture : NSObject

/** 状态变化回调代理 */
@property(nonatomic,weak)id <MHVideoCaptureDelegate> delegate;
/** 预览图层，把这个图层加在View上就能播放 */
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *videoPreviewLayer;
/** 是否已经在采集 */
@property (nonatomic, assign,readonly) BOOL isCapturing;

/** 开始采集 */
-(void)startCapture;

/** 停止采集 */
- (void)stopCapture;

/** 翻转摄像头 */
- (void)reverseCamera;

/**
 采集过程中动态修改视频分辨率
 @param sessionPreset 分辨率
 */
- (void)changeSessionPreset:(AVCaptureSessionPreset)sessionPreset;

/**
 获取当前图片
 @param completion 回调image
 */
- (void)imageCapture:(void(^)(UIImage *image))completion;

/**
 设置帧率
 @param minFrameRate 最小帧率
 @param maxFrameRate 最大帧率
 */
-(void)adjustFrameRate:(NSInteger)minFrameRate max:(NSInteger)maxFrameRate;
@end



@protocol MHVideoCaptureDelegate <NSObject>

@optional
/**
 摄像头采集数据输出
 @param sampleBuffer 采集的数据
 */
- (void)mhVideoCaptureOutputDataCallback:(CMSampleBufferRef)sampleBuffer;

@end
