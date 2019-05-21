//
//  AppDelegate.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/4.
//  Copyright © 2019 mh. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"

#import <IOTCamera/IOTCamera.h>
#import <IOTCamera/AVAPIs.h>

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [self setupTUTKSDK];
    
    _window = [[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    _window.backgroundColor = [UIColor whiteColor];
    
    UINavigationController * nav = [[UINavigationController alloc]initWithRootViewController:[[ViewController alloc]init]];
    
    _window.rootViewController = nav;
    [_window makeKeyAndVisible];
    
    return YES;
}
-(void)setupTUTKSDK
{
    NSString *appKey = @"ryGSxIhp11KZapqC0H4c4aIF5J2yCjr3fGy880kULTY=";//包名:com.xrobot.ls.ios，对应的注册码
    NSLog(@"TK_InitIOTC---appKey:%@",appKey);
    [Camera TK_InitIOTC:appKey success:^{
        NSLog(@"TK_InitIOTC:success");
    } failure:^(NSError * _Nonnull error) {
        if (error.code == TKPrivateKeyErrorNoValid) {
            NSLog(@"TK_InitIOTC:false(TKPrivateKeyErrorNoValid)");
        }else if (error.code == TKPrivateKeyErrorExpire) {
            NSLog(@"TK_InitIOTC:false(TKPrivateKeyErrorExpire)");
        }
    }];
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


@end
