//
//  Camera.h
//  IOTCamera
//
//  Created by steven_yang on 16/12/5.
//  Copyright © 2016年 tutksz_ios. All rights reserved.
//

#import "Defines.h"
#import "AVIOCTRLDEFs.h"
#import <CoreMedia/CoreMedia.h>
#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, TKPrivateKeyError)
{
    TKPrivateKeyErrorNoValid   = -10000, //privateKey无效
    TKPrivateKeyErrorExpire    = -10003, //privateKey过期
};

typedef NS_ENUM(NSInteger, TKRegion)
{
    TKREGION_ALL,
    TKREGION_CN,
    TKREGION_EU,
    TKREGION_US ,
};

typedef NS_ENUM(NSInteger, TKRegionResult)
{
    TKREGION_ER_NoERROR                     = 0,//set success
    TKREGION_ER_INVALID_ARG                 = -46,//the region is invalid
    TKREGION_ER_ALREADY_INITIALIZED         = -3,//IOTC module is already initialized
};

struct SUB_STREAM
{
    int index;
    int channel;
};
typedef struct SUB_STREAM SubStream_t;

typedef struct st_LanSearchInfo LanSearch_t;

@protocol CameraDelegate;

@interface Camera : NSObject

@property (nonatomic, weak)      id<CameraDelegate>  delegate;                             // Camera代理

@property (nonatomic, copy)        NSString            *name;                              // 设备名称
@property (nonatomic, copy)        NSString            *uid;                               // 设备UID
@property (nonatomic, copy)        NSString            *viewAcc;                           // 设备用户名
@property (nonatomic, copy)        NSString            *viewPwd;                           // 设备密码
@property (nonatomic, assign)      NSInteger           mainChannel;                        // av主通道

@property (nonatomic, readonly)    NSInteger           sessionID;                          // session号
@property (nonatomic, readonly)    NSInteger           sessionMode;                        // session连线模式
@property (nonatomic, readonly)    NSInteger           sessionState;                       // session状态
@property (nonatomic, readonly)    NSInteger           natD;                               // 远端NAT类型
@property (nonatomic, readonly)    NSInteger           natC;                               // 本地NAT类型
@property (nonatomic, readonly)    NSInteger           connTimes;                          // 连线次数
@property (nonatomic, readonly)    NSInteger           connFailErrCode;                    // 连线错误码
@property (nonatomic, readonly)    int                 nAvResend;                          // 是否有开启重送
@property (nonatomic, readonly)    unsigned int        avChannelIndex;                     // av通道号

@property (nonatomic, assign)      BOOL                isShowInLiveView;                   // 是否在实时画面
@property (nonatomic, assign)      BOOL                isShowInMultiView;                  // 是否在多分格画面

@property (nonatomic, assign)      BOOL                isRecording;                        // 是否录像

@property (nonatomic, assign)      BOOL                isListening;                        // 是否开启监听
@property (nonatomic, assign)      float               mfGaPCMAmplifier_Gain_AfterMIC;     // 麦克风音量
@property (nonatomic, assign)      float               mfGaPCMAmplifier_Gain_BeforeSpeak;  // 扬声器音量
@property (nonatomic, assign)      BOOL                isUsingAEC;                         // 是否开启第三方回音消除

@property (nonatomic, readonly)    unsigned int        nRecvFrmPreSec;                     // 接收的fps
@property (nonatomic, readonly)    unsigned int        nDispFrmPreSec;                     // 显示的fps
@property (nonatomic, readonly)    unsigned int        cmdNum;                             // 指令类型值
@property (nonatomic, readonly)    unsigned int        cmdReturn;                          // 指令返回值

#pragma mark Init Methods

/**
 初始化Camera

 @param name camera名称
 @return Camera初始化后的实例对象
 */
- (id)TK_initWithName:(NSString *)name;


/**
 获取指定通道的用户名

 @param channel 指定通道
 @return 获取的用户名
 */
