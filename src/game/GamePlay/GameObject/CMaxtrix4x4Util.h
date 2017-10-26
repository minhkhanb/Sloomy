#if !CMATRIX_4X4_UTIL_H
#define CMATRIX_4X4_UTIL_H 1

#define MAX_CACHED_REVERT_TRANSFORM		(5)

class CMatrix4x4Util
{	
	void InitTransplate(float x, float y, float z);


	int _currentRevertTranformStore;
	void StoreCurrentToRevert();
	struct revertStore
	{
		float _m0;
		float _m1;
		float _m2;
		float _m3;
		float _m4;
		float _m5;
		float _m6;
		float _m7;
		float _m8;
		float _m9;
		float _m10;
		float _m11;
		float _m12;
		float _m13;
		float _m14;
		float _m15;
	};
	revertStore _revertStore[MAX_CACHED_REVERT_TRANSFORM];

public:
	CMatrix4x4Util()
	{
		reNew();
	};

	float _m0;
	float _m1;
	float _m2;
	float _m3;
	float _m4;
	float _m5;
	float _m6;
	float _m7;
	float _m8;
	float _m9;
	float _m10;
	float _m11;
	float _m12;
	float _m13;
	float _m14;
	float _m15;

	PineTransformParam toPineTranform();
	void Init(PineTransformParam param);
	void Init(CMatrix4x4Util matrix);
	void Init(float x, float y);
	void Init(float x, float y, float w, float h);

	void Scale(float scaleX, float scaleY, float cx, float cy);
	void Rotate(float angle, float cx, float cy);
	void Mul(CMatrix4x4Util matrix);
	void Translate(float x, float y);

	void DoScale(float scaleX, float scaleY, float cx, float cy);
	void DoRotate(float angle, float cx, float cy);
	void DoMul(CMatrix4x4Util matrix);
	void DoTranslate(float x, float y);

	void GetCurrentXYWH(float &x, float &y, float &w, float &h);

	void RevertTransform();

	void reNew();
};

#endif