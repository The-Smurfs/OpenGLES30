package com.neutron.afocamera;

import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import com.neutron.afocamera.utils.ResReadUtils;
import com.neutron.afocamera.utils.ShaderUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyGLRender implements GLSurfaceView.Renderer {

    private final MySurfaceListener mSurfaceListener;

    private float[] VERTEX = new float[]{
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
    };

    private final float[] TEX_VERTEX = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };

    private final short[] VERTEX_INDEX = {
            0, 1, 2,
            0, 2, 3,
    };

    private float[] transformMatrix = new float[16];

    private int curFragmentShader;

    private FloatBuffer mVertexBuffer, mTexVertexBuffer;

    private ShortBuffer mVertexIndexBuffer = null;

    private int mProgram;

    private int textureId;
    private SurfaceTexture mSurfaceTexture;

    private int uTextureMatrixLocation;

    private int uTextureSamplerLocation;

    private int uTextureFilterColorLocation;

    //四个值对应RGBA的系数，以达到滤镜效果
    private float[] FILTER_COLOR = new float[]{1f, 1f, 1f, 1f};

    public MyGLRender(MySurfaceListener mySurfaceListener) {
        mSurfaceListener = mySurfaceListener;

        mVertexBuffer = ByteBuffer.allocateDirect(VERTEX.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(VERTEX);
        mVertexBuffer.position(0);

        mTexVertexBuffer = ByteBuffer.allocateDirect(TEX_VERTEX.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(TEX_VERTEX);
        mTexVertexBuffer.position(0);

        mVertexIndexBuffer = ByteBuffer.allocateDirect(VERTEX_INDEX.length * 2)
                .order(ByteOrder.nativeOrder())
                .asShortBuffer()
                .put(VERTEX_INDEX);
        mVertexIndexBuffer.position(0);

    }

    /**
     * 加载外部纹理
     *
     * @return
     */
    public int loadTexture() {
        int[] tex = new int[1];
        //创建一个纹理
        GLES30.glGenTextures(1, tex, 0);
        //绑定到外部纹理上
        GLES30.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, tex[0]);
        //设置纹理过滤参数
        GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_NEAREST);
        GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR);
        GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);
        GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
        //解除纹理绑定
        GLES30.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, 0);
        return tex[0];
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        //设置背景颜色
        GLES30.glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
        //编译
        curFragmentShader = ShaderUtils.compileVertexShader(ResReadUtils.readResource(R.raw.vertex_camera_shader));
        final int fragmentShaderId = ShaderUtils.compileFragmentShader(ResReadUtils.readResource(R.raw.fragment_camera_shader));
        //链接程序片段
        mProgram = ShaderUtils.linkProgram(curFragmentShader, fragmentShaderId);

        //获取着色程序中定义的变量位置
        uTextureMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uTextureMatrix");
        uTextureSamplerLocation = GLES30.glGetUniformLocation(mProgram, "yuvTexSampler");
        uTextureFilterColorLocation = GLES30.glGetUniformLocation(mProgram, "vFilterColor");

        //加载纹理
        textureId = loadTexture();

        //加载SurfaceTexture
        loadSurfaceTexture(textureId);

        mSurfaceListener.onSurfaceCreated(mSurfaceTexture);
    }

    public void loadSurfaceTexture(int textureId) {
        //根据纹理ID创建SurfaceTexture
        mSurfaceTexture = new SurfaceTexture(textureId);
        mSurfaceTexture.setOnFrameAvailableListener(mSurfaceListener);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        GLES30.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);

        //使用程序片段
        GLES30.glUseProgram(mProgram);

        //更新纹理图像
        mSurfaceTexture.updateTexImage();
        mSurfaceTexture.getTransformMatrix(transformMatrix);

        //激活纹理单元0
        GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
        //绑定外部纹理到纹理单元0
        GLES30.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId);
        //将此纹理单元床位片段着色器的uTextureSampler外部纹理采样器
        GLES30.glUniform1i(uTextureSamplerLocation, 0);

        //将滤镜颜色传给片段着色器
        GLES30.glUniform4f(uTextureFilterColorLocation, FILTER_COLOR[0], FILTER_COLOR[1], FILTER_COLOR[2], FILTER_COLOR[3]);
        //将纹理矩阵传给片段着色器
        GLES30.glUniformMatrix4fv(uTextureMatrixLocation, 1, false, transformMatrix, 0);

        GLES30.glEnableVertexAttribArray(0);
        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 0, mVertexBuffer);

        GLES30.glEnableVertexAttribArray(1);
        GLES30.glVertexAttribPointer(1, 2, GLES30.GL_FLOAT, false, 0, mTexVertexBuffer);

        // 绘制
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, VERTEX_INDEX.length, GLES20.GL_UNSIGNED_SHORT, mVertexIndexBuffer);
    }
}
