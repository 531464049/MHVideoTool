//
//  MHFishEyeVideoPlayer.h
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/8.
//  Copyright © 2019 mh. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MHFishEyeVideoPlayer : UIView

@property (nonatomic,assign) CGFloat     vAngle;//[-360, 360]
@property (nonatomic,assign) CGFloat     hAngle;//[-360, 360]
@property (nonatomic,assign) CGFloat     zDepth;//[0, 1.0]


/**
 贴图接口-输入YUV数据源
 @param yuv yuv数据源
 @param width 原始宽
 @param height 原始高
 */
-(void)inputYuvDataTexture:(char *)yuv width:(int)width height:(int)height;

@end
