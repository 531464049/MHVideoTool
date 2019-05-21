//
//  MHVideoCapture.m
//  MHVideoCoder
//
//  Created by Junky on 2019/4/16.
//  Copyright © 2019年 mh. All rights reserved.
//

#import "MHVideoCapture.h"

@interface MHVideoCapture ()<AVCaptureVideoDataOutputSampleBufferDelegate>

/** 采集会话 */
@property (nonatomic, strong) AVCaptureSession *captureSession;
/** 采集输入设备 也就是摄像头 */
@property (nonatomic, strong) AVCaptureDeviceInput *captureDeviceInput;
/** 采集输出 */
@property (nonatomic, strong) AVCaptureVideoDataOutput *captureVideoDataOutput;
/** 抓图输出 */
@property (nonatomic, strong) AVCaptureStillImageOutput *captureStillImageOutput;
/** 输出连接 */
@property (nonatomic, strong) AVCaptureConnection *captureConnection;

@end

@implementation MHVideoCapture
-(void)dealloc
{
    [self stopCapture];
}
-(instancetype)init
{
    self = [super init];
    if (self) {
        [self initCapture];
    }
    return self;
}
#pragma mark - 初始化输入输出设备
-(void)initCapture
{
    //获得摄像头输入设备
    AVCaptureDevice *captureDevice=[self getCameraDeviceWithPosition:AVCaptureDevicePositionFront];//取得前置摄像头
    if (!captureDevice) {
        NSLog(@"取得摄像头时出现问题.");
        return;
    }
    NSError *error=nil;
    //根据输入设备初始化设备输入对象，用于获得输入数据
    self.captureDeviceInput = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];
    if (error) {
        NSLog(@"取得设备输入对象时出错，错误原因：%@",error.localizedDescription);
        return;
    }
    // 设置视频输出
    self.captureVideoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
    
    NSDictionary *videoSetting = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange], kCVPixelBufferPixelFormatTypeKey, nil];
    [self.captureVideoDataOutput setVideoSettings:videoSetting];
    // 设置输出串行队列和数据回调
    dispatch_queue_t outputQueue = dispatch_queue_create("MHVideoCapturerOutputQueue", DISPATCH_QUEUE_SERIAL);
    [self.captureVideoDataOutput setSampleBufferDelegate:self queue:outputQueue];
    // 丢弃延迟的帧
    self.captureVideoDataOutput.alwaysDiscardsLateVideoFrames = YES;
    
    // 设置抓图输出
    self.captureStillImageOutput = [[AVCaptureStillImageOutput alloc] init];
    [self.captureStillImageOutput setOutputSettings:@{AVVideoCodecKey:AVVideoCodecJPEG}];
    
    //初始化会话
    self.captureSession = [[AVCaptureSession alloc] init];
    self.captureSession.usesApplicationAudioSession = NO;
    //设置分辨率
    if ([_captureSession canSetSessionPreset:AVCaptureSessionPreset1280x720]) {
        _captureSession.sessionPreset=AVCaptureSessionPreset1280x720;
    }
    // 添加输入设备到会话
    if ([self.captureSession canAddInput:self.captureDeviceInput]){
        [self.captureSession addInput:self.captureDeviceInput];
    }else{
        NSLog(@"输入设备无法添加");
        return;
    }
    // 添加输出设备到会话
    if ([self.captureSession canAddOutput:self.captureVideoDataOutput]){
        [self.captureSession addOutput:self.captureVideoDataOutput];
    } else {
        NSLog(@"输出设备无法添加");
        return;
    }
    if ([self.captureSession canAddOutput:self.captureStillImageOutput]){
        [self.captureSession addOutput:self.captureStillImageOutput];
    } else {
        NSLog(@"抓图设备无法添加");
        return;
    }
    //初始化连接
    self.captureConnection = [self.captureVideoDataOutput connectionWithMediaType:AVMediaTypeVideo];
    self.captureConnection.videoMirrored = YES;
    self.captureConnection.videoOrientation = AVCaptureVideoOrientationPortrait;
    
    self.videoPreviewLayer = [AVCaptureVideoPreviewLayer layerWithSession:self.captureSession];
    self.videoPreviewLayer.connection.videoOrientation = AVCaptureVideoOrientationPortrait;
    self.videoPreviewLayer.videoGravity = AVLayerVideoGravityResizeAspect;
    
    [self adjustFrameRate:15 max:30];
}
#pragma mark - 设置帧率
-(void)adjustFrameRate:(NSInteger)minFrameRate max:(NSInteger)maxFrameRate
{
    NSError *error = nil;
    [self.captureDeviceInput.device lockForConfiguration:&error];
    self.captureDeviceInput.device.activeVideoMinFrameDuration = CMTimeMake(1, (int)minFrameRate);
    self.captureDeviceInput.device.activeVideoMaxFrameDuration = CMTimeMake(1, (int)maxFrameRate);
    [self.captureDeviceInput.device unlockForConfiguration];
}
#pragma mark - 开始采集
-(void)startCapture
{
    if (self.isCapturing) {
        NSLog(@"已经在采集视频了");
        return;
    }
    // 摄像头权限判断
    AVAuthorizationStatus videoAuthStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if (videoAuthStatus != AVAuthorizationStatusAuthorized) {
        NSLog(@"无摄像头权限");
        return;
    }
    [self.captureSession startRunning];
    _isCapturing = YES;
}
#pragma mark - 停止采集
- (void)stopCapture
{
    if (!self.isCapturing) {
        NSLog(@"当前并未开始采集视频");
        return;
    }
    [self.captureSession stopRunning];
    _isCapturing = NO;
    [self.videoPreviewLayer removeFromSuperlayer];
}
#pragma mark - 翻转摄像头
- (void)reverseCamera
{
    // 获取当前摄像头方向
    AVCaptureDevicePosition currentPosition = self.captureDeviceInput.device.position;
    AVCaptureDevicePosition toPosition = AVCaptureDevicePositionUnspecified;
    if (currentPosition == AVCaptureDevicePositionBack || currentPosition == AVCaptureDevicePositionUnspecified) {
        toPosition = AVCaptureDevicePositionFront;
    } else {
        toPosition = AVCaptureDevicePositionBack;
    }
    AVCaptureDevice *captureDevice=[self getCameraDeviceWithPosition:toPosition];
    if (!captureDevice) {
        NSLog(@"无法获取摄像头");
        return;
    }
    
    NSError *error = nil;
    AVCaptureDeviceInput *newInput = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];
    // 修改输入设备
    [self.captureSession beginConfiguration];
    [self.captureSession removeInput:self.captureDeviceInput];
    if ([_captureSession canAddInput:newInput])
    {
        [_captureSession addInput:newInput];
        self.captureDeviceInput = newInput;
    }
    [self.captureSession commitConfiguration];
    
    // 重新获取连接并设置方向
    self.captureConnection = [self.captureVideoDataOutput connectionWithMediaType:AVMediaTypeVideo];
    // 设置摄像头镜像，不设置的话前置摄像头采集出来的图像是反转的
    if (toPosition == AVCaptureDevicePositionFront && self.captureConnection.supportsVideoMirroring)
    {
        self.captureConnection.videoMirrored = YES;
    }
    self.captureConnection.videoOrientation = AVCaptureVideoOrientationPortrait;
    self.videoPreviewLayer.connection.videoOrientation = AVCaptureVideoOrientationPortrait;
}
#pragma mark - 采集过程中动态修改视频分辨率
- (void)changeSessionPreset:(AVCaptureSessionPreset)sessionPreset
{
    if ([self.captureSession canSetSessionPreset:sessionPreset])
    {
        self.captureSession.sessionPreset = sessionPreset;
    }
}
#pragma mark - 获取当前图片
- (void)imageCapture:(void(^)(UIImage *image))completion
{
    // 根据连接取得设备输出的数据
    [self.captureStillImageOutput captureStillImageAsynchronouslyFromConnection:self.captureConnection completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
        if (imageDataSampleBuffer && completion)
        {
            UIImage *image = [UIImage imageWithData:[AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer]];
            completion(image);
        }
    }];
}
#pragma mark - 获取指定方向的摄像头
-(AVCaptureDevice *)getCameraDeviceWithPosition:(AVCaptureDevicePosition )position{
    NSArray *cameras= [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *camera in cameras) {
        if ([camera position]==position) {
            return camera;
        }
    }
    return nil;
}
#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate
- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    [connection setVideoOrientation:AVCaptureVideoOrientationPortrait];

    if (self.delegate && [self.delegate respondsToSelector:@selector(mhVideoCaptureOutputDataCallback:)]) {
        [self.delegate mhVideoCaptureOutputDataCallback:sampleBuffer];
    }
    
}

@end
