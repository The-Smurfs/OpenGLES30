#include "TriangleSample.h"
#include "../util/GLUtils.h"
#include "../util/LogUtil.h"

TriangleSample::TriangleSample()
{

}

TriangleSample::~TriangleSample()
{
}


//void TriangleSample::LoadImage(NativeImage *pImage)
//{
	//null implement

//}

void TriangleSample::Init()
{
	if(mProgramObj != 0)
		return;
	char vShaderStr[] =
			"#version 300 es                          \n"
			"layout(location = 0) in vec4 vPosition;  \n"
			"void main()                              \n"
			"{                                        \n"
			"   gl_Position = vPosition;              \n"
			"}                                        \n";

	char fShaderStr[] =
			"#version 300 es                              \n"
			"precision mediump float;                     \n"
			"out vec4 fragColor;                          \n"
			"void main()                                  \n"
			"{                                            \n"
			"   fragColor = vec4 ( 1.0, 0.5, 0.2, 1.0 );  \n"
			"}                                            \n";

	mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, mVertexShader, mFragmentShader);

	GLfloat vVertices[] = {
			0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
	// Load the vertex data
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0 );
	glEnableVertexAttribArray (0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleSample::Draw(int screenW, int screenH)
{
	LOGCATE("TriangleSample::Draw");


	if(mProgramObj == 0)
		return;

	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the program object
	glUseProgram (mProgramObj);
	glBindVertexArray(VAO);
	glDrawArrays (GL_TRIANGLES, 0, 3);
	glUseProgram (GL_NONE);

}

void TriangleSample::Destroy()
{
	if (mProgramObj)
	{
		glDeleteProgram(mProgramObj);
		mProgramObj = GL_NONE;
	}

}
