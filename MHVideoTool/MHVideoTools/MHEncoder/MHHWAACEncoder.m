//
//  MHHWAACEncoder.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/21.
//  Copyright © 2019 mh. All rights reserved.
//

#import "MHHWAACEncoder.h"

@implementation MHHWAACEncoderParam

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.bitrate = 100000;
        self.channelCount = 1;
        self.sampleRate = 44100;
    }
    return self;
}

@end

@interface MHHWAACEncoder ()

@property (nonatomic,assign) AudioConverterRef aConverter;

@end

@implementation MHHWAACEncoder

-(instancetype)initWithParam:(MHHWAACEncoderParam *)param
{
    self = [super init];
    if (self) {
        if (!param) {
            param = [[MHHWAACEncoderParam alloc] init];
        }
        _param= param;
        [self buildEncoder];
    }
    return self;
}
-(void)buildEncoder
{
    //1.输入流
    AudioStreamBasicDescription inputFormat = {0};
    inputFormat.mSampleRate = self.param.sampleRate;//采样率
    inputFormat.mFormatID = kAudioFormatLinearPCM;//PCM采样
    inputFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked;
    inputFormat.mChannelsPerFrame = (UInt32)self.param.channelCount;//每帧声道数
    inputFormat.mFramesPerPacket = 1;//每包帧数
    inputFormat.mBitsPerChannel = 16;//每声道位数
    inputFormat.mBytesPerFrame = inputFormat.mBitsPerChannel / 8 * inputFormat.mChannelsPerFrame;//每帧的字节数
    inputFormat.mBytesPerPacket = inputFormat.mBytesPerFrame * inputFormat.mFramesPerPacket;//每包字节数
    
    //2.输出流
    AudioStreamBasicDescription outputFormat;
    //初始清零
    memset(&outputFormat, 0, sizeof(outputFormat));
    //音频流，在正常播放情况下的帧率。如果是压缩的格式，这个属性表示解压缩后的帧率。帧率不能为0。
    outputFormat.mSampleRate = inputFormat.mSampleRate;
    //AAC编码 kAudioFormatMPEG4AAC kAudioFormatMPEG4AAC_HE_V2
    outputFormat.mFormatID = kAudioFormatMPEG4AAC;
    //无损编码，0则无
    outputFormat.mFormatFlags = kMPEG4Object_AAC_LC;
    //每一个packet的音频数据大小。如果的动态大小设置为0。动态大小的格式需要用AudioStreamPacketDescription来确定每个packet的大小。
    outputFormat.mBytesPerPacket = 0;
    //每帧的声道数
    outputFormat.mChannelsPerFrame = (UInt32)self.param.channelCount;
    //每个packet的帧数。如果是未压缩的音频数据，值是1。动态帧率格式，
    //这个值是一个较大的固定数字，比如说AAC的1024。如果是动态大小帧数（比如Ogg格式）设置为0。
    outputFormat.mFramesPerPacket = 1024;
    //每帧的bytes数，每帧的大小。每一帧的起始点到下一帧的起始点。如果是压缩格式，设置为0 。
    outputFormat.mBytesPerFrame = 0;
    //语音每采样点占用位数 压缩格式设置为0
    outputFormat.mBitsPerChannel = 0;
    //字节对齐，填0.
    outputFormat.mReserved = 0;
    
    //3.编码器参数
    AudioClassDescription requestedCodecs[2] = {
        {kAudioEncoderComponentType, kAudioFormatMPEG4AAC, kAppleSoftwareAudioCodecManufacturer},
        {kAudioEncoderComponentType, kAudioFormatMPEG4AAC, kAppleHardwareAudioCodecManufacturer}
    };
    //不知道为什么真机测试时这里总是会卡一个断点.....
    //4.编码器
    OSStatus result = AudioConverterNewSpecific(&inputFormat, &outputFormat, 2, requestedCodecs, &_aConverter);
    if (result == noErr) {
        NSLog(@"硬编码AAC创建成功");
    }else{
        NSLog(@"硬编码AAC创建失败");
        _aConverter = nil;
    }
    
    //设置码率
    uint32_t aBitrate = (uint32_t)self.param.bitrate;
    uint32_t aBitrateSize = sizeof(aBitrate);
    result = AudioConverterSetProperty(_aConverter, kAudioConverterEncodeBitRate, aBitrateSize, &aBitrate);
    if (result != noErr) {
        NSLog(@"AAC编码-码率设置失败");
    }
}
-(void)audioEncodeInpuBuffer:(CMSampleBufferRef)sampleBuffer
{
    if (!_aConverter) {
        return;
    }
    //设置输入
    AudioBufferList inAaudioBufferList;
    CMBlockBufferRef blockBuffer;
    CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer(sampleBuffer, NULL, &inAaudioBufferList, sizeof(inAaudioBufferList), NULL, NULL, 0, &blockBuffer);
    
    //设置输出
    uint32_t bufferSize = inAaudioBufferList.mBuffers[0].mDataByteSize;
    uint8_t *buffer = (uint8_t *)malloc(bufferSize);
    memset(buffer, 0, bufferSize);
    AudioBufferList outAudioBufferList;
    outAudioBufferList.mNumberBuffers = 1;
    outAudioBufferList.mBuffers[0].mNumberChannels = inAaudioBufferList.mBuffers[0].mNumberChannels;
    outAudioBufferList.mBuffers[0].mDataByteSize = bufferSize;
    outAudioBufferList.mBuffers[0].mData = buffer;
    
    UInt32 ioOutputDataPacketSize = 1;

    NSData *data = nil;
    NSError *error = nil;
    OSStatus status = AudioConverterFillComplexBuffer(_aConverter,
                                                      inputDataProc,
                                                      &inAaudioBufferList,
                                                      &ioOutputDataPacketSize,
                                                      &outAudioBufferList, NULL);
    if (status == 0){
        NSData *rawAAC = [NSData dataWithBytes:outAudioBufferList.mBuffers[0].mData length:outAudioBufferList.mBuffers[0].mDataByteSize];
        NSData *adtsHeader = [self getADTSDataWithPacketLength:rawAAC.length];
        NSMutableData *fullData = [NSMutableData dataWithData:adtsHeader];
        [fullData appendData:rawAAC];
        data = fullData;
    }else{
        error = [NSError errorWithDomain:NSOSStatusErrorDomain code:status userInfo:nil];
        NSLog(@"音频编码失败---%@",error);
    }

    if (data && self.delegate && [self.delegate respondsToSelector:@selector(mhHWAACEncoderOutputData:)]) {
        [self.delegate mhHWAACEncoderOutputData:data];
    }
    free(buffer);
    CFRelease(blockBuffer);
}
#pragma mark -- AudioCallBack
OSStatus inputDataProc(AudioConverterRef inConverter, UInt32 *ioNumberDataPackets, AudioBufferList *ioData,AudioStreamPacketDescription **outDataPacketDescription, void *inUserData) {
    //填充PCM到缓冲区
    AudioBufferList bufferList = *(AudioBufferList*)inUserData;
    ioData->mBuffers[0].mNumberChannels = 1;
    ioData->mBuffers[0].mData           = bufferList.mBuffers[0].mData;
    ioData->mBuffers[0].mDataByteSize   = bufferList.mBuffers[0].mDataByteSize;
    ioData->mNumberBuffers              = 1;
    return noErr;
}
/**
 *  Add ADTS header at the beginning of each and every AAC packet.
 *  This is needed as MediaCodec encoder generates a packet of raw
 *  AAC data.
 *
 *  Note the packetLen must count in the ADTS header itself.
 *  See: http://wiki.multimedia.cx/index.php?title=ADTS
 *  Also: http://wiki.multimedia.cx/index.php?title=MPEG-4_Audio#Channel_Configurations
 **/
