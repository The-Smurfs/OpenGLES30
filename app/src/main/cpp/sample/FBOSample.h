//
// Created by fanwe on 2021/3/23.
//

#ifndef OPENGLES_NDK_FBOSAMPLE_H
#define OPENGLES_NDK_FBOSAMPLE_H
#include "GLSampleBase.h"

class FBOSample : public GLSampleBase{
public:
    FBOSample();
    ~FBOSample();

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void LoadImage(NativeImage *pImage);
private:
    bool createFrameBufferObject();
private:
    GLuint mImageTextureId;
    GLuint mFboTextureId;
    GLuint mFboId;
    GLint mSamplerLoc;
    NativeImage mRenderImage;

    GLuint mFboVertexShader;
    GLuint mFboFragmentShader;
    GLuint mFboProgramObj;
    GLint mFboSamplerLoc;

    GLuint mVaoIds[2];
    GLuint mVboIds[4];
};


#endif //OPENGLES_NDK_FBOSAMPLE_H
