//
//  ViewController.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/4.
//  Copyright © 2019 mh. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()<UITableViewDelegate,UITableViewDataSource>

@property(nonatomic,strong)UITableView * tableview;

@property(nonatomic,copy)NSArray * titleArr;
@property(nonatomic,copy)NSArray * classArr;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.tableview = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStylePlain];
    self.tableview.delegate = self;
    self.tableview.dataSource = self;
    [self.view addSubview:self.tableview];
    
    self.titleArr = @[@"GPUImage滤镜+采集",@"原生相机采集+编码+解码+播放",@"解码本地视频+播放",@"关于openGL",@"stream播放器",@"fishEye",@"完整视频采集-推流-直播"];
    self.classArr = @[@"TestGPUImageVC",@"TestEDcoderVC",@"LocalVideoDecoderVC",@"AboutOpenGLVC",@"TestStreamVideoPlayer",@"TestFishSyeVC",@"TestPushStreamListVC"];
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.titleArr.count;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * cellid = @"opopopopopop";
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:cellid];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellid];
    }
    cell.textLabel.text = self.titleArr[indexPath.row];
    return cell;
}
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    NSString * className = self.classArr[indexPath.row];
    UIViewController * vc = (UIViewController *)[NSClassFromString(className) new];
    [self.navigationController pushViewController:vc animated:YES];
}
@end
