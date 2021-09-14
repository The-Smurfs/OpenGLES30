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
    if(mProgramObj != 0)
        return;
    GLfloat verticesCoords[] = {
            -1.0f,  0.5f, 0.0f,  0.0f,  0.0f,// Position 0
            -1.0f, -0.5f, 0.0f,  0.0f,  1.0f,// Position 1
            1.0f, -0.5f, 0.0f,  1.0f,  1.0f,// Position 2
            1.0f,  0.5f, 0.0f,  1.0f,  0.0f // Position 3
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // 生成纹理
    glGenBuffers(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec3 vPosition;  \n"
            "layout(location = 1) in vec2 a_texCoord; \n"
            "out vec2 v_texCoord;                     \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vec4(vPosition, 1.0);              \n"
            "   v_texCoord = a_texCoord;                \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "in vec2 v_texCoord;                          \n"
            "out vec4 fragColor;                          \n"
            "uniform sampler2D s_TextureMap;              \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = texture(s_TextureMap, v_texCoord); \n"
            "}                                            \n";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, mVertexShader, mFragmentShader);

    if (mProgramObj)
    {
        mSamplerLoc = glGetUniformLocation(mProgramObj, "s_TextureMap");
    }
    else
    {
        LOGCATE("TextureMapSample::Init create program fail");
    }
}

void TextureMapSample::Draw(int screenW, int screenH) {
    LOGCATE("TextureMapSample::Draw()");

    if(mProgramObj == GL_NONE || mTextureId == GL_NONE) {
        return;
    }

    // Use the program object
    glUseProgram (mProgramObj);
    // Set the RGBA map sampler to texture unit to 0
    glUniform1i(mSamplerLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glBindVertexArray(VAO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TextureMapSample::Destroy() {
    if (mProgramObj)
    {
        glDeleteProgram(mProgramObj);
        glDeleteTextures(1, &mTextureId);
        mProgramObj = GL_NONE;
    }
}

void TextureMapSample::LoadImage(NativeImage *pImage) {
    LOGCATE("TextureMapSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage)
    {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}