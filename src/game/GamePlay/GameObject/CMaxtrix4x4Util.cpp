#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
#include "CMaxtrix4x4Util.h"

void CMatrix4x4Util::Init(PineTransformParam param)
{
	reNew();
	_m0 = param.m11;
	_m1 = param.m12;
	_m4 = param.m21;
	_m5 = param.m22;
	_m12 = param.TranslateX;
	_m13 = param.TranslateY;
}

void CMatrix4x4Util::Init(CMatrix4x4Util matrix)
{
	_m0 = matrix._m0;
	_m1 = matrix._m1;
	_m2 = matrix._m2;
	_m3 = matrix._m3;

	_m4 = matrix._m4;
	_m5 = matrix._m5;
	_m6 = matrix._m6;
	_m7 = matrix._m7;

	_m8 = matrix._m8;
	_m9 = matrix._m9;
	_m10 = matrix._m10;
	_m11 = matrix._m11;

	_m12 = matrix._m12;
	_m13 = matrix._m13;
	_m14 = matrix._m14;
	_m15 = matrix._m15;
}

void CMatrix4x4Util::Init(float x, float y)
{
	_m0 = x;
	_m1 = y;
	_m2 = 0;
	_m3 = 1;

	_m4 = 0;
	_m5 = 0;
	_m6 = 0;
	_m7 = 1;

	_m8 = 0;
	_m9 = 0;
	_m10 = 0;
	_m11 = 1;

	_m12 = 0;
	_m13 = 0;
	_m14 = 0;
	_m15 = 1;
}

void CMatrix4x4Util::Init(float x, float y, float w, float h)
{
	_m0 = x;
	_m1 = y;
	_m2 = 0;
	_m3 = 1;

	_m4 = x + w;
	_m5 = y;
	_m6 = 0;
	_m7 = 1;

	_m8 = x + w;
	_m9 = y + h;
	_m10 = 0;
	_m11 = 1;

	_m12 = x;
	_m13 = y + h;
	_m14 = 0;
	_m15 = 1;
}

void CMatrix4x4Util::reNew()
{
	_m0 = 1;
	_m1 = 0;
	_m2 = 0;
	_m3 = 0;

	_m4 = 0;
	_m5 = 1;
	_m6 = 0;
	_m7 = 0;

	_m8 = 0;
	_m9 = 0;
	_m10 = 1;
	_m11 = 0;

	_m12 = 0;
	_m13 = 0;
	_m14 = 0;
	_m15 = 1;

	_currentRevertTranformStore = 0;

	StoreCurrentToRevert();
}

