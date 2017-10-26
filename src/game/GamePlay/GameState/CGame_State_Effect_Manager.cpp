#include "pch.h"
#include "../../GameCore/CGame.h"

void FDGame::EffectLight::Init(float x, float y, int opac, float speedopac, float speed)
{
	_x = x;
	_y = y;
	_opac = opac;
	_speed = speed;
	_speed_opac = speedopac;
	_dir = CMath::RANDOM(0, 1);
	if (_dir == 0)
	{
		_y = 0;
	}
	else
	{
		_x = 0;
	}
}
void FDGame::EffectLight::Render(float x, float y, int opac)
{
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	G()->SetOpacity(_opac*(opac / 100.0f));
	G()->SetScale(1.5f, 1.5f, 0, 0);
	GET_SPRITE(SPRITEID_HUD_WEARTHER_01)->DrawFrame(G(), 0, _x + x, _y + y);
	G()->ClearScale();
	G()->SetOpacity(opac);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
}
void FDGame::EffectLight::Update()
{
	_opac = (_opac - _speed_opac);
	if (_opac <= 0 && _speed_opac > 0.0f)
	{
		_opac = 0;
		_speed_opac = -(CMath::RANDOM(1, 2) / 2.0f);
		_speed = CMath::RANDOM(1, 5) / 20.0f;
		if (_dir == 0)
		{
			_x = CMath::RANDOM(1, 10) * 15;
			_y = 0;
		}
		else
		{
			_y = CMath::RANDOM(1, 10) * 15;
			_x = 0;
		}

	}
	if (_opac >= 40)
	{
		_speed_opac *= -1;
	}
	if (_dir == 0)
	{
		_x -= _speed;
	}
	else
	{
		_y -= _speed;
	}
}
void FDGame::ParticleLight::Init(float speedopac, float speed)
{
	_num = 0;
	for (int i = 0; i < PARTICLE_LIGHT_NUM_MAX; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_opac[i] = CMath::RANDOM(120, 160);
		_speed[i] = CMath::RANDOM(1, 5) / 20.0f + speed;
		_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f) + speedopac;
		_speedx[i] = CMath::RANDOM(-5, 5) / 5.0f;
		_speedy[i] = CMath::RANDOM(5, 15) / 10.0f;
		_scale[i] = CMath::RANDOM(1, 10) / 10.0f;
		_frame_id[i] = 0;
		_state[i] = 0;
	}
}
void FDGame::ParticleLight::Render(float x, float y, int opac)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		if (_frame_id[i] == 1)
		{
			G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		}

		G()->SetOpacity(_opac[i] * (opac / 100.0f));
		G()->SetScale(_scale[i], _scale[i], _x[i] + x, _y[i] + y);
		GET_SPRITE(SPRITEID_HUD_WEARTHER_01)->DrawFrame(G(), _frame_id[i], _x[i] + x, _y[i] + y);
		G()->ClearScale();
		G()->SetOpacity(opac);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
}
void FDGame::ParticleLight::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		_opac[i] -= _speed_opac[i];
		if (_opac[i] <= 0)
		{
			_opac[i] = CMath::RANDOM(120, 160);
			_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f);
			_x[i] = CMath::RANDOM(-5, 60) * 15;
			_y[i] = -50;
			_speedx[i] = CMath::RANDOM(-5, 5) / 5.0f;
			_speedy[i] = CMath::RANDOM(5, 15) / 10.0f;
			_scale[i] = CMath::RANDOM(1, 10) / 10.0f;
		}
		if (_y[i] > k_SCREEN_HEIGHT / 2)
		{
			_speed_opac[i] *= 1.02f;
		}
		_speedx[i] *= 0.99f;
		if (CMath::ABSOLUTE_VALUE(_speedx[i]) <= 0.1f)
		{
			_speedx[i] *= -1;
			_speedx[i] = (0.5 - CMath::ABSOLUTE_VALUE(_speedx[i]));
		}
		//_speed[i] *= 0.95f;
		if (_y[i] > k_SCREEN_HEIGHT*0.80f)
		{
			_speedy[i] *= 0.98f;
			_speed[i] *= 0.99f;
			if (_speedy[i] <= 2.0f)
			{
				_speedy[i] = 2.0f;
			}
		}
		else
		{
			if (_speed[i] > 0)
			{
				_speedy[i] *= 0.995f;
			}
			if (_speedy[i] <= 2.0f)
			{
				_speedy[i] = 2.0f;
			}
		}
		_x[i] += _speedx[i] + _speed[i];
		_y[i] += _speedy[i];
	}
}

void FDGame::ParticleLight::AddParticleLight(float x, float y, float speedopac, float speed, int frame, int numframe)
{
	if (_num >= PARTICLE_LIGHT_NUM_MAX)
	{
		return;
	}
	_x[_num] = CMath::RANDOM(-15, 40) * 15 + x;
	_y[_num] = -CMath::RANDOM(1, 10) * 10 + y;
	_opac[_num] = CMath::RANDOM(120, 160);
	_speed[_num] = speed / 3;
	_speed_opac[_num] = /*(CMath::RANDOM(1, 5) / 10.0f)*/ +speedopac;
	_speedx[_num] = CMath::RANDOM(-5, 5) / 5.0f;
	_speedy[_num] = CMath::RANDOM(5, 15) / 10.0f + speed;
	_scale[_num] = CMath::RANDOM(8, 10) / 10.0f;
	_frame_id[_num] = CMath::RANDOM(0, numframe - 1) + frame;
	_state[_num] = 1;
	_num++;
}
void FDGame::ParticleLight::RenderParticleLight(float x, float y, int opac)
{
	Render(x, y, opac);
}
void FDGame::ParticleLight::UpdateParticleLight()
{
	Update();
}
void FDGame::ParticleLight::Active(float x, float y, float speedopac, float speed, int frame, int numframe, int num)
{
	_num = num;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = CMath::RANDOM(-5, 60) * 15 + x;
		_y[i] = -CMath::RANDOM(5, 10) * 10 + y;
		_opac[i] = CMath::RANDOM(120, 160);
		_speed[i] = speed;
		_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f) + speedopac;
		_speedx[i] = CMath::RANDOM(-5, 5) / 5.0f;
		_speedy[i] = CMath::RANDOM(5, 15) / 10.0f;
		_scale[i] = CMath::RANDOM(1, 10) / 10.0f;
		_frame_id[i] = CMath::RANDOM(0, numframe - 1) + frame;
		_state[i] = 1;
	}
}

