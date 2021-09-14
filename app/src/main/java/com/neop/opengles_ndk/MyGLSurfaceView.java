package com.neop.opengles_ndk;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class MyGLSurfaceView extends GLSurfaceView {
    private final static String TAG = "MyGLSurfaceView";
    private MyGLRender mRender;
    private MyNativeRender mNativeRender;

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    public MyGLSurfaceView(Context context) {
        this(context, null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        // 设置OpenGL ES的版本
        this.setEGLContextClientVersion(3);//3.0版本
        // 设置渲染器
        mNativeRender = new MyNativeRender();// 创建native render对象
        mNativeRender.native_SetAssetManager(context.getAssets());
        getRGBAImage();

        mRender = new MyGLRender(mNativeRender);
        this.setRenderer(mRender);
        this.setRenderMode(RENDERMODE_CONTINUOUSLY);// 设置渲染模式为连续渲染模式
    }

    public void getRGBAImage() {
        @SuppressLint("ResourceType") InputStream is = this.getResources().openRawResource(R.drawable.huge);

        Bitmap bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int bytes = bitmap.getByteCount();
                ByteBuffer buf = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                byte[] byteArray = buf.array();
                mNativeRender.native_SetImageData(IMAGE_FORMAT_RGBA, bitmap.getWidth(), bitmap.getHeight(), byteArray);
            }
        }
        finally
        {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }
    }
}
