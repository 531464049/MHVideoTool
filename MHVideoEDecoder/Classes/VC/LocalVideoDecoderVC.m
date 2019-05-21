//
//  LocalVideoDecoderVC.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/5.
//  Copyright © 2019 mh. All rights reserved.
//

#import "LocalVideoDecoderVC.h"
#import "MHLocalVideoDecoder.h"
#import "MHStreamPlayer.h"

@interface LocalVideoDecoderVC ()<MHLocalVideoDecoderDelegate>

@property(nonatomic,strong)MHStreamPlayer * playLayer;

@end

@implementation LocalVideoDecoderVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
    
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"mp4"];
    NSURL * videoUrl = [NSURL fileURLWithPath:path];
    
    MHLocalVideoDecoder * devocer = [[MHLocalVideoDecoder alloc] init];
    devocer.delegate = self;
    [devocer startProcessingVideoUrl:videoUrl];
    
    //解码后的播放器
    self.playLayer = [[MHStreamPlayer alloc] initWithFrame:CGRectMake(0, 100, self.view.frame.size.width, self.view.frame.size.height-200)];
    [self.view addSubview:self.playLayer];
}
-(void)mhLocalVideoDecoderOutputDataCallback:(CVImageBufferRef)imageBuffer
{
    [self.playLayer inputPixelBuffer:imageBuffer];
    CVPixelBufferRelease(imageBuffer);
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