void FDGame::ParticleRain::Init()
{
	_x = 0;
	_y = 0;
	_cenx = _x;
	_ceny = _y;
	_speed = 1.0f;
	_speed_opac = 1.0f;
	_scalex = 1.0f;
	_scaley = 1.0f;
	_state = 0;
	_opac = 100;
}
void FDGame::ParticleRain::Active(float x, float y, float scalex, float scaley, int aoe, float speed)
{
	_state = 1;
	_cenx = x;
	_ceny = y;
	_aoe = aoe;
	_x = CMath::RANDOM(-5, 5)*(_aoe / 6.0f) + _cenx;
	_y = CMath::RANDOM(-5, 2)*(_aoe / 10.0f) + _ceny;
	_scalex = CMath::RANDOM(1, 5) / (scalex*10.0f);
	_scaley = CMath::RANDOM(3, 10) / (scaley*10.0f);
	_opac = CMath::RANDOM(40, 100);
	_speed_opac = CMath::RANDOM(4, 8);
	_speed = CMath::RANDOM(5, 10) / (speed);
}
void FDGame::ParticleRain::Render(float x, float y, int opac)
{
	if (_state == 0)
	{
		return;
	}
	G()->SetOpacity(_opac*(opac / 100.0f));
	G()->SetScale(_scalex, _scaley, _x + x, _y + y);
	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 13, _x + x, _y + y);
	G()->ClearScale();
	G()->SetOpacity(opac);
}
void FDGame::ParticleRain::ForceEnd()
{
	if (_state != 0)
	{
		_state = 2;
	}

}
void FDGame::ParticleRain::Update()
{
	if (_state == 0)
	{
		return;
	}
	_y += _speed;
	if (_y >= _ceny + _aoe / 4)
	{
		_opac -= _speed_opac;
		if (_opac <= 0)
		{
			if (_state == 2)
			{
				_state = 0;
			}
			else
			{
				_x = CMath::RANDOM(-5, 5)*(_aoe / 6.0f) + _cenx;
				_y = CMath::RANDOM(-5, 2)*(_aoe / 10.0f) + _ceny;
				_opac = CMath::RANDOM(40, 100);
				_speed_opac = CMath::RANDOM(4, 8);
			}
		}
	}
}
void FDGame::EffectPoint::Init()
{
	_x = 0;
	_y = 0;
	_scroll_y = 0;
	_speed = 2.0f;
	_opac = 0;
	_state = 0;
	_flower_id = 0;
	_scale = 1.0f;
	_type = EFFECT_POINT_TYPE_NORMAL;
	_rotate = 0;
}
void FDGame::EffectPoint::Active(int type, int floid, float x, float y, int value, float speed)
{
	_x = x;
	_y = y;
	_scroll_y = 0;
	_opac = 0;
	_type = type;
	_state = 1;
	_value = value;
	_speed = speed;
	_flower_id = floid;
	_scale = 1.0f;
	_opac_hight_light = 0;
	_opac_hight_light_max = false;
	_rotate = 0;
	/*if (_type >= EFFECT_POINT_TYPE_NORMAL)
	{
		_opac_hight_light = 0;
	}*/
	if (_type >= EFFECT_POINT_TYPE_COMBO && _type <= EFFECT_POINT_TYPE_MISS)
	{
		_opac = 00;
		_scale = 4.0f;
		_speed_scale = 0.05f;
	}
}
void FDGame::EffectPoint::Render(float x, float y, int opac)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	if (_state == 0)
	{
		return;
	}
	G()->SetOpacity(_opac);
	COLOR color = 0xffff0000;
	switch (_flower_id)
	{
	case CHARACTER_TYPE_LILY:
		color = 0xffffffff;
		break;
	case CHARACTER_TYPE_LOTUS:
		color = 0xffff5bc2;
		break;
	case CHARACTER_TYPE_ORCHILD:
		color = 0xff0cecef;
		break;
	case CHARACTER_TYPE_ROSE:
		color = 0xffff0000;
		break;
	case CHARACTER_TYPE_SUNFLOWER:
		color = 0xfff4d400;
		break;
	case CHARACTER_TYPE_VOILET:
		color = 0xffd804ff;
		break;
	default:
		break;
	}
	float scale = 1.0f;
	switch (_type)
	{
	case EFFECT_POINT_TYPE_NORMAL:
		break;
	case EFFECT_POINT_TYPE_REGULAR:
		scale = 2.0f;
		break;
	case EFFECT_POINT_TYPE_BIG:
		scale = 3.0f;
		break;
	default:
		break;
	}
	if (_type >= EFFECT_POINT_TYPE_COMBO && _type <= EFFECT_POINT_TYPE_MISS)
	{
		G()->SetOpacity(_opac);
		int shakex = 0;
		int shakey = 0;

		if (_opac >= 160 && _opac <= 180)
		{
			/*shakex = CMath::RANDOM(-10, 10);
			shakey = CMath::RANDOM(-5, 5);*/
		}
		int spriteid = SPRITEID_FONT_COMBO_01;
		float scale_plus = 1.0f + _value / 10.0f;
		if (_type == EFFECT_POINT_TYPE_FEVER || _type == EFFECT_POINT_TYPE_SUPER_BLAST)
		{
			if (_type == EFFECT_POINT_TYPE_SUPER_BLAST)
			{
				scale_plus = 0.75f;
			}
			else
			{
				scale_plus = 1.0f + _value / 20.0f;
			}
			spriteid = SPRITEID_FONT_FEVER_01;
		}
		G()->SetScale(_scale*1.2f*scale_plus, _scale*1.2f*scale_plus, _x + x + shakex, _y + y - _scroll_y + shakey);
		if (_type == EFFECT_POINT_TYPE_SUPER_BLAST)
		{
			GET_SPRITE(spriteid)->DrawAnsiTextFormat(G(), _x + x + shakex, _y + y - _scroll_y + shakey - (_scale - 1.0f) * 5, _HCENTER | _VCENTER, ";");
		}
		else if (_type == EFFECT_POINT_TYPE_MISS)
		{
			GET_SPRITE(spriteid)->DrawAnsiTextFormat(G(), _x + x + shakex, _y + y - _scroll_y + shakey - (_scale - 1.0f) * 5, _HCENTER | _VCENTER, ";");
		}
		else
		{
			GET_SPRITE(spriteid)->DrawAnsiTextFormat(G(), _x + x + shakex, _y + y - _scroll_y + shakey - (_scale - 1.0f) * 5, _HCENTER | _VCENTER, "< %d", _value);
		}

		G()->ClearScale();
	}
	else if (_type == EFFECT_POINT_TYPE_TIME_PLUS)
	{
		float sec = _value / 1000.0f;
		GET_SPRITE(SPRITEID_FONT_TIMELINE_01)->DrawAnsiTextFormat(G(), _x + x, _y + y - _scroll_y - (_scale - 1.0f) * 5, _LEFT | _VCENTER, "+%0.01f:", sec);
	}
	else
	{
		G()->SetOpacity(_opac);
		G()->SetScale(scale, scale, _x + x, _y + y - _scroll_y);
		G()->SetColorMask(color);
		GET_SPRITE(SPRITEID_FONT_POINT_INGAME_01)->DrawAnsiTextFormat(G(), _x + x, _y + y - _scroll_y, _HCENTER, "%d", _value);
		G()->ClearColorMask();

		G()->SetOpacity((_opac / 100.0f)*_opac_hight_light);
		if (_flower_id == CHARACTER_TYPE_LILY)
		{
			G()->SetColorMask(0x9900ff00);
		}
		else
		{
			G()->SetColorMask(0xffffffff);
		}

		GET_SPRITE(SPRITEID_FONT_POINT_INGAME_01)->DrawAnsiTextFormat(G(), _x + x, _y + y - _scroll_y, _HCENTER, "%d", _value);
		G()->ClearColorMask();

		G()->ClearScale();
	}

	G()->SetOpacity(opac);
}
void FDGame::EffectPoint::Update()
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	if (_state == 0)
	{
		return;
	}
	switch (_type)
	{
	case EFFECT_POINT_TYPE_NORMAL:
		break;
	case EFFECT_POINT_TYPE_REGULAR:
		break;
	case EFFECT_POINT_TYPE_BIG:
		break;
	case EFFECT_POINT_TYPE_COMBO:
		break;
	default:
		break;
	}
	if (_type >= EFFECT_POINT_TYPE_COMBO && _type <= EFFECT_POINT_TYPE_MISS)
	{
		_scale -= _speed_scale;
		if (_scale <= 1.0f)
		{
			_scale = 1.0f;
		}
		//printf("\n _scale = %f,_opac = %d", _scale, _opac);
		_speed_scale *= 1.1f;
		if (_scale <= 1.0f)
		{
			_opac -= 1;
			if (_opac <= 100)
			{
				_opac -= 8;
				if (_opac <= 0)
				{
					_state = 0;
				}
			}
		}
		else
		{
			_opac += 25 / _scale;
			if (_opac >= 180)
			{
				_opac = 180;
			}
		}
		if (_opac >= 160 && _opac <= 180)
		{
			/*shakex = CMath::RANDOM(-10, 10);
			shakey = CMath::RANDOM(-5, 5);*/
			if (_type == EFFECT_POINT_TYPE_SUPER_BLAST)
			{
				game->_screen_shake_x = CMath::RANDOM(-5, 5);
				game->_screen_shake_y = CMath::RANDOM(-5, 5);
			}
		}
	}
	else
	{
		_scroll_y += _speed;
		if (_scroll_y <= 40)
		{
			_opac += 10 / (1 + _type*1.0f);
			if (_opac >= 100)
			{
				_opac = 100;
			}
		}
		else
		{
			_opac -= 5;
			_speed *= 1.1f;
			if (_opac <= 0)
			{
				_state = 0;
			}
		}
	}

	if (_type > EFFECT_POINT_TYPE_NORMAL && _type <= EFFECT_POINT_TYPE_BIG)
	{
		//printf("\n _opac_hight_light = %d", _opac_hight_light);
		if (_opac_hight_light_max)
		{
			_opac_hight_light -= 100;
			if (_opac_hight_light <= -500)
			{
				_opac_hight_light_max = false;
			}
		}
		else
		{
			_opac_hight_light += 100;
			if (_opac_hight_light >= 800)
			{
				_opac_hight_light_max = true;
			}
		}
	}

}
void FDGame::EffectPointInit()
{
	for (int i = 0; i < EFFECT_POINT_NUM_MAX; i++)
	{
		_effect_point[i].Init();
	}
}
void FDGame::EffectPointAdd(int type, int floid, float x, float y, int val, float speed)
{
	for (int i = 0; i < EFFECT_POINT_NUM_MAX; i++)
	{
		if (type >= EFFECT_POINT_TYPE_COMBO && type <= EFFECT_POINT_TYPE_MISS)
		{
			if (_effect_point[i]._type >= EFFECT_POINT_TYPE_COMBO && _effect_point[i]._type <= EFFECT_POINT_TYPE_MISS)
			{
				_effect_point[i]._state = 0;;
			}
		}
	}
	for (int i = 0; i < EFFECT_POINT_NUM_MAX; i++)
	{

		if (_effect_point[i]._state == 0)
		{
			_effect_point[i].Active(type, floid, x, y, val, speed);
			break;
		}
	}
}
void FDGame::EffectPointRender(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_POINT_NUM_MAX; i++)
	{
		if (_effect_point[i]._type < EFFECT_POINT_TYPE_COMBO)
		{
			_effect_point[i].Render(x, y, opac);
		}

	}
	for (int i = 0; i < EFFECT_POINT_NUM_MAX; i++)
	{
		if (_effect_point[i]._type >= EFFECT_POINT_TYPE_COMBO)
		{
			_effect_point[i].Render(x, y, opac);
		}
	}
}
void FDGame::EffectPointUddate()
{
	for (int i = 0; i < EFFECT_POINT_NUM_MAX; i++)
	{
		_effect_point[i].Update();
	}
}

