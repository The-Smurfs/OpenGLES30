//
// Created by fanwe on 2021/3/23.
//

#include "FBOSample.h"
#include <GLUtils.h>

FBOSample::FBOSample() {

}

FBOSample::~FBOSample() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
}

void FBOSample::Init() {
    // 顶点坐标
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
    };

    // 纹理坐标
    GLfloat texcoords[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
    };

    // 纹理坐标
    GLfloat fboTexcoords[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
    };

    // 索引
    GLushort indices[] = {
            0,1,2,
            0,2,3,
    };

    char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec2 a_texCoord;   \n"
            "out vec2 v_texCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   v_texCoord = a_texCoord;                \n"
            "}                                          \n";

    // 用于普通渲染的片段着色器脚本，简单纹理映射
    char fShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec2 v_texCoord;\n"
            "layout(location = 0) out vec4 outColor;\n"
            "uniform sampler2D s_TextureMap;\n"
            "void main()\n"
            "{\n"
            "    outColor = texture(s_TextureMap, v_texCoord);\n"
            "}";

    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFboShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec2 v_texCoord;\n"
            "layout(location = 0) out vec4 outColor;\n"
            "uniform sampler2D s_TextureMap;\n"
            "void main()\n"
            "{\n"
            "    vec4 tempColor = texture(s_TextureMap, v_texCoord);\n"
            "    float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
            "    outColor = vec4(vec3(luminance), tempColor.a);\n"
            "}"; // 输出灰度图

    // 编译链接用于普通渲染的着色器程序
    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, mVertexShader, mFragmentShader);

    // 编译链接用于离屏渲染的着色器程序
    mFboProgramObj = GLUtils::CreateProgram(vShaderStr, fFboShaderStr, mFboVertexShader, mFboFragmentShader);

    if (mProgramObj == GL_NONE || mFboProgramObj == GL_NONE)
    {
        LOGCATE("FBOSample::Init m_ProgramObj == GL_NONE");
        return;
    }
    mSamplerLoc = glGetUniformLocation(mProgramObj, "s_TextureMap");
    mFboSamplerLoc = glGetUniformLocation(mFboProgramObj, "s_TextureMap");

    // 生成 VBO ，加载顶点数据和索引数据
    // Generate VBO Ids and load the VBOs with data
    glGenBuffers(4, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fboTexcoords), fboTexcoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GO_CHECK_GL_ERROR();

    // 生成 2 个 VAO，一个用于普通渲染，另一个用于离屏渲染
    // Generate VAO Ids
    glGenVertexArrays(2, mVaoIds);
    // 初始化用于普通渲染的 VAO
    // Normal rendering VAO
    glBindVertexArray(mVaoIds[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);


    // 初始化用于离屏渲染的 VAO
    // FBO off screen rendering VAO
    glBindVertexArray(mVaoIds[1]);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[2]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);

    // 创建并初始化图像纹理
    glGenTextures(1, &mImageTextureId);
    glBindTexture(GL_TEXTURE_2D, mImageTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    GO_CHECK_GL_ERROR();

    if (!createFrameBufferObject())
    {
        LOGCATE("FBOSample::Init CreateFrameBufferObj fail");
        return;
    }
}

void FBOSample::Draw(int screenW, int screenH) {
    // 离屏渲染
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glViewport(0, 0, mRenderImage.width, mRenderImage.height);

    // Do FBO off screen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glUseProgram(mFboProgramObj);
    glBindVertexArray(mVaoIds[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTextureId);
    glUniform1i(mFboSamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 普通渲染
    // Do normal rendering
    glViewport(0, 0, screenW, screenH);
    glUseProgram(mProgramObj);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(mVaoIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFboTextureId);
    glUniform1i(mSamplerLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);
    GO_CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void FBOSample::Destroy() {

}

void FBOSample::LoadImage(NativeImage *pImage) {
    LOGCATE("TextureMapSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage)
    {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

bool FBOSample::createFrameBufferObject() {
    // 创建并初始化 FBO 纹理
    glGenTextures(1, &mFboTextureId);
    glBindTexture(GL_TEXTURE_2D, mFboTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // 创建并初始化 FBO
    glGenFramebuffers(1, &mFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glBindTexture(GL_TEXTURE_2D, mFboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}