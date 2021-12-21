package com.neutron.afocamera;

import android.content.Intent;
import android.graphics.SurfaceTexture;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.navigation.fragment.NavHostFragment;

import com.neutron.afocamera.camera2.cameraHelper;
import com.neutron.afocamera.databinding.FragmentCameraBinding;


public class CameraFragment extends Fragment {

    private FragmentCameraBinding binding;

    private cameraHelper mCameraHelper;

    private static Uri sPhotoUri;

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState
    ) {
        binding = FragmentCameraBinding.inflate(inflater, container, false);

        mCameraHelper = new cameraHelper(this.getContext(), binding.glSurfaceView);
        return binding.getRoot();
    }

    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        binding.toRecordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                NavHostFragment.findNavController(CameraFragment.this)
                        .navigate(R.id.action_CameraFragment_to_RecordFragment);
            }
        });

        binding.cameraSwitchButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mCameraHelper.switchCamera();
            }
        });

        binding.photoViewButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (sPhotoUri != null) {
                    Intent intent = new Intent(Intent.ACTION_VIEW);
                    intent.setDataAndType(sPhotoUri, requireContext().getContentResolver().getType(sPhotoUri));
                    intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                    startActivity(intent);
                }
            }
        });
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

}