- (NSData *)getADTSDataWithPacketLength:(NSInteger)packetLength {
    
    int adtsLength = 7;
    char *packet = malloc(sizeof(char) * adtsLength);
    // Variables Recycled by addADTStoPacket
    int profile = 2;  //AAC LC
    //39=MediaCodecInfo.CodecProfileLevel.AACObjectELD;
    int freqIdx = 4;  //44.1KHz
    int chanCfg = 1;  //MPEG-4 Audio Channel Configuration. 1 Channel front-center
    NSUInteger fullLength = adtsLength + packetLength;
    // fill in ADTS data
    packet[0] = (char)0xFF; // 11111111     = syncword
    packet[1] = (char)0xF9; // 1111 1 00 1  = syncword MPEG-2 Layer CRC
    packet[2] = (char)(((profile-1)<<6) + (freqIdx<<2) +(chanCfg>>2));
    packet[3] = (char)(((chanCfg&3)<<6) + (fullLength>>11));
    packet[4] = (char)((fullLength&0x7FF) >> 3);
    packet[5] = (char)(((fullLength&7)<<5) + 0x1F);
    packet[6] = (char)0xFC;
    NSData *data = [NSData dataWithBytesNoCopy:packet length:adtsLength freeWhenDone:YES];
    return data;
}
- (void)dealloc {
    AudioConverterDispose(_aConverter);
}
@end
