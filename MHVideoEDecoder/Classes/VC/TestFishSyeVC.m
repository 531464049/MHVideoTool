//
//  TestFishSyeVC.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/9.
//  Copyright © 2019 mh. All rights reserved.
//

#import "TestFishSyeVC.h"

#import <IOTCamera/IOTCamera.h>
#import <IOTCamera/AVAPIs.h>

#import "MHFishEyeVideoPlayer.h"
#import "MHStreamPlayer.h"

@interface TestFishSyeVC ()<CameraDelegate>

@property(nonatomic,strong)MHStreamPlayer * player;

@end

@implementation TestFishSyeVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    Camera * cam = [[Camera alloc] initWithName:@"test"];
    cam.delegate = self;
    [cam start:0 viewAccount:@"admin" viewPassword:@"chw123456" is_playback:FALSE];
    [cam connect:@"UZRKP65GUVXSBM5V111A"];
    [cam TK_startShowWithYUV:0 isSWDecode:NO];
    
    self.player = [[MHStreamPlayer alloc] initWithFrame:CGRectMake(0, 100, self.view.frame.size.width, self.view.frame.size.width)];
    [self.view addSubview:self.player];
}
-(void)camera:(Camera *)camera didChangeSessionStatus:(NSInteger)status
{
    if (status == CONNECTION_STATE_CONNECTED) {
        NSLog(@"----------> 设备连线状态回调 status = CONNECTED");
    } else if (status == CONNECTION_STATE_CONNECTING) {
        NSLog(@"----------> 设备连线状态回调 status = CONNECTING");
    } else if (status == CONNECTION_STATE_CONNECT_FAILED) {
        NSLog(@"----------> 设备连线状态回调 status = FAILED");
    } else if (status == CONNECTION_STATE_TIMEOUT) {
        NSLog(@"----------> 设备连线状态回调 status = TIMEOUT");
    } else if (status == CONNECTION_STATE_DISCONNECTED) {
        NSLog(@"----------> 设备连线状态回调 status = DISCONNECTED");
    }
}
-(void)camera:(Camera *)camera outputDecodeVideoYUVData:(NSData *)data width:(int)outWidth height:(int)outHeight timestamp:(uint64_t)timestamp channel:(NSInteger)channel
{
    //NSLog(@"%d---%d",outWidth,outHeight);
    [self.player inputYuvDataTexture:(char *)data.bytes width:outWidth height:outHeight];
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
