//
//  MHLocalVideoDecoder.h
//  MHFishEyeVideo
//
//  Created by mahao on 2019/5/2.
//  Copyright © 2019年 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@protocol MHLocalVideoDecoderDelegate <NSObject>

@optional
/** 本地视频解码数据回调 */
- (void)mhLocalVideoDecoderOutputDataCallback:(CVImageBufferRef)imageBuffer;

@end


@interface MHLocalVideoDecoder : NSObject

@property(nonatomic,weak)id <MHLocalVideoDecoderDelegate> delegate;

@property(readonly,strong) NSURL *url;
@property(assign,nonatomic)BOOL playAtActualSpeed;//是否以视频原始速度播放

- (void)startProcessingVideoUrl:(NSURL *)videoUrl;
@end

