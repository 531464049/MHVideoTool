//
//  MHAVConfig.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/20.
//  Copyright © 2019 mh. All rights reserved.
//

#import "MHAVConfig.h"

@implementation AWVideoConfig
- (instancetype)init
{
    self = [super init];
    if (self) {
        self.fps = 20;
    }
    return self;
}

@end


@implementation AWAudioConfig
- (instancetype)init
{
    self = [super init];
    if (self) {
        self.channelCount = 1;
        self.sampleSize = 16;
        self.sampleRate = 44100;
    }
    return self;
}
@end