- (NSString *)TK_getViewAccountOfChannel:(NSInteger)channel;


/**
 获取指定通道的密码

 @param channel 指定通道
 @return 获取的密码
 */
- (NSString *)TK_getViewPasswordOfChannel:(NSInteger)channel;

#pragma mark IOTCApis Methods


/**
 IOTC初始化
 
 @param privateKey 客户私有key(有效期内使用才有效)
 @param success 验证成功
 @param failure 验证失败
 */
+ (void)TK_InitIOTC:(NSString *)privateKey
            success:(void (^)(void))success
            failure:(void (^)(NSError *error))failure;


/**
 IOTC反初始化
 */
+ (void)TK_uninitIOTC;


/**
 获取IOTCamera版本号

 @return IOTCamera版本号
 */
+ (NSString *)TK_getIOTCameraVersion;


/**
 获取IOTCAPIs版本号

 @return IOTCAPIs版本号
 */
+ (NSString *)TK_getIOTCAPIsVerion;


/**
 获取AVAPIs版本号

 @return AVAPIs版本号
 */
+ (NSString *)TK_getAVAPIsVersion;


/**
 设置分区分流方案的地区
 
 @param region 设置分流到TKRegion中的哪个地区
 @return TKRegionResult  设置返回值参照下方结构体TKRegionResult
 */
+ (TKRegionResult)TK_setMasterRegion:(TKRegion)region;

/**
 搜索本地局域网内的设备

 @param num 搜索到设备的数量
 @param timeoutVal 搜索超时时间
 @return 搜索到的设备信息结构体
 */
+ (LanSearch_t *)TK_LanSearch:(int *)num timeout:(int)timeoutVal;


/**
 开始连接设备

 @param uid 设备UID
 */
- (void)TK_connect:(NSString *)uid;


/**
 断开设备连线
 */
- (void)TK_disconnect;


/**
 开启AV通道

 @param channel av通道号
 @param viewAccount 设备用户名
 @param viewPassword 设备密码
 @param bPlaybackMode 是否是录像回放模式
 */
- (void)TK_start:(NSInteger)channel viewAccount:(NSString *)viewAccount viewPassword:(NSString *)viewPassword is_playback:(BOOL)bPlaybackMode;


/**
 关闭AV通道

 @param channel av通道号
 */
- (void)TK_stop:(NSInteger)channel;

#pragma mark - Public Methods

/**
 给指定通道发送指令

 @param channel av通道
 @param type 指令类型
 @param buff 指令内容
 @param buffer_size 指令内容大小
 */
- (void)TK_sendIOCtrlToChannel:(NSInteger)channel Type:(NSInteger)type Data:(char *)buff DataSize:(NSInteger)buffer_size;


/**
 获取指定通道的ServiceTYpe

 @param channel av通道
 @return serviceType值
 */
- (unsigned int)TK_getServiceTypeOfChannel:(NSInteger)channel;


/**
 设置指定通道的解码方式
 
 @param channel av通道
 @param bEnable 是否硬解码
 @param isHWYUVRender 硬解是否进行渲染
 */
- (BOOL)TK_setHWDecodingAbility:(NSInteger)channel requestHWDecode:(BOOL)bEnable isHWYUVRender:(BOOL)isHWYUVRender;


/**
 获取指定通道的连线状态

 @param channel av通道
 @return 连线状态值
 */
- (NSInteger)TK_getConnectionStateOfChannel:(NSInteger)channel;


/**
 指定通道是否需要进行解码

 @param isDecode 是否进行解码 默认YES（进行解码）
 @param channel av通道
 */
- (void)TK_setIsDecode:(BOOL)isDecode withChannel:(int)channel;//for DVR MultiView

/**
 指定通道是否同时上抛视频裸流
 
 @param isOutput 是否上抛
 @param channel av通道
 */
- (void)TK_setOutputFrameData:(BOOL)isOutput withChannel:(int)channel;

/**
 指定通道是否同时上抛音频裸流
 
 @param isOutput 是否上抛
 @param channel av通道
 */
