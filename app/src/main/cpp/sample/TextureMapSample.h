//
// Created by fanwe on 2021/3/17.
//

#ifndef OPENGLES_NDK_TEXTUREMAPSAMPLE_H
#define OPENGLES_NDK_TEXTUREMAPSAMPLE_H


#include "GLSampleBase.h"

class TextureMapSample : public GLSampleBase {
public:
    TextureMapSample();
    ~TextureMapSample();

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void LoadImage(NativeImage *pImage);

private:
    GLuint mTextureId;
    GLint mSamplerLoc;
    NativeImage mRenderImage;
    GLint mSamplerLoc2;
    GLuint mTextureId2;
    NativeImage mRenderImage2;
};


#endif //OPENGLES_NDK_TEXTUREMAPSAMPLE_H
