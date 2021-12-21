//
// Created by fanwe on 2021/3/17.
//

#include <GLUtils.h>
#include "TextureMapSample.h"

TextureMapSample::TextureMapSample() {

}

TextureMapSample::~TextureMapSample() {

}

void TextureMapSample::Init() {
    Destroy();

    GLfloat verticesCoords[] = {
            -1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,// Position 0
            -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  1.0f,// Position 1
            1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f,// Position 2
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f // Position 3
    };

    GLint indices[] = {
            0, 1, 2,
            0, 2, 3
    };
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(6*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);//在VAO解绑前可解绑也可以不解绑

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//在VAO解绑前一定不要解绑索引
    glBindVertexArray(0);

    // 生成纹理
    glGenBuffers(1, &mTextureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glActiveTexture(GL_TEXTURE1);
    glGenBuffers(1, &mTextureId2);
    glBindTexture(GL_TEXTURE_2D, mTextureId2);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage2.width, mRenderImage2.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage2.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec3 vPosition;  \n"
            "layout(location = 1) in vec3 vColor;     \n"
            "layout(location = 2) in vec2 a_texCoord; \n"
            "out vec4 v_texColor;                     \n"
            "out vec2 v_texCoord;                     \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vec4(vPosition, 1.0);   \n"
            "   v_texColor = vec4(vColor, 0.3f);      \n"
            "   v_texCoord = a_texCoord;              \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "in vec4 v_texColor;                          \n"
            "in vec2 v_texCoord;                          \n"
            "out vec4 fragColor;                          \n"
            "uniform sampler2D s_TextureMap;              \n"
            "uniform sampler2D s_TextureMulti;            \n"
            "void main()                                  \n"
            "{                                            \n"
            "   //fragColor = texture(s_TextureMap, v_texCoord)*v_texColor; \n"
            "   fragColor = mix(texture(s_TextureMap, v_texCoord), texture(s_TextureMulti, v_texCoord), 0.2); \n"
            "}                                            \n";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, mVertexShader, mFragmentShader);

    if (mProgramObj)
    {
        mSamplerLoc = glGetUniformLocation(mProgramObj, "s_TextureMap");
        mSamplerLoc2 = glGetUniformLocation(mProgramObj, "s_TextureMulti");
    }
    else
    {
        LOGCATE("TextureMapSample::Init create program fail");
    }

    mInitialized = true;
}

void TextureMapSample::Draw(int screenW, int screenH) {
    if (!mInitialized) {
        Init();
    }
    LOGCATE("TextureMapSample::Draw()");

    if(mProgramObj == GL_NONE || mTextureId == GL_NONE) {
        return;
    }

    // Use the program object
    glUseProgram (mProgramObj);
    // Set the RGBA map sampler to texture unit to 0
    glUniform1i(mSamplerLoc, 0);
    glUniform1i(mSamplerLoc2, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextureId2);

    glBindVertexArray(VAO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TextureMapSample::Destroy() {
    if (mProgramObj)
    {
        glDeleteProgram(mProgramObj);
        mProgramObj = GL_NONE;
    }
    if (mTextureId) {
        glDeleteTextures(1, &mTextureId);
        mTextureId = GL_NONE;
    }

    if (mTextureId2) {
        glDeleteTextures(1, &mTextureId2);
        mTextureId2 = GL_NONE;
    }

    if (VAO != GL_NONE) {
        glDeleteVertexArrays(1, &VAO);
        VAO = GL_NONE;
    }

    if (VBO != GL_NONE) {
        glDeleteBuffers(1, &VBO);
        VBO = GL_NONE;
    }

    if (EBO != GL_NONE) {
        glDeleteBuffers(1, &EBO);
        EBO = GL_NONE;
    }
}

void TextureMapSample::LoadImage(NativeImage *pImage) {
    LOGCATE("TextureMapSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage)
    {
        if (mRenderImage.ppPlane[0] == NULL) {
            mRenderImage.width = pImage->width;
            mRenderImage.height = pImage->height;
            mRenderImage.format = pImage->format;
            NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
        } else {
            mRenderImage2.width = pImage->width;
            mRenderImage2.height = pImage->height;
            mRenderImage2.format = pImage->format;
            NativeImageUtil::CopyNativeImage(pImage, &mRenderImage2);
        }

    }
    mInitialized = false;
}