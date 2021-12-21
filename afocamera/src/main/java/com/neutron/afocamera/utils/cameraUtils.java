package com.neutron.afocamera.utils;

import static android.hardware.camera2.CameraDevice.StateCallback.ERROR_CAMERA_DEVICE;
import static android.hardware.camera2.CameraDevice.StateCallback.ERROR_CAMERA_DISABLED;
import static android.hardware.camera2.CameraDevice.StateCallback.ERROR_CAMERA_IN_USE;
import static android.hardware.camera2.CameraDevice.StateCallback.ERROR_CAMERA_SERVICE;
import static android.hardware.camera2.CameraDevice.StateCallback.ERROR_MAX_CAMERAS_IN_USE;

import android.util.Log;
import android.util.Size;

import java.util.ArrayList;
import java.util.List;

public class cameraUtils {
    private static final String TAG = cameraUtils.class.getSimpleName();

    /**
     * 在系统支持的Size中选出比例最接近ratio的，
     * 此处还限制了size的短边必须>=预览surface的短边。
     * 返回符合条件的面积最小的一个，
     * 找不到就返回数组第一个。
     * @param choices 系统支持的Size数组
     * @param width 预览Surface的width
     * @param height 预览Surface的height
     * @param ratio 比例
     * @return Size
     */
    public static Size chooseOptimalSize(Size[] choices, int width, int height, double ratio) {
        List<Size> bigEnough = new ArrayList<>();
        for (Size size : choices) {
            if (Math.abs(size.getWidth() / (double) size.getHeight() - ratio) < 0.1f && size.getHeight() >= Math.min(width, height)) {
                bigEnough.add(size);
            }
        }

        if (bigEnough.size() > 0) {
            long minArea = Long.MAX_VALUE;
            Size result = bigEnough.get(0);
            for (Size size : bigEnough) {
                long tempArea = size.getWidth() * size.getHeight();
                if (tempArea < minArea) {
                    result = size;
                    minArea = tempArea;
                }
            }
            return result;
        } else {
            Log.e(TAG, "Couldn't find any suitable preview size");
            return choices[0];
        }
    }

    /**
     * 返回Camera错误信息
     * @param errorCode 错误码
     * @return Camera错误信息
     */
    public static String getErrorMessage(int errorCode){
        String msg;
        switch (errorCode){
            case ERROR_CAMERA_DEVICE:
                msg="Fatal (device)";
                break;
            case ERROR_CAMERA_DISABLED:
                msg="Device policy";
                break;
            case ERROR_CAMERA_IN_USE:
                msg="Camera in use";
                break;
            case ERROR_CAMERA_SERVICE:
                msg="Fatal (service)";
                break;
            case ERROR_MAX_CAMERAS_IN_USE:
                msg="Maximum cameras in use";
                break;
            default:
                msg="Unknown";
        }
        return msg;
    }
}
