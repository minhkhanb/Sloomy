#include "pch.h"
#include "CDialog.h"
#include "../../GameCore/CGame.h"
CDialog::CDialog()
{
	_state = STATE_NOTUSE;
	float _x = 0;
	float _y = 0;
}
void CDialog::Init(int context, int type, int typetransaction, const char* title, const char* body, float x, float y)
{
	_main_opac = 0;
	_context = context;
	_type = type;
	_typetransaction = typetransaction;
	_x = x;
	_y = y;
	sprintf(_title, "%s", title);
	sprintf(_body, "%s", body);
	if (GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01) == NULL)
	{
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_CONGRATULATION_01,true,!CGame::_this->_isNeedOptimize);
	}
	if (type == DIALOGTYPE_YESNO)
	{
		for (int i = 0; i < 2; i++)
		{
			_yesno[i].Init(BUTTON_STATE_NORMAL, x, y, A_HVCENTER);
			_yesno[i].SetAnim(SPRITEID_HUD_CONGRATULATION_01, 4 + i * 4);
            _yesno[i].SetPos(0,0);
		}
	}
	else
	{
		_ok.Init(BUTTON_STATE_NORMAL, x, y, A_HVCENTER);
		_ok.SetAnim(SPRITEID_HUD_CONGRATULATION_01, 0);
		_ok.SetPos(0, 0);
		if (_context == DIALOG_TYPE_TUTORIAL)
		{
			_ok.SetPos(0, 0 + 400);
		}
	}
	//GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_LUCKY_WHEEL_01);
	_state = STATE_OPEN;
	if (_typetransaction == ZOOM_IN_OUT_TRANSACTION)
	{
		_scale= 0;
		_cenX = Button::GetFrameX(GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01), 0) + Button::GetFrameWidth(GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01), 0) / 2;
		_cenY = Button::GetFrameY(GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01), 0) + Button::GetFrameHeight(GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01), 0) / 2;
	}
	else
	{
		_opa = 0;
	}
	
}
int CDialog::GetState()
{
	return _state;
}
void CDialog::SetState(int state)
{
	_state = state;
}
void CDialog::Update()
{
	switch (_typetransaction)
	{
	case ZOOM_IN_OUT_TRANSACTION:
		UpdateZoomTransaction();
		break;
	case OPACITY_TRANSACTION:
		UpdateOpacityTransaction();
		break;
	}
}
void CDialog::UpdateZoomTransaction()
{
	switch (_state)
	{
	case STATE_OPEN:
		_scale += 0.1f;
		if (_scale >= 1.0f)
		{
			_scale = 1.0f;
			_state = STATE_NORMAL;
		}
		break;
	case STATE_NORMAL:
		switch (_type)
		{
		case DIALOGTYPE_OK:
			UpdateTouchOK();
			break;
		case DIALOGTYPE_YESNO:
			UpdateTouchYesNo();
			break;
		}

		break;
	case STATE_CLOSE:
		_scale -= 0.1f;
		if (_scale <= 0.0f)
		{
			_scale = 0.0f;
			_state = STATE_NOTUSE;
		}
		break;
	}
}
void CDialog::UpdateOpacityTransaction()
{
	switch (_state)
	{
	case STATE_OPEN:
		_opa += 5.0f;
		if (_opa >= 100.0f)
		{
			_opa = 100.0f;
			_state = STATE_NORMAL;
		}
		break;
	case STATE_NORMAL:
		switch (_type)
		{
		case DIALOGTYPE_OK:
			UpdateTouchOK();
			break;
		case DIALOGTYPE_YESNO:
			UpdateTouchYesNo();
			break;
		}

		break;
	case STATE_CLOSE:
		_opa -= 5.0f;
		if (_opa <= 0.0f)
		{
			_opa = 0.0f;
			_state = STATE_NOTUSE;
		}
		break;
	}
}
void CDialog::Render(float x,float y)
{
	FDGame*game = (FDGame*)(GAME()->_current_game);
	if (_typetransaction == ZOOM_IN_OUT_TRANSACTION)
	{
		G()->SetScale(_scale, _scale, _cenX+x, _cenY+y);
	}
	else
	{
		G()->SetOpacity(_opa);
	}
	if (_context == DIALOG_TYPE_NOTIFY_POPUP || _context == DIALOG_TYPE_ICLOUD_CONFIRM || _context == DIALOG_TYPE_ICLOUD_SYNC_TO
        || _context == DIALOG_TYPE_NETWORK_ERROR || _context == DIALOG_TYPE_RATE_US || _context == DIALOG_TYPE_FOLLOW_TW)
	{
		GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01)->DrawFrame(G(), 7, _x + x, _y + y);
		int char_space = GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing;
        int line_space = GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_line_spacing;
		GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = 4;
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_line_spacing = 15;
		GET_SPRITE(SPRITEID_FONT_POP_UP_01)->DrawAnsiTextPageFormatCustomColor(G(), _x + x, _y + y - 80, 440, _HCENTER | _VCENTER, _body);
		GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = char_space;
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_line_spacing = line_space;
	}
	else if (_context == DIALOG_TYPE_TUTORIAL)
	{
        if(GAME()->_device._version == SupportVersion::RESVER_1024_768)
        {
            GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01)->DrawFrame(G(), 17, _x + x, _y + y);
        }
        else
        {
            GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01)->DrawFrame(G(), 16, _x + x, _y + y);
        }
		
	}
	else
	{
		GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01)->DrawFrame(G(), 0, _x + x, _y + y);
		/*int mx = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterX(4 + _context * 4, 1);
		int my = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterY(4 + _context * 4, 1);*/

		GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 22 + _context, _x + x + 35, _y + y);
		int char_space = GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing;
		GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = 4;
		GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->DrawAnsiTextPageFormatCustomColor(G(), _x + x - 75, _y + y + 48, 420, _HCENTER | _VCENTER, _body);
		GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = char_space;
	}
	
	if (_type == DIALOGTYPE_YESNO)
	{
		for (int i = 0; i < 2; i++)
		{
			_yesno[i].SetPos(_x+x,_y+y);
			_yesno[i].Update();
			_yesno[i].Render(G());
		}
	}
	else
	{
		_ok.SetPos(_x + x, _y + y);
		if (_context == DIALOG_TYPE_TUTORIAL)
		{
			_ok.SetPos(_x + x, _y + y + 400);
		}
		_ok.Update();
		_ok.Render(G());
	}
	if (_typetransaction == ZOOM_IN_OUT_TRANSACTION)
	{
		G()->ClearScale();
	}
	else
	{
		G()->SetOpacity(_opa);
	}
}
bool CDialog::UpdateTouchYesNo()
{
	bool ret = false;
	int index = -1;

	for (int i = 0; i < 2; i++)
	{
		if (_yesno[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_yesno[index].NeedAction())
		{
			PROCESSING_DIALOG_YESNO(_context, index);
			_state = STATE_CLOSE;
			if (index == 0)
			{
				//PLAY_SFX(SFX_BUTTON);
			}
			else
			{
				//PLAY_SFX(SFX_BUTTON);
			}
			for (int i = 0; i < 2; i++)
			{
				if (_yesno[i]._state != BUTTON_STATE_LOCK && _yesno[i]._state != BUTTON_STATE_HIDE)
				{
					_yesno[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
			//CGame::_this->ClearAllTouch();
		}
		return true;
	}
	bool pressed = false;
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
			for (int touch = 0; touch < 2; touch++)
			{
				if (_yesno[touch]._state == BUTTON_STATE_LOCK || _yesno[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}
				int x = _yesno[touch].TZ_GetX();
				int y = _yesno[touch].TZ_GetY();
				int w = _yesno[touch].TZ_GetWidth();
				int h = _yesno[touch].TZ_GetHeight();
				Button *button = &_yesno[touch];
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_touch_id == -1)
						{
							button->SetState(BUTTON_STATE_SELECT);
							button->_touch_id = TOUCHS(i)->_id;
							PLAY_SFX(SFX_BUTTON);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->_touch_id == -1)
						{
							button->SetState(BUTTON_STATE_SELECT);
							button->_touch_id = TOUCHS(i)->_id;
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->IsAction(TOUCHS(i)->_id))
						{
							button->SetState(BUTTON_STATE_ACTIVE);
							
						}
					}
					if (button->_touch_id != -1)
					{
						pressed = true;
					}
				}
				else
				{
					if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->IsAction(TOUCHS(i)->_id))
						{
							button->_touch_id = -1;
							button->SetState(BUTTON_STATE_NORMAL);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
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
	return pressed;
}
bool CDialog::UpdateTouchOK()
{
	bool ret = false;
	int index = -1;

	if (_ok._state == BUTTON_STATE_ACTIVE)
	{
		ret = true;
		
	}

	if (ret)
	{
		if (_ok.NeedAction())
		{
			_state = STATE_CLOSE;
	
			if (_ok._state != BUTTON_STATE_LOCK && _ok._state != BUTTON_STATE_HIDE)
				{
				PROCESSING_DIALOG_OK(_context);
				_ok.SetState(BUTTON_STATE_NORMAL);
				//PLAY_SFX(SFX_BUTTON);
				}
		
			//CGame::_this->ClearAllTouch();
		}
		return true;
	}
	bool pressed = false;
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			
			{
				if (_ok._state == BUTTON_STATE_LOCK || _ok._state == BUTTON_STATE_HIDE)
				{
					continue;
				}
				int x = _ok.TZ_GetX();
				int y = _ok.TZ_GetY();
				int w = _ok.TZ_GetWidth();
				int h = _ok.TZ_GetHeight();
				Button *button = &_ok;
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_touch_id == -1)
						{
							button->SetState(BUTTON_STATE_SELECT);
                            button->_touch_id = TOUCHS(i)->_id;
							PLAY_SFX(SFX_BUTTON);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->_touch_id == -1)
						{
							button->SetState(BUTTON_STATE_SELECT);
							button->_touch_id = TOUCHS(i)->_id;
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->IsAction(TOUCHS(i)->_id))
						{
							button->SetState(BUTTON_STATE_ACTIVE);
							button->_touch_id = -1;
						}
					}
					if (button->_touch_id != -1)
					{
						pressed = true;
					}
				}
				else
				{
					if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->IsAction(TOUCHS(i)->_id))
						{
							button->_touch_id = -1;
							button->SetState(BUTTON_STATE_NORMAL);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
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
	}
	return pressed;
}