- (void)TK_setOutputAudioData:(BOOL)isOutput withChannel:(int)channel;


#pragma mark Video Methods

/**
 开始接收、解码并显示画面

 @param channel av通道
 @param obScreen 屏幕显示对象
 @param isSWDecode 是否软解
 */
- (void)TK_startShow:(NSInteger)channel ScreenObject:(NSObject*)obScreen isSWDecode:(BOOL)isSWDecode;


/**
 开始接收、解码并上抛YUV数据
 
 @param channel av通道
 @param isSWDecode 是否软解
 */
- (void)TK_startShowWithYUV:(NSInteger)channel isSWDecode:(BOOL)isSWDecode;


/**
 停止接收、解码和画面显示

 @param channel av通道
 */
- (void)TK_stopShow:(NSInteger)channel;


/**
 开始接收视频裸流数据

 @param channel av通道
 @param obScreen 屏幕显示对象
 */
- (void)TK_startRecvFrame:(NSInteger)channel ScreenObject:(NSObject*)obScreen;


/**
 停止接收视频裸流数据

 @param channel av通道
 */
- (void)TK_stopRecvFrame:(NSInteger)channel;


#pragma mark - Audio Methods

/**
 开始接收、解码并监听设备声音

 @param channel av通道
 */
- (void)TK_startSoundToPhone:(NSInteger)channel;


/**
 停止接收、解码和监听设备声音

 @param channel av通道
 */
- (void)TK_stopSoundToPhone:(NSInteger)channel;


/**
 开始对讲（发送声音到设备）

 @param channel av通道
 */
- (void)TK_startSoundToDevice:(NSInteger)channel isResend:(BOOL)isResend;


/**
 停止对讲

 @param channel av通道
 */
- (void)TK_stopSoundToDevice:(NSInteger)channel;


/**
 开始接收音频数据

 @param channel av通道
 */
- (void)TK_startRecvAudio:(NSInteger)channel;


/**
 停止接受音频数据

 @param channel av通道
 */
- (void)TK_stopRecvAudio:(NSInteger)channel;


#pragma mark - USingAEC Methods
/**
 播放回音消除后的音频数据
 
 @param aecData aec音频数据
 @param channel av通道
 */
- (void)TK_playAECAudio:(NSData *)aecData Channel:(NSInteger)channel;

/**
 发送回音消除后的音频数据
 
 @param aecAudioData aec音频数据
 @param length  aec音频数据长度
 @param codec 设备端音频编码格式
 @param channel av通道
 */
- (void)TK_sendAecAudio:(NSData *)aecAudioData Length:(NSInteger)length Codec:(NSInteger)codec Channel:(NSInteger)channel;

/** 开启对讲发送
 
  @param channel av通道
 */
- (void)TK_startSendAudioChannel:(NSInteger)channel;

/** 关闭对讲发送
 
 @param channel av通道
 */
- (void)TK_stopSendAudioChannel:(NSInteger)channel;


#pragma mark - SnapShot

/**
 获取截图图片
 
 @param channel av通道
 @return 截图图片
 */
- (UIImage *)TK_getSnapShotImageWithChannel:(NSInteger)channel;


#pragma mark - Video Record

/**
 本地录像功能初始化

 @param url 沙盒路径
 */
-(void)TK_initVideoGenerator:(NSURL*)url;


/**
 指定通道并开始录像

 @param channel av通道
 @param duration 最大录像时长
 */
-(void)TK_startRecordingForChannel:(NSInteger)channel withDuration:(NSTimeInterval)duration;


/**
 停止录像
 */
-(void)TK_stopRecording;


/**
 设置是否保存到系统相册
 
 @param isSaveToSystemAlbum 是否保存到系统相册 (isSaveToSystemAlbum defult is yes)
 */
-(void)TK_setIsSaveToSystemAlbum:(BOOL)isSaveToSystemAlbum;


