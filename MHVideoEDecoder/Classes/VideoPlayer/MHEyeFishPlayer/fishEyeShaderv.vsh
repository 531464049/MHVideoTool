

precision highp float;
attribute highp vec4  position;
attribute highp vec2  textCoordinate;

uniform   highp int   ScaleAspectFit;
uniform   int         Mode;    //模型模式
uniform   highp vec2  ImgSize; //贴图(宽,高)
uniform   highp vec3  Circular;//校准圆心,半径：(x,y,r)
uniform   highp vec3  CoorAxes;//坐标变换(x,y,z)

uniform   highp float Rotate;  //贴图旋转角度
uniform   highp float hDegrees;//水平转动角度//Level
uniform   highp float vDegrees;//垂直转动角度//Vertical
uniform   highp float Focus;   //焦距
uniform   highp float Zoomxy;  //模型X、Y放大缩小倍数
uniform   highp float Zoomz;   //模型Z放大缩小倍数
uniform   highp float WScale;  //贴图x轴放大缩小倍数
uniform   highp float HScale;  //贴图y轴放大缩小倍数

varying   highp vec2  varyTextCoord;

#define M_PI  3.14159265358979323846264338327950288//acos(-1.0)
#define CIRCULAR_T 1.0//贴图半径最大值
#define CIRCULAR_R 1.0//模型半径最大值

#define CIRCULAR_LZ 1.7 //焦点距离
#define CIRCULAR_LD 1.05//景深幅度
void main( )
{
    //绕Z轴旋转
    highp mat4 RadiansZ = mat4( cos(Rotate), -sin(Rotate), 0.0, 0.0,
                          sin(Rotate),  cos(Rotate), 0.0, 0.0,
                          0.0,           0.0,          1.0, 0.0,
                          0.0,           0.0,          0.0, 1.0);
    //绕X轴旋转
    highp mat4 RadiansX = mat4( 1.0,          0.0,           0.0, 0.0,
                          0.0,  cos(vDegrees),  -sin(vDegrees), 0.0,
                          0.0,  sin(vDegrees),   cos(vDegrees), 0.0,
                          0.0,          0.0,           0.0, 1.0);
    //绕X轴旋转
    highp mat4 RadiansEX = mat4( 1.0,          0.0,           0.0, 0.0,
                               0.0,  cos(CoorAxes.y),  -sin(CoorAxes.y), 0.0,
                               0.0,  sin(CoorAxes.y),   cos(CoorAxes.y), 0.0,
                               0.0,          0.0,           0.0, 1.0);
    //绕Y轴旋转
    highp mat4 RadiansEY = mat4(cos(CoorAxes.x), 0.0, -sin(CoorAxes.x), 0.0,
                              0.0,         1.0,          0.0, 0.0,
                              sin(CoorAxes.x), 0.0,  cos(CoorAxes.x), 0.0,
                              0.0,         0.0,          0.0, 1.0);
    
    highp mat4 RadiansZoom = mat4(Zoomxy, 0.0, 0.0, 0.0,
                            0.0, Zoomxy, 0.0, 0.0,
                            0.0, 0.0, Zoomz, 0.0,
                            0.0, 0.0, 0.0, 1.0);
    
    highp mat4 WScaleMat = mat4(WScale, 0.0, 0.0, 0.0,
                          0.0, HScale, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0);
    
    vec2 st = textCoordinate;
    highp vec4 Position;
    highp float lsin = sin(Rotate);
    highp float lcos = cos(Rotate);
    
    if((Mode == 1)||(Mode == 2)){//鱼眼elgEyemodeEye，壁挂elgEyemodeWall
        highp vec4 pt = position;
        
        highp float u = (textCoordinate.x-0.5)*2.0;
        highp float v = (textCoordinate.y-0.5)*2.0;
        //旋转贴图
        highp float sx = u*lcos+v*lsin;
        highp float sy = -u*lsin+v*lcos;
        //渲染坐标[-1,1] -> 贴图坐标[0,1]
        st.x = (sx + CIRCULAR_T) / (CIRCULAR_T*2.0);
        st.y = (CIRCULAR_T - sy) / (CIRCULAR_T*2.0);
        //-----------------------
        highp float ly = 1.0*sin(acos(pt.x/1.000));
        highp float agx = atan(pt.z/pt.y);
        if(pt.y < sin(0.0)){
            agx += acos(-1.0);
        }else if(pt.y == sin(0.0)){
            agx = acos(0.0);
        }
        
        pt.z  = ly*sin(agx-vDegrees);
        pt.y  = ly*cos(agx-vDegrees);//pt.y
        pt.x  = pt.x;
        //-----------------------
        highp float lx = 1.0*sin(acos(pt.y/1.000));
        highp float agy = atan(pt.z/pt.x);
        if(pt.x < sin(0.0)){
            agy += acos(-1.0);
        }else if(pt.x == sin(0.0)){
            agy = acos(0.0);
        }
        
        pt.z  = lx*sin(agy-hDegrees);
        pt.y  = pt.y;
        pt.x  = lx*cos(agy-hDegrees);//pt.x;
        
        highp float focus = Focus;
        if(focus < 0.0){
            focus = 0.0;
        }else if(focus > 1.0){
            focus = 1.0;
        }
        highp float lz = CIRCULAR_LZ * focus;
        pt.x = (lz+1.0)/((lz+pt.z)/pt.x) * (CIRCULAR_LZ/(CIRCULAR_LZ+1.0));
        pt.y = (lz+1.0)/((lz+pt.z)/pt.y) * (CIRCULAR_LZ/(CIRCULAR_LZ+1.0));
        pt.z += CIRCULAR_LD*focus-CIRCULAR_LD+CoorAxes.z;

        highp float xyScale = (1.0-focus)*1.5+1.0;//x、y放大倍数：1.5
        highp mat4 XYScaleMat = mat4(xyScale, 0.0, 0.0, 0.0,
                                    0.0, xyScale, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    0.0, 0.0, 0.0, 1.0);
    
        Position = pt * WScaleMat * RadiansEX * RadiansEY*XYScaleMat;
        
        //Position = pt * WScaleMat * RadiansEX * RadiansEY;//*XYScaleMat;
    }else if((Mode == 4) || (Mode == 5) || (Mode == 6)){//180S,180D,360
        highp float x = textCoordinate.x * (CIRCULAR_T*2.0) - CIRCULAR_T;
        highp float y = CIRCULAR_T - textCoordinate.y * (CIRCULAR_T*2.0);
        highp float sx   = x*lcos+y*lsin;
        highp float sy   = -x*lsin+y*lcos;
        st.x = (sx + CIRCULAR_T) / (CIRCULAR_T*2.0);
        st.y = (CIRCULAR_T - sy) / (CIRCULAR_T*2.0);
        Position   = position * RadiansX * RadiansZoom * WScaleMat;
    }else if(Mode == 3){//圆筒
        highp vec4 pt = position;
        pt.x *= 0.5;
        pt.y *= 0.5;
        Position   = RadiansZ * pt * RadiansX * RadiansZoom * WScaleMat;
    }else if(Mode == 7){//壁挂展开
        highp float focus = Focus;
        if(focus < 0.0){
            focus = 0.0;
        }else if(focus > 1.0){
            focus = 1.0;
        }
        
        Position   = RadiansZ * position * RadiansX * RadiansZoom * WScaleMat;
    }else{//方形
        Position   = RadiansZ * position * RadiansX * RadiansZoom * WScaleMat;
    }
    
    gl_Position = Position;

    
    highp float stScale = min(ImgSize.x/ImgSize.y,ImgSize.y/ImgSize.x);

    if(ImgSize.x > ImgSize.y){
        st.y = st.y/stScale+(1.0-1.0/stScale)/2.0;
    }else{
        st.x = st.x/stScale+(1.0-1.0/stScale)/2.0;
    }
    
    st.x = st.x*Circular.z+(1.0-Circular.z)/2.0;
    st.y = st.y*Circular.z+(1.0-Circular.z)/2.0;
    
    st.x += Circular.x/ImgSize.x;
    st.y += Circular.y/ImgSize.y;
    
    if((Mode == 0) && (ScaleAspectFit == 0)){
        varyTextCoord = textCoordinate;
    }else{
        varyTextCoord = st;
    }
}


