#include "pch.h"
#include "../../GameCore/CGame.h"

EffectExplo::EffectExplo()
{
}

EffectExplo::~EffectExplo()
{
}

void EffectExplo::Init(int spriteid, int type)
{
	for (int i = 0; i < PARTICLE_EXPLOSION_NUM_MAX; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_speed[i] = 1.0f;
		_scale[i] = 1.0f;
		_speed_scale[i] = 0.1f;
		_state[i] = 0;
		_opac[i] = 100;
		_frame[i] = 0;
		_angle[i] = 0;
		_time_hold[i] = 0;
		_scrolly[i] = 0;
		_speed_scroll[i] = 0;
	}
	_num = 0;
	_type = type;
	_spriteid = spriteid;
	_anim_effect_fire.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim_effect_fire.SetAnim(3, false);
	_anim_effect_fire.SetPos(0, 0);
	_size = 0;
}
void EffectExplo::Active(int type, float x, float y, float speed, float speedscale, int num, int frame, int numframe, int timehold, int size)
{
	_num = num;
	//_type = type;
	if (_num >= PARTICLE_EXPLOSION_NUM_MAX)
	{
		_num = PARTICLE_EXPLOSION_NUM_MAX;
		printf("\n out mem particle");
	}
	for (int i = 0; i < _num; i++)
	{
		_x[i] = x;
		_y[i] = y;
		_speed[i] = speed + speed*(CMath::RANDOM(0, 5)/10.0f);
		_scale[i] = 1.0f;
		_speed_scale[i] = speedscale + speedscale/(CMath::RANDOM(5, 10)*1.0f);
		_state[i] = 1;
		_opac[i] = 100;
		_frame[i] = CMath::RANDOM(0, numframe - 1) + frame;
		_angle[i] = i * (360 / _num);
		_time_hold[i] = timehold + timehold*CMath::RANDOM(0, 5);
		_scrolly[i] = 0;
		_speed_scroll[i] = 0;
		if (_type == EXPLOSION_TYPE_CIRCLE)
		{
			//_speed_scroll[i] = 0;
			_time_hold[i] = 0;
			_speed[i] = speed;
		}
		else if (_type == EXPLOSION_TYPE_FALL)
		{
			_speed_scroll[i] = CMath::RANDOM(2, 5) / 10.0f;
		}
	}
	_anim_effect_fire.SetAnim(3, false);
	_anim_effect_fire.SetPos(x, y);
	_size = size;
}
void EffectExplo::Render(float x, float y, int opac)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		G()->SetOpacity(_opac[i]);
		//printf("\n explosion[%d] scale = %f", i, _scale[i]);
		G()->SetScale(_scale[i], _scale[i], _x[i] + x, _y[i] + y + _scrolly[i]);
		GET_SPRITE(_spriteid)->DrawFrame(G(), _frame[i], _x[i] + x, _y[i] + y + _scrolly[i]);
		G()->ClearScale();
	}
	if (_size >= 1)
	{
		if (_size == 2)
		{
			G()->SetScale(1.5f, 1.5f, _anim_effect_fire._x, _anim_effect_fire._y);
			_anim_effect_fire.DrawAnimObject(G());
			G()->ClearScale();
		}
		else if (_size == 3)
		{
			G()->SetScale(0.5f, 0.5f, _anim_effect_fire._x, _anim_effect_fire._y);
			_anim_effect_fire.DrawAnimObject(G());
			G()->ClearScale();
		}
		else
		{
			_anim_effect_fire.DrawAnimObject(G());
		}
	}
	G()->SetOpacity(opac);
}
void EffectExplo::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		_time_hold[i]--;
		if (_time_hold[i] <= 0)
		{
			_time_hold[i] = 0;
			_scale[i] -= _speed_scale[i];
			
			if (_type == EXPLOSION_TYPE_DEFAULT)
			{
				if (_scale[i] <= 0.6f)
				{
					_speed[i] *= 0.9f;
					_speed_scale[i] *= 0.9f;
				}
				else
				{
					_speed_scale[i] *= 1.001f;
				}
			}
			else
			{
				_speed_scale[i] *= 1.001f;
			}
			if (_scale[i] <= 0)
			{
				_scale[i] = 0;
				_state[i] = 0;
			}
			
		}
		if (_type == EXPLOSION_TYPE_FALL)
		{
			_speed[i] *= 0.99f;
		}
		else if (_type == EXPLOSION_TYPE_DEFAULT)
		{
			_speed[i] *= 0.95f;
		}
		if (_speed[i] <= 0.1f)
		{
			_speed[i] = 0.1f;
		}
		_x[i] += _speed[i] * CMath::COS(_angle[i]);
		_y[i] += _speed[i] * CMath::SIN(_angle[i]) + _scrolly[i];
		_scrolly[i] += _speed_scroll[i];
		if (_type == EXPLOSION_TYPE_FALL)
		{
			if (_scrolly[i] >= 30)
			{
				_speed_scroll[i] *= 0.9f;
			}
			else
			{
				_speed_scroll[i] *= 1.001f;
			}
		}
		
	}
	if (_size >= 1)
	{
		_anim_effect_fire.UpdateAnimObject();
	}
}
bool EffectExplo::IsEnd()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] != 0)
		{
			return false;
		}
	}
	return true;
}