void FDGame::EffectExplosionInit()
{
	for (int i = 0; i < EFFECT_EXPLOSION_NUM_MAX; i++)
	{
		_effect_explosion[i].Init(SPRITEID_EFFECT_EXPLOSION_01, EXPLOSION_TYPE_FALL);
	}
}
void FDGame::EffectExplosionAdd(int type, float x, float y, float speed, float speedscale, int num, int frame, int numframe, int timehold, int size)
{
	for (int i = 0; i < EFFECT_EXPLOSION_NUM_MAX; i++)
	{
		if (_effect_explosion[i].IsEnd())
		{
			_effect_explosion[i].Active(type, x, y, speed, speedscale, num, frame, numframe, timehold, size);
			break;
		}
	}
}
void FDGame::EffectExplosionRender(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_EXPLOSION_NUM_MAX; i++)
	{
		_effect_explosion[i].Render(x, y, opac);
	}
}
void FDGame::EffectExplosionUpdate()
{
	for (int i = 0; i < EFFECT_EXPLOSION_NUM_MAX; i++)
	{
		_effect_explosion[i].Update();
	}
}
void FDGame::EffectEvaporate::Init(int anim)
{
	_x = 0;
	_y = 0;
	_state = 0;
	_speed_opac = 0;
	_speed_scale = 0;
	_speed_opac = 0;
	_scale = 0;
	_anim.SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_EXPLOSION_01), SPRITEID_EFFECT_EXPLOSION_01);
	_anim.SetAnim(anim, true);
	_anim.SetPos(_x, _y);
	_timehold = 0;
}
void FDGame::EffectEvaporate::Active(float x, float y, float speedscale, float speedopac, int timehold)
{
	_x = x;
	_y = y;
	_state = 1;
	_speed_scale = speedscale;
	_speed_opac = speedopac;
	_opac = 140;
	_scale = 0;
	//_anim.SetAnim(0, true);
	_anim.SetPos(_x, _y);
	_timehold = timehold;
}
void FDGame::EffectEvaporate::Render(float x, float y, int opac)
{
	if (_state == 0)
	{
		return;
	}
	G()->SetOpacity(_opac);
	G()->SetScale(_scale, _scale, _x + x, _y + y);
	_anim.DrawAnimObject(G());
	G()->ClearScale();
	G()->SetOpacity(opac);
}
void FDGame::EffectEvaporate::Update()
{

	if (_state == 0)
	{
		return;
	}
	_timehold--;
	if (_timehold > 0)
	{
		return;
	}
	_timehold = 0;
	if (_opac > 0)
	{
		_opac -= _speed_opac;
		_scale += _speed_scale;
		if (_opac < 40)
		{
			_speed_scale *= 1.01f;
		}
	}
	else
	{
		_opac = 0;
		_state = 0;
	}
	_anim.UpdateAnimObject();
}
void FDGame::EvaporateInit()
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		_effect_eva[i].Init(0);

	}
}
void FDGame::EvaporateAdd(float x, float y, float speedscale, float speedopac, int timehold)
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		if (_effect_eva[i]._state == 0)
		{
			//printf("\n timehold = %d", timehold);
			_effect_eva[i].Active(x, y, speedscale, speedopac, timehold);
			break;
		}
	}
}
void FDGame::EvaporateRender(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		_effect_eva[i].Render(x, y, opac);
	}
}
void FDGame::EvaporateUpdate()
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		//printf("\n eva holdtime[%d] = %d", i,_effect_eva[i]._timehold);
		_effect_eva[i].Update();
	}
}

void FDGame::UnEvaporateInit()
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		_effect_uneva[i].Init(1);

	}
}
void FDGame::UnEvaporateAdd(float x, float y, float speedscale, float speedopac, int timehold)
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		if (_effect_uneva[i]._state == 0)
		{
			_effect_uneva[i].Active(x, y, speedscale, speedopac, timehold);
			break;
		}
	}
}
void FDGame::UnEvaporateRender(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		_effect_uneva[i].Render(x, y, opac);
	}
}
void FDGame::UnEvaporateUpdate()
{
	for (int i = 0; i < EFFECT_EVAPORATE_NUM_MAX; i++)
	{
		_effect_uneva[i].Update();
	}
}

