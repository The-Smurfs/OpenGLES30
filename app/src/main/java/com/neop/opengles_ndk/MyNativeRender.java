package com.neop.opengles_ndk;

import android.content.res.AssetManager;

public class MyNativeRender {
    static {
        System.loadLibrary("native-lib");
    }

    // JNI方法
    public native void native_SetAssetManager(AssetManager assetManager);

    public native void native_OnSurfaceCreated();

    public native void native_OnSurfaceChanged(int width, int height);

    public native void native_OnDrawFrame();

    public native void native_SetImageData(int format, int width, int height, byte[] bytes);
}
