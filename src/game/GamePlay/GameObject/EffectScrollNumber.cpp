#include "pch.h"
#include "../../GameCore/CGame.h"

CScrollNumber::CScrollNumber()
{
}

CScrollNumber::~CScrollNumber()
{
}


void CScrollNumber::Init(int sprite, int value, int speed)
{
	_sprite = sprite;
	_speed_scroll = speed;
	_value_delta = 0;
	_value_current = value;
	_value_target = value;
	_type = SCROLL_TYPE_NONE;
	_type_old = SCROLL_TYPE_NONE;
}
void CScrollNumber::Render(float x, float y, int anchor, int spacing)
{
	int char_space = GET_SPRITE(_sprite)->_char_spacing;
	GET_SPRITE(_sprite)->_char_spacing = spacing;
	GET_SPRITE(_sprite)->DrawAnsiTextFormat(G(), x, y, anchor, "%lld", _value_current);
	GET_SPRITE(_sprite)->_char_spacing = char_space;
}
void CScrollNumber::StartTo(INT_TIME target)
{
	_value_target = target;
	//printf("\n _value_target = %lld -- _value_current=  %lld", _value_target, _value_current);
	if (_value_target > _value_current)
	{
		_type = SCROLL_TYPE_UP;
	}
	else
	{
		_type = SCROLL_TYPE_DOWN;
	}
	int scroll_delta = CMath::ABSOLUTE_VALUE((int)(_value_target - _value_current)) / _speed_scroll;
	if (scroll_delta <= 0)
	{
		scroll_delta = 1;
	}
	_value_delta = scroll_delta;
}
void CScrollNumber::ModifyValue(INT_TIME target)
{
	_value_target = target;
	_value_current = target;
}

void CScrollNumber::Hold()
{
	_type_old = _type;
	_type = SCROLL_TYPE_HOLD;
}
void CScrollNumber::Resume()
{
	_type = _type_old;
	_type_old = SCROLL_TYPE_HOLD;
}

int CScrollNumber::GetType()
{
	return _type;
}

void CScrollNumber::SetValue(INT_TIME value)
{
	_value_target = value;
	_value_current = value;
}

INT_TIME CScrollNumber::GetValueCurrent()
{
	return _value_current;
}
INT_TIME CScrollNumber::GetValueTarget()
{
	return _value_target;
}
void CScrollNumber::Update()
{
	_value_cur_delta = 0;
	switch (_type)
	{
	case SCROLL_TYPE_UP:
		if (_value_target - _value_delta > _value_current)
		{
			_value_current += _value_delta;
			_value_cur_delta = _value_delta;
		}
		else
		{
			_value_cur_delta = _value_target - _value_current;
			_value_current = _value_target;
			_type = SCROLL_TYPE_END;
		}
		break;
	case SCROLL_TYPE_DOWN:
		if (_value_target < _value_current - _value_delta)
		{
			_value_current -= _value_delta;
			_value_cur_delta = _value_delta;
		}
		else
		{
			_value_cur_delta = _value_current - _value_target;
			_value_current = _value_target;

			_type = SCROLL_TYPE_END;
		}
		break;
	default:
		break;
	}
	if (_value_current == _value_target && _value_current == 0)
	{
		_type = SCROLL_TYPE_END;
	}
}