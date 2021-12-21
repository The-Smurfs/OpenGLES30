#version 300 es
//OpenGL ES3.0外部纹理扩展
#extension GL_OES_EGL_image_external_essl3 : require
precision mediump float;
uniform samplerExternalOES yuvTexSampler;
uniform vec4 vFilterColor;
in vec2 yuvTexCoords;
out vec4 vFragColor;
void main() {
    vec4 vCameraColor = texture(yuvTexSampler, yuvTexCoords);
    float fGrayColor = (0.3*vCameraColor.r + 0.40*vCameraColor.g + 0.3*vCameraColor.b);
    float tryColor=(0.3*vCameraColor.r+0.3*vCameraColor.g+0.3*vCameraColor.b);
    //        vFragColor = vec4(fGrayColor, fGrayColor, fGrayColor, 1.0); 黑白滤镜，r=g=b
    vFragColor=vCameraColor;//原色
    //vFragColor=vec4(vFilterColor.x*vCameraColor.r, vFilterColor.y*vCameraColor.g, vFilterColor.z*vCameraColor.b, vFilterColor.y);//随便调的滤镜
}

