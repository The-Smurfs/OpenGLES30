package com.neop.opengles_ndk;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class  MyGLRender implements GLSurfaceView.Renderer {
    private final static String TAG = MyGLRender.class.getSimpleName();
    private MyNativeRender mNativeRender;

    MyGLRender(MyNativeRender nativeRender) {
        mNativeRender = nativeRender;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        if(mNativeRender != null) {
            mNativeRender.native_OnSurfaceCreated();
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        if(mNativeRender != null) {
            mNativeRender.native_OnSurfaceChanged(width, height);
        }
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        if(mNativeRender != null) {
            mNativeRender.native_OnDrawFrame();
        }
    }
}
