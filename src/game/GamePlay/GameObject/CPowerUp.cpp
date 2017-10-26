#include "pch.h"
#include"../../GameCore/CGame.h"

CPowerUp::CPowerUp()
{
}

CPowerUp::~CPowerUp()
{
}
void CPowerUp::Reset()
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_energy = 0;
	_button.Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
	_button.SetAnim(SPRITEID_HUD_POWERUP_01, 12 + _id * 4);
	_button.SetPos(0, 0);
	InitScale();
	_has_actived = false;
	_clip_current = 0;
	_clip_target = 0;
	_just_full = false;
	/*_effect_explo.Init(SPRITEID_EFFECT_EXPLOSION_01, EXPLOSION_TYPE_CIRCLE);
	_icon.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_POWERUP_01), SPRITEID_HUD_POWERUP_01);
	if (_num > 0)
	{
		_icon.SetAnim(13 + _id * 4, true);
	}
	else
	{
		_icon.SetAnim(12 + _id * 4, true);
	}
	_icon.SetPos(_x, _y);*/
}
void CPowerUp::Init(int id, int energy, int num, float x, float y)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_id = id;
	_energy_max = game->GetDesignGeneral(6 + _id);
	_energy = energy;
	_x = x;
	_y = y;
	_num = num;
	_button.Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
	_button.SetAnim(SPRITEID_HUD_POWERUP_01, 12 + _id*4);
	_button.SetPos(0,0);
	InitScale();
	_has_actived = false;
	_clip_current = 0;
	_clip_target = 0;
	_just_full = false;
	/*_effect_explo.Init(SPRITEID_EFFECT_EXPLOSION_01,EXPLOSION_TYPE_CIRCLE);
	_icon.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_POWERUP_01), SPRITEID_HUD_POWERUP_01);
	if (_num > 0)
	{
		_icon.SetAnim(13 + _id * 5, true);
	}
	else
	{
		_icon.SetAnim(12 + _id * 5, true);
	}
	_icon.SetPos(_x, _y);*/
	_anim_energy_full.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_POWERUP_01), SPRITEID_HUD_POWERUP_01);
	_anim_energy_full.SetAnim(0, false);
	_anim_energy_full.SetPos(_x, _y);
}
void CPowerUp::Render(float x, float y)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	
	//GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 3 + _id, x, y);
	int clipx = 0;
	int clipy = 0;
	
	int mx = 0;
	int my = 0;
	
	int mh = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFrameHeight(6);
	/*int cliph = ((game->_energy_total*1.0f) / (_energy_max*1.0f))*mh;
	if (cliph >= mh)
	{
		cliph = mh;
	}
	_clip_target = cliph;
	if (_clip_current < _clip_target)
	{
		_just_full = false;
		_clip_current += 2;
		if (_clip_current >= _clip_target)
		{
			_clip_current = _clip_target;
		}
	}
	if (_clip_current > _clip_target)
	{
		_clip_current -= 2;
		if (_clip_current <= _clip_target)
		{
			_clip_current = _clip_target;
		}
	}
	if (_clip_current >= mh)
	{
		if (!_just_full)
		{
			_just_full = true;
			_effect_explo.Active(0, mx, my, 7.0f, 0.04f, 30, 54, 1, 10);
			_anim_energy_full.SetAnim(24, false);
			
		}
		if (_icon.GetCurrentAnim() != (14 + _id * 5))
		{
			_icon.SetAnim(14 + _id * 5, true);
		}
		if (_anim_energy_full.IsCurrentAnimStop())
		{
			if (_anim_energy_full.GetCurrentAnim() == 24)
			{
				_anim_energy_full.SetAnim(25, true);
			}
		}
		_anim_energy_full.SetPos(mx, my);
	}*/
	int board_frame = 8;
	//if (_energy_max <= game->_energy_total)
	//{
	//	if (_icon.GetCurrentAnim() != (14 + _id * 5))
	//	{
	//		_icon.SetAnim(14 + _id * 5, true);
	//	}
	//}
	//else
	//{
	//	if(_num > 0)
	//	{ 
	//		if (_icon.GetCurrentAnim() != (13 + _id * 5))
	//		{
	//			_icon.SetAnim(13 + _id * 5, true);
	//		}
	//	}
	//	//board_frame = 62;
	//	_anim_energy_full.SetAnim(0, true);
	//}
	/*G()->SetClip(clipx, clipy + my + mh / 2 - _clip_current, k_SCREEN_WIDTH, _clip_current);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 6, x + mx, y + my);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	G()->ClearClip();*/
	float dis1 = CMath::ABSOLUTE_VALUE((mh / 2.0f) - _clip_current);
	float scale = 1.0f - (dis1 / (mh/2.0f));
	if (scale <= 0)
	{
		scale = 0;
	}
	if (scale < 1.0f)
	{
		scale *= ((2.0f - scale));
	}
	if(scale >= 0.85f)
	{
		scale = 0.85f;
	}
	G()->SetScale(scale, scale, x + mx, y + my + mh / 2 - _clip_current);
	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 7, x + mx, y + my + mh / 2 - _clip_current);
	G()->ClearScale();
	_button.Render(G());
	_button.Update();
	//_icon.DrawAnimObject(G());
	
	/*if (_anim_energy_full.GetCurrentAnim() >= 24)
	{
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		_anim_energy_full.DrawAnimObject(G());
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
	_anim_energy_full.UpdateAnimObject();*/
	/*int frame_index = _icon._anim_sprite->_aframe_id[_icon._current_aframes];
	mx = _icon._anim_sprite->GetFModuleCenterX(frame_index, 0);
	my = _icon._anim_sprite->GetFModuleCenterY(frame_index, 0);*/
	int spacing = GET_SPRITE(SPRITEID_FONT_POWER_UP_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_POWER_UP_01)->_char_spacing = 3;
	//GET_SPRITE(SPRITEID_FONT_POWER_UP_01)->DrawAnsiTextFormat(G(), mx + _x + x + 30, my + _y + y + 11, _HCENTER | _VCENTER, ":%d", _num);
	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), board_frame + _id, x, y);
	mx = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterX(8 + _id, 0);
	my = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterY(8 + _id, 0);
	//GET_SPRITE(SPRITEID_FONT_POWER_UP_01)->DrawAnsiTextFormat(G(), mx + _x + x, my + _y + y, _HCENTER|_VCENTER, "%d", _energy_max);
	GET_SPRITE(SPRITEID_FONT_POWER_UP_01)->DrawAnsiTextFormat(G(), mx + _x + x, my + _y + y, _HCENTER | _VCENTER, "%d", _num);
	GET_SPRITE(SPRITEID_FONT_POWER_UP_01)->_char_spacing = spacing;
	
	if (_scale > 0)
	{
		int posx;
		int posy;
		int frame;
		char text[128];
		switch (_id)
		{
		case POWER_UP_TYPE_CREATE_GROUP:
			sprintf(text, "%s", "Tap on screen to turn random flowers into huge flowers.");
			break;
		case POWER_UP_TYPE_SORT:
			sprintf(text, "%s", "Tap on screen to rearrange these flowers on the field.");
			break;
		case POWER_UP_TYPE_EAT_RANDOM:
			sprintf(text, "%s", "Tap on any flower to destroy all flowers of same kind.");
			break;

		default:
			break;
		}
		posx = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterX(0 + _id, 1);
		posy = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterY(0 + _id, 1);
		frame = 0 + _id;
		G()->SetScale(_scale, _scale, posx + x, posy + y);
		GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), frame, x, y);
		spacing = GET_SPRITE(SPRITEID_FONT_INFO_POWERUPL)->_char_spacing;
		GET_SPRITE(SPRITEID_FONT_INFO_POWERUPL)->_char_spacing = 3;
		GET_SPRITE(SPRITEID_FONT_INFO_POWERUPL)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2 + x, posy + y - 36, _HCENTER | _VCENTER, "%s", text);
		GET_SPRITE(SPRITEID_FONT_INFO_POWERUPL)->_char_spacing = spacing;
		G()->ClearScale();
	}
	//_effect_explo.Render(x, y, 100);
}
void CPowerUp::Update()
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	if (_num <= 0 /*&& game->_energy_total < _energy_max*/)
	{
		if (_button._state != BUTTON_STATE_LOCK)
		{
			_button.SetState(BUTTON_STATE_LOCK);
			//_icon.SetAnim(12 + _id * 5, true);
		}
	}
	else
	{
		if (_button._state == BUTTON_STATE_LOCK)
		{
			_button.SetState(BUTTON_STATE_NORMAL);
			//_icon.SetAnim(13 + _id * 5, true);
		}

	}
    //_icon.UpdateAnimObject();
    if(_id == POWER_UP_TYPE_EAT_RANDOM)
    {
        _energy_max = game->GetDesignGeneral(6 + _id) + (game->GetDesignGeneral(FD_GENERAL_DESIGN_XENERGY_PARAM)*game->_use_power_up_num)*0.2f;
    }
    else
    {
        _energy_max = game->GetDesignGeneral(6 + _id) + game->GetDesignGeneral(FD_GENERAL_DESIGN_XENERGY_PARAM)*game->_use_power_up_num;
    }
	
	UpdateScale();
	//_effect_explo.Update();
}
bool CPowerUp::UpdateTouch()
{
	
	bool press = false;
	bool ret = false;
	int index = -1;
	if (_button._state == BUTTON_STATE_ACTIVE)
	{
		ret = true;
	}
	if (ret)
	{
		if (_button.NeedAction())
		{
			//printf("\n index = %d",index);
			switch (_id)
			{
			case POWER_UP_TYPE_CREATE_GROUP:
				break;
			case POWER_UP_TYPE_SORT:
				//game->SortFlower();
				break;
			case POWER_UP_TYPE_EAT_RANDOM:
				break;
			default:
				break;
			}
			if (_button._state != BUTTON_STATE_LOCK && _button._state != BUTTON_STATE_HIDE)
			{
				_button.SetState(BUTTON_STATE_NORMAL);
				//_icon.SetAnim(13 + _id * 5, true);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < GAMEPLAY_BUTTON_NUM; touch++)
		{
			int x = _button.TZ_GetX();
			int y = _button.TZ_GetY();
			int w = _button.TZ_GetWidth();
			int h = _button.TZ_GetHeight();
			Button *button = &_button;
			if (_button._state == BUTTON_STATE_LOCK )
			{
				continue;
			}
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
						//_has_actived = !_has_actived;
						/*if (!_has_actived)
						{
							_has_actived = true;
							ActiveScale(0, 1.15f, 0.9f, 0.1f, 1);
						}
						else
						{
							_has_actived = false;
							ActiveScale(1.0f, 1.25f, 0.8f, 0.1f, 4);
						}*/
						//TOUCHS(i)->_state = TOUCH_DRAG;
						//_icon.SetAnim(12 + _id * 5, true);
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
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						press = true;
						//button->SetState(BUTTON_STATE_NORMAL);
						button->SetState(BUTTON_STATE_ACTIVE);
						if (!_has_actived)
						{
							_has_actived = true;
							ActiveScale(0, 1.0f, 1.0f, 0.1f, 1);
						}
						else
						{
							_has_actived = false;
							ActiveScale(1.0f, 1.1f, 1.0f, 0.1f, 4);
						}
						button->_touch_id = -1;
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
					}
				}
				else if(TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
			}
		}

	}
	return press;
}

