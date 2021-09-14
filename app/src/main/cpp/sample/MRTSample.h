//
// Created by fanwe on 2021/3/24.
//

#ifndef OPENGLES_NDK_MRTSAMPLE_H
#define OPENGLES_NDK_MRTSAMPLE_H
#include "GLSampleBase.h"
#define ATTACHMENT_NUM 4
class MRTSample : public GLSampleBase{
public:
    MRTSample();
    ~MRTSample();

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void LoadImage(NativeImage *pImage);
private:
    bool createFrameBufferObject();
private:
    GLuint mImageTextureId;
    GLuint mFboTextureId[4];
    GLuint mFboId;
    GLint mSamplerLoc;
    NativeImage mRenderImage;

    GLuint mFboVertexShader;
    GLuint mFboFragmentShader;
    GLuint mFboProgramObj;
    GLint mFboSamplerLoc;

    GLuint mVaoIds[2];
    GLuint mVboIds[4];
    const GLenum attachments[ATTACHMENT_NUM] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3
    };
};


#endif //OPENGLES_NDK_MRTSAMPLE_H
