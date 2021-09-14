
#include "MyGLRenderContext.h"
#include "TriangleSample.h"
#include <android/asset_manager.h>

MyGLRenderContext* MyGLRenderContext::mContext = nullptr;

MyGLRenderContext::MyGLRenderContext() : mAAssetManager(NULL) {
	//m_Sample = new TriangleSample;
}

MyGLRenderContext::~MyGLRenderContext() {
}

void MyGLRenderContext::OnSurfaceCreated() {
	glClearColor(0.0f,0.0f,0.0f, 1.0f);
    mSample->Init();
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
	glViewport(0, 0, width, height);
	mScreenW = width;
	mScreenH = height;
}

void MyGLRenderContext::OnDrawFrame() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mSample->Draw(mScreenW, mScreenH);
}

void MyGLRenderContext::SetAssetManager(AAssetManager *nativeasset) {
    mAAssetManager = nativeasset;
}

AAssetManager *MyGLRenderContext::GetAssetManager() {
    return mAAssetManager;
}

MyGLRenderContext *MyGLRenderContext::GetInstance() {
	if (mContext == nullptr) {
		mContext = new MyGLRenderContext();
	}
	return mContext;
}

void MyGLRenderContext::DestroyInstance() {
	if (mContext) {
		delete mContext;
		mContext = nullptr;
	}

}

void MyGLRenderContext::SetImageData(int format, int width, int height, uint8_t *pData)
{
	LOGCATE("MyGLRenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format, width, height, pData);
	NativeImage nativeImage;
	nativeImage.format = format;
	nativeImage.width = width;
	nativeImage.height = height;
	nativeImage.ppPlane[0] = pData;

	switch (format)
	{
		case IMAGE_FORMAT_NV12:
		case IMAGE_FORMAT_NV21:
			nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
			break;
		case IMAGE_FORMAT_I420:
			nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
			nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
			break;
		default:
			break;
	}

	if (mSample)
	{
		mSample->LoadImage(&nativeImage);
	}

}



