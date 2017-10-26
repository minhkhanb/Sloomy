#include "pch.h"
#include "../../GameCore/CGame.h"

CCharacter::CCharacter()
{
}

CCharacter::~CCharacter()
{
}

void CCharacter::Init(float x, float y)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_type = CHARACTER_TYPE_NULL;
	_state = CHARACTER_STATE_EMPTY;
	SetPos(x, y);
	SetTar(x, y);
	_spriteid = game->_character_sprite[0];
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(0, true);
	_touch_id = -1;
	_disable_hold_time = 0;
	SetScaleState(CHARACTER_SCALE_STATE_NORMAL);
	SetSize(CHARACTER_SIZE_NORMAL);
	_speed_scale = 0.04f;
	_xp_collected = 0;
	_scale_min = 0.8;
	_scale_max = 1.1f;
	_size_tar = _size;
}
void CCharacter::CreateNew(int type, int timehold,float speedscale)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_type = type;
	_state = CHARACTER_STATE_NORMAL;
	_spriteid = game->_character_sprite[_type*3];
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(CMath::RANDOM(0,1), false);
	_anim.SetPos(_x, _y);
	_touch_id = -1;
	_disable_hold_time = timehold;
	SetScaleState(CHARACTER_SCALE_STATE_OUT);
	SetSize(CHARACTER_SIZE_NORMAL);
	_speed = 30;
	_speed_scale = speedscale;
	_size_tar = _size;
	//printf("\n _disable_hold_time = %d", _disable_hold_time);
}
void CCharacter::Modify(int type)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_type = type;
	_spriteid = game->_character_sprite[_type * 3];
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(CMath::RANDOM(0, 1), false);
	SetSize(CHARACTER_SIZE_NORMAL);
}
void CCharacter::Modify(CCharacter *flower)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_type = flower->GetType();
	_spriteid = game->_character_sprite[_type*3];
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(CMath::RANDOM(0, 1), false);
	_disable_hold_time = flower->_disable_hold_time;
	SetPos(flower->GetPosX(), flower->GetPosY());
	_xp_collected = flower->_xp_collected;
	//SetScaleState(flower->_scale_state);
	//SetSize(flower->_size);
	//_scale = flower->_scale;
	_state = flower->GetState();
}
void CCharacter::AddRender(float x, float y, int opac)
{
	if (_state == CHARACTER_STATE_EMPTY)
	{
		return;
	}
	FDGame* game = (FDGame*)(GAME()->_current_game);
	game->_draw_manager.Add(&_anim, _x + x, _y + y, _scale);
	G()->SetOpacity(opac);
}
void CCharacter::Render(float x, float y, int opac)
{
	
	if (_state == CHARACTER_STATE_EMPTY)
	{
		return;
	}
	_anim.SetPos(_x + x, _y + y);
	G()->SetScale(_scale, _scale, _x + x, _y + y);
	_anim.DrawAnimObject(G());
	G()->ClearScale();
	G()->SetOpacity(opac);
}
void CCharacter::Update()
{
	_anim.UpdateAnimObject();
	if (_anim.IsCurrentAnimStop())
	{
		if (_anim.GetCurrentAnim() == 2)
		{
			_anim.SetAnim(CMath::RANDOM(0, 1), false);
		}
		else
		{
			_anim.SetAnim(2, false);
		}
	}
	//printf("\n _disable_hold_time = %d", _disable_hold_time);
	if (_disable_hold_time > 0)
	{
		_disable_hold_time--;
		return;
	}
	if (_y < GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0))
	{
		_speed = 50;
	}
	else
	{
		_speed = 30;
	}
	switch (_state)
	{
	case CHARACTER_STATE_NORMAL:
		break;
	case CHARACTER_STATE_DISABLE:
		break;
	case CHARACTER_STATE_EMPTY:
		break;
	case CHARACTER_STATE_MOVING:
	{
		
		if (CHECK_POINT_IN_CIRCLE(_x,_y,_tarx,_tary,_speed))
		{
			_x = _tarx;
			_y = _tary;
			_state = CHARACTER_STATE_NORMAL;
		}
		else
		{
			float angle = CMath::ANGLE(_x, _y, _tarx, _tary);
			_x += _speed*CMath::COS(angle);
			_y += _speed*CMath::SIN(angle);
		}
	}
		break;
	case CHARACTER_STATE_FIRE:
		SetState(CHARACTER_STATE_EMPTY);
		break;
	case CHARACTER_STATE_ACTIVE:
		break;
	case CHARACTER_STATE_TOUCH_IT:
		if (_anim.GetCurrentAnim() != 4)
		{
			_anim.SetAnim(4, false);
			SetState(CHARACTER_STATE_NORMAL);
		}
		break;
	default:
		break;
	}
	switch (_scale_state)
	{
	case CHARACTER_SCALE_STATE_NORMAL:
		if (!CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _speed))
		{
			_state = CHARACTER_STATE_MOVING;
		}
		break;
	case CHARACTER_SCALE_STATE_OUT:
		if (_scale < _scale_max)
		{
			//_scale += 0.06f;
			_scale += _speed_scale*1.5;
			if (_scale >= _scale_max)
			{
				_scale_state = CHARACTER_SCALE_STATE_IN;
			}
		}

		break;
	case CHARACTER_SCALE_STATE_IN:
		if (_scale > _scale_min)
		{
			//_scale -= 0.06f;
			_scale -= _speed_scale*1.5;
			if (_scale <= _scale_min)
			{
				_scale_state = CHARACTER_SCALE_STATE_TO_NORMAL;
			}
		}
		break;
	case CHARACTER_SCALE_STATE_TO_NORMAL:
		if (_scale < 1.0f)
		{
			//_scale += 0.04f;
			_scale += _speed_scale;
			if (_scale >= 1.0f)
			{
				_scale = 1.0f;
			}
		}
		else
		{
			_scale_state = CHARACTER_SCALE_STATE_NORMAL;
		}
		break;
	case CHARACTER_SCALE_STATE_HIDE:
		if (_scale > 0)
		{
			_scale-= _speed_scale*1.5f;
		}
		else
		{
			_scale = 0.0f;
			_size = _size_tar;
			//_scale_state = CHARACTER_SCALE_STATE_NORMAL;
		}
		break;
	default:
		break;
	}
	if (_state == CHARACTER_STATE_DISABLE)
	{
		_state = CHARACTER_STATE_NORMAL;
		_touch_id = -1;
	}
}
void CCharacter::SetPos(float x, float y)
{
	_x = x;
	_y = y;
	_anim.SetPos(_x, _y);
}
void CCharacter::SetTar(float x, float y)
{
	_tarx = x;
	_tary = y;
}
float CCharacter::GetTarX()
{
	return _tarx;
}
float CCharacter::GetTarY()
{
	return _tary;
}
void CCharacter::SetType(int type)
{
	_type = type;
}
void CCharacter::SetState(int state)
{
	_state = state;
}
int CCharacter::GetState()
{
	return _state;
}
int CCharacter::GetType()
{
	return _type;
}
float CCharacter::GetPosX()
{
	return _x;
}
float CCharacter::GetPosY()
{
	return _y;
}
int CCharacter::GetSpriteID()
{
	return _spriteid;
}
bool CCharacter::UpdateTouch()
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	bool press = false;
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		int x = _x - FD_SLOT_WIDTH/2;
		int y = _y - FD_SLOT_WIDTH/2 - 25;
