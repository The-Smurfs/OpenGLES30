package com.neutron.afocamera;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

public class MyGLSurfaceView extends GLSurfaceView {
    private static final String TAG = MyGLSurfaceView.class.getSimpleName();

    private int mRatioWidth = 0;
    private int mRatioHeight = 0;

    public MyGLSurfaceView(Context context) {
        super(context, null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void setAspectRatio(int width, int height) {
        if (width < 0 || height < 0) {
            throw new IllegalArgumentException("Size cannot be negative.");
        }
        mRatioWidth = width;
        mRatioHeight = height;
        getHolder().setFixedSize(width, height);
        Log.d(TAG, "AspectRatio set: " + width + "/" + height);
        requestLayout();
    }
}