void FDGame::ParticleFlyTo::Init()
{
	for (int i = 0; i < PARTICLE_FLY_TO_NUM_MAX; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_tarx[i] = 0;
		_tary[i] = 0;
		_speed[i] = 0;
		_speed_delta[i] = 0;
		_count_frame_free_fly[i] = 0;
		_state[i] = 0;
		_scale[i] = 1.0f;
		_speed_deltax[i] = 0;
	}
	_state_main = 0;
	_num = PARTICLE_FLY_TO_NUM_MAX;
	_type = PARTICLE_FLY_TO_TYPE_SUNFLOWER;
}
void FDGame::ParticleFlyTo::Render(float x, float y)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	if (_state_main == 0)
	{
		return;
	}
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		float suny_delta = game->_interface_time_scrollx - 130;
		if (suny_delta < 0)
		{
			suny_delta = 0;
		}
		float sun_posy = game->_interface_time_scrollx + suny_delta*1.5f;
		float dis = CMath::DISTANCE(_x[i], _y[i], k_SCREEN_WIDTH / 2, 170 - sun_posy);
		int opac = (dis / 150.0f) * 100;
		//printf("\n dis = %f,opac = %d", dis, opac);
		G()->SetOpacity(opac);
		G()->SetScale(_scale[i], _scale[i], _x[i] + x, _y[i] + y);
		if (_type == PARTICLE_FLY_TO_TYPE_SUNFLOWER)
		{
			GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 38, _x[i] + x, _y[i] + y);
		}
		else if (_type == PARTICLE_FLY_TO_TYPE_FLOWER)
		{
			GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 48, _x[i] + x, _y[i] + y);
		}
		G()->ClearScale();
		G()->SetOpacity(100);
	}
}
void FDGame::ParticleFlyTo::Update()
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	//game->_sun_frame_index = 14;
	if (_state_main == 0)
	{
		return;
	}
	bool ended = true;
	for (int i = 0; i < _num; i++)
	{

		if (_state[i] == 0)
		{
			continue;
		}
		ended = false;
		if (_count_frame_free_fly[i] > 0)
		{
			_count_frame_free_fly[i]--;
			_y[i] -= _speed_delta[i];
			_speed_delta[i] -= 0.01f;
			if (_speed_delta[i] <= 0)
			{
				_count_frame_free_fly[i] = 0;
			}
			_speed_deltax[i] *= 0.98f;
			_x[i] += _speed_deltax[i];
		}
		else
		{
			float suny_delta = game->_interface_time_scrollx - 130;
			if (suny_delta < 0)
			{
				suny_delta = 0;
			}
			float sun_posy = game->_interface_time_scrollx + suny_delta*1.5f;
			if (_type == PARTICLE_FLY_TO_TYPE_SUNFLOWER)
			{
				_tarx[i] = k_SCREEN_WIDTH / 2;
				_tary[i] = 170 - sun_posy;
			}
			else if (_type == PARTICLE_FLY_TO_TYPE_FLOWER)
			{
				float posx = 0;
				for (int i = 0; i < FD_FARMER_NUM; i++)
				{
					if (game->_farmer_render[i]._state == 1)
					{
						posx = game->_farmer_render[i]._x - 60;
						break;
					}
				}
				_tarx[i] = posx;
				_tary[i] = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleY(42, 6) - 140;
			}

			if (CHECK_POINT_IN_CIRCLE(_x[i], _y[i], _tarx[i], _tary[i], _speed[i]))
			{
				_state[i] = 0;
				if (_type == PARTICLE_FLY_TO_TYPE_SUNFLOWER)
				{
					game->_sun_frame_smile = 0.5f*FPS_MAX;
					if (!game->_has_effect_sunrise)
					{
						game->_sunrise_rotate = CMath::RANDOM(0, 20);
						game->_sunrise_scale = 1.1f;
						game->_sunrise_frame_render1 = CMath::RANDOM(0, 3);
						game->_sunrise_frame_render2 = 4 + CMath::RANDOM(0, 3);
					}

					if (!game->_game_time.isNullDay())
					{
						game->_game_time.AddMillisecond(_value);
						if (game->_time_plus > 0)
						{
							int mky = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterY(44, 0);
							game->EffectPointAdd(EFFECT_POINT_TYPE_TIME_PLUS, 0, k_SCREEN_WIDTH / 2 + 30, mky, game->_time_plus, 1.0f);
							game->_time_plus = 0;
						}
						game->_has_effect_sunrise = true;
						game->_effect_sunrise_max = false;
					}
				}
				else if (_type == PARTICLE_FLY_TO_TYPE_FLOWER)
				{
					game->FlowerCollectedAdd(_value);
				}
			}
			else
			{
				float angle = CMath::ANGLE(_x[i], _y[i], _tarx[i], _tary[i]);
				_x[i] += _speed[i] * CMath::COS(angle);
				_y[i] += (_speed[i] * CMath::SIN(angle))*1.5f;
			}
		}
	}
	if (ended)
	{
		_state_main = 0;
	}
}
void FDGame::ParticleFlyTo::Active(float x, float y, int num, float tarx, float tary, float speed, int value, int type)
{
	_num = num;
	_state_main = 1;
	_value = value;
	_type = type;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = x + CMath::RANDOM(-5, 5) * 7;
		_y[i] = y + CMath::RANDOM(-5, 5) * 7;
		_tarx[i] = tarx;// +CMath::RANDOM(-10, 10) * 5;
		_tary[i] = tary;// +CMath::RANDOM(-10, 10) * 5;
		_speed[i] = speed;
		_speed_delta[i] = CMath::RANDOM(20, 30) / 10.0f;
		_count_frame_free_fly[i] = CMath::RANDOM(40, 60);
		_state[i] = 1;
		_scale[i] = CMath::RANDOM(3, 7) / 10.0f;
		_speed_deltax[i] = CMath::RANDOM(-3, 3);
	}

}
void FDGame::ParticleFlyToInit()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_particle_time_plus[i].Init();
	}
}
void FDGame::ParticleFlyToActive(float x, float y, int num, float tarx, float tary, float speed, int value, int type)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_particle_time_plus[i]._state_main == 0)
		{
			if (type == PARTICLE_FLY_TO_TYPE_SUNFLOWER)
			{
				_time_plus += value;
			}
			_particle_time_plus[i].Active(x, y, num, tarx, tary, speed, value, type);
			break;
		}

	}
}
void FDGame::ParticleFlyToRender(float x, float y)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_particle_time_plus[i].Render(x, y);
	}
}
void FDGame::ParticleFlyToUpdate()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_particle_time_plus[i].Update();
	}
}
void FDGame::EffectFireWork::Init(int spriteid, int frameid, int type, bool loop)
{
	for (int i = 0; i < PARTICLE_FIRE_WORK_NUM_MAX; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_scale[i] = 0;
		_scale_speed[i] = 0;
		_rotate[i] = 0;
		_speed_rotate[i] = 0;
		_opac[i] = 0;
		_opac_speed[i] = 0;
		_color[i] = 0xffffffff;
		_speed[i] = 0;
		_angle[i] = 0;
		_frameid[i] = 0;
	}
	_is_loop = loop;
	_type = type;
	_state = 0;
	_cenx = 0;
	_ceny = 0;
	_num = 0;
	_spriteid = spriteid;
	_frameid_main = frameid;
}
void FDGame::EffectFireWork::Active(float x, float y, float scale, float speedscale, float rotatespeed, float opacspeed, float speed, int num, int numframe)
{
	_num = num;
	_cenx = x;
	_ceny = y;
	_state = 1;
	_speed_main = speed;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = _cenx;
		_y[i] = _ceny;
		_scale[i] = scale;
		_scale_speed[i] = speedscale + speedscale*(CMath::RANDOM(-5, 5) / 10.0f);
		_rotate[i] = 0;
		_speed_rotate[i] = rotatespeed + rotatespeed*(CMath::RANDOM(-5, 5) / 10.0f);
		_opac[i] = CMath::RANDOM(100, 160);
		_opac_speed[i] = opacspeed + opacspeed*(CMath::RANDOM(0, 5) / 10.0f);
		_angle[i] = CMath::RANDOM(-10, 10) + i*(360 / _num);
		_speed[i] = _speed_main + _speed_main*(CMath::RANDOM(0, 10) / 10.0f);
		_frameid[i] = _frameid_main + CMath::RANDOM(0, numframe - 1);
		if (_type == PARTICLE_FIRE_WORK_TYPE_CIRCLE)
		{
			_opac_speed[i] = opacspeed;
			_scale_speed[i] = speedscale;
			_angle[i] = i*(360 / _num);
			_speed[i] = _speed_main;
			_scale_speed[i] = speedscale;
			_speed_rotate[i] = rotatespeed;
			_opac[i] = 130;
		}
		else if (_type == PARTICLE_FIRE_WORK_TYPE_COLOR)
		{
			int rand_color = CMath::RANDOM(0, 5);
			switch (rand_color)
			{
			case CHARACTER_TYPE_LILY:
				_color[i] = 0xffffffff;
				break;
			case CHARACTER_TYPE_LOTUS:
				_color[i] = 0xffff5bc2;
				break;
			case CHARACTER_TYPE_ORCHILD:
				_color[i] = 0xff0cecef;
				break;
			case CHARACTER_TYPE_ROSE:
				_color[i] = 0xffff0000;
				break;
			case CHARACTER_TYPE_SUNFLOWER:
				_color[i] = 0xfff4d400;
				break;
			case CHARACTER_TYPE_VOILET:
				_color[i] = 0xffd804ff;
				break;
			default:
				break;
			}
		}
	}

}
void FDGame::EffectFireWork::Render(float x, float y, int opac)
{
	if (_state == 0)
	{
		return;
	}
	for (int i = 0; i < _num; i++)
	{
		G()->SetOpacity(_opac[i]);
		G()->SetRotate(_rotate[i], x + _x[i], y + _y[i]);
		G()->SetScale(_scale[i], _scale[i], x + _x[i], y + _y[i]);
		if (_type == PARTICLE_FIRE_WORK_TYPE_COLOR)
		{
			G()->SetColorMask(_color[i]);
		}
		GET_SPRITE(_spriteid)->DrawFrame(G(), _frameid[i], x + _x[i], y + _y[i]);
		G()->ClearColorMask();
		G()->ClearScale();
		G()->ClearRotate();
		G()->SetOpacity(opac);
	}
}
void FDGame::EffectFireWork::Update()
{
	if (_state == 0)
	{
		return;
	}
	bool ending = true;
	for (int i = 0; i < _num; i++)
	{
		_opac[i] -= _opac_speed[i];
		if (_opac[i] >= 130)
		{
			continue;
		}
		_x[i] += _speed[i] * CMath::COS(_angle[i]);
		_y[i] += _speed[i] * CMath::SIN(_angle[i]);
		_rotate[i] += _speed_rotate[i];
		if (_rotate[i] >= 360)
		{
			_rotate[i] -= 360;
		}
		_scale[i] += _scale_speed[i];
		if (_state == 2)
		{
			_opac[i] -= 4;
		}
		if (_type == PARTICLE_FIRE_WORK_TYPE_DEFAULT)
		{
			_speed[i] *= 0.995f;
		}
		else if (_type == PARTICLE_FIRE_WORK_TYPE_CIRCLE)
		{
			_speed[i] *= 0.98f;
			if (_opac[i] <= 100)
			{
				_opac[i] -= _opac_speed[i];
				_speed[i] *= 0.98f;
			}
		}
		if (_opac[i] <= 0)
		{
			if (_state != 2 && _is_loop)
			{
				_x[i] = _cenx /*+ CMath::RANDOM(-10, 10) * 10*/;
				_y[i] = _ceny /*+ CMath::RANDOM(-10, 10) * 10*/;
				_opac[i] = CMath::RANDOM(100, 150);
				CMath::RANDOM(-10, 10) + i*(360 / _num);
				_scale[i] = 0;// CMath::RANDOM(1, 3) / 10.0f;
				_speed[i] = _speed_main + _speed_main*(CMath::RANDOM(0, 10) / 10.0f);
				if (_type == PARTICLE_FIRE_WORK_TYPE_CIRCLE)
				{
					_angle[i] = i*(360 / _num);
					_speed[i] = _speed_main;
					_opac[i] = 130;
				}
				int rand_color = CMath::RANDOM(0, 5);
				switch (rand_color)
				{
				case CHARACTER_TYPE_LILY:
					_color[i] = 0xffffffff;
					break;
				case CHARACTER_TYPE_LOTUS:
					_color[i] = 0xffff5bc2;
					break;
				case CHARACTER_TYPE_ORCHILD:
					_color[i] = 0xff0cecef;
					break;
				case CHARACTER_TYPE_ROSE:
					_color[i] = 0xffff0000;
					break;
				case CHARACTER_TYPE_SUNFLOWER:
					_color[i] = 0xfff4d400;
					break;
				case CHARACTER_TYPE_VOILET:
					_color[i] = 0xffd804ff;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			ending = false;
		}
	}
	if (ending)
	{
		_state = 0;
	}
}
void FDGame::EffectFireWork::ForceEnd()
{
	_state = 2;
}

void FDGame::ParticleFireUp::Init(int spriteid, bool isloop)
{
	for (int i = 0; i < PARTICLE_FIREUP_NUM; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_opac[i] = 0;
		_opac_speed[i] = 0;
		_speedx[i] = 0;
		_speedy[i] = 0;
		_rotate[i] = 0;
		_rotate_speed[i] = 0;
		_frameid[i] = 0;
		_state[i] = 0;
		_hold_time[i] = 0;
		_scale[i] = 0;
		_scale_speed[i] = 0;
	}
	_centerx = 0;
	_centery = 0;
	_spriteid = spriteid;
	_num = 0;
	_width = 0;
	_height = 0;
	_isloop = isloop;
	_main_speedx = 0;
	_main_speedy = 0;
	_main_opac = 0;
	_main_opac_speed = 0;
}
void FDGame::ParticleFireUp::Active(float x, float y, float spx, float spy, int opac, float opacsp, float rosp, int frameindex, int numframe, int num, int width, int height)
{
	_num = num;
	_centerx = x;
	_centery = y;
	_width = width;
	_height = height;
	_main_speedx = spx;
	_main_speedy = spy;
	_main_opac = opac;
	_main_opac_speed = opacsp;
	for (int i = 0; i < _num; i++)
	{
		float wx = _width / _num;
		float wy = _height / _num;
		_x[i] = _centerx - _width / 2 + CMath::RANDOM(0, _num) * wx;
		_y[i] = _centery - _height / 2 + CMath::RANDOM(0, _num) * wy;
		_opac[i] = _main_opac + _main_opac / CMath::RANDOM(4, 10);
		_opac_speed[i] = _main_opac_speed + _main_opac_speed / CMath::RANDOM(4, 10);
		_speedx[i] = _main_speedx + _main_speedx / CMath::RANDOM(4, 10);
		_speedy[i] = _main_speedy + _main_speedy / CMath::RANDOM(4, 10);
		_rotate[i] = 0;
		_hold_time[i] = CMath::RANDOM(0, 10) * 3;
		_rotate_speed[i] = rosp;
		_frameid[i] = frameindex + CMath::RANDOM(0, numframe - 1);
		_state[i] = 1;
		_scale[i] = 1.3f;
		_scale_speed[i] = 0.01f;
	}
}
void FDGame::ParticleFireUp::Render(float x, float y)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0 || _hold_time[i] > 0)
		{
			continue;
		}
		//G()->SetOpacity(_opac[i]);
		G()->SetRotate(_rotate[i], _x[i] + x, _y[i] + y);
		G()->SetScale(_scale[i], _scale[i], _x[i] + x, _y[i] + y);
		GET_SPRITE(_spriteid)->DrawFrame(G(), _frameid[i], _x[i] + x, _y[i] + y);
		G()->ClearScale();
		G()->ClearRotate();
		G()->SetOpacity(100);
	}
}
void FDGame::ParticleFireUp::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{

			continue;
		}
		if (_hold_time[i] > 0)
		{
			_hold_time[i]--;
			continue;
		}
		if (_state[i] == 2)
		{
			if (_opac[i] > 0)
			{
				_opac[i] -= 10;
			}
			if (_scale[i] > 0)
			{
				_scale[i] -= 0.1f;
				if (_scale[i] < 0)
				{
					_scale[i] = 0;
				}

			}
		}
		_x[i] += _speedx[i];
		_y[i] += _speedy[i];
		_speedx[i] *= 0.92f;
		_speedy[i] *= 0.92f;
		_rotate[i] += _rotate_speed[i];
		if (_rotate[i] >= 360)
		{
			_rotate[i] -= 360;
		}
		float spx = CMath::ABSOLUTE_VALUE(_speedx[i]);
		float spy = CMath::ABSOLUTE_VALUE(_speedy[i]);
		if (spx <= 0.1f)
		{
			if (_speedx[i] > 0)
			{
				_speedx[i] = 0.1f;
			}
			if (_speedx[i] < 0)
			{
				_speedx[i] = -0.1f;
			}
		}
		if (spy <= 0.1f)
		{
			if (_speedy[i] > 0)
			{
				_speedy[i] = 0.1f;
			}
			if (_speedy[i] < 0)
			{
				_speedy[i] = -0.1f;
			}
		}
		if (spx <= 2.0f && spy <= 2.0f)
		{
			_opac_speed[i] *= 1.1f;
			_opac[i] -= _opac_speed[i];
			_scale_speed[i] *= 1.1f;
			_scale[i] -= _scale_speed[i];
			if (_scale[i] <= 0)
			{
				_scale[i] = 0;
			}
		}
		//if (_opac[i] <= 0)
		if (_scale[i] <= 0)
		{
			if (_isloop && _state[i] < 2)
			{
				float wx = _width / _num;
				float wy = _height / _num;
				_x[i] = _centerx - _width / 2 + CMath::RANDOM(0, _num) * wx;
				_y[i] = _centery - _height / 2 + CMath::RANDOM(0, _num) * wy;
				_opac[i] = _main_opac + _main_opac / CMath::RANDOM(4, 10);
				_opac_speed[i] = _main_opac_speed + _main_opac_speed / CMath::RANDOM(4, 10);
				_speedx[i] = _main_speedx + _main_speedx / CMath::RANDOM(4, 10);
				_speedy[i] = _main_speedy + _main_speedy / CMath::RANDOM(4, 10);
				_hold_time[i] = CMath::RANDOM(0, 10) * 3;
				_scale[i] = 1.3f;
				_scale_speed[i] = 0.01f;
			}
			else
			{
				_state[i] = 0;
			}
		}
	}
}
void FDGame::ParticleFireUp::ForceEnd()
{
	for (int i = 0; i < _num; i++)
	{
		_state[i] = 2;
	}
}