void CPowerUp::InitScale()
{
	_scale = 0;
	_scale_max = 0;
	_scale_min = 0;
	_speed_scale = 0;
	_state_text_scale = 0;
}
void CPowerUp::ActivedFunc(bool cancel)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_has_actived = false;
	if (!cancel)
	{
		game->_use_power_up_num++;
		if (_energy_max <= game->_energy_total)
		{
			game->AddEnergy(-_energy_max);
		}
		else
		{
			_num--;
			if (_num <= 0)
			{
				_num = 0;
			}
		}
	}
}
void CPowerUp::ActiveScale(float scale, float scalemax, float scalemin, float speedscale, int scalestate)
{
	_scale = scale;
	_scale_max = scalemax;
	_scale_min = scalemin;
	_speed_scale = speedscale;
	_state_text_scale = scalestate;
}
void CPowerUp::UpdateScale()
{
	switch (_state_text_scale)
	{
	case 0:
		break;
	case 1:
		if (_scale < _scale_max)
		{
			_scale += _speed_scale;
			if (_scale >= _scale_max)
			{
				_state_text_scale = 2;
			}
		}
		break;
	case 2:
		if (_scale > _scale_min)
		{
			_scale -= _speed_scale;
			if (_scale <= _scale_min)
			{
				_state_text_scale = 3;
			}
		}
		break;
	case 3:
		if (_scale < 1.0f)
		{
			_scale += _speed_scale;
			if (_scale >= 1.0f)
			{
				_scale = 1.0f;
				_state_text_scale = 0;
			}
		}
		break;
	case 4:
		if (_scale < _scale_max)
		{
			_scale += _speed_scale/2;
			if (_scale >= _scale_max)
			{
				_state_text_scale = 5;
			}
		}
		break;
	case 5:
		if (_scale > 0)
		{
			_scale -= _speed_scale*2;
			if (_scale <= 0)
			{
				_scale = 0;
				_state_text_scale = 0;
			}
		}
		break;
	default:
		break;
	}
}