void CMatrix4x4Util::DoScale(float scaleX, float scaleY, float cx, float cy)
{
	CMatrix4x4Util scaleMatrix, transplateMaxtrix;

	scaleMatrix._m0 = scaleX;
	scaleMatrix._m5 = scaleY;
	scaleMatrix._m10 = 1.0f;
	transplateMaxtrix.InitTransplate(-cx, -cy, 0);

	if (_currentRevertTranformStore == 1) //first transform
	{
		//do scale
		Mul(transplateMaxtrix);
		Mul(scaleMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		Mul(transplateMaxtrix);
	}
	else
	{
		//do scale
		CMatrix4x4Util tempMaxtrix;
		tempMaxtrix.Mul(transplateMaxtrix);
		tempMaxtrix.Mul(scaleMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		tempMaxtrix.Mul(transplateMaxtrix);

		//tranform with parent translate
		Mul(tempMaxtrix);
	}

	StoreCurrentToRevert();
}

void CMatrix4x4Util::Scale(float scaleX, float scaleY, float cx, float cy)
{
	CMatrix4x4Util scaleMatrix, transplateMaxtrix;
	
	scaleMatrix._m0 = scaleX;
	scaleMatrix._m5 = scaleY;
	scaleMatrix._m10 = 1.0f;
	transplateMaxtrix.InitTransplate(-cx, -cy, 0);

	if (_currentRevertTranformStore == 1) //first transform
	{
		//do scale
		Mul(transplateMaxtrix);
		Mul(scaleMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		Mul(transplateMaxtrix);
	}
	else
	{
		//do scale
		CMatrix4x4Util tempMaxtrix;
		tempMaxtrix.Mul(transplateMaxtrix);
		tempMaxtrix.Mul(scaleMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		tempMaxtrix.Mul(transplateMaxtrix);

		//tranform with parent translate
		tempMaxtrix.Mul(*this);

		Init(tempMaxtrix);
	}

	StoreCurrentToRevert();
}

void CMatrix4x4Util::DoRotate(float angle, float cx, float cy)
{
	CMatrix4x4Util rotateMatrix, transplateMaxtrix;

	rotateMatrix._m0 = CMath::COS(angle);
	rotateMatrix._m1 = CMath::SIN(angle);
	rotateMatrix._m4 = -CMath::SIN(angle);
	rotateMatrix._m5 = CMath::COS(angle);
	transplateMaxtrix.InitTransplate(-cx, -cy, 0);

	if (_currentRevertTranformStore == 1) //first transform
	{
		//do rotate
		Mul(transplateMaxtrix);
		Mul(rotateMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		Mul(transplateMaxtrix);
	}
	else
	{
		//do rotate
		CMatrix4x4Util tempMaxtrix;
		tempMaxtrix.Mul(transplateMaxtrix);
		tempMaxtrix.Mul(rotateMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		tempMaxtrix.Mul(transplateMaxtrix);

		//tranform with parent translate
		Mul(tempMaxtrix);
	}

	StoreCurrentToRevert();
}

void CMatrix4x4Util::Rotate(float angle, float cx, float cy)
{
	CMatrix4x4Util rotateMatrix, transplateMaxtrix;

	rotateMatrix._m0 = CMath::COS(angle);
	rotateMatrix._m1 = CMath::SIN(angle);
	rotateMatrix._m4 = -CMath::SIN(angle);
	rotateMatrix._m5 = CMath::COS(angle);
	transplateMaxtrix.InitTransplate(-cx, -cy, 0);

	if (_currentRevertTranformStore == 1) //first transform
	{
		//do rotate
		Mul(transplateMaxtrix);
		Mul(rotateMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		Mul(transplateMaxtrix);
	}
	else
	{
		//do rotate
		CMatrix4x4Util tempMaxtrix;
		tempMaxtrix.Mul(transplateMaxtrix);
		tempMaxtrix.Mul(rotateMatrix);
		transplateMaxtrix.InitTransplate(cx, cy, 0);
		tempMaxtrix.Mul(transplateMaxtrix);

		//tranform with parent translate
		tempMaxtrix.Mul(*this);

		Init(tempMaxtrix);
	}

	StoreCurrentToRevert();
}

void CMatrix4x4Util::Translate(float x, float y)
{
	CMatrix4x4Util transplateMaxtrix;
	transplateMaxtrix.InitTransplate(x, y, 0);

	if (_currentRevertTranformStore == 1) //first transform
	{
		//do rotaion
		Mul(transplateMaxtrix);
	}
	else
	{
		//do scale
		CMatrix4x4Util tempMaxtrix;
		tempMaxtrix.Mul(transplateMaxtrix);		

		//tranform with parent translate
		tempMaxtrix.Mul(*this);

		Init(tempMaxtrix);
	}

	StoreCurrentToRevert();
}

void CMatrix4x4Util::RevertTransform()
{
	_currentRevertTranformStore -= 2;
	if (_currentRevertTranformStore < 0)
	{
		_currentRevertTranformStore = 0;
	}

	_m0 = _revertStore[_currentRevertTranformStore]._m0;
	_m1 = _revertStore[_currentRevertTranformStore]._m1;
	_m2 = _revertStore[_currentRevertTranformStore]._m2;
	_m3 = _revertStore[_currentRevertTranformStore]._m3;

	_m4 = _revertStore[_currentRevertTranformStore]._m4;
	_m5 = _revertStore[_currentRevertTranformStore]._m5;
	_m6 = _revertStore[_currentRevertTranformStore]._m6;
	_m7 = _revertStore[_currentRevertTranformStore]._m7;

	_m8 = _revertStore[_currentRevertTranformStore]._m8;
	_m9 = _revertStore[_currentRevertTranformStore]._m9;
	_m10 = _revertStore[_currentRevertTranformStore]._m10;
	_m11 = _revertStore[_currentRevertTranformStore]._m11;

	_m12 = _revertStore[_currentRevertTranformStore]._m12;
	_m13 = _revertStore[_currentRevertTranformStore]._m13;
	_m14 = _revertStore[_currentRevertTranformStore]._m14;
	_m15 = _revertStore[_currentRevertTranformStore]._m15;

	_currentRevertTranformStore++;
}

PineTransformParam CMatrix4x4Util::toPineTranform()
{
	PineTransformParam param;

	param.m11 = _m0;
	param.m12 = _m1;
	param.m21 = _m4;
	param.m22 = _m5;
	param.TranslateX = _m12;
	param.TranslateY = _m13;

	return param;
}

//util
void CMatrix4x4Util::GetCurrentXYWH(float &x, float &y, float &w, float &h)
{
	x = _m0;
	y = _m1;
	w = _m4 - _m0;
	h = _m9 - _m5;
}

void CMatrix4x4Util::StoreCurrentToRevert()
{
	_revertStore[_currentRevertTranformStore]._m0 = _m0;
	_revertStore[_currentRevertTranformStore]._m1 = _m1;
	_revertStore[_currentRevertTranformStore]._m2 = _m2;
	_revertStore[_currentRevertTranformStore]._m3 = _m3;

	_revertStore[_currentRevertTranformStore]._m4 = _m4;
	_revertStore[_currentRevertTranformStore]._m5 = _m5;
	_revertStore[_currentRevertTranformStore]._m6 = _m6;
	_revertStore[_currentRevertTranformStore]._m7 = _m7;

	_revertStore[_currentRevertTranformStore]._m8 = _m8;
	_revertStore[_currentRevertTranformStore]._m9 = _m9;
	_revertStore[_currentRevertTranformStore]._m10 = _m10;
	_revertStore[_currentRevertTranformStore]._m11 = _m11;

	_revertStore[_currentRevertTranformStore]._m12 = _m12;
	_revertStore[_currentRevertTranformStore]._m13 = _m13;
	_revertStore[_currentRevertTranformStore]._m14 = _m14;
	_revertStore[_currentRevertTranformStore]._m15 = _m15;

	_currentRevertTranformStore++;
	if (_currentRevertTranformStore >= MAX_CACHED_REVERT_TRANSFORM)
	{
		_currentRevertTranformStore = MAX_CACHED_REVERT_TRANSFORM;
	}
}

void CMatrix4x4Util::InitTransplate(float x, float y, float z)
{
	_m12 = x;
	_m13 = y;
	_m14 = z;
}

void CMatrix4x4Util::Mul(CMatrix4x4Util matrix)
{
	CMatrix4x4Util result;

	result._m0 = _m0*matrix._m0 + _m1*matrix._m4 + _m2*matrix._m8 + _m3*matrix._m12;
	result._m1 = _m0*matrix._m1 + _m1*matrix._m5 + _m2*matrix._m9 + _m3*matrix._m13;
	result._m2 = _m0*matrix._m2 + _m1*matrix._m6 + _m2*matrix._m10 + _m3*matrix._m14;
	result._m3 = _m0*matrix._m3 + _m1*matrix._m7 + _m2*matrix._m11 + _m3*matrix._m15;

	result._m4 = _m4*matrix._m0 + _m5*matrix._m4 + _m6*matrix._m8 + _m7*matrix._m12;
	result._m5 = _m4*matrix._m1 + _m5*matrix._m5 + _m6*matrix._m9 + _m7*matrix._m13;
	result._m6 = _m4*matrix._m2 + _m5*matrix._m6 + _m6*matrix._m10 + _m7*matrix._m14;
	result._m7 = _m4*matrix._m3 + _m5*matrix._m7 + _m6*matrix._m11 + _m7*matrix._m15;

	result._m8 = _m8*matrix._m0 + _m9*matrix._m4 + _m10*matrix._m8 + _m11*matrix._m12;
	result._m9 = _m8*matrix._m1 + _m9*matrix._m5 + _m10*matrix._m9 + _m11*matrix._m13;
	result._m10 = _m8*matrix._m2 + _m9*matrix._m6 + _m10*matrix._m10 + _m11*matrix._m14;
	result._m11 = _m8*matrix._m3 + _m9*matrix._m7 + _m10*matrix._m11 + _m11*matrix._m15;

	result._m12 = _m12*matrix._m0 + _m13*matrix._m4 + _m14*matrix._m8 + _m15*matrix._m12;
	result._m13 = _m12*matrix._m1 + _m13*matrix._m5 + _m14*matrix._m9 + _m15*matrix._m13;
	result._m14 = _m12*matrix._m2 + _m13*matrix._m6 + _m14*matrix._m10 + _m15*matrix._m14;
	result._m15 = _m12*matrix._m3 + _m13*matrix._m7 + _m14*matrix._m11 + _m15*matrix._m15;

	///

	_m0 = result._m0;
	_m1 = result._m1;
	_m2 = result._m2;
	_m3 = result._m3;

	_m4 = result._m4;
	_m5 = result._m5;
	_m6 = result._m6;
	_m7 = result._m7;

	_m8 = result._m8;
	_m9 = result._m9;
	_m10 = result._m10;
	_m11 = result._m11;

	_m12 = result._m12;
	_m13 = result._m13;
	_m14 = result._m14;
	_m15 = result._m15;
}