void FDGame::Farmer::Init(int id)
{
	_id = id;
	_state = 0;
	_x = -300;
	_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleY(42, 6) + 1;
	_targetx = 0;
	_anim.SetAnimSprite(GET_SPRITE(SPRITEID_CHARACTER_FARMER_01), SPRITEID_CHARACTER_FARMER_01);
	_anim.SetAnim(_id * 2, true);
	_anim.SetPos(_x, _y);
	_speed = 4;
}
void FDGame::Farmer::Active(int id)
{
	_id = id;
	_state = 1;
	_targetx = 0;
	_anim.SetAnimSprite(GET_SPRITE(SPRITEID_CHARACTER_FARMER_01), SPRITEID_CHARACTER_FARMER_01);
	_anim.SetAnim(_id * 2, true);
	_anim.SetPos(_x, _y);
	_speed = 4;
	_x = -300;
	_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleY(42, 6) + 1;
}
void FDGame::Farmer::Render(float x, float y)
{
	if (_state == 0)
	{
		return;
	}
	_anim.SetPos(_x, _y);
	_anim.DrawAnimObject(G());
}
void FDGame::Farmer::Update()
{
	if (_state == 0)
	{
		return;
	}
	FDGame* game = (FDGame*)(GAME()->_current_game);
	//printf("\n _x[%d] = %f,y = %f ,anim = %d", _id,_x,_y, _anim.GetCurrentAnim());
	int param_numof = CGameDesign::FileGeneral_design::CSheetScore::number_of_flowers;
	int numofflower = game->GetDesignScoreMutiply(game->_scoreX_level - 1, param_numof);
	float per = (1.0f*game->_num_flower_collected) / (1.0f*numofflower);
	_targetx = 200 + per * 600;
	_anim.UpdateAnimObject(); 
	if (_state == 2)
	{
		_targetx = game->_screen_width*2.0f;
		if (_x >= game->_screen_width*2.0f)
		{
			_state = 0;
			return;
		}
		//printf("\n end _x[%d] = %f,y = %f", _id, _x, _y);
	}
	if (_x < _targetx)
	{
		//int mulx = 1;
		if (_anim.GetCurrentAnim() != _id * 2 + 1)
		{
			_anim.SetAnim(_id * 2 + 1, true);
		}
		_x += _speed;
		if (_x >= _targetx)
		{
			_x = _targetx;
			_anim.SetAnim(_id * 2, true);
		}
		if (_x > _targetx - 50)
		{
			_speed *= 0.92f;
			if (_speed <= 0.5f)
			{
				_speed = 0.5f;
			}
		}
		else
		{
			_speed *= 1.1f;
			if (_speed >= 4.0f)
			{
				_speed = 4.0f;
			}
		}
	}
	else
	{
		if (_anim.GetCurrentAnim() != _id * 2)
		{
			_anim.SetAnim(_id * 2, true);
		}
	}
}
void FDGame::Farmer::ForceEnd()
{
	if (_state != 0)
	{
		_state = 2;
	}
}

