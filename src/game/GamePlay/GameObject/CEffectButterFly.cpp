#include "pch.h"
#include "../../GameCore/CGame.h"

CButterFly::CButterFly()
{
}

CButterFly::~CButterFly()
{
}
void CButterFly::Init(int spriteid, int anim)
{
	_x = 0;
	_y = 0;
	_tarx = 0;
	_tary = 0;
	_scale = 1.0f;
	_scale_speed = 0.1f;
	_speed = 1;
	_state = 0;
	_animid = anim;
	_count_frame_change_orien = 0;
	_spriteid = spriteid;
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(_animid, true);
	_anim.SetPos(_x,_y);
	_fly_y = 0;
}
void CButterFly::Active(float x, float y, float speed, float scale, float scalespeed)
{
	_x = x;
	_y = y;
	_tarx = CMath::RANDOM(10, 65) * 10;
	_tary = CMath::RANDOM(10,100)*10;
	_scale = scale;
	_scale_speed = scalespeed;
	_speed = speed;
	_state = 1;
	_count_frame_change_orien = CMath::RANDOM(3,10)*60;
	if (_x < _tarx)
	{
		_anim.SetAnim(_animid, true);
	}
	else if (_x > _tarx)
	{
		_anim.SetAnim(_animid + 3, true);
	}
	_anim.SetPos(_x, _y);
	_fly_y = 0;
	
}
void CButterFly::Render(float x, float y, int opac)
{
	if (_state == 0)
	{
		return;
	}
	G()->SetScale(_scale, _scale, x + _x, y + _y);
	_anim.SetPos(_x, _y);
	_anim.DrawAnimObject(G());
	G()->ClearScale();
	G()->SetOpacity(opac);
}
void CButterFly::Update()
{
	if (_state == 0)
	{
		return;
	}
	_anim.UpdateAnimObject();
	_count_frame_change_orien--;
	if (_anim._current_aframes - GET_SPRITE(_spriteid)->GetFirstAFrameID(_anim.GetCurrentAnim()) == 1)
	{
		_fly_y = 1.5f;
	}
	if (_count_frame_change_orien == 0)
	{
		_count_frame_change_orien = CMath::RANDOM(3, 10) * 60;
		_tarx = CMath::RANDOM(10, 65) * 10;
		_tary = CMath::RANDOM(10, 100) * 10;
	}
	if (CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _speed))
	{
		_count_frame_change_orien = CMath::RANDOM(3, 10) * 60;
		_tarx = CMath::RANDOM(10, 65) * 10;
		_tary = CMath::RANDOM(10, 100) * 10;
		if (_x < _tarx)
		{
			_anim.SetAnim(_animid, true);
		}
		else if (_x > _tarx)
		{
			_anim.SetAnim(_animid + 3, true);
		}
	}
	else
	{
		float angle = CMath::ANGLE(_x, _y, _tarx, _tary);
		_x += (_speed + _fly_y/2.0f)*CMath::COS(angle);
		_y += (_speed + _fly_y)*CMath::SIN(angle);
	}
	_fly_y *= 0.95f;
}
bool CButterFly::UpdateTouch()
{
	return false;
}