/*
 highp vec4 pt = position;
 highp float u = (textCoordinate.x-0.5)*2.0;
 highp float v = (textCoordinate.y-0.5)*2.0;
 //旋转贴图
 highp float sx = u*lcos+v*lsin;
 highp float sy = -u*lsin+v*lcos;
 //映射坐标 -> 贴图坐标
 st.x = (sx + CIRCULAR_T) / (CIRCULAR_T*2.0);
 st.y = (CIRCULAR_T - sy) / (CIRCULAR_T*2.0);
 
 highp float ly = 1.0*sin(acos(pt.x/1.000));
 highp float agx = atan(pt.z/pt.y);
 if(pt.y < sin(0.0)){
 agx += acos(-1.0);
 }else if(pt.y == sin(0.0)){
 agx = acos(0.0);
 }
 
 highp float z  = ly*sin(agx-vDegrees);
 highp float y  = ly*cos(agx-vDegrees);//pt.y
 highp float x  = pt.x;//lx*cos(agy-vDegrees);//pt.x;
 
 pt.x = x;
 pt.y = y;
 pt.z = z;
 
 highp float Scale = ZoomAg;
 if(Scale < 0.0){
 Scale = 0.0;
 }else if(Scale > 1.0){
 Scale = 1.0;
 }
 highp float lz = CIRCULAR_LZ * Scale;
 pt.x = (lz+1.0)/((lz+pt.z)/pt.x) * (CIRCULAR_LZ/(CIRCULAR_LZ+1.0));
 pt.y = (lz+1.0)/((lz+pt.z)/pt.y) * (CIRCULAR_LZ/(CIRCULAR_LZ+1.0));
 pt.z += CIRCULAR_LD*Scale-CIRCULAR_LD+Depth;
 
 highp float xyScale = (1.0-Scale)*1.5+1.0;//x、y放大倍数：1.5
 highp mat4 XYScaleMat = mat4(xyScale, 0.0, 0.0, 0.0,
 0.0, xyScale, 0.0, 0.0,
 0.0, 0.0, 1.0, 0.0,
 0.0, 0.0, 0.0, 1.0);
 Position = pt * WScaleMat * RadiansEX * RadiansEY*XYScaleMat;
 */