void FDGame::CCloud::Init()
{
	_x = 0;
	_y = 0;
	_tarx = 0;
	_tary = 0;
	_speed = 0;
	_state = 0;
	_frameid = 0;
	_layer = 0;
}
void FDGame::CCloud::Add(int frameid, float x, float y, float tarx, float tary, float speed)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_x = x;
	_y = y;
	_tarx = tarx;
	_tary = tary;
	_speed = speed;
	_state = 1;
	_frameid = frameid;
	_layer = CMath::RANDOM(1, 4) == 1 ? 1 : 0;
	if (game->_interface_weather_opac_night >= 5)
	{
		_speed *= 2;
	}
}
void FDGame::CCloud::Render(float x, float y)
{
	if (_state == 0)
	{
		return;
	}
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), _frameid, _x + x, _y + y);
}
void FDGame::CCloud::Update()
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	if (_state == 0)
	{
		return;
	}
	if (game->_interface_weather_opac_night >= 100)
	{
		_speed *= 1.007f;
	}
	if (CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _speed))
	{
		_x = _tarx;
		_y = _tary;
		_state = 0;
	}
	else
	{
		float angle = CMath::ANGLE(_x, _y, _tarx, _tary);
		_x += _speed*CMath::COS(angle);
		_y += _speed*CMath::SIN(angle);
	}
}
void FDGame::CBoard::setPosY(float y)
{
	_y += y;
}
void FDGame::CBoard::Init(float x, float y)
{
	_x = x;
	_y = y;
	_time_hold = 0;
	_frameid = 0;
	_opac = 100;
	_state = 0;
	_main_time = 0;
}
void FDGame::CBoard::Render(float x, float y)
{
	if (_state == 0)
	{
		return;
	}
	if (_time_hold > 10)
	{
		return;
	}
	GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), _frameid, _x + x, _y + y);
}
void FDGame::CBoard::Update()
{
	if (_state == 0)
	{
		return;
	}
	_time_hold--;
	if (_time_hold > 10)
	{
		return;
	}

	if (_time_hold <= 0)
	{
		_state = 0;
		//_time_hold = _main_time;
		//_state = 0;
	}
}
void FDGame::CBoard::Active(int timehold, int frameid, int numframe)
{
	_frameid = frameid + CMath::RANDOM(0, numframe - 1);
	_main_time = timehold;
	_time_hold = _main_time;
	_state = 1;
}
void FDGame::CBoard::ForceEnd()
{
	if (_state != 0)
	{
		_state = 0;
	}
}



void FDGame::BoardInit()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		int row = i / FD_NUM_COL;
		int col = i % FD_NUM_COL;
		int mk_x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleX(0, 0);
		int mk_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0);
		float x = FD_SLOT_WIDTH / 2 + col*FD_SLOT_WIDTH + mk_x;
		float y = FD_SLOT_WIDTH / 2 + row*FD_SLOT_WIDTH + mk_y;
		_board_light[i].Init(x, y);
	}
	_board_light_type_active = -1;
}
void FDGame::BoardSetBeginPos()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_board_light[i].setPosY(-90);
	}
}
void FDGame::BoardRender(float x, float y)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_board_light[i].Render(x, y);
	}
}
void FDGame::BoardActive()
{
	if (_board_light_type_active == -1)
	{
		int typeold = _board_light_type_active;
		_board_light_type_active = CMath::RANDOM(0, 1);
		/*while (_board_light_type_active == typeold)
		{
			_board_light_type_active = CMath::RANDOM(0, 1);
		}*/
	}
	int count = 0;
	int count_row = 0;
	int count_col = 0;
	//_board_light_type_active = 2;
	for (int i = 0; i < FD_NUM_ARR; i++)
	{

		int row = i / FD_NUM_COL;
		int col = i % FD_NUM_COL;
		switch (_board_light_type_active)
		{
		case 0:
			count = (row + col) * 2;
			break;
		case 1:
			count = (row + FD_NUM_COL - col) * 2;
			break;
		case 2:
			if (row > 3)
			{
				count_row = row - 4;
			}
			else
			{
				count_row = 3 - row;
			}
			if (col > 3)
			{
				count_col = col - 4;
			}
			else
			{
				count_col = 3 - col;
			}
			count = (FD_NUM_COL - count_row) * (FD_NUM_ROW - count_col);

			break;
		case 3:
		{
			if (row > row *col)
			{

			}
			count = (row *col) * 2;
		}
		break;
		case 4:
			count = (int)(CMath::ABSOLUTE_VALUE(3.5f - row)) * 4;
			break;
		case 5:
			count = (int)(CMath::ABSOLUTE_VALUE(3.5f - col)) * 4;
			break;
		case 6:
			count = row * 2;
			break;
		case 7:
			count = (FD_NUM_COL - col) * 2;
			break;
		default:
			break;
		}
		_board_light[i].Active(10 + count * 3.0f, 42, 1);
	}
}
void FDGame::BoardUpdate()
{
	bool hasloop = true;
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_board_light[i].Update();
		if (_board_light[i]._state != 0)
		{
			hasloop = false;
		}
	}
	if (hasloop && _fever_mode_active)
	{
		BoardActive();
	}
}
void FDGame::BoardForceEnd()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_board_light[i].ForceEnd();
	}
	_board_light_type_active = -1;
}

void FDGame::EffectFireInit()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_effect_fire[i].Init();
	}
}
void FDGame::EffectFireRender(float x, float y)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_effect_fire[i].Render(x, y);
	}
}
void FDGame::EffectFireActive(int sprite, int anim, int size, float x, float y, int timehold)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_effect_fire[i]._state == 0)
		{
			_effect_fire[i].Active(sprite, anim, size, x, y, timehold);
			break;
		}

	}
}
void FDGame::EffectFireUpdate()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_effect_fire[i].Update();
	}

}
void FDGame::EffectFire::Init()
{
	_x = 0;
	_y = 0;
	_state = 0;
	_spriteid = -1;
	_time_hold = 0;
}
void FDGame::EffectFire::Active(int sprite, int anim, int size, float x, float y, int timehold)
{
	_x = x;
	_y = y;
	_size = size;
	_spriteid = sprite;
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(anim, false);
	_anim.SetPos(_x, _y);
	_state = 1;
	_time_hold = timehold;
}
void FDGame::EffectFire::Render(float x, float y)
{
	if (_state == 0)
	{
		return;
	}
	if (_time_hold > 0)
	{
		return;
	}
	float scale = 1.0f;
	if (_size == 1)
	{
		scale = 0.3f;
	}
	else if (_size == 2)
	{
		scale = 0.6f;
	}
	_anim.SetPos(_x + x, _y + y);
	G()->SetScale(scale, scale, _x + x, _y + y);
	_anim.DrawAnimObject(G());
	G()->ClearScale();
}
void FDGame::EffectFire::Update()
{
	if (_state == 0)
	{
		return;
	}
	_time_hold--;
	if (_time_hold > 0)
	{
		return;
	}
	_anim.UpdateAnimObject();
	if (_anim.IsCurrentAnimStop())
	{
		_state = 0;
	}
}

