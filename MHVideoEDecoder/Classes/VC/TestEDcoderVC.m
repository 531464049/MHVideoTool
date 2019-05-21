//
//  TestEDcoderVC.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/4.
//  Copyright © 2019 mh. All rights reserved.
//

#import "TestEDcoderVC.h"
#import "MHVideoCapture.h"
#import "MHVideoEncoder.h"
#import "MHH264Decoder.h"

#import "MHStreamPlayer.h"


@interface TestEDcoderVC ()<MHVideoCaptureDelegate,MHVideoEncoderDelegate,MHH264DecoderDelegate>

@property(nonatomic,strong)MHVideoCapture * videoCapture;
@property(nonatomic,strong)MHVideoEncoder * videoEncoder;
@property(nonatomic,strong)MHH264Decoder * videoDecoder;


@property(nonatomic,strong)MHStreamPlayer * playLayer;

@end

@implementation TestEDcoderVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
    
    //初始化相机
    self.videoCapture = [[MHVideoCapture alloc] init];
    self.videoCapture.delegate = self;
    self.videoCapture.videoPreviewLayer.frame = CGRectMake(0, 64, self.view.frame.size.width, (self.view.frame.size.height - 64)/2);
    [self.view.layer addSublayer:self.videoCapture.videoPreviewLayer];
    [self.videoCapture startCapture];
    
    //初始化视频编码器-使用默认参数
    self.videoEncoder = [[MHVideoEncoder alloc] initWithParam:nil];
    self.videoEncoder.delegate = self;
    [self.videoEncoder startVideoEncode];
    
    //初始化视频解码器
    self.videoDecoder = [[MHH264Decoder alloc] init];
    self.videoDecoder.delegate = self;
    
    //解码后的播放器
    self.playLayer = [[MHStreamPlayer alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(self.videoCapture.videoPreviewLayer.frame), self.view.frame.size.width, (self.view.frame.size.height - 64)/2)];
    [self.view addSubview:self.playLayer];
}
#pragma mark - 采集回调CMSampleBufferRef
-(void)mhVideoCaptureOutputDataCallback:(CMSampleBufferRef)sampleBuffer
{
    CVImageBufferRef pixelBuffer = (CVImageBufferRef)CMSampleBufferGetImageBuffer(sampleBuffer);
    [self.videoEncoder videoEncodeInputData:pixelBuffer forceKeyFrame:NO];
}
#pragma mark - H264编码回调
-(void)mhVideoEncodeOutputDataCallback:(NSData *)data isKeyFrame:(BOOL)isKeyFrame
{
    [self.videoDecoder decodeH264VideoData:data];
}
#pragma mark - H264解码回调
-(void)mhVideoDecodeOutputDataCallback:(CVImageBufferRef)imageBuffer
{
    [self.playLayer inputPixelBuffer:imageBuffer];
    //CVPixelBufferRelease(imageBuffer);
}
@end
