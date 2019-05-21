//
//  MHBufferTools.m
//  MHVideoEDecoder
//
//  Created by mahao on 2019/5/20.
//  Copyright © 2019 mh. All rights reserved.
//

#import "MHBufferTools.h"

#define clamp(a) (a>255?255:(a<0?0:a))

@implementation MHBufferTools

+(NSData *)charToData:(char *)ar
{
    NSData *data = [NSData dataWithBytes:ar length:strlen(ar)];
    return data;
}
+(char *)dataToChar:(NSData *)data
{
    char * a= (char *)[data bytes];
    return a;
}
#pragma mark - CVImageBufferRef 转 YUV
+(void)imageBuffer:(CVImageBufferRef)pixelBuffer toYuvData:(void (^)(NSData *, int, int))callBack
{
    size_t w = CVPixelBufferGetWidth(pixelBuffer);
    size_t h  = CVPixelBufferGetHeight(pixelBuffer);
    unsigned char* buffer = (unsigned char*) malloc(w * h * 1.5);
    
    {
        CVPixelBufferLockBaseAddress(pixelBuffer, 0);
        if (CVPixelBufferIsPlanar(pixelBuffer)) {
            size_t d = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
            unsigned char* src = (unsigned char*) CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
            unsigned char* dst = buffer;
            for (unsigned int rIdx = 0; rIdx < h; ++rIdx, dst += w, src += d) {
                memcpy(dst, src, w);
            }
            d = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);
            src = (unsigned char *) CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
            h = h >> 1;
            for (unsigned int rIdx = 0; rIdx < h; ++rIdx, dst += w, src += d) {
                memcpy(dst, src, w);
            }
        }
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    }
    NSData *yuvData = [NSData dataWithBytes:buffer length:sizeof(unsigned char)*(w*h*1.5)];
    free(buffer);
    buffer = nil;
    callBack(yuvData,(int)w,(int)h);
    /*
     // 获取yuv数据
     //表示开始操作数据
     CVPixelBufferLockBaseAddress(pixelBuffer, 0);
     
     //图像宽度（像素）
     size_t pixelWidth = CVPixelBufferGetWidth(pixelBuffer);
     //图像高度（像素）
     size_t pixelHeight = CVPixelBufferGetHeight(pixelBuffer);
     //yuv中的y所占字节数
     size_t y_size = pixelWidth * pixelHeight;
     //yuv中的uv所占的字节数
     size_t uv_size = y_size / 2;
     
     uint8_t *yuv_frame = malloc(uv_size + y_size);
     
     //获取CVImageBufferRef中的y数据
     uint8_t *y_frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
     memcpy(yuv_frame, y_frame, y_size);
     
     //获取CMVImageBufferRef中的uv数据
     uint8_t *uv_frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
     memcpy(yuv_frame + y_size, uv_frame, uv_size);
     
     CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
     
     //返回数据
     NSData * yuvData = [NSData dataWithBytesNoCopy:yuv_frame length:y_size + uv_size];
     //NSLog(@"%lu",(unsigned long)yuvData.length);
     callBack(yuvData,(int)pixelWidth,(int)pixelHeight);
     */
}
#pragma mark - 裁剪YUV数据
+(void)cutxYUV420:(char *)srcBuffer srcW:(int)srcW srcH:(int)srcH desBuffer:(char *)desBuffer cutX:(int)x cutY:(int)y cutW:(int)desW cutH:(int)desH
{
    int tmpRange;
    int bufferIndex;
    
    int yIndex = 0;
    bufferIndex = 0 + x + y * srcW;
    tmpRange = srcW * desH;
    for (int i = 0; i < tmpRange;) {
        memcpy(desBuffer + yIndex, srcBuffer + bufferIndex + i, desW);
        i += srcW;
        yIndex += desW;
    }
    
    int uIndex = desW * desH;
    int uIndexStep = srcW / 2;
    int uWidthCopy = desW / 2;
    bufferIndex = srcW * srcH + x / 2 + y / 2 * srcW / 2;
    tmpRange = srcW * desH / 4;
    for (int i = 0; i < tmpRange;) {
        memcpy(desBuffer + uIndex, srcBuffer + bufferIndex + i, uWidthCopy);
        i += uIndexStep;
        uIndex += uWidthCopy;
    }
    
    int vIndex = desW * desH + desW * desH / 4;
    int vIndexStep = srcW / 2;
    int vWidthCopy = desW / 2;
    bufferIndex = srcW * srcH + srcW * srcH / 4 + x / 2 + y / 2 * srcW / 2;
    tmpRange = srcW * desH / 4;
    for (int i = 0; i < tmpRange;) {
        memcpy(desBuffer + vIndex, srcBuffer + bufferIndex + i, vWidthCopy);
        i += vIndexStep;
        vIndex += vWidthCopy;
    }
}
#pragma mark - CVImageBufferRef转换为data数据
+(NSData *)imageBufferToData:(CVImageBufferRef)imageBuffer
{
    CVPixelBufferLockBaseAddress(imageBuffer,0);
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    //    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    void *src_buff = CVPixelBufferGetBaseAddress(imageBuffer);
    NSData *data = [NSData dataWithBytes:src_buff length:bytesPerRow * height];
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    return data;
}
#pragma mark - CVImageBufferRef转换为CGImageRef
+(CGImageRef)imageRefFromimageBuffer:(CVImageBufferRef)imageBuffer
{
    CVPixelBufferLockBaseAddress(imageBuffer,0);
    
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    uint8_t *yBuffer = CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
    size_t yPitch = CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0);
    uint8_t *cbCrBuffer = CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 1);
    size_t cbCrPitch = CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 1);
    
    int bytesPerPixel = 4;
    uint8_t *rgbBuffer = malloc(width * height * bytesPerPixel);
    
    for(int y = 0; y < height; y++) {
        uint8_t *rgbBufferLine = &rgbBuffer[y * width * bytesPerPixel];
        uint8_t *yBufferLine = &yBuffer[y * yPitch];
        uint8_t *cbCrBufferLine = &cbCrBuffer[(y >> 1) * cbCrPitch];
        
        for(int x = 0; x < width; x++) {
            int16_t y = yBufferLine[x];
            int16_t cb = cbCrBufferLine[x & ~1] - 128;
            int16_t cr = cbCrBufferLine[x | 1] - 128;
            
            uint8_t *rgbOutput = &rgbBufferLine[x*bytesPerPixel];
            
            int16_t r = (int16_t)roundf( y + cr *  1.4 );
            int16_t g = (int16_t)roundf( y + cb * -0.343 + cr * -0.711 );
            int16_t b = (int16_t)roundf( y + cb *  1.765);
            
            rgbOutput[0] = 0xff;
            rgbOutput[1] = clamp(b);
            rgbOutput[2] = clamp(g);
            rgbOutput[3] = clamp(r);
        }
    }
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(rgbBuffer, width, height, 8, width * bytesPerPixel, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaNoneSkipLast);
    CGImageRef quartzImage = CGBitmapContextCreateImage(context);
    
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    free(rgbBuffer);
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    return quartzImage;
}
#pragma mark - CVImageBufferRef转换为UIImage
+(UIImage *)imageFromimageBuffer:(CVImageBufferRef)imageBuffer
{
    CGImageRef cgImage = [self imageRefFromimageBuffer:imageBuffer];
    UIImage *image = [UIImage imageWithCGImage:cgImage];
    CGImageRelease(cgImage);
    return image;
}

@end
