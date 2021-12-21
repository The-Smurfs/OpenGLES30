package com.neutron.afocamera;

import android.graphics.SurfaceTexture;

/**
 * 监听MyRender的Surface创建情况
 */
public interface MySurfaceListener extends SurfaceTexture.OnFrameAvailableListener {


    /**
     * 将SurfaceTexture传给监听者，便于接收相机数据
     *
     * @param surfaceTexture MyRender绑定的SurfaceTexture
     */
    void onSurfaceCreated(SurfaceTexture surfaceTexture);
}