/**
 填充指定通道的视频数据

 @param channel av通道
 @param imgData 视频buffer
 @param size 视频buffer长度
 @param codec_id 视频编码格式
 @param width 视频画面宽度
 @param height 视频画面长度
 @return 实际填充的数据长度
 */
-(unsigned int)TK_getChannel:(NSInteger)channel Snapshot:(char *)imgData DataSize:(unsigned int)size ImageType:(unsigned int*)codec_id WithImageWidth:(unsigned int *)width ImageHeight:(unsigned int *)height;


#pragma mark - Deprecated Methods
__attribute__((deprecated("deprecated")));
@property (nonatomic, assign)      NSInteger           tpnsInterval;

__attribute__((deprecated("deprecated")));
@property (nonatomic, assign)      BOOL                isUsingGaAudioUnit;

__attribute__((deprecated("deprecated")));
- (BOOL)isAVChannelStartShow:(int)channel;

__attribute__((deprecated("deprecated")));
+ (unsigned int)getIOTCamerVersion;

__attribute__((deprecated("deprecated")));
- (UIImage*)getThumbnail:(unsigned int)channel;

__attribute__((deprecated("deprecated")));
- (void)deleteThumbnail:(unsigned int)channel;

__attribute__((deprecated("deprecated")));
- (void)reStartShow:(NSInteger)channel withCompleteBlock:(void (^) (void))onCompleted;

__attribute__((deprecated("deprecated")));
- (BOOL)setHWDecodingAbility:(UInt32)nAvIdx requestHWDecode:(BOOL)bEnable;

__attribute__((deprecated("deprecated")));
- (id)initWithName:(NSString *)name;

__attribute__((deprecated("deprecated")));
- (NSString *)getViewAccountOfChannel:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (NSString *)getViewPasswordOfChannel:(NSInteger)channel;


__attribute__((deprecated("deprecated")));
+ (void)uninitIOTC;

__attribute__((deprecated("deprecated")));
+ (NSString *)getIOTCameraVersion;

__attribute__((deprecated("deprecated")));
+ (NSString *)getIOTCAPIsVerion;

__attribute__((deprecated("deprecated")));
+ (NSString *)getAVAPIsVersion;

__attribute__((deprecated("deprecated")));
+ (LanSearch_t *)LanSearch:(int *)num timeout:(int)timeoutVal;

__attribute__((deprecated("deprecated")));
- (void)connect:(NSString *)uid_;

__attribute__((deprecated("deprecated")));
- (void)disconnect;

__attribute__((deprecated("deprecated")));
- (void)start:(NSInteger)channel viewAccount:(NSString *)viewAccount viewPassword:(NSString *)viewPassword is_playback:(BOOL)bPlaybackMode;

__attribute__((deprecated("deprecated")));
- (void)stop:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)sendIOCtrlToChannel:(NSInteger)channel Type:(NSInteger)type Data:(char *)buff DataSize:(NSInteger)buffer_size;

__attribute__((deprecated("deprecated")));
- (unsigned int)getServiceTypeOfChannel:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (NSInteger)getConnectionStateOfChannel:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)startShow:(NSInteger)channel ScreenObject:(NSObject*)obScreen;
- (void)TK_setIsDecode:(BOOL)isDecode withChannel:(int)channel;//for DVR MultiView

__attribute__((deprecated("deprecated")));
- (void)stopShow:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)startRecvFrame:(NSInteger)channel ScreenObject:(NSObject*)obScreen;

__attribute__((deprecated("deprecated")));
- (void)stopRecvFrame:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)startSoundToPhone:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)stopSoundToPhone:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)startSoundToDevice:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)stopSoundToDevice:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)startRecvAudio:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)stopRecvAudio:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
-(void)initVideoGenerator:(NSURL*)url;

__attribute__((deprecated("deprecated")));
-(void)startRecordingForChannel:(NSInteger)channel withDuration:(NSTimeInterval)duration;

__attribute__((deprecated("deprecated")));
-(void)stopRecording;

