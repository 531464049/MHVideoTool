
precision highp float;
varying   highp vec2  varyTextCoord;

uniform   highp vec4      ModelColor;
uniform   highp vec4      BgColor;
uniform   highp int       FragMode;
//模式FragMode = 1 (RGBA)
uniform   highp sampler2D ColorRGB;//默认编号为0的纹理
//模式FragMode = 2 (Y-UV)
uniform   highp sampler2D SamplerY;
uniform   highp sampler2D SamplerUV;
uniform   highp mat3      Matrix;
//模式FragMode = 3 (Y-U-V)
uniform   highp sampler2D SaY;
uniform   highp sampler2D SaU;
uniform   highp sampler2D SaV;
//模式FragMode = 4 (RGB)
uniform   highp sampler2D RGBd;

void main()
{
    //highp vec4  pixelColor = texture2D(colorMap, varyTextCoord);
    highp vec4 pixelColor;
    if(FragMode == 1){//CGImageRef->RGBA
        pixelColor = texture2D(ColorRGB, varyTextCoord);
    }else if(FragMode == 2){//CVPixelBufferRef-> Y-UV
        highp vec3 yuv;
        highp vec3 rgb;
        yuv.x  = (texture2D(SamplerY, varyTextCoord).r);//- (16.0/255.0));
        yuv.yz = (texture2D(SamplerUV, varyTextCoord).ra - vec2(0.5, 0.5));
        rgb = Matrix * yuv;

        pixelColor = vec4(rgb,1.0);
    }else if(FragMode == 3){//Y-U-V
        highp vec3 Yuv;
        highp vec3 Rgb;
        Yuv.x = texture2D(SaY,varyTextCoord).r;
        Yuv.y = texture2D(SaU,varyTextCoord).r-0.5;
        Yuv.z = texture2D(SaV,varyTextCoord).r-0.5;
        //Rgb = Matrix * Yuv;
        Rgb = mat3(    1.0,      1.0,     1.0,
                       0.0, -0.39465, 2.03211,
                   1.13983, -0.58080,     0.0) * Yuv;
        pixelColor = vec4(Rgb,1.0);
    }else if(FragMode == 4){//RGB
        highp vec3 Rgb;
        Rgb = texture2D(RGBd,varyTextCoord).rgb;
        pixelColor = vec4(Rgb,1.0);
    }else if(FragMode == 5){//RGBA
        pixelColor = texture2D(RGBd,varyTextCoord);
    }

    if(varyTextCoord.x > 1.0 || varyTextCoord.x < 0.0 || varyTextCoord.y > 1.0 || varyTextCoord.y < 0.0){
        pixelColor = ModelColor;
    }
    
    gl_FragColor = pixelColor;
}

//highp float x = varyTextCoord.x - 0.50;
//highp float y = varyTextCoord.y - 0.50;
//highp float r = sqrt(pow(x,2.0)+pow(y,2.0));
//if(r > 0.499){
//    //pixelColor = BgColor;
//    pixelColor = vec4(1.0, 0.0, 0.0, 1.0);
//}



