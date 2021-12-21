package com.neop.opengles_ndk;

import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_FBO;
import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_KEY_MRT;
import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_TEXTURE_MAP;

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
        // 3.0版本
        this.setEGLContextClientVersion(3);

        // 创建native render对象
        mNativeRender = new MyNativeRender();
        mNativeRender.native_SetAssetManager(context.getAssets());

        // native层加载图像
        getRGBAImage(R.drawable.lye);
        getRGBAImage(R.drawable.dzzz);

        // 设置渲染器
        mRender = new MyGLRender(mNativeRender);
        this.setRenderer(mRender);

        // 设置渲染模式为连续渲染模式
        this.setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    public void getRGBAImage(int id) {
        @SuppressLint("ResourceType") InputStream is = this.getResources().openRawResource(id);

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

    public void setRenderType(int type) {
        mNativeRender.native_SetExampleType(type);

        switch (type) {
            case SAMPLE_TYPE_TEXTURE_MAP:
                // native层加载图像
                getRGBAImage(R.drawable.lye);
                getRGBAImage(R.drawable.dzzz);
                break;
            case SAMPLE_TYPE_FBO:
                getRGBAImage(R.drawable.dzzz);
                break;
            case SAMPLE_TYPE_KEY_MRT:
                getRGBAImage(R.drawable.huge);
                break;
            default:
                break;
        }
    }
}
