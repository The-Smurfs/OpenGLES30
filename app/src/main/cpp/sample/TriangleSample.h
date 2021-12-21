//
// Created by William on 2021/3/16
//

#ifndef OPENGLES_NDK_TRIANGLESAMPLE_H
#define OPENGLES_NDK_TRIANGLESAMPLE_H


#include "GLSampleBase.h"

class TriangleSample : public GLSampleBase
{
public:
	TriangleSample();
	virtual ~TriangleSample();

	virtual void Init();

	virtual void Draw(int screenW, int screenH);

	virtual void Destroy();

	int64_t getCurrentLocalTimeStamp();

private:
	int mTimeColorLocation;
};


#endif //OPENGLES_NDK_TRIANGLESAMPLE_H
