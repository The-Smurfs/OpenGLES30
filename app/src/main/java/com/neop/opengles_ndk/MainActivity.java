package com.neop.opengles_ndk;

import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_FBO;
import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_KEY_MRT;
import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_TEXTURE_MAP;
import static com.neop.opengles_ndk.MyNativeRender.SAMPLE_TYPE_TRIANGLE;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.Menu;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.PopupMenu;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private MyGLSurfaceView mGLSurfaceView;
    private ImageButton mSwitchButton;

    private static final String[] SAMPLE_TITLES = {
            "DrawTriangle",
            "TextureMap",
            "YUV Rendering",
            "VAO&VBO",
            "FBO Offscreen Rendering",
            "EGL Background Rendering",
            "FBO Stretching",
            "Coordinate System",
            "Basic Lighting",
            "Transform Feedback",
            "Complex Lighting",
            "Depth Testing",
            "Instancing",
            "Stencil Testing",
            "Blending",
            "Particles",
            "SkyBox",
            "Assimp Load 3D Model",
            "PBO",
            "Beating Heart",
            "Cloud",
            "Time Tunnel",
            "Bezier Curve",
            "Big Eyes",
            "Face Slender",
            "Big Head",
            "Rotary Head",
            "Visualize Audio",
            "Scratch Card",
            "3D Avatar",
            "Shock Wave",
            "MRT",
            "FBO Blit",
            "Texture Buffer",
            "Uniform Buffer"
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //setContentView(mGLSurfaceView);
        mGLSurfaceView = (MyGLSurfaceView) this.findViewById(R.id.gl_surface_view);
        mSwitchButton = this.findViewById(R.id.switch_button);
        mSwitchButton.setOnClickListener(v -> {
            PopupMenu popupMenu = new PopupMenu(this, v);
            // 获取布局文件
            popupMenu.getMenuInflater().inflate(R.menu.examples, popupMenu.getMenu());
            popupMenu.show();
            popupMenu.setOnMenuItemClickListener(menuItem -> {
                switch (menuItem.getItemId()) {
                    case R.id.DrawTriangle:
                        mGLSurfaceView.setRenderType(SAMPLE_TYPE_TRIANGLE);
                        setTitle("三角形");
                        break;
                    case R.id.TextureMix:
                        mGLSurfaceView.setRenderType(SAMPLE_TYPE_TEXTURE_MAP);
                        setTitle("纹理混合");
                        break;
                    case R.id.FBO:
                        mGLSurfaceView.setRenderType(SAMPLE_TYPE_FBO);
                        setTitle("FBO");
                        break;
                    case R.id.MRT:
                        mGLSurfaceView.setRenderType(SAMPLE_TYPE_KEY_MRT);
                        setTitle("MRT");
                        break;
                    default:
                        break;
                }
                return true;
            });
        });
        setTitle("纹理混合");
    }
}