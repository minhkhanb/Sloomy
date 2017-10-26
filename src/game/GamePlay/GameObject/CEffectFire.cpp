#include "pch.h"
#include "../../GameCore/CGame.h"

CEffectFire::CEffectFire()
{
}

CEffectFire::~CEffectFire()
{
}

void CEffectFire::Init(int sprite, int frame, int numframe, int num, float scale, bool circle, float min_speed)
{
	_num = num;
    if(_num > NUM_MAX_PARTICAL_FIRE)
    {
        _num = NUM_MAX_PARTICAL_FIRE;
    }
	_sprite = sprite;
	
	_status = EFFECT_FIRE_STATE_NORMAL;
	_centerx = 0;
	_centery = 0;
	_is_circle = circle;
	_min_speed = min_speed;
	for (int i = 0; i < _num; i++)
	{
		_frame[i] = frame + CMath::RANDOM(0, numframe - 1);
		_x[i] = 0;
		_y[i] = 0;
		_opac[i] = 0;
		_scale[i] = scale;
		_rotate[i] = 0;
		_state[i] = 0;
		_angle[i] = CMath::RANDOM(1, 360);
		_speed_delta[i] = CMath::RANDOM(1, 10) / 10.0f;
		_speed_opac[i] = 0;
		_speed_rotate[i] = 0;
		_speed_scale[i] = 0;
		_speed[i] = 0;
	}

}
void CEffectFire::Active(float centerx, float centery, float speed, float speedopac, float speedrot, float speedscale, float scalemax, float opac)
{
	
	_status = EFFECT_FIRE_STATE_START;
	_centerx = centerx;
	_centery = centery;
	_scale_max = scalemax;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_opac[i] = opac;
		_scale[i] += CMath::RANDOM(5, 10) / 10.0f;
		_state[i] = 1;
		if (_is_circle)
		{
			_scale[i] = 1.0f;
			_speed[i] = speed;
			_speed_delta[i] = CMath::RANDOM(25, 25) / 100.0f;
			_angle[i] = i*(360 / _num);
		}
		else
		{
			_speed[i] = CMath::RANDOM(speed, speed * 2);
			_speed_delta[i] = CMath::RANDOM(30, 50) / 100.0f;
			_angle[i] = CMath::RANDOM(1, 360);
		}

		_speed_opac[i] = speedopac;
		_speed_rotate[i] = speedrot;
		_speed_scale[i] = speedscale;


	}
}
void CEffectFire::Render(float x, float y, int opac)
{
	x += _centerx;
	y += _centery;
	if (_status == EFFECT_FIRE_STATE_NORMAL)
	{
		return;
	}
	float opac_temp = opac / 100.0f;
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			return;
		}
		G()->SetOpacity(_opac[i] * opac_temp);
		G()->SetRotate(_rotate[i], _x[i] + x, _y[i] + y);
		G()->SetScale(_scale[i], _scale[i], _x[i] + x, _y[i] + y);
		GET_SPRITE(_sprite)->DrawFrame(G(), _frame[i], _x[i] + x, _y[i] + y);
		G()->ClearScale();
		G()->ClearRotate();
		G()->SetOpacity(opac);
	}
}
void CEffectFire::Update()
{
	if (_status == EFFECT_FIRE_STATE_NORMAL)
	{
		return;
	}
	bool isend = true;
	//printf("\n effect fire _num = %d", _num);
	for (int i = 0; i < _num; i++)
	{
		//printf("\n effect fire _speed[i] = %d", _speed[i]);
		if (_state[i] == 0)
		{
			return;
		}
		if (_opac[i] > 0)
		{
			isend = false;
		}

		_x[i] += _speed[i] * CMath::COS(_angle[i]);
		_y[i] += _speed[i] * CMath::SIN(_angle[i]);
		_rotate[i] += _speed_rotate[i];
		if (_rotate[i] >= 360)
		{
			_rotate[i] -= 360;
		}
		_scale[i] += _speed_scale[i];
		if (_scale[i] >= _scale_max)
		{
			_scale[i] = _scale_max;
		}
		if (_scale[i] <= 0)
		{
			_scale[i] = 0;
		}
		if (_state[i] == 2)
		{
			if (_opac[i] > 0)
			{
				_opac[i] -= _speed_opac[i] * 2;
				if (_opac[i] <= 0)
				{
					_state[i] = -1;
				}
				continue;
			}
		}
		if (_speed[i] <= _min_speed)
		{
			_speed[i] /= 1.5f;
			_state[i] = 2;
		}
		else
		{
			_speed[i] -= _speed_delta[i];
			if (_opac[i] < 100)
			{
				_opac[i] += _speed_opac[i];
			}
		}
	}
	if (isend)
	{
		_status = EFFECT_FIRE_STATE_FREE;
	}
}
void CEffectFire::ForceEnd()
{
	for (int i = 0; i < _num; i++)
	{
		_state[i] = EFFECT_FIRE_STATE_END;
	}
}
bool CEffectFire::IsFree()
{
	if (_status == EFFECT_FIRE_STATE_FREE || _status == EFFECT_FIRE_STATE_NORMAL)
	{
		return true;
	}
	return false;
}