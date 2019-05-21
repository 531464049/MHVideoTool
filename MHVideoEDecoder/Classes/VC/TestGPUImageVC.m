//
//  TestGPUImageVC.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/4.
//  Copyright © 2019 mh. All rights reserved.
//

#import "TestGPUImageVC.h"
#import "MHFilter.h"

@interface TestGPUImageVC ()

@property(nonatomic, strong)GPUImageVideoCamera * videoCamera;//拍视频-相机
@property(nonatomic, strong)GPUImageView * videoPreView; //相机预览图层
@property(nonatomic,strong)GPUImageOutput<GPUImageInput> * filter;//当前选择滤镜
@property(nonatomic,strong)GPUImageRawDataOutput *rawDataOutput;//视频输出

@end

@implementation TestGPUImageVC
-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    
    [self.videoCamera stopCameraCapture];
    [self.videoCamera removeInputsAndOutputs];
    [self.videoCamera removeAllTargets];
    [self.filter removeAllTargets];
    
    self.rawDataOutput = nil;
    self.filter = nil;
    self.videoCamera = nil;
}
-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.videoCamera startCameraCapture];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor blackColor];
    
    //初始化相机 默认后置摄像头
    self.videoCamera = [[GPUImageVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPreset1280x720 cameraPosition:AVCaptureDevicePositionFront];
    self.videoCamera.outputImageOrientation = UIInterfaceOrientationPortrait;
    self.videoCamera.horizontallyMirrorFrontFacingCamera = YES;
    // 该句可防止允许声音通过的情况下，避免录制第一帧黑屏闪屏(====)
    [self.videoCamera addAudioInputsAndOutputs];
    
    //初始化相机预览图层
    self.videoPreView = [[GPUImageView alloc] initWithFrame:self.view.bounds];
    self.videoPreView.backgroundColor = [UIColor blackColor];
    self.videoPreView.fillMode = kGPUImageFillModePreserveAspectRatioAndFill;
    [self.view addSubview:self.videoPreView];
    
    self.filter = [[GPUImageSwirlFilter alloc] init];
    [self.videoCamera addTarget:self.filter];
    [self.filter addTarget:self.videoPreView];
    
    self.rawDataOutput = [[GPUImageRawDataOutput alloc] initWithImageSize:CGSizeMake(640, 480) resultsInBGRAFormat:YES];
    [self.filter addTarget:self.rawDataOutput];

    __weak GPUImageRawDataOutput *weakOutput = self.rawDataOutput;
    __weak typeof(self) weakSelf = self;

    [self.rawDataOutput setNewFrameAvailableBlock:^{
        __strong GPUImageRawDataOutput *strongOutput = weakOutput;
        [strongOutput lockFramebufferForReading];

        // 这里就可以获取到添加滤镜的数据了
        GLubyte *outputBytes = [strongOutput rawBytesForImage];
        NSInteger bytesPerRow = [strongOutput bytesPerRowInOutput];
        CVPixelBufferRef pixelBuffer = NULL;
        CVPixelBufferCreateWithBytes(kCFAllocatorDefault, 640, 480, kCVPixelFormatType_32BGRA, outputBytes, bytesPerRow, nil, nil, nil, &pixelBuffer);

        // 之后可以利用VideoToolBox进行硬编码再结合rtmp协议传输视频流了
        //CVPixelBufferRef = CVImageBufferRef
        [weakSelf encodeWithCVPixelBufferRef:pixelBuffer];

        [strongOutput unlockFramebufferAfterReading];
        CFRelease(pixelBuffer);
    }];
}
#pragma mark - filter处理后的视频帧
-(void)encodeWithCVPixelBufferRef:(CVImageBufferRef)imageBuffer
{
    int bufferWidth = (int) CVPixelBufferGetWidth(imageBuffer);
    int bufferHeight = (int) CVPixelBufferGetHeight(imageBuffer);
    NSLog(@"filter处理后的视频帧 %d----%d",bufferWidth,bufferHeight);
}

@end