void FDGame::SBFire::Init()
{
	_index = -1;
	_time_hold = 0;
}
void FDGame::SBFire::Active(int index, int timehold)
{
	_index = index;
	_time_hold = timehold;
}
void FDGame::SBFire::Update()
{
	if (_index == -1)
	{
		return;
	}
	_time_hold--;
	if (_time_hold < 0)
	{
		_time_hold = 0;
	}
}
void FDGame::SuperBlastFireInit()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_sb_fire[i].Init();
	}
}
void FDGame::SuperBlastFireAdd(int index, int timehold)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_sb_fire[i]._index == -1)
		{
			_sb_fire[i].Active(index, timehold);
			break;
		}

	}
}
void FDGame::SuperBlastFireUpdate()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_sb_fire[i].Update();
		if (_sb_fire[i]._index != -1 && _sb_fire[i]._time_hold == 0)
		{
			int row = _sb_fire[i]._index / FD_NUM_ROW;
			int col = _sb_fire[i]._index%FD_NUM_COL;
			_character_arr[row][col].Fire(true);
			_sb_fire[i]._index = -1;
		}
	}
}

bool FDGame::SuperBlastFiring()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_sb_fire[i]._index != -1)
		{
			return true;
		}
	}
	return false;
}
void FDGame::FriendRank::Init(float x, float y, int rank, bool up, PineImage avatar, int score, int avatarstate)
{
	_x = x;
	_y = y;
	_rank = rank;
	_speed = 0;
	_speed_max = 10;
	_state = -1;
	_rank_up = up;
	_tarx1 = 0;
	_tary1 = 0;
	_tarx = 0;
	_tary = 0;
	_avatar_state = avatarstate;
	_avatar = avatar;
	_score = score;
}
void FDGame::FriendRank::Render(float x, float y, int opac)
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	if (_state == -1)
	{
		return;
	}
	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 120, x + _x, y + _y);
	//G()->SetScale(1.0f, 1.0f, x + _x - 101, y + _y - 101);
	if (_avatar_state == AVATAR_STATE_INIT_SUCCESS)
	{
		float im_scale = 200.0f / _avatar.GetWidth();
		G()->SetScale(im_scale, im_scale, x + _x - 101, y + _y - 101);
		_avatar.DrawImageWithRect(G(), x + _x - 101, y + _y - 101);
		G()->ClearScale();
	}
	else
	{
		G()->SetScale(2.0f, 2.0f, x + _x, y + _y);
		GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 126, x + _x, y + _y + 5);
		G()->ClearScale();
	}

	//G()->ClearScale();
	GAME()->_freeTypeText.SetText(_userNameUnicode, _userNameUnicodeLenght, 6);
	int stringX = _x + x;
	int stringY = _y + y - 170;
	G()->SetScale(1.2f, 1.2f, stringX, stringY);
	GAME()->_freeTypeText.Render(stringX, stringY, _HCENTER);
	G()->ClearScale();

	int spacing = GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = 3;
	GET_SPRITE(SPRITEID_FONT_POP_UP_01)->DrawAnsiTextFormat(G(), stringX, stringY + 30, _HCENTER | _VCENTER, "#%d", _rank);
	GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = spacing;

	stringY = _y + y + 150;
	spacing = GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = 5;
	GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->DrawAnsiTextFormat(G(), stringX, stringY, _HCENTER | _VCENTER, "%d", _score);
	GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = spacing;

	if (_rank_up)
	{
		GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 121, x + _x, y + _y);
	}
	else
	{
		GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 122, x + _x, y + _y);
	}
}
void FDGame::FriendRank::Update()
{
	if (_state == -1)
	{
		return;
	}
	switch (_state)
	{
	case 1:
	{
		if (_speed < _speed_max)
		{
			_speed *= 1.1f;
		}
		else
		{
			_speed = _speed_max;
		}
		float angle = CMath::ANGLE(_x, _y, _tarx, _tary);
		if (!CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _speed))
		{
			_x += _speed*CMath::COS(angle);
			_y += _speed*CMath::SIN(angle);
		}
		else
		{
			_state = 2;
			_x = _tarx;
			_y = _tary;
		}
	}
	break;
	case 2:
	{
		if (_speed < _speed_max)
		{
			_speed *= 1.1f;
		}
		else
		{
			_speed = _speed_max;
		}
		float angle = CMath::ANGLE(_x, _y, _tarx1, _tary1);
		if (!CHECK_POINT_IN_CIRCLE(_x, _y, _tarx1, _tary1, _speed))
		{
			_x += _speed*CMath::COS(angle);
			_y += _speed*CMath::SIN(angle);

		}
		else
		{
			_speed = 0;
			_state = 0;
			_x = _tarx1;
			_y = _tary1;
			if (_rank_up)
			{
				_rank--;
			}
			else
			{
				_rank++;
			}
		}
	}
	break;
	case 3:
	{
		if (_speed > 0)
		{
			_speed *= 0.98f;
		}
		if (_speed <= 0.1f)
		{
			_speed = 0.1f;
		}
		float angle = CMath::ANGLE(_x, _y, _tarx, _tary);
		if (!CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _speed))
		{
			_x += _speed*CMath::COS(angle);
			_y += _speed*CMath::SIN(angle);
		}
		else
		{
			_speed = 0;
			_state = 0;
			_x = _tarx;
			_y = _tary;
		}
	}
	break;
	default:
		break;
	}
}
void FDGame::FriendRank::SetRank(int rank)
{
	_rank = rank;
}
void FDGame::FriendRank::Run(float tarx, float tary, float tarx1, float tary1)
{
	_state = 1;
	if (_speed == 0)
	{
		_speed = 1;
	}
	_tarx = tarx;
	_tary = tary;
	_tarx1 = tarx1;
	_tary1 = tary1;
}
void FDGame::FriendRank::Stop()
{
	if (_state != 0)
	{
		_state = 3;
	}
}
void FDGame::RankUpInit()
{
	/*float x = k_SCREEN_WIDTH/2 - 140;
	float y = k_SCREEN_HEIGHT/2 - 120;
	int rank = 0;
	char url[1024];
	sprintf(url, "%s", "url");
	_friend_rank.Init(x, y, rank, url,false);
	x = k_SCREEN_WIDTH / 2 + 140;
	y = k_SCREEN_HEIGHT / 2 + 120;
	rank = 1;
	_my_rank.Init(x, y, rank, url, true);*/
	for (int i = 0; i < RANK_UP_NUM_BUTTON; i++)
	{
		int anim_start = 0;
		if (i == RANK_UP_BUTTON_CLOSE)
		{
			anim_start = 79;
		}
		_rank_up_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
		_rank_up_button[i].SetAnim(SPRITEID_HUD_RESULT_01, anim_start + i * 4);
		_rank_up_button[i].SetPos(0, 0);
	}
	_rankup_scroll_state = 0;
	_rankup_scrolly = k_SCREEN_HEIGHT;
	_rankup_scroll_speed = 70;
	_rank_up_is_close = false;
	_rank_up_particle.Init();
}
void FDGame::RankUpActive()
{
	_rankup_scroll_state = 0;
	_rankup_scrolly = k_SCREEN_HEIGHT;
	_rankup_scroll_speed = 70;
	_rank_up_is_close = false;

}
void FDGame::RankUpRender(float x, float y, int opac)
{
	if (!_rank_up_active)
	{
		return;
	}
	G()->SetColor(0xf50f1d24);
	G()->FillRect(_point_o.X, _point_o.Y + _result_fill_scrolly, _screen_width, _screen_height - _result_fill_scrolly, true);
	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 115, x, y - _rankup_scrolly);

	_friend_rank.Render(x, y - _rankup_scrolly, opac);
	_my_rank.Render(x, y - _rankup_scrolly, opac);
	for (int i = 0; i < RANK_UP_NUM_BUTTON; i++)
	{
		int posy = 0;
		int posx = 0;
		if (i == RANK_UP_BUTTON_SHARE)
		{
			posy = 110;
			posx = 60;
		}
		_rank_up_button[i].SetPos(0 + posx, 0 - _rankup_scrolly + posy);
		_rank_up_button[i].Render(G());
		_rank_up_button[i].Update();
	}
	_rank_up_particle.Render(x, y, opac);
}
void FDGame::RankUpUpdate()
{
	_rank_up_particle.Update();
	if (!_rank_up_active)
	{
		return;
	}

	_friend_rank.Update();
	_my_rank.Update();
	if (_rank_up_active)
	{
		if (!_rank_up_is_close)
		{
			switch (_rankup_scroll_state)
			{
			case 0:
				if (_rankup_scrolly > -150)
				{
					if (_rankup_scrolly <= -50)
					{
						_rankup_scroll_speed *= 0.5f;
						if (_rankup_scroll_speed < 5)
						{
							_rankup_scroll_speed = 5;
						}
					}
					//printf("\n 000 speed = %f - scrolly = %f", _rankup_scroll_speed, _rankup_scrolly);
					_rankup_scrolly -= _rankup_scroll_speed;
					if (_rankup_scrolly <= -150)
					{
						_rankup_scroll_state = 1;
						_rankup_scroll_speed = 20;
					}
				}
				break;
			case 1:
				if (_rankup_scrolly < 0)
				{
					if (_rankup_scrolly >= -70)
					{
						_rankup_scroll_speed *= 0.7f;
					}
					if (_rankup_scroll_speed <= 0.2f)
					{
						_rankup_scroll_speed = 0.2f;
					}
					//printf("\n 111 _rankup_scroll_speed = %f", _rankup_scroll_speed);
					_rankup_scrolly += _rankup_scroll_speed;
					if (_rankup_scrolly >= 0)
					{
						_rankup_scroll_state = 2;
						_rankup_scrolly = 0;
						_friend_rank.Run(k_SCREEN_WIDTH / 2 - 140, k_SCREEN_HEIGHT / 2 + 120, k_SCREEN_WIDTH / 2 + 140, k_SCREEN_HEIGHT / 2 + 120);
						_my_rank.Run(k_SCREEN_WIDTH / 2 + 140, k_SCREEN_HEIGHT / 2 - 120, k_SCREEN_WIDTH / 2 - 140, k_SCREEN_HEIGHT / 2 - 120);
					}
				}

				break;

			default:
				break;
			}
		}
		else
		{
			if (_rankup_scrolly < k_SCREEN_HEIGHT)
			{
				_rankup_scrolly += 70;
			}
			else
			{
				_rank_up_is_close = false;
				_rank_up_active = false;
			}
		}
		RankUpUpdateTouch();
	}
}
bool FDGame::RankUpUpdateTouch()
{
	bool press = false;
	bool ret = false;
	int index = -1;
	if (_dialogMng.isOpenDialog() || _gift_render_state > 0 || _menu_ext_active || _menu_ext_touch_id != -1 || GAME()->_purchase_procesing_opac > 0)
	{
		return false;
	}
#if defined (MAC_OS)
	if ((PineAdBox::showAdState == SHOW_AD_PROCESSING || PineAdBox::showAdState == SHOW_AD_COMPLETED) && _want_to_retry)
	{
		if (PineAdBox::showAdState != SHOW_AD_COMPLETED)
		{
			return  false;
		}
	}
#endif
	for (int i = 0; i < RANK_UP_NUM_BUTTON; i++)
	{
		if (_rank_up_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_rank_up_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case RANK_UP_BUTTON_SHARE:
				_share_score_state = 1;
				_share_score_opac = 0;
				_share_score_type = FB_SHARE_TYPE_RANK;
				break;
			case RANK_UP_BUTTON_CLOSE:
				_rank_up_is_close = true;
				_rank_up_particle.ForceEnd();
				break;
			default:
				break;
			}
			if (_rank_up_button[index]._state != BUTTON_STATE_LOCK && _rank_up_button[index]._state != BUTTON_STATE_HIDE)
			{
				_rank_up_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < RANK_UP_NUM_BUTTON; touch++)
		{
			if (_rank_up_button[touch]._state == BUTTON_STATE_LOCK || _rank_up_button[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}
			int w = _rank_up_button[touch].TZ_GetWidth();
			int h = _rank_up_button[touch].TZ_GetHeight();
			int x = _rank_up_button[touch].TZ_GetX();
			int y = _rank_up_button[touch].TZ_GetY();
			Button *button = &_rank_up_button[touch];
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
						PLAY_SFX(SFX_BUTTON);
					}
					else
					{
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
					}
					else
					{
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
		}

	}
	return press;
}

void FDGame::ParticleColor::Init()
{
	_num = 0;
	_x_center = 0;
	_y_center = 0;
	_width = 0;
	for (int i = 0; i < NUM_PARTICLE_COLOR; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_scale[i] = 0;
		_scale_max[i] = 0;
		_speed_scale[i] = 0;
		_opac[i] = 0;
		_opac_speed[i] = 0;
		_dir[i] = 0;
		_speed_dir[i] = 0;
		_color[i] = 0;
		_state[i] = 0;
		_speed[i] = 0;
	}
}
void FDGame::ParticleColor::Active(float x, float y, int width, float speed, float speedscale, int speedopac, int num)
{
	PLAY_SFX(SFX_BEATING_FRIEND);
	_num = num;
	_x_center = x;
	_y_center = y;
	_width = width;
	_speed_main = speed;
	for (int i = 0; i < _num; i++)
	{
		float deltaw = _width / 20.0f;
		_x[i] = _x_center + CMath::RANDOM(-10, 10) * deltaw;
		_y[i] = _y_center - CMath::RANDOM(0, 30) * 40;
		_speed[i] = _speed_main + _speed_main / CMath::RANDOM(2, 10);
		_scale_max[i] = CMath::RANDOM(5, 10) / 10.0f;
		_scale[i] = CMath::RANDOM(1, 10) / 10.0f;
		_scale_out[i] = true;
		if (_scale[i] > _scale_max[i])
		{
			_scale[i] = _scale_max[i];
			_scale_out[i] = false;
		}
		_speed_scale[i] = speedscale + speedscale / CMath::RANDOM(4, 10);
		_opac[i] = 100;
		_opac_speed[i] = speedopac + speedopac / CMath::RANDOM(4, 10);
		_dir[i] = CMath::RANDOM(0, 1);
		_speed_dir[i] = _speed[i] / 3.5f;
		int type_color = CMath::RANDOM(0, 5);
		COLOR color = 0xffff0000;
		switch (type_color)
		{
		case CHARACTER_TYPE_LILY:
			color = 0xffffffff;
			break;
		case CHARACTER_TYPE_LOTUS:
			color = 0xffff5bc2;
			break;
		case CHARACTER_TYPE_ORCHILD:
			color = 0xff0cecef;
			break;
		case CHARACTER_TYPE_ROSE:
			color = 0xffff0000;
			break;
		case CHARACTER_TYPE_SUNFLOWER:
			color = 0xfff4d400;
			break;
		case CHARACTER_TYPE_VOILET:
			color = 0xffd804ff;
			break;
		default:
			break;
		}
		_color[i] = color;
		_state[i] = 1;
	}
}
void FDGame::ParticleColor::Render(float x, float y, int opac)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		G()->SetOpacity(_opac[i] * opac / 100.0f);
		G()->SetScale(_scale[i], _scale_max[i], _x[i] + x, _y[i] + y);
		G()->SetColorMask(_color[i]);
		GET_SPRITE(SPRITEID_EFFECT_EXPLOSION_01)->DrawFrame(G(), 53, _x[i] + x, _y[i] + y);
		G()->ClearColorMask();
		G()->ClearScale();
		G()->SetOpacity(opac);
	}
}
void FDGame::ParticleColor::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		if (_scale_out[i])
		{
			_scale[i] += _speed_scale[i];
			if (_scale[i] > _scale_max[i])
			{
				_scale[i] = _scale_max[i];
				_scale_out[i] = false;
			}
		}
		else
		{
			_scale[i] -= _speed_scale[i];
			if (_scale[i] <= 0)
			{
				_scale[i] = 0;
				_scale_out[i] = true;
			}
		}
		if (_dir[i] == 0)
		{
			_x[i] += _speed_dir[i];
		}
		else
		{
			_x[i] -= _speed_dir[i];
		}
		_y[i] += _speed[i];
		if (_y[i] > 0)
		{
			_speed[i] *= 1.01f;
		}

		if (_y[i] >= k_SCREEN_HEIGHT / 2)
		{
			_opac[i] -= _opac_speed[i];
			if (_state[i] == 2)
			{
				_opac[i] -= 10;
				if (_opac[i] <= 0)
				{
					_state[i] = 0;
				}
			}
			else
			{
				if (_opac[i] <= 0)
				{
					float deltaw = _width / 20.0f;
					_x[i] = _x_center + CMath::RANDOM(-10, 10) * deltaw;
					_y[i] = _y_center - CMath::RANDOM(0, 10) * 20;
					_scale_max[i] = CMath::RANDOM(5, 10) / 10.0f;
					_scale[i] = CMath::RANDOM(1, 10) / 10.0f;
					_scale_out[i] = true;
					_speed[i] = _speed_main + _speed_main / CMath::RANDOM(2, 10);
					if (_scale[i] > _scale_max[i])
					{
						_scale[i] = _scale_max[i];
						_scale_out[i] = false;
					}
					_opac[i] = 100;
					_dir[i] = CMath::RANDOM(0, 1);
					_speed_dir[i] = _speed[i] / 3.5f;
				}
			}
		}
	}
}
void FDGame::ParticleColor::ForceEnd()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state != 0)
		{
			_state[i] = 2;
		}
	}
}