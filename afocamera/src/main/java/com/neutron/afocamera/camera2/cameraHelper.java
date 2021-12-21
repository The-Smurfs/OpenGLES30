package com.neutron.afocamera.camera2;

import static com.neutron.afocamera.utils.cameraUtils.*;

import static java.util.List.*;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.util.Size;
import android.view.Surface;
import android.view.SurfaceView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.core.app.ActivityCompat;

import com.neutron.afocamera.MyGLRender;
import com.neutron.afocamera.MyGLSurfaceView;
import com.neutron.afocamera.MySurfaceListener;

import java.util.List;

public class cameraHelper {
    private static final String TAG = cameraHelper.class.getSimpleName();
    public static final String BACK_CAMERA = "0";
    public static final String FRONT_CAMERA = "1";

    Context mContext;
    private MyGLSurfaceView mSurfaceView;
    private MyGLRender mRender;

    private SurfaceTexture mSurfaceTexture;
    private Size mPreviewSize;

    private CameraManager mCameraManager;
    private CameraDevice mCameraDevice;
    private String mCameraId = FRONT_CAMERA;

    private CaptureRequest.Builder mPreviewBuilder;
    private CameraCaptureSession mPreviewSession;

    private HandlerThread mCameraThread;
    private Handler mCameraHandler;

    private CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice cameraDevice) {
            mCameraDevice = cameraDevice;
            startPreview();
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice cameraDevice) {
            cameraDevice.close();
            mCameraDevice = null;
        }

        @Override
        public void onError(@NonNull CameraDevice cameraDevice, int error) {
            cameraDevice.close();
            mCameraDevice = null;
            String msg = getErrorMessage(error);
            Toast.makeText(mContext, msg, Toast.LENGTH_SHORT).show();
        }
    };

    public cameraHelper(Context context, SurfaceView view) {
        mContext = context;
        mSurfaceView = (MyGLSurfaceView) view;
        init();
    }

    private void init() {
        mCameraThread = new HandlerThread("CameraBackground");
        mCameraThread.start();
        mCameraHandler = new Handler(mCameraThread.getLooper());

        mCameraManager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);

        mSurfaceView.setEGLContextClientVersion(3);
        mSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        mRender = new MyGLRender(new MySurfaceListener() {
            @Override
            public void onSurfaceCreated(SurfaceTexture surfaceTexture) {
                mSurfaceTexture = surfaceTexture;
                openCamera();
            }

            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                //mSurfaceView.requestRender();
            }
        });
        mSurfaceView.setRenderer(mRender);
        mSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    public void openCamera() {
        releaseCamera();
        try {
            CameraCharacteristics characteristics = mCameraManager.getCameraCharacteristics(mCameraId);
            StreamConfigurationMap map = characteristics
                    .get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);

            mPreviewSize = chooseOptimalSize(map.getOutputSizes(SurfaceTexture.class), mSurfaceView.getWidth(), mSurfaceView.getHeight(), 1.0);
            //mSurfaceView.setAspectRatio(mPreviewSize.getHeight(), mPreviewSize.getWidth());
            if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                return;
            }
            mCameraManager.openCamera(mCameraId, mStateCallback, mCameraHandler);
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(mContext, "Cannot access the camera.", Toast.LENGTH_SHORT).show();
        }
    }

    private void releaseCamera() {
        if ( null != mCameraDevice ) {
            mCameraDevice.close();
            mCameraDevice = null;
        }
    }

    private void startPreview() {
        if ( null == mCameraDevice ) {
            return;
        }
        try {
            mSurfaceTexture.setDefaultBufferSize(mPreviewSize.getWidth(), mPreviewSize.getHeight());
            mPreviewBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            mPreviewBuilder.addTarget(new Surface(mSurfaceTexture));

            mCameraDevice.createCaptureSession(List.of(new Surface(mSurfaceTexture)),
                    new CameraCaptureSession.StateCallback() {

                        @Override
                        public void onConfigured(@NonNull CameraCaptureSession session) {
                            mPreviewSession = session;
                            mPreviewBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
                            try {
                                mPreviewSession.setRepeatingRequest(mPreviewBuilder.build(), null, mCameraHandler);
                            } catch (CameraAccessException e) {
                                e.printStackTrace();
                            }
                        }

                        @Override
                        public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                            if (null != mContext) {
                                Toast.makeText(mContext, "Failed", Toast.LENGTH_SHORT).show();
                            }
                        }
                    }, mCameraHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    /**
     * 切换相机
     * @return 新的相机ID
     */
    public void switchCamera() {
        if (mCameraId.equals(BACK_CAMERA)) {
            mCameraId = FRONT_CAMERA;
        } else {
            mCameraId = BACK_CAMERA;
        }

        openCamera();
    }
}
