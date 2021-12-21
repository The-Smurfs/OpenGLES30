//
// Created by william on 2021/3/3.
//

#ifndef NDK_OPENGLES_MYGLRENDERCONTEXT_H
#define NDK_OPENGLES_MYGLRENDERCONTEXT_H

#include "stdint.h"
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>
#include <mutex>
#include <GLSampleBase.h>
#include <android/asset_manager.h>
#include <MRTSample.h>
#include "TriangleSample.h"
#include "TextureMapSample.h"
#include "FBOSample.h"

class MyGLRenderContext
{
	MyGLRenderContext();

	~MyGLRenderContext();

public:
    void SetAssetManager(AAssetManager *nativeasset);

    AAssetManager *GetAssetManager();

	void OnSurfaceCreated();

	void OnSurfaceChanged(int width, int height);

	void OnDrawFrame();

	void SetImageData(int format, int width, int height, uint8_t *pData);

	void setExampleType(int type);

	static MyGLRenderContext* GetInstance();
	static void DestroyInstance();

private:
	static MyGLRenderContext *mContext;
	AAssetManager* mAAssetManager;
	int mScreenW;
	int mScreenH;
    GLSampleBase* m_pBeforeSample;
    GLSampleBase* m_pCurSample;

    std::mutex mLock;
};


#endif //NDK_OPENGLES_MYGLRENDERCONTEXT_H
