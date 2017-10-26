#ifndef _C_SROLL_NUMBER_
#define _C_SROLL_NUMBER_
#include "../../Lib/DataType/CType.h"
#include "../../GameCore/CAnimObject.h"

#define SCROLL_TYPE_NONE		-1
#define SCROLL_TYPE_UP			0
#define SCROLL_TYPE_DOWN		1
#define SCROLL_TYPE_HOLD		2
#define SCROLL_TYPE_END			3

class CScrollNumber
{
public:
	CScrollNumber();
	~CScrollNumber();
	

	void Init(int sprite, int value , int speed);
	void Render(float x, float y, int anchor, int spacing);
	void StartTo(INT_TIME target);
	void ModifyValue(INT_TIME target);
	void Hold();
	void Resume();
	void Update();
	int GetType();
	void SetValue(INT_TIME value);
	INT_TIME GetValueCurrent();
	INT_TIME GetValueTarget();
	int _value_cur_delta;
private:
	INT_TIME _value_target;
	INT_TIME _value_current;
	int _speed_scroll;
	int _sprite;
	INT_TIME _value_delta;
	int _type;
	int _type_old;
};


#endif