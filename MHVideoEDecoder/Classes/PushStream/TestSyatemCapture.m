//
//  TestSyatemCapture.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/20.
//  Copyright © 2019 mh. All rights reserved.
//

#import "TestSyatemCapture.h"
#import "MHCapture.h"

@interface TestSyatemCapture ()

@property(nonatomic,strong)MHCapture * capture;

@end

@implementation TestSyatemCapture

- (void)viewDidLoad {
    [super viewDidLoad];
//    self.capture = [[MHCapture alloc] initWithCaptureType:MHAVCaptureTypeGPUImage sessionPreset:AVCaptureSessionPreset1280x720 preViewRect:self.view.bounds];
    
    self.capture = [[MHCapture alloc] initWithCaptureType:MHAVCaptureTypeSystem sessionPreset:AVCaptureSessionPreset1280x720 preViewRect:self.view.bounds];
    [self.view addSubview:self.capture.preview];
    
    [self.capture startCapture];
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
