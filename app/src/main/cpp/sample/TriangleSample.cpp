#include "TriangleSample.h"
#include "../util/GLUtils.h"
#include "../util/LogUtil.h"

#include <time.h>

TriangleSample::TriangleSample()
{

}

TriangleSample::~TriangleSample()
{
}

void TriangleSample::Init()
{
	Destroy();

	char vShaderStr[] =
			"#version 300 es                          \n"
			"layout(location = 0) in vec3 vPosition;  \n"
			"layout(location = 1) in vec3 vColor;     \n"
            "out vec4 vertexColor;                    \n"
			"void main()                              \n"
			"{                                        \n"
			"  gl_Position = vec4(vPosition.xyz,1.0f);\n"
            "  vertexColor = vec4(vColor.xyz, 1.0f);  \n"
			"}                                        \n";

	char fShaderStr[] =
			"#version 300 es                              \n"
			"precision mediump float;                     \n"
			"in vec4 vertexColor;                         \n"
			"out vec4 fragColor;                          \n"
            "uniform vec4 timeColor;                      \n"
			"void main()                                  \n"
			"{                                            \n"
			"   fragColor = vertexColor;                  \n"
			"}                                            \n";

	mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, mVertexShader, mFragmentShader);

	mTimeColorLocation = glGetUniformLocation(mProgramObj, "timeColor");

	GLfloat vVertices[] = {
			 // position           // color
			 0.0f,   0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int index[] = {
			0,1,2
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
	// Load the vertex data
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)0 );
	glEnableVertexAttribArray (0);

	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)(3*sizeof (GL_FLOAT)));
	glEnableVertexAttribArray (1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glBindVertexArray(0);

	mInitialized = true;
}

void TriangleSample::Draw(int screenW, int screenH)
{
	if (!mInitialized) {
		Init();
	}
	LOGCATE("TriangleSample::Draw");

	if(mProgramObj == 0) {
		return;
	}

	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the program object
	glUseProgram (mProgramObj);
	glUniform4f(mTimeColorLocation, 1.0f, 0.8f, 0.0f, 1.0f);
	glBindVertexArray(VAO);
	//glDrawArrays (GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glUseProgram (GL_NONE);
	glBindVertexArray(0);
}

void TriangleSample::Destroy()
{
	if (mProgramObj)
	{
		glDeleteProgram(mProgramObj);
		mProgramObj = GL_NONE;
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

// get current local time stamp
int64_t TriangleSample::getCurrentLocalTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	return tmp.count();

	// return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
