#include "pch.h"
#include "CDialogManager.h"
#include "../../GameCore/CGame.h"
void CDialogManager::Update()
{
	for (int i = 0; i < MAX_DIALOG_NUMS; i++)
	{
		if (_dialog[i].GetState() != STATE_NOTUSE)
		{
			_dialog[i].Update();
		}
	}
}
void CDialogManager::Render(float x, float y)
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	for (int i = 0; i < MAX_DIALOG_NUMS; i++)
	{
		if (_dialog[i].GetState() != STATE_NOTUSE)
		{
			if (_dialog[i].GetState() == STATE_CLOSE)
			{
				if (_dialog[i]._main_opac > 0)
				{
					_dialog[i]._main_opac -= 5;
				}
				else
				{
					_dialog[i]._main_opac = 0;
				}
			}
			else
			{
				if (_dialog[i]._main_opac < 70)
				{
					_dialog[i]._main_opac += 5;
				}
				else
				{
					_dialog[i]._main_opac = 70;
				}
			}
			
			G()->SetOpacity(_dialog[i]._main_opac);
			G()->SetColor(0xff000000);
			//G()->FillFullScreen(true);
			G()->FillRect(game->_point_o.X, game->_point_o.Y, game->_screen_width, game->_screen_height,true);
			G()->SetOpacity(100);
			game->_congra_fire_work1.Render(0, 0,100);
			_dialog[i].Render(x,y);
			game->_congra_fire_work2.Render(0, 0, 100);
		}
	}
}
void CDialogManager::Add(int context, int type, int typetransaction, const char* title, const char* body, float x, float y)
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	game->_dialog_process_type = -1;
	_opac = 0;
	for (int i = 0; i < MAX_DIALOG_NUMS; i++)
	{
		if (_dialog[i].GetState() == STATE_NOTUSE)
		{
			_dialog[i].Init(context, type, typetransaction, title, body, x, y);
			break;
		}
	}
}
bool CDialogManager::isOpenDialog()
{
	for (int i = 0; i < MAX_DIALOG_NUMS; i++)
	{
		if (_dialog[i].GetState() != STATE_NOTUSE)
		{
			return true;
		}
	}
	return false;
}
void CDialogManager::ForceClose()
{
	for (int i = 0; i < MAX_DIALOG_NUMS; i++)
	{
		if (_dialog[i].GetState() != STATE_CLOSE)
		{
			_dialog[i].SetState(STATE_CLOSE);
		}
	}
}