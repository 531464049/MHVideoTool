//
//  TestStreamVideoPlayer.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/15.
//  Copyright © 2019 mh. All rights reserved.
//

#import "TestStreamVideoPlayer.h"
#import "MHVideoHeader.h"
#import "MHStreamPlayer.h"

@interface TestStreamVideoPlayer ()<MHVideoCaptureDelegate,MHVideoEncoderDelegate,MHH264DecoderDelegate>

@property(nonatomic,strong)MHVideoCapture * videoCapture;
@property(nonatomic,strong)MHVideoEncoder * videoEncoder;
@property(nonatomic,strong)MHH264Decoder * videoDecoder;
@property(nonatomic,strong)MHStreamPlayer * playLayer;

@property(nonatomic,assign)NSInteger index;

@end

@implementation TestStreamVideoPlayer

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
    [self updateTools];
}
-(void)updateTools
{
    //初始化相机
    self.videoCapture = [[MHVideoCapture alloc] init];
    self.videoCapture.delegate = self;
    self.videoCapture.videoPreviewLayer.frame = CGRectMake(0, 0, self.view.frame.size.width/2, self.view.frame.size.height/2);
    [self.view.layer addSublayer:self.videoCapture.videoPreviewLayer];
    [self.videoCapture startCapture];
    
    [self buildOBJ];
    
    //解码后的播放器
    self.playLayer = [[MHStreamPlayer alloc] initWithFrame:CGRectMake(self.view.frame.size.width/2, 0, self.view.frame.size.width/2, self.view.frame.size.height/2)];
    [self.view addSubview:self.playLayer];
    
    self.index = 1;//默认CVPixelBufferRef播放
    /*
    mhTextureFragModeCgImage = 1,
    mhTextureFragModePixelBuf = 2,
    mhTextureFragModeYuv = 3,
    mhTextureFragModeRgb = 4,
    mhTextureFragModeRgba = 5,
     */
    NSArray * titleArr = @[@"CGImageRef",@"CVPixelBufferRef",@"yuv",@"RGB",@"RGBA"];
    for (int i = 0; i < 5; i ++) {
        UIButton * btn = [UIButton buttonWithType:0];
        btn.frame = CGRectMake(10, CGRectGetMaxY(self.playLayer.frame) + 20 + 50*i, self.view.frame.size.width-20, 40);
        btn.layer.cornerRadius = 5;
        btn.layer.borderColor = [UIColor orangeColor].CGColor;
        btn.layer.borderWidth = 2;
        btn.layer.masksToBounds = YES;
        [btn setTitle:titleArr[i] forState:0];
        [btn setTitleColor:[UIColor whiteColor] forState:0];
        btn.titleLabel.font = [UIFont systemFontOfSize:14];
        if (i == self.index) {
            btn.layer.borderColor = [UIColor redColor].CGColor;
            [btn setTitleColor:[UIColor redColor] forState:0];
        }
        btn.tag = 8888 + i;
        [btn addTarget:self action:@selector(itemClick:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:btn];
    }
}
-(void)buildOBJ
{
    //初始化视频编码器
    MHVideoEncoderParam * param  = [[MHVideoEncoderParam alloc] init];
    param.profileLevel = MHVideoEncoderProfileLevelHP;
    param.encodeWidth = 720;
    param.encodeHeight = 1280;
    param.frameRate = 30;
    
    self.videoEncoder = [[MHVideoEncoder alloc] initWithParam:param];
    self.videoEncoder.delegate = self;
    [self.videoEncoder startVideoEncode];
    
    
    //初始化视频解码器
    self.videoDecoder = [[MHH264Decoder alloc] init];
    self.videoDecoder.delegate = self;
}
-(void)itemClick:(UIButton *)sender
{
    NSInteger tag = sender.tag - 8888;
    if (tag == self.index) {
        return;
    }
    self.index = tag;
    
    [self removeOBJ];
    [self buildOBJ];
    
    if (self.index == 0) {
        self.videoDecoder.outPutType = MHH264DecoderOutPutType_imageref;
    }else if (self.index == 1) {
        self.videoDecoder.outPutType = MHH264DecoderOutPutType_y_uv;
    }else if (self.index == 2) {
        self.videoDecoder.outPutType = MHH264DecoderOutPutType_y_u_v;
    }
    
    [self.videoCapture startCapture];
    
    for (int i = 0; i < 5; i ++) {
        UIButton * btn = (UIButton *)[self.view viewWithTag:8888 + i];
        btn.layer.borderColor = [UIColor orangeColor].CGColor;
        [btn setTitleColor:[UIColor whiteColor] forState:0];
        if (i == self.index) {
            btn.layer.borderColor = [UIColor redColor].CGColor;
            [btn setTitleColor:[UIColor redColor] forState:0];
        }
    }
}
-(void)removeOBJ
{
    self.videoEncoder.delegate = nil;
    [self.videoEncoder stopVideoEncode];
    self.videoEncoder = nil;
    
    self.videoDecoder.delegate = nil;
    [self.videoDecoder releaseH264Decoder];
    self.videoDecoder = nil;
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
    if (self.videoDecoder) {
        [self.videoDecoder decodeH264VideoData:data];
    }
}
#pragma mark - H264解码回调
-(void)mhVideoDecoderOuPutPixel:(CVPixelBufferRef)pixelBuffer
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.playLayer inputPixelBuffer:pixelBuffer];
        CVPixelBufferRelease(pixelBuffer);
    });
}
-(void)mhVideoDecoderOutPutImageref:(CGImageRef)imageref
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.playLayer inputCgImageTexture:imageref];
        CGImageRelease(imageref);
    });
}
-(void)mhVideoDecoderOuPutYUVData:(NSData *)yuvData frameWidth:(int)width height:(int)height
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.playLayer inputYuvDataTexture:[MHBufferTools dataToChar:yuvData] width:width height:height];
    });
}
#pragma mark - 释放资源
-(void)dealloc
{
    self.videoCapture.delegate = nil;
    [self.videoCapture stopCapture];
    self.videoCapture = nil;
    
    [self removeOBJ];

    [self.playLayer cleanPlayer];
    self.playLayer = nil;
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