__attribute__((deprecated("deprecated")));
-(unsigned int)getChannel:(NSInteger)channel Snapshot:(char *)imgData DataSize:(unsigned int)size ImageType:(unsigned int*)codec_id WithImageWidth:(unsigned int *)width ImageHeight:(unsigned int *)height;

@end

__attribute__((deprecated("deprecated")));
@protocol VideoRecorderDelegate <NSObject>
@end

#pragma mark - Camera Delegate

@protocol CameraDelegate <NSObject>
@optional

/**
 指定通道的视频宽高/fps/bps/在线人数/帧数/丢帧数等调试信息回调

 @param camera camera对象
 @param channel av通道
 @param videoWidth 视频宽
 @param videoHeight 视频高
 @param fps fps
 @param videoBps 视频bps
 @param audioBps 音频bps
 @param onlineNm 在线人数
 @param frameCount 帧数
 @param incompleteFrameCount 丢帧数
 @param isHwDecode 是否是硬解
 */
- (void)camera:(Camera *)camera didReceiveFrameInfoWithChannel:(NSInteger)channel videoWidth:(NSInteger)videoWidth VideoHeight:(NSInteger)videoHeight VideoFPS:(NSInteger)fps VideoBPS:(NSInteger)videoBps AudioBPS:(NSInteger)audioBps OnlineNm:(NSInteger)onlineNm FrameCount:(unsigned long)frameCount IncompleteFrameCount:(unsigned long)incompleteFrameCount isHwDecode:(BOOL)isHwDecode;


/**
 指定通道的连线状态回调

 @param camera camera对象
 @param channel av通道
 @param status 连线状态
 */
- (void)camera:(Camera *)camera didChangeChannelStatus:(NSInteger)channel ChannelStatus:(NSInteger)status;

/**
 指定通道指令接收回调
>>>>>>> feature/syncAPI

 @param camera camera对象
 @param type 指令类型
 @param data 指令数据
 @param size 指令数据长度
 @param channel av通道
 */
- (void)camera:(Camera *)camera didReceiveIOCtrlWithType:(NSInteger)type Data:(const char*)data DataSize:(NSInteger)size Channel:(NSInteger)channel;


/**
 指定通道硬解码后的数据回调

 @param camera camera对象
 @param sampleBuffer 解码后的数据
 @param timestamp 时间戳
 @param channel av通道
 */
- (void)camera:(Camera *)camera didUpdateDecodedSampleBuffer: (CMSampleBufferRef)sampleBuffer timestamp:(uint64_t)timestamp Channel:(NSInteger)channel;


/**
 上抛指定通道解码后的yuv数据格式为i420
 
 @param camera camera对象
 @param data      解码后的yuv数据
 @param outWidth  解码后的数据宽度
 @param outHeight 解码后的数据高度
 @param timestamp 时间戳
 @param channel av通道
 */
- (void)camera:(Camera *)camera outputDecodeVideoYUVData:(NSData *)data width:(int)outWidth height:(int)outHeight  timestamp:(uint64_t)timestamp channel:(NSInteger)channel;


/**
 对讲通道建立成功的回调

 @param camera camera对象
 @param isSuccess 是否建立成功
 @param errorCode 错误码
 */
- (void)camera:(Camera *)camera didStartTalkSuccess:(BOOL)isSuccess ErrorCode:(NSInteger) errorCode;


/**
 监听通道建立成功的回调

 @param camera camera对象
 @param isSuccess 是否建立成功
 @param channel av通道
 */
- (void)camera:(Camera *)camera didStartListenSuccess:(BOOL)isSuccess Channel:(NSInteger)channel;


/**
 连线失败错误码的回调

 @param camera camera对象
 @param connFailErrCode 连线失败的错误码
 */
- (void)camera:(Camera *)camera didconnFailErrCode:(NSInteger)connFailErrCode;


/**
 指定通道接收的视频帧时间戳

 @param camera camera对象
 @param frmTimestamp 视频帧时间戳
 @param channel av通道
 */
