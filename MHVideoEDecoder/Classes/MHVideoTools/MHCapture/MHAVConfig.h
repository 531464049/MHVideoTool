//
//  MHAVConfig.h
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/20.
//  Copyright © 2019 mh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface AWVideoConfig : NSObject

@property (nonatomic, copy) AVCaptureSessionPreset sessionPreset;//自由设置
@property (nonatomic, assign) NSInteger fps;//自由设置
@end


@interface AWAudioConfig : NSObject
@property (nonatomic, assign) NSInteger channelCount;//可选 1 2
@property (nonatomic, assign) NSInteger sampleRate;//可选 44100 22050 11025 5500
@property (nonatomic, assign) NSInteger sampleSize;//可选 16 8

@end