#if defined (ANDROID_OS)
		y = _y - FD_SLOT_WIDTH / 2 - 25 -(GAME()->increa);
#endif
		int w = FD_SLOT_WIDTH;
		int h = FD_SLOT_WIDTH;
		if (_state != CHARACTER_STATE_NORMAL)
		{
			continue;
		}
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (_touch_id == -1)
				{
					_touch_id = TOUCHS(i)->_id;
					TOUCHS(i)->_state = TOUCH_DRAG;
					press = true;
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
				if (_touch_id == -1)
				{
					_touch_id = TOUCHS(i)->_id;
					//TOUCHS(i)->_state = TOUCH_DRAG;
					press = true;
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_touch_id == TOUCHS(i)->_id)
				{
					_touch_id = -1;
				}
			}
		}
		else
		{
			if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_touch_id == TOUCHS(i)->_id)
				{
					_touch_id = -1;
				}
			}
		}
	}
	return press;
}
void CCharacter::Fire(bool spblast)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	int param = CGameDesign::FileGeneral_design::CSheetFlower_Design::Point;
	int level = 0;// game->_user.FlowerGetLevel(_type);
	//printf("\n level = %d,_size = %d", level, _size);
	int point = game->GetDesignFlowerLevel(level, param);
	int timehold = -5;
	if (_size > CHARACTER_SIZE_NORMAL)
	{
		point *= game->GetDesignGeneral(FD_GENERAL_DESIGN_XSCORE_PARAM + _size);
		game->FireCharacterFake(_x, _y);
		if (_type != CHARACTER_TYPE_NULL)
		{
			if (game->_fever_mode_active)
			{
				if (game->_combo_num == game->_combo_num_max + FD_BLAST_MODE_TRIGGER)
				{
					game->EffectFireActive(SPRITEID_EFFECT_POWER_BLAST_01, 0, 3, _x, _y, 0);
				}
			}
		}
	}
	else
	{
		if (_type != CHARACTER_TYPE_NULL)
		{
			if (spblast)
			{
				game->EffectFireActive(SPRITEID_EFFECT_POWER_BLAST_01, 0, 3, _x, _y, 0);
			}
			if (game->_fever_mode_active)
			{
				game->EffectExplosionAdd(_type, _x, _y, 5.0f, 0.04f, 40, 29, 3, timehold);
				//game->EffectExplosionAdd(_type, _x, _y, 4.0f, 0.04f, 10, 1 + _type * 3, 2, 15);
				int sprite_eff = SPRITEID_EFFECT_EXPLOSION_FEVER_MODE_01;
				int size = 1;
				//printf("\n game->_combo_num = %d", game->_combo_num);
				if (game->_combo_num == game->_combo_num_max + FD_BLAST_MODE_TRIGGER)
				{
					sprite_eff = SPRITEID_EFFECT_POWER_BLAST_01;
					size = 3;
				}
				game->EffectFireActive(sprite_eff, 0, size, _x, _y,0);

				//game->EffectExplosionAdd(_type, _x, _y, 5.0f, 0.05f, 30, 29, 3, timehold);
			}
			else
			{
				game->EffectExplosionAdd(_type, _x, _y, 5.0f, 0.05f, 30, 29, 3, timehold);
			}
		}
		game->AddEnergy(1);
		//game->FlowerCollectedAdd(1);
	}
	int param_score = CGameDesign::FileGeneral_design::CSheetScore::score;
	point *= game->GetDesignScoreMutiply(game->_scoreX_level - 1, param_score);
	if (game->_fever_mode_active)
	{
		point *= 2;
	}
	_disable_hold_time = 300;
	game->_game_score += point;
	if (_size == CHARACTER_SIZE_NORMAL)
	{
		//game->EffectPointAdd(EFFECT_POINT_TYPE_NORMAL,_type, _x, _y, point, 1.5f);
		game->ParticleFlyToActive(_x, _y, 1, 0, 0, 10, 1, PARTICLE_FLY_TO_TYPE_FLOWER);
	}
	game->_game_score_interface.StartTo(game->_game_score);
	if (_type == CHARACTER_TYPE_SUNFLOWER)
	{
		int time_bonus = game->GetDesignGeneral(FD_GENERAL_DESIGN_TIME_BONUS)*1000;
		if (_size > CHARACTER_SIZE_NORMAL)
		{
			time_bonus *= game->GetDesignGeneral(FD_GENERAL_DESIGN_XTIME_PARAM + _size);
		}
		else
		{
			game->ParticleFlyToActive(_x, _y, 1, 0, 0,10, time_bonus,PARTICLE_FLY_TO_TYPE_SUNFLOWER);
		}
		//game->_game_time.AddMillisecond(time_bonus);
	}
	else
	{
		game->FlowerRenderAddCollected(_type);
	}
	_type = CHARACTER_TYPE_NULL;
	_state = CHARACTER_STATE_EMPTY;
	_touch_id = -1;
	_disable_hold_time = 0;
	SetScaleState(CHARACTER_SCALE_STATE_NORMAL);
	//SetSize(CHARACTER_SIZE_NORMAL);
}

void CCharacter::SetScaleState(int state)
{
	_scale_state = state;
	switch (_scale_state)
	{
	case CHARACTER_SCALE_STATE_NORMAL:
		_scale = 1.0f;
		break;
	case CHARACTER_SCALE_STATE_OUT:
		_scale = 0;
		break;
	case CHARACTER_SCALE_STATE_IN:
		_scale = 1.2f;
		break;
	case CHARACTER_SCALE_STATE_HIDE:

		break;
	default:
		break;
	}
}
int CCharacter::GetSize()
{
	return _size;
}
void CCharacter::SetSizeTar(int size)
{
	_size_tar = size;
}
void CCharacter::SetSize(int size)
{
	_size = size;
}
void CCharacter::SetSpriteID(int sprite)
{
	_spriteid = sprite;
	_anim.SetAnimSprite(GET_SPRITE(_spriteid), _spriteid);
	_anim.SetAnim(CMath::RANDOM(0, 1), false);
	_anim.SetPos(_x, _y);
}