- (void)camera:(Camera *)camera didReceiveTimestamp:(unsigned int)frmTimestamp Channel:(NSInteger)channel;



/**
 获取FrameInfo帧信息

 @param camera camera对象
 @param frameData 帧信息(FRAMEINFO_t结构体转换的NSData *数据)
 @param channel av通道
 */
- (void)camera:(Camera *)camera didReceiveFrameDataInfo:(NSData *)frameData Channel:(NSInteger)channel;


/**
 指定通道接收的视频帧数据回调

 @param camera camera对象
 @param frameData 视频帧数据
 @param size 视频帧数据长度
 @param infoData 视频帧数据信息
 @param channel av通道
 */
- (void)camera:(Camera *)camera didReceiveFrameData:(const char *)frameData DataSize:(unsigned int)size FrmInfo:(NSData *)infoData Channel:(NSInteger)channel;


/**
 指定通道接收的音频帧数据回调

 @param camera camera对象
 @param data 音频帧数据
 @param size 音频帧数据长度
 @param infoData 音频帧数据信息
 @param channel av通道
 */
- (void)camera:(Camera *)camera didReceiveAudioData:(const char *)data DataSize:(unsigned int)size FrmInfo:(NSData *)infoData Channel:(NSInteger)channel;


/**
 视频帧编码格式回调

 @param camera camera对象
 @param pointer 编码格式指针
 @param channel av通道
 */
- (void)camera:(Camera *)camera didreportCodecId:(NSValue *)pointer Channel:(NSInteger)channel;


/**
 输出解码后的音频数据

 @param camera camera对象
 @param pcmData pcm音频数据
 @param channel av通道
 */
- (void)camera:(Camera *)camera didRecvAudioOutput:(NSData *)pcmData Channel:(int)channel;


/**
 输出Mic采集的音频数据

 @param camera camera对象
 @param audioData 音频数据
 @param length 音频数据长度
 @param codec 设备端音频编码格式
 @param channel av通道
 */
- (void)camera:(Camera *)camera didSendAudioOutput:(NSData *)audioData Length:(NSInteger)length Codec:(NSInteger)codec Channel:(NSInteger)channel;



__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveFrameInfoWithChannel:(NSInteger)channel videoWidth:(NSInteger)videoWidth VideoHeight:(NSInteger)videoHeight VideoFPS:(NSInteger)fps VideoBPS:(NSInteger)videoBps AudioBPS:(NSInteger)audioBps OnlineNm:(NSInteger)onlineNm FrameCount:(unsigned int)frameCount IncompleteFrameCount:(unsigned int)incompleteFrameCount;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveRawDataFrame:(const char *)imgData VideoWidth:(NSInteger)width VideoHeight:(NSInteger)height;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveImageData:(NSData *)imgData DataSize:(NSInteger)size EventName:(STimeDay)time Channel:(NSInteger)channel Recvflag:(NSInteger)flag;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveJPEGDataFrame:(const char *)imgData DataSize:(NSInteger)size;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveJPEGDataFrame2:(NSData *)imgData;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didChangeSessionStatus:(NSInteger)status;

__attribute__((deprecated("deprecated")));
 - (void)camera:(Camera *)camera didStartServerSuccess:(BOOL)isSuccess;

__attribute__((deprecated("deprecated")));
- (void)CameraUpdateDecodedH264SampleBuffer: (CMSampleBufferRef)sampleBuffer;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didStartListenSuccess:(BOOL)isSuccess;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didreportCodecId:(NSValue *)pointer;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didUpdateDecodedSampleBuffer: (CMSampleBufferRef)sampleBuffer Channel:(NSInteger)channel;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveAudioData:(const char *)data DataSize:(unsigned int)size FrmInfo:(NSData *)infoData;

__attribute__((deprecated("deprecated")));
- (void)camera:(Camera *)camera didReceiveIOCtrlWithType:(NSInteger)type Data:(const char*)data DataSize:(NSInteger)size;

@end



