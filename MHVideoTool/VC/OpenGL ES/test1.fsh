
precision highp float;
varying   highp vec2  varyTextCoord;//纹理坐标
uniform   highp sampler2D ColorRGB;//默认编号为0的纹理

void main()
{
    highp vec4 pixelColor = texture2D(ColorRGB,varyTextCoord);
    gl_FragColor = pixelColor;
}
