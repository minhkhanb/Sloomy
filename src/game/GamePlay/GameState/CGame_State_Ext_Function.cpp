#include "pch.h"
#include "../../GameCore/CGame.h"
void FDGame::FirstFillCube()
{
	int count = 0;
	int maxgroup = 8;
	int rand_easy = CMath::RANDOM(1, 100);
	_game_level = CMath::RANDOM(GAMEPLAY_LEVEL_MEDIUM,GAMEPLAY_LEVEL_MEDIUM3);
	if (rand_easy <= _game_easy_percent)
	{
		_game_easy_percent = 0;
		_game_level = CMath::RANDOM(GAMEPLAY_LEVEL_EASY, GAMEPLAY_LEVEL_EASY2);
	}
	else
	{
		_game_easy_percent += 20;
		int rand_hard = CMath::RANDOM(1, 100);
		if (rand_hard <= _game_hard_percent)
		{
			_game_hard_percent = 0;
			_game_level = CMath::RANDOM(GAMEPLAY_LEVEL_HARD, GAMEPLAY_LEVEL_HARD3);
		}
		else
		{
			_game_hard_percent += 20;
		}
	}
	//printf("\n 000 _game_level = %d", _game_level);
	while (count < maxgroup)
	{
		int num_cube = GetDesignBeginning(count, _game_level);
		if (num_cube > 3)
		{
			int frand = CMath::RANDOM(1, 100);
			int f_data_index = CMath::RANDOM(1, _game_design.General_design.SheetPercent_Spawn.int32Value._totalData) - 1;
			int percent = 0;
			int ftype = CHARACTER_TYPE_NULL;
			for (int i = 0; i <= CHARACTER_TYPE_ORCHILD; i++)
			{
				int fpercent = GetDesignFlowerPercent(f_data_index, i);
				if (frand <= percent + fpercent && frand > percent)
				{
					ftype = i;
					break;
				}
				percent += fpercent;
			}
			//printf("\n 111 ftype = %d,num_cube = %d", ftype, num_cube);
			CubeGroupRandom(ftype, num_cube);
		}
		count++;
	}
}
void FDGame::CubeGroupRandom(int type, int num)
{
	int state_temp[FD_NUM_ROW][FD_NUM_COL];
	int ran1 = CMath::RANDOM(1, FD_NUM_ARR) - 1;
	int row = ran1 / FD_NUM_COL;
	int col = ran1 % FD_NUM_COL;
	int num_slotempty = _cube_num_slot_ready;
	_cube_num_slot_ready = 0;
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			state_temp[i][j] = _character_arr[i][j].GetState();
		}
	}
	FindCubeSur(ran1, CHARACTER_TYPE_NULL, FD_FIND_SUR_TYPE_NORMAL,_fever_mode_active);
	
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].GetState() == CHARACTER_STATE_WAITING)
			{
				_character_arr[i][j].SetState(state_temp[i][j]);
			}
		}
	}
	int count_while = 0;
	while (_cube_num_slot_ready < num && count_while < 30)
	{
		//printf("\n _cube_num_slot_ready = %d", _cube_num_slot_ready);
		ran1 = CMath::RANDOM(1, FD_NUM_ARR) - 1;
		row = ran1 / FD_NUM_COL;
		col = ran1 % FD_NUM_COL;
		count_while++;
	}
	if (count_while >= 30)
	{
		return;
	}
	_cube_num_slot_ready = num_slotempty;
	int count = 0;
	int dir = CMath::RANDOM(0, 3);
	//printf("\n 11 _cube_num_slot_ready = %d", _cube_num_slot_ready);
	while (count <= num)
	{

		int index1 = row*FD_NUM_COL + col;
		int ftype = _character_arr[row][col].GetType();
		int mk_x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleX(0, 0);
		int mk_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0);
		//printf("\n count:%d,num:%d,index1:%d,dir:%d", count, num, index1, dir);
		switch (dir)
		{
		case 0:
			index1 = row*FD_NUM_COL + col + 1;
			if (col + 1 < FD_NUM_COL)
			{
				ftype = _character_arr[row][col + 1].GetType();
				if (ftype == CHARACTER_TYPE_NULL)
				{
					float x = FD_SLOT_WIDTH / 2 + (col + 1)*FD_SLOT_WIDTH + mk_x;
					float y = FD_SLOT_WIDTH / 2 + (row)*FD_SLOT_WIDTH + mk_y;
					_character_arr[row][col + 1].CreateNew(type, 0, 0.04f);
					_character_arr[row][col + 1].SetPos(x, y);
					_character_arr[row][col + 1].SetTar(x, y);
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}

			break;
		case 1:
			index1 = row*FD_NUM_COL + col - 1;
			if (col - 1 >= 0)
			{
				ftype = _character_arr[row][col + 1].GetType();
				if (ftype == CHARACTER_TYPE_NULL)
				{
					float x = FD_SLOT_WIDTH / 2 + (col - 1)*FD_SLOT_WIDTH + mk_x;
					float y = FD_SLOT_WIDTH / 2 + (row)*FD_SLOT_WIDTH + mk_y;
					_character_arr[row][col - 1].CreateNew(type, 0, 0.04f);
					_character_arr[row][col - 1].SetPos(x, y);
					_character_arr[row][col - 1].SetTar(x, y);
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}
			break;
		case 2:
			index1 = (row - 1)*FD_NUM_COL + col;
			if (row - 1 >= 0)
			{
				ftype = _character_arr[row][col + 1].GetType();
				if (ftype == CHARACTER_TYPE_NULL)
				{
					float x = FD_SLOT_WIDTH / 2 + (col)*FD_SLOT_WIDTH + mk_x;
					float y = FD_SLOT_WIDTH / 2 + (row - 1)*FD_SLOT_WIDTH + mk_y;
					_character_arr[row - 1][col].CreateNew(type, 0, 0.04f);
					_character_arr[row - 1][col].SetPos(x, y);
					_character_arr[row - 1][col].SetTar(x, y);
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}
			break;
		case 3:
			index1 = (row + 1)*FD_NUM_COL + col;
			if (row + 1 < FD_NUM_ROW)
			{
				ftype = _character_arr[row][col + 1].GetType();
				if (ftype == CHARACTER_TYPE_NULL)
				{
					
					float x = FD_SLOT_WIDTH / 2 + (col)*FD_SLOT_WIDTH + mk_x;
					float y = FD_SLOT_WIDTH / 2 + (row + 1)*FD_SLOT_WIDTH + mk_y;
					_character_arr[row + 1][col].CreateNew(type, 0, 0.04f);
					_character_arr[row + 1][col].SetPos(x, y);
					_character_arr[row + 1][col].SetTar(x, y);
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}
			break;
		default:
			break;
		}
		
		row = index1 / FD_NUM_COL;
		col = index1 % FD_NUM_COL;
	}
	
}
void FDGame::FillAllCube(bool isinit)
{
	bool hasnew = false;
	int count = 0;
	int type_init = 0;
	if (isinit)
	{
		type_init = CMath::RANDOM(0, 7);
		FirstFillCube();
	}
	
	int ftype_old = CHARACTER_TYPE_NULL;
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			
			int frand = CMath::RANDOM(1, 100);
			int f_data_index = CMath::RANDOM(1, _game_design.General_design.SheetPercent_Spawn.int32Value._totalData) - 1;
			int percent = 0;
			int ftype = CHARACTER_TYPE_NULL;
			for (int i = 0; i <= CHARACTER_TYPE_ORCHILD; i++)
			{
				int fpercent = GetDesignFlowerPercent(f_data_index, i);
				if (frand <= percent + fpercent && frand > percent)
				{
					ftype = i;
					break;
				}
				percent += fpercent;
			}
			if (_character_arr[i][j].GetState() != CHARACTER_STATE_NORMAL)
			{
				_is_need_find_fl_group = true;
			}
			if (_character_arr[i][j].GetState() == CHARACTER_STATE_EMPTY)
			{
				int index = i*FD_NUM_COL + j;
				int mk_x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleX(0, 0);
				int mk_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0);
				float x = FD_SLOT_WIDTH / 2 + j*FD_SLOT_WIDTH + mk_x;
				float y = FD_SLOT_WIDTH / 2 + i*FD_SLOT_WIDTH + mk_y;
				if(!isinit)
				{
					int fsame_per = 0;
					if (_game_level <= GAMEPLAY_LEVEL_EASY2)
					{
						int param = CGameDesign::FileGeneral_design::CSheetFlower_Respawn::Easy;
						fsame_per = GetDesignSpawPercent(0, param);
					}
					else if (_game_level <= GAMEPLAY_LEVEL_MEDIUM3)
					{
						int param = CGameDesign::FileGeneral_design::CSheetFlower_Respawn::Medium;
						fsame_per = GetDesignSpawPercent(0, param);
					}
					else if (_game_level <= GAMEPLAY_LEVEL_HARD3)
					{
						int param = CGameDesign::FileGeneral_design::CSheetFlower_Respawn::Hard;
						fsame_per = GetDesignSpawPercent(0, param);
					}
					//printf("\n fsame_per = %d,_game_level=%d,frand = %d", fsame_per, _game_level, frand);
					int frand = CMath::RANDOM(1, 100);
					if (frand <= fsame_per )
					{
						if (ftype_old != CHARACTER_TYPE_NULL)
						{
							ftype = ftype_old;
						}
					}
					else
					{
						while (ftype == ftype_old && ftype_old != CHARACTER_TYPE_NULL)
						{
							percent = 0;
							frand = CMath::RANDOM(1, 100);
							f_data_index = CMath::RANDOM(1, _game_design.General_design.SheetPercent_Spawn.int32Value._totalData) - 1;
							for (int f = 0; f <= CHARACTER_TYPE_ORCHILD; f++)
							{
								int fpercent = GetDesignFlowerPercent(f_data_index, f);
								if (frand <= percent + fpercent && frand > percent)
								{
									ftype = f;
									break;
								}
								percent += fpercent;
							}
						}
					}
					count++;
					ftype_old = ftype;
				}
				int timehold = count * 2;
				if (isinit)
				{
					_character_arr[i][j].CreateNew(ftype, timehold, 0.04f);
					_character_arr[i][j].SetPos(x, y);
					_character_arr[i][j].SetTar(x, y);
				}
				else
				{
					//DropAllCube(isinit);
				}
				_is_need_find_fl_group = true;
				hasnew = true;
			}
			
			if (isinit)
			{
				switch (type_init)
				{
				case 0:
					count = (i + j) * 2;
					break;
				case 1:
					count = i * 2;
					break;
				case 2:
					count = (FD_NUM_COL - j) * 2;
					break;
				case 3:
					count = CMath::ABSOLUTE_VALUE(FD_NUM_COL * 2 - (i + j)) * 2;
					break;
				case 4:
					count = (int)(CMath::ABSOLUTE_VALUE(3.5f - i)) * 4;
					break;
				case 5:
					count = (int)(CMath::ABSOLUTE_VALUE(3.5f - j)) * 4;
					break;
				case 6:
					count = (CMath::ABSOLUTE_VALUE((FD_NUM_COL) / 2 - j) + CMath::ABSOLUTE_VALUE((FD_NUM_ROW) / 2 - i)) * 2;
					break;
				case 7:
					count = ((FD_NUM_COL) / 2 - 1 - (CMath::ABSOLUTE_VALUE((FD_NUM_COL) / 2 - j) + CMath::ABSOLUTE_VALUE((FD_NUM_ROW) / 2 - i))) * 3;
					break;
				default:
					break;
				}
				_character_arr[i][j]._disable_hold_time = count * 2;
				_is_need_find_fl_group = true;
			}
		}
	}
	if (!isinit && hasnew)
	{
		DropAllCube(isinit);
		_powerup_eat_random_type = CHARACTER_TYPE_NULL;
	}
	if (hasnew && !isinit)
	{
		int num_temp = _cube_num_slot_ready;
		bool can_eat = false;
		int num_group = 0;
		for (int i = 0; i < FD_NUM_ROW; i++)
		{
			for (int j = 0; j < FD_NUM_COL; j++)
			{
				int state = _character_arr[i][j].GetState();
				int index = i*FD_NUM_COL + j;
				if (state != CHARACTER_STATE_NORMAL )
				{
					_character_arr[i][j].Update();
					_is_need_find_fl_group = true;
					continue;
				}
				_cube_num_slot_ready = 0;
				FindCubeSur(index, _character_arr[i][j].GetType(), FD_FIND_SUR_TYPE_NORMAL, _fever_mode_active);
				if (_cube_num_slot_ready >= FD_ACTIVE_NUM_MIN)
				{
					can_eat = true;
					num_group++;
					break;
				}
			}
			/*if (can_eat) 
			{
				break; 
			}*/
		}
		for (int i = 0; i < FD_NUM_ROW; i++)
		{
			for (int j = 0; j < FD_NUM_COL; j++)
			{
				if (_character_arr[i][j].GetState() == CHARACTER_STATE_WAITING)
				{
					_character_arr[i][j].SetState(CHARACTER_STATE_NORMAL);
				}
			}
		}
		if (num_group <= 1)
		{
			CubeRandom();
			_count_frame_randomcube = CMath::RANDOM(5, 8);
		}
		_cube_num_slot_ready = num_temp;
	}
	if(_is_need_find_fl_group)
	{
		FindAndSetFlowerGroup();
	}
}
void FDGame::FindAndSetFlowerGroup()
{
	
	//_is_need_find_fl_group = false;
	//printf("\n teset _is_need_find_fl_group = %d", _is_need_find_fl_group);
	_is_need_find_fl_group = false;
	ResetCharacterSize();
	ResetCharacterFake();
	FindFlower3x3();
	FindFlower2x2();
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if ((_character_arr[i][j].GetSize() == CHARACTER_SIZE_NORMAL && _character_arr[i][j]._size_tar == CHARACTER_SIZE_NORMAL) && _character_size[i][j] == 1)
			{
				_character_arr[i][j]._scale = 1.0f;
				_character_arr[i][j]._scale_state = CHARACTER_SCALE_STATE_NORMAL;
			}
		}
	}
	
}
bool FDGame::FindCubeSur(int index, int type, int type_find, bool fevermode)
{
	//printf("\n find index = %d-type = %d", index, type);
	bool ret = false;
	int row = index / FD_NUM_COL;
	int col = index % FD_NUM_COL;
	_cube_num_slot_ready++;
	if (type_find == FD_FIND_SUR_TYPE_TOUCH)
	{
		AddCubeReadyToQueue(index);
		//if (fevermode)
		//{
		//	for (int i = row - 1; i < row + 2; i++)
		//	{
		//		if (i < 0 || i >= FD_NUM_ROW)
		//		{
		//			continue;
		//		}
		//		for (int j = col - 1; j < col + 2; j++)
		//		{
		//			if (j < 0 || j >= FD_NUM_COL)
		//			{
		//				continue;
		//			}
		//			int around_index = i*FD_NUM_COL + j;
		//			//printf("\n Add cube fever around_index = %d", around_index);
		//			AddCubeReadyToQueue(around_index);
		//		}
		//	}
		//}
	}
	else if (type_find == FD_FIND_SUR_TYPE_HINT)
	{

	}
	int state_temp = _character_arr[row][col].GetState();
	_character_arr[row][col].SetState(CHARACTER_STATE_WAITING);
	int k = row + 1;
	bool same = true;
	while (k < FD_NUM_ROW && same)
	{
		int ctype = _character_arr[k][col].GetType();
		int state = _character_arr[k][col].GetState();
		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = k*FD_NUM_COL + col;
			FindCubeSur(ind, type, type_find, fevermode);
			ret = true;
		}
		else
		{
			same = false;
		}
	}
	k = row - 1;
	same = true;
	while (k >= 0 && same)
	{

		int ctype = _character_arr[k][col].GetType();
		int state = _character_arr[k][col].GetState();
		
		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = k*FD_NUM_COL + col;
			FindCubeSur(ind, type, type_find, fevermode);
			ret = true;
		}
		else
		{
			same = false;
		}
	}
	k = col + 1;
	same = true;
	while (k < FD_NUM_COL && same)
	{
		int ctype = _character_arr[row][k].GetType();
		int state = _character_arr[row][k].GetState();
		
		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = row*FD_NUM_COL + k;
			FindCubeSur(ind, type, type_find, fevermode);
			ret = true;
		}
		else
		{
			same = false;
		}
	}
	k = col - 1;
	same = true;
	while (k >= 0 && same)
	{
		int ctype = _character_arr[row][k].GetType();
		int state = _character_arr[row][k].GetState();
		
		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = row*FD_NUM_COL + k;
			FindCubeSur(ind, type, type_find, fevermode);
			ret = true;
		}
		else
		{
			same = false;
		}
	}
	
	return ret;
}
int FDGame::GetIndexHasNull()
{
	int index = -1;
	for (int i = FD_NUM_ROW-1 ; i >= 0; i--)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].GetState() == CHARACTER_STATE_EMPTY)
			{
				index = i*FD_NUM_COL + j;
				break;
			}
		}
	}
	return index;
}

void FDGame::CubeRender(float x, float y, int opac)
{
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].GetSize() > CHARACTER_SIZE_NORMAL /*&&  _character_arr[i][j]._scale <= 0*/)
			{
				continue;
			}
			_character_arr[i][j].AddRender(x, y,opac);
		}
	}
}
void FDGame::CubeUpdate()
{
	bool has_init = true;
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			_character_arr[i][j].Update();
			if (/*(_character_arr[i][j]._scale_state != CHARACTER_SCALE_STATE_NORMAL && _character_arr[i][j]._scale != 0) ||*/ _character_arr[i][j].GetType() == CHARACTER_TYPE_NULL)
			{
				has_init = false;
			}
		}
	}
	if (has_init)
	{
		if (!_has_tutorial_completed)
		{
			if (!_dialogMng.isOpenDialog())
			{
			char text[128];
			sprintf(text, "");
			_dialogMng.Add(DIALOG_TYPE_TUTORIAL, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
			}
		}
	}
}
void FDGame::CubeInit()
{
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			_character_arr[i][j].Init(0, 0);
		}
	}
}
bool FDGame::CubeUpdateTouch()
{
	bool ret = false;
	if (_dialogMng.isOpenDialog())
	{
		return ret;
	}
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].UpdateTouch())
			{
				_has_started = true;
				int type = _character_arr[i][j].GetType();
				if (_power_up_icon[POWER_UP_TYPE_EAT_RANDOM]._has_actived)
				{
					_powerup_eat_random_type = type;
					//printf("\n type = %d", type);
					EatFlowerType(type);
					_power_up_icon[POWER_UP_TYPE_EAT_RANDOM].ActivedFunc(false);
					//_power_up_icon[POWER_UP_TYPE_EAT_RANDOM]._has_actived = false;
					_power_up_icon[POWER_UP_TYPE_EAT_RANDOM].ActiveScale(1.0f, 1.1f, 1.0f, 0.1f, 4);
				}
				else
				{
					int index = i*FD_NUM_COL + j;
					ret = true;
					_cube_sur_finding = true;
					FindCubeSur(index, type, FD_FIND_SUR_TYPE_TOUCH,_fever_mode_active);
					if (_cube_num_slot_ready > 0)
					{
						if (_cube_num_slot_ready >= FD_ACTIVE_NUM_MIN)
						{
							//if (_combo_touch_time > 0)
							{
								_combo_num++;
								if (_combo_num > 1)
								{
									if (_combo_num <= _combo_num_max)
									{
										//EffectPointAdd(EFFECT_POINT_TYPE_COMBO, 0, k_SCREEN_WIDTH / 2, 245, _combo_num, 1.0f);
										_combo_touch_time = FD_COMBO_TOUCH_TIME;
									}
									else
									{
										if (_combo_num < _combo_num_max + FD_BLAST_MODE_TRIGGER)
										{
											//EffectPointAdd(EFFECT_POINT_TYPE_FEVER, 0, k_SCREEN_WIDTH / 2, 210, _combo_num - FD_FEVER_MODE_TRIGGER, 1.0f);
											EffectPointAdd(EFFECT_POINT_TYPE_FEVER, 0, k_SCREEN_WIDTH / 2, 210, 2, 1.0f);
										}
										_combo_touch_time = FD_FEVER_MODE_COMBO_TIME;
									}
									_fever_last_index = index;
								}
								else
								{
									_combo_touch_time = FD_COMBO_TOUCH_TIME;
								}
							}
							/*else
							{
								if (!_fever_mode_active)
								{
									_combo_touch_time = FD_COMBO_TOUCH_TIME;
									_combo_num = 1;
								}
							}*/
							int soundid = SFX_SCORE_1 + _combo_num;
							_count_frame_randomcube--;
							if (_combo_num < FD_BLAST_MODE_TRIGGER + _combo_num_max)
							{
								FireCubeQueue();
								if (soundid > SFX_SCORE_1 + _combo_num_max)
								{
									soundid = SFX_SCORE_INFEVER;
								}
								PLAY_SFX(soundid);
							}
							//FireCubeQueue();
							ResetFlowerHint();
						}
						else
						{
							if (_combo_num + 1 >= FD_BLAST_MODE_TRIGGER + _combo_num_max)
							{
								_fever_last_index = index;
                                SuperBlastActive();
                                
							}
							else
							{
								if (_fever_mode_active)
								{
									_combo_num = _combo_num_max;
								}
								else
								{
									FeverModeDisable();
								}
								
								EffectPointAdd(EFFECT_POINT_TYPE_MISS, 0, k_SCREEN_WIDTH / 2, 245, _combo_num, 1.0f);
								PLAY_SFX(SFX_MISSED);
							}
							DisableCubeQueue();
							_character_arr[i][j].SetState(CHARACTER_STATE_TOUCH_IT);
						}
					}

					ResetCubeReadyQueue();
				}
				return ret;
			}
		}
	}
	return ret;
}

void FDGame::ResetCubeReadyQueue()
{
	//printf("\n ------_cube_num_slot_ready:%d", _cube_num_slot_ready);
	_cube_num_slot_ready = 0;
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		_character_queue[i] = -1;
	}
}
void FDGame::AddCubeReadyToQueue(int index)
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_character_queue[i] == index)
		{
			_character_queue[i] = index;
			return;
		}
	}
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_character_queue[i] == -1)
		{
			_character_queue[i] = index;
			break;
		}
	}
}
void FDGame::FireCubeQueue()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_character_queue[i] != -1)
		{
			int row = _character_queue[i] / FD_NUM_COL;
			int col = _character_queue[i] % FD_NUM_COL;
			_character_arr[row][col].Fire(false);
		}
	}
}
void FDGame::DisableCubeQueue()
{
	for (int i = 0; i < FD_NUM_ARR; i++)
	{
		if (_character_queue[i] != -1)
		{
			int row = _character_queue[i] / FD_NUM_COL;
			int col = _character_queue[i] % FD_NUM_COL;
			_character_arr[row][col].SetState(CHARACTER_STATE_DISABLE);
		}
	}
}

void FDGame::CubeRandom()
{
	int ran1 = CMath::RANDOM(1, FD_NUM_ARR) - 1;
	int row = ran1 / FD_NUM_COL;
	int col = ran1 % FD_NUM_COL;
	int type = _character_arr[row][col].GetType();
	int num_while = 0;
	while (num_while < 30 && (_character_arr[row][col].GetState() != CHARACTER_STATE_NORMAL || _character_arr[row][col]._scale_state != CHARACTER_SCALE_STATE_NORMAL))
	{
		ran1 = CMath::RANDOM(1, FD_NUM_ARR) - 1;
		row = ran1 / FD_NUM_COL;
		col = ran1 % FD_NUM_COL;
		type = _character_arr[row][col].GetType();
		num_while++;
	}
	if (num_while >= 30)
	{
		return;
	}
	int num = CMath::RANDOM(2, 5);
	int count = 0;
	int dir = CMath::RANDOM(0, 3);
	
	while (count <= num)
	{
		
		int index1 = row*FD_NUM_COL + col;
		int type1 = _character_arr[row][col].GetType();
		//printf("\n count:%d,num:%d,index1:%d,dir:%d", count, num, index1, dir);
		switch (dir)
		{
		case 0:
			index1 = row*FD_NUM_COL + col + 1;
			if (col + 1 < FD_NUM_COL)
			{
				type1 = _character_arr[row][col + 1].GetType();
				//printf("\n 01 type1[%d] = %d", index1, type1);
				if (type1 != type)
				{
					_character_arr[row][col + 1].Modify(type);
					//printf("\n 0-1 type1[%d] = %d", index1, _character_arr[row][col+1].GetType());
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}

			break;
		case 1:
			index1 = row*FD_NUM_COL + col - 1;
			if (col - 1 >= 0)
			{
				type1 = _character_arr[row][col - 1].GetType();
				//printf("\n 0-1 type1[%d] = %d", index1, type1);
				if (type1 != type)
				{
					_character_arr[row][col - 1].Modify(type);
					//printf("\n 0-1 type1[%d] = %d", index1, _character_arr[row][col - 1].GetType());
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}
			break;
		case 2:
			index1 = (row - 1)*FD_NUM_COL + col;
			if (row - 1 >= 0)
			{
				type1 = _character_arr[row - 1][col].GetType();
				//printf("\n -10 type1[%d] = %d", index1, type1);
				if (type1 != type)
				{
					_character_arr[row - 1][col].Modify(type);
					//printf("\n 0-1 type1[%d] = %d", index1, _character_arr[row - 1][col].GetType());
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}
			break;
		case 3:
			index1 = (row + 1)*FD_NUM_COL + col;
			if (row + 1 < FD_NUM_ROW)
			{
				type1 = _character_arr[row + 1][col].GetType();
				//printf("\n 10 type1[%d] = %d", index1, type1);
				if (type1 != type)
				{
					_character_arr[row + 1][col].Modify(type);
					//printf("\n 0-1 type1[%d] = %d", index1, _character_arr[row + 1][col].GetType());
					count++;
				}
				dir = CMath::RANDOM(0, 3);
			}
			else
			{
				index1 = row*FD_NUM_COL + col;
				dir = CMath::RANDOM(0, 3);
			}
			break;
		default:
			break;
		}
		row = index1 / FD_NUM_COL;
		col = index1 % FD_NUM_COL;
	}
}
void FDGame::ResetCharacterSize()
{
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			//printf("\n char[%d] type = %d", i*FD_NUM_COL + j, _character_arr[i][j].GetType());
			_character_size[i][j] = -1;
			if (_character_arr[i][j].GetSize() > CHARACTER_SIZE_NORMAL || _character_arr[i][j]._size_tar > CHARACTER_SIZE_NORMAL)
			{
				_character_size[i][j] = 1;
			}
			_character_arr[i][j].SetSize(CHARACTER_SIZE_NORMAL);
			_character_arr[i][j]._size_tar = CHARACTER_SIZE_NORMAL;
		}
	}
}
void FDGame::ResetCharacterFake()
{
	for (int i = 0; i < FD_NUM_COL * 4; i++)
	{
		_character_arr_fake[i].Init(0,0);
	}
}
void FDGame::AddCharacterFake(float x, float y, int type, int size, int timehold, float scale, int statescale)
{
	if (type < 0)
	{
		return;
	}
	for (int i = 0; i < FD_NUM_COL * 4;i++)
	{
		int ctype = _character_arr_fake[i].GetType();
		if (ctype == CHARACTER_TYPE_NULL)
		{
			_character_arr_fake[i].CreateNew(type, timehold, 0.04f);
			_character_arr_fake[i].SetPos(x, y);
			_character_arr_fake[i].SetTar(x, y);
			int sprite = _character_sprite[type * 3 + size];
			_character_arr_fake[i]._size_tar = size;
			_character_arr_fake[i]._scale = scale;
			_character_arr_fake[i]._scale_state = statescale;
			_character_arr_fake[i].SetSpriteID(sprite);
			break;
		}
		/*else if (ctype != CHARACTER_TYPE_NULL)
		{
			if (type == ctype && _character_arr_fake[i].GetSize() == size)
			{
				_character_arr_fake[i].Modify(type);
			}
			else
			{
				_character_arr_fake[i].CreateNew(type, timehold, 0.04f);
				_character_arr_fake[i].SetPos(x, y);
				_character_arr_fake[i].SetTar(x, y);
				int sprite = _character_sprite[type * 3 + size];
				_character_arr_fake[i]._scale = scale;
				_character_arr_fake[i]._scale_state = statescale;
				_character_arr_fake[i].SetSpriteID(sprite);
			}
			
			break;
		}*/
	}
}
void FDGame::RenderCharacterFake(float x, float y, int opac)
{
	for (int i = 0; i < FD_NUM_COL * 4; i++)
	{
		if (_character_arr_fake[i].GetType() != CHARACTER_TYPE_NULL)
		{
			//printf("\n char holdtime[%d] = %d", i,_character_arr_fake[i]._disable_hold_time);
			_character_arr_fake[i].AddRender(x, y, opac);
		}
	}
}
void FDGame::UpdateCharacterFake()
{
	for (int i = 0; i < FD_NUM_COL * 4; i++)
	{
		_character_arr_fake[i].Update();
	}
}
void FDGame::FireCharacterFake(float x, float y)
{
	for (int i = 0; i < FD_NUM_COL * 4; i++)
	{
		int type = _character_arr_fake[i].GetType();
		if (type == CHARACTER_TYPE_NULL)
		{
			continue;
		}
		int size = (_character_arr_fake[i]._size_tar+1)*FD_SLOT_WIDTH;
		int cx = _character_arr_fake[i].GetPosX() ;
		int cy = _character_arr_fake[i].GetPosY() ;
		int w = size;
		int h = size;
		if (CHECK_POINT_IN_RECT(x, y, cx - size / 2, cy - size / 2, w, h))
		{
			float multi = 1.0f;
			/*if (_fever_mode_active)
			{
				multi = 1.5f;
			}*/
			EffectExplosionAdd(type, cx, cy, 6.0f, 0.05f, 30 * _character_arr_fake[i]._size_tar, 29, 3, 10);
			EffectExplosionAdd(type, cx, cy, 6.0f, 0.04f, 50* _character_arr_fake[i]._size_tar, type*3, 3,15, _character_arr_fake[i]._size_tar);
			int num_flower = (_character_arr_fake[i]._size_tar + 1)*(_character_arr_fake[i]._size_tar + 1);
			int level = 0;// _user.FlowerGetLevel(type);
			int param = CGameDesign::FileGeneral_design::CSheetFlower_Design::Point;
			int point = GetDesignFlowerLevel(level, param)*num_flower;
			point *= GetDesignGeneral(FD_GENERAL_DESIGN_XSCORE_PARAM + _character_arr_fake[i]._size_tar);
			//FlowerCollectedAdd(1);
			int param_score = CGameDesign::FileGeneral_design::CSheetScore::score;
			point *= GetDesignScoreMutiply(_scoreX_level - 1, param_score);
			if (_fever_mode_active)
			{
				point *= 2;
			}
			EffectPointAdd(_character_arr_fake[i]._size_tar,type, cx, cy, point, 1.5f);
			int time_bonus = GetDesignGeneral(FD_GENERAL_DESIGN_TIME_BONUS) * 1000;
			time_bonus *= GetDesignGeneral(FD_GENERAL_DESIGN_XTIME_PARAM + _character_arr_fake[i]._size_tar);
			ParticleFlyToActive(cx, cy, 1, 0, 0, 10, num_flower, PARTICLE_FLY_TO_TYPE_FLOWER);
			if (type == CHARACTER_TYPE_SUNFLOWER)
			{
				ParticleFlyToActive(cx, cy, 1, 0, 0, 10, time_bonus, PARTICLE_FLY_TO_TYPE_SUNFLOWER);
			}
			AddEnergy(GetDesignGeneral(9 + (_character_arr_fake[i]._size_tar - 1)));
			if (_fever_mode_active)
			{
				int sprite_eff = SPRITEID_EFFECT_EXPLOSION_FEVER_MODE_01;
				if (_combo_num < _combo_num_max + FD_BLAST_MODE_TRIGGER)
				{
					EffectFireActive(sprite_eff, 0, _character_arr_fake[i]._size_tar + 1, cx, cy, 0);
				}
			}
			
			_character_arr_fake[i].SetType(CHARACTER_TYPE_NULL);
			
			return;
		}
	}
}
void FDGame::FindFlower2x2()
{
	for (int i = 0; i < FD_NUM_ROW-1; i++)
	{
		for (int j = 0; j < FD_NUM_COL-1; j++)
		{
			if (_character_arr[i][j].GetSize() >= CHARACTER_SIZE_REGULAR || _character_arr[i][j]._size_tar >= CHARACTER_SIZE_REGULAR)
			{
				continue;
			}
			if (_character_arr[i][j].GetState() != CHARACTER_STATE_NORMAL)
			{
				_is_need_find_fl_group = true;
			}
			_flower_has_new = false;
			if (HasFlower2x2(i, j))
			{
				FillSizeFlower2x2(i, j);
				int mk_x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleX(0, 0);
				int mk_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0);
				float x = FD_SLOT_WIDTH + j*FD_SLOT_WIDTH + mk_x;
				float y = FD_SLOT_WIDTH + i*FD_SLOT_WIDTH + mk_y;
				int scalestate = CHARACTER_SCALE_STATE_NORMAL;
				float scale = 1.0f;
				if (_flower_has_new || _character_size[i][j] == -1)
				{
					scalestate = CHARACTER_SCALE_STATE_OUT;
					scale = 0;
					UnEvaporateAdd(x, y, 0.05f, 4, _character_arr[i][j]._disable_hold_time);
					EvaporateAdd(x + CMath::RANDOM(-2,2)*10, y + CMath::RANDOM(-2, 2) * 10, 0.05f, 3,_character_arr[i][j]._disable_hold_time);
				}
				AddCharacterFake(x, y, _character_arr[i][j].GetType(), CHARACTER_SIZE_REGULAR, _character_arr[i][j]._disable_hold_time, scale, scalestate);
			}
		}
	}
}

void FDGame::FindFlower3x3()
{
	for (int i = 0; i < FD_NUM_ROW-2; i++)
	{
		for (int j = 0; j < FD_NUM_COL-2; j++)
		{
			if (_character_arr[i][j].GetSize() >= CHARACTER_SIZE_BIG || _character_arr[i][j]._size_tar >= CHARACTER_SIZE_BIG)
			{
				continue;
			}
			_flower_has_new = false;
			if (_character_arr[i][j].GetState() != CHARACTER_STATE_NORMAL)
			{
				_is_need_find_fl_group = true;
			}
			if (HasFlower3x3(i, j))
			{
				FillSizeFlower3x3(i, j);
				int mk_x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleX(0, 0);
				int mk_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0);
				float x = FD_SLOT_WIDTH / 2 + (j+1)*FD_SLOT_WIDTH + mk_x;
				float y = FD_SLOT_WIDTH / 2 + (i+1)*FD_SLOT_WIDTH + mk_y;
				int scalestate = CHARACTER_SCALE_STATE_NORMAL;
				float scale = 1.0f;
				if (_flower_has_new || _character_size[i][j] == -1)
				{
					scalestate = CHARACTER_SCALE_STATE_OUT;
					scale = 0;
					UnEvaporateAdd(x, y, 0.05f, 4, _character_arr[i][j]._disable_hold_time);
					EvaporateAdd(x + CMath::RANDOM(-2, 2) * 10, y + CMath::RANDOM(-4, 2) * 10, 0.05f, 3, _character_arr[i][j]._disable_hold_time);
				}
				AddCharacterFake(x, y, _character_arr[i][j].GetType(), CHARACTER_SIZE_BIG,_character_arr[i][j]._disable_hold_time, scale, scalestate);
			}
		}
	}
}

bool FDGame::HasFlower2x2(int row, int col)
{
	int type = _character_arr[row][col].GetType();
	bool has2x2 = true;
	int index = row*FD_NUM_COL + col;
	
	if (type != _character_arr[row][col+1].GetType() 
		|| _character_arr[row][col + 1].GetSize() >= CHARACTER_SIZE_REGULAR
		|| _character_arr[row][col + 1]._size_tar >= CHARACTER_SIZE_REGULAR
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has2x2 = false;
		return has2x2;
	}
	
	if (type != _character_arr[row+1][col + 1].GetType() 
		|| _character_arr[row + 1][col + 1].GetSize() >= CHARACTER_SIZE_REGULAR
		|| _character_arr[row + 1][col + 1]._size_tar >= CHARACTER_SIZE_REGULAR
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has2x2 = false;
		return has2x2;
	}
	
	if (type != _character_arr[row+1][col].GetType() 
		|| _character_arr[row + 1][col].GetSize() >= CHARACTER_SIZE_REGULAR
		|| _character_arr[row + 1][col]._size_tar >= CHARACTER_SIZE_REGULAR
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has2x2 = false;
		return has2x2;
	}
	if (_character_arr[row][col]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row][col + 1]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 1][col + 1]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 1][col]._scale_state > CHARACTER_SCALE_STATE_NORMAL)
	{
		_flower_has_new = true;
	}
	return has2x2;
}
void FDGame::CreateFlower2x2(int row, int col)
{
	int type = _character_arr[row][col].GetType();
	_character_arr[row][col + 1].Modify(type);
	_character_arr[row+1][col + 1].Modify(type);
	_character_arr[row + 1][col].Modify(type);
}
bool FDGame::HasFlower3x3(int row, int col)
{
	int type = _character_arr[row][col].GetType();
	bool has3x3 = true;
	if (type != _character_arr[row][col + 1].GetType() 
		|| _character_arr[row][col + 1].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row + 1][col + 1].GetType() 
		|| _character_arr[row + 1][col + 1].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row + 1][col + 1]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row + 1][col].GetType() 
		|| _character_arr[row + 1][col].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row + 1][col]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row][col + 2].GetType() 
		|| _character_arr[row][col + 2].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 2]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row + 1][col + 2].GetType() 
		|| _character_arr[row + 1][col + 2].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row + 1][col + 2]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row + 2][col + 2].GetType() 
		|| _character_arr[row + 2][col + 2].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row + 2][col + 2]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row + 2][col+1].GetType() 
		|| _character_arr[row + 2][col + 1].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row + 2][col + 1]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}

	if (type != _character_arr[row + 2][col].GetType() 
		|| _character_arr[row + 2][col].GetSize() >= CHARACTER_SIZE_BIG
		|| _character_arr[row + 2][col]._size_tar >= CHARACTER_SIZE_BIG
		|| _character_arr[row][col + 1].GetState() != CHARACTER_STATE_NORMAL)
	{
		has3x3 = false;
		return has3x3;
	}
	if (_character_arr[row][col]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row][col + 1]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row][col + 2]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 1][col]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 1][col+1]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 1][col+2]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 2][col]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 2][col+1]._scale_state > CHARACTER_SCALE_STATE_NORMAL
		|| _character_arr[row + 2][col+2]._scale_state > CHARACTER_SCALE_STATE_NORMAL)
	{
		_flower_has_new = true;
	}
	return has3x3;
}

void FDGame::CreateFlower3x3(int row, int col)
{
	int type = _character_arr[row][col].GetType();

	_character_arr[row][col + 1].Modify(type);
	_character_arr[row][col + 2].Modify(type);
	_character_arr[row+1][col].Modify(type);
	_character_arr[row+1][col + 1].Modify(type);
	_character_arr[row+1][col + 2].Modify(type);
	_character_arr[row+2][col].Modify(type);
	_character_arr[row+2][col + 1].Modify(type);
	_character_arr[row+2][col + 2].Modify(type);
}
void FDGame::FillSizeFlower2x2(int row, int col)
{
	_character_arr[row][col].SetSizeTar(CHARACTER_SIZE_REGULAR);
	_character_arr[row][col+1].SetSizeTar(CHARACTER_SIZE_REGULAR);
	_character_arr[row+1][col+1].SetSizeTar(CHARACTER_SIZE_REGULAR);
	_character_arr[row+1][col].SetSizeTar(CHARACTER_SIZE_REGULAR);

	_character_arr[row][col].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row][col + 1].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 1][col + 1].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 1][col].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	
	
}
void FDGame::FillSizeFlower3x3(int row, int col)
{
	_character_arr[row][col].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row][col + 1].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row][col + 2].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row + 1][col].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row + 1][col + 1].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row + 1][col + 2].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row + 2][col].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row + 2][col + 1].SetSizeTar(CHARACTER_SIZE_BIG);
	_character_arr[row + 2][col + 2].SetSizeTar(CHARACTER_SIZE_BIG);

	_character_arr[row][col].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row][col + 1].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row][col + 2].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 1][col].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 1][col + 1].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 1][col + 2].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 2][col].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 2][col + 1].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
	_character_arr[row + 2][col + 2].SetScaleState(CHARACTER_SCALE_STATE_HIDE);
}
void FDGame::AddEffectLight()
{
	for (int i = 0; i < EFFECT_LIGHT_NUM_MAX; i++)
	{
		
		float x = CMath::RANDOM(1, 10) * 15;
		float y = CMath::RANDOM(1, 10) * 15;
		float speed = CMath::RANDOM(1, 5)/20.0f;
		
		_effect_lighting[i].Init(x,y,0,-(CMath::RANDOM(1,2)/2.0f), speed);
	}
}
void FDGame::RenderEffectLight(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_LIGHT_NUM_MAX; i++)
	{
		_effect_lighting[i].Render(x, y, opac);
	}
}
void FDGame::UpdateEffectLight()
{
	for (int i = 0; i < EFFECT_LIGHT_NUM_MAX;i++)
	{
		_effect_lighting[i].Update();
	}
}




void FDGame::DropAllCube(bool isinit)
{
	if (SuperBlastFiring())
	{
		return;
	}
	int ftype_old = CHARACTER_TYPE_NULL;
	int fsame_per = 0;
	if (_game_level <= GAMEPLAY_LEVEL_EASY2)
	{
		int param = CGameDesign::FileGeneral_design::CSheetFlower_Respawn::Easy;
		fsame_per = GetDesignSpawPercent(0, param);
	}
	else if (_game_level <= GAMEPLAY_LEVEL_MEDIUM3)
	{
		int param = CGameDesign::FileGeneral_design::CSheetFlower_Respawn::Medium;
		fsame_per = GetDesignSpawPercent(0, param);
	}
	else if (_game_level <= GAMEPLAY_LEVEL_HARD3)
	{
		int param = CGameDesign::FileGeneral_design::CSheetFlower_Respawn::Hard;
		fsame_per = GetDesignSpawPercent(0, param);
	}
	int f_data_index = CMath::RANDOM(1, _game_design.General_design.SheetPercent_Spawn.int32Value._totalData) - 1;
	for (int i = 0; i < FD_NUM_COL; i++)
	{
		int count = 0; 
		int slot_null = -1;
		int num_empty = GetColNumEmpty(i);
		count = num_empty;
		while (num_empty > 0)
		{
			ftype_old = GetCubeAroundType(num_empty - 1, i);
			int indexnull = -1;
			for (int j = FD_NUM_ROW - 1; j >= 0; j--)
			{
				if (_character_arr[j][i].GetState() != CHARACTER_STATE_EMPTY)
				{
					continue;
				}
				else
				{
					indexnull = j;
					break;
				}
			}
			//printf("\n indexnull = %d,num_empty = %d", indexnull, num_empty);
			for (int j = indexnull; j > 0; j--)
			{
				int type = _character_arr[j-1][i].GetType();
				if (type != CHARACTER_TYPE_NULL && _character_arr[j-1][i].GetState() != CHARACTER_STATE_EMPTY)
				{
					_character_arr[j][i].Modify(&_character_arr[j - 1][i]);
					_character_arr[j][i]._scale = 1.0f;
					if (_character_arr[j - 1][i]._scale_state == CHARACTER_SCALE_STATE_HIDE)
					{
						_character_arr[j - 1][i]._scale_state = CHARACTER_SCALE_STATE_NORMAL;
					}
					//_character_arr[j][i].SetPos(_character_arr[j - 1][i].GetPosX(), _character_arr[j - 1][i].GetPosY());
					if (_character_arr[j-1][i]._scale_state == CHARACTER_SCALE_STATE_NORMAL)
					{
						_character_arr[j][i].SetState(CHARACTER_STATE_MOVING);
						if (_fever_mode_active)
						{
							_character_arr[j][i]._disable_hold_time = 10;
						}
						
					}
					else
					{
						/*_character_arr[j][i].SetState(CHARACTER_STATE_NORMAL);
						_character_arr[j][i]._scale_state = _character_arr[j - 1][i]._scale_state;
						_character_arr[j][i]._scale = _character_arr[j - 1][i]._scale;
						_character_arr[j][i]._disable_hold_time = _character_arr[j - 1][i]._disable_hold_time;
						_character_arr[j][i]._speed_scale = _character_arr[j - 1][i]._speed_scale;*/
					}
					//_character_arr[j][i]._disable_hold_time = (count - num_empty) * 15;
				}
			}
			
			//printf("\n ftype_old = %d,index=%d,%d", ftype_old, num_empty - 1,i);
			int ftype = GetCubeAroundType(num_empty - 1, i);
			int percent = 0;
			int frand = CMath::RANDOM(1, 100);
			for (int i = 0; i <= CHARACTER_TYPE_ORCHILD; i++)
			{
				int fpercent = GetDesignFlowerPercent(f_data_index, i);
				if (frand <= percent + fpercent && frand > percent)
				{
					ftype = i;
					break;
				}
				percent += fpercent;
			}
			if (frand <= fsame_per)
			{
				if (ftype_old != CHARACTER_TYPE_NULL)
				{
					ftype = ftype_old;
				}
			}
			else
			{
				while (ftype == ftype_old && ftype_old != CHARACTER_TYPE_NULL)
				{
					percent = 0;
					frand = CMath::RANDOM(1, 100);
					f_data_index = CMath::RANDOM(1, _game_design.General_design.SheetPercent_Spawn.int32Value._totalData) - 1;
					for (int f = 0; f <= CHARACTER_TYPE_ORCHILD; f++)
					{
						int fpercent = GetDesignFlowerPercent(f_data_index, f);
						if (frand <= percent + fpercent && frand > percent)
						{
							ftype = f;
							break;
						}
						percent += fpercent;
					}
				}
			}
			

			/*for (int f = 0; f <= CHARACTER_TYPE_ORCHILD; f++)
			{
				int fpercent = GetDesignFlowerPercent(f_data_index, f);
				if (frand <= percent + fpercent && frand > percent)
				{
					ftype = f;
					break;
				}
				percent += fpercent;
			}
			int count = 0;
			while (ftype == _powerup_eat_random_type && count < 30)
			{
				percent = 0;
				frand = CMath::RANDOM(1, 100);
				f_data_index = CMath::RANDOM(1, _game_design.General_design.SheetPercent_Spawn.int32Value._totalData) - 1;
				for (int f = 0; f <= CHARACTER_TYPE_ORCHILD; f++)
				{
					int fpercent = GetDesignFlowerPercent(f_data_index, f);
					if (frand <= percent + fpercent && frand > percent)
					{
						ftype = f;
						break;
					}
					percent += fpercent;
				}
				count++;
			}
			*/
			int time_hold = 0;
			if (_fever_mode_active)
			{
				time_hold = 10;
			}
			_character_arr[0][i].CreateNew(ftype, time_hold, 0.08f);
			_character_arr[0][i].SetState(CHARACTER_STATE_MOVING);
			float x = _character_arr[0][i].GetPosX();
			//float y = _character_arr[0][i].GetPosY() - (count - num_empty )*FD_SLOT_WIDTH;
			float y = _character_arr[0][i].GetPosY() - num_empty*FD_SLOT_WIDTH;
			_character_arr[0][i].SetPos(x, y);
			num_empty = GetColNumEmpty(i);
			
		}
	}
}
int FDGame::GetColNumEmpty(int col)
{
	int num = 0;
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		if (_character_arr[i][col].GetState() == CHARACTER_STATE_EMPTY)
		{
			num++;
		}
	}
	return num;
}
void FDGame::FlowerRenderInit()
{
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		_flower_render_index[i] = i;
		_flower_render_ingame[i].Init(0, 0);
        //printf("\n _user._flower_arr[%d]._id = %d",i,_user._flower_arr[i]._id);
		_flower_render_ingame[i].CreateNew(_user._flower_arr[i]._id, i*10, 0.05f);
		if (_user._flower_arr[i]._id == CHARACTER_TYPE_SUNFLOWER)
		{
			_flower_render_ingame[i]._xp_collected = -1;
		}
	}
}
void FDGame::FlowerRenderAddCollected(int id)
{
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		if (_flower_render_ingame[i].GetType() == id)
		{
			_flower_render_ingame[i]._xp_collected++;
			break;
		}
	}
}

void FDGame::ResetFlowerHint()
{
	_hint_frame_countdown = 3 * FPS_MAX;
	_hint_group_active = -1;
	_hint_group_num = 0;
	_hint_has_ready = false;
	for (int i = 0; i < FD_NUM_COL*FD_NUM_ROW; i++)
	{
		_flower_hint[i]._index = -1;
		_flower_hint[i]._group = -1;
	}
}
void FDGame::ResetFlowerHint(int group)
{
	for (int i = 0; i < FD_NUM_COL*FD_NUM_ROW; i++)
	{
		if (_flower_hint[i]._group == group)
		{
			_flower_hint[i]._index = -1;
			_flower_hint[i]._group = -1;
		}
	}
}
void FDGame::AddFlowerHint(int index, int group)
{
	for (int i = 0; i < FD_NUM_COL*FD_NUM_ROW; i++)
	{
		if (_flower_hint[i]._index == -1)
		{
			_flower_hint[i]._index = index;
			_flower_hint[i]._group = group;
			break;
		}
	}
}
void FDGame::FindFlowerHint()
{
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			int state = _character_arr[i][j].GetState();
			int index = i*FD_NUM_COL + j;
			if (state != CHARACTER_STATE_NORMAL)
			{
				continue;
			}
			_hint_num_ready = 0;
			AddFlowerHint(index, _character_arr[i][j].GetType(), _hint_group_num);
			if (_hint_num_ready >= FD_ACTIVE_NUM_MIN)
			{
				_hint_has_ready = true;
				_hint_group_num++;
			}
			else
			{
				ResetFlowerHint(_hint_group_num);
			}
		}
	}
}
void FDGame::AddFlowerHint(int index, int type, int group)
{
	bool ret = false;
	int row = index / FD_NUM_COL;
	int col = index % FD_NUM_COL;
	_character_arr[row][col].SetState(CHARACTER_STATE_WAITING);
	int k = row + 1;
	bool same = true;
	AddFlowerHint(index, group);
	_hint_num_ready++;
	while (k < FD_NUM_ROW && same)
	{
		int ctype = _character_arr[k][col].GetType();
		int state = _character_arr[k][col].GetState();
		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = k*FD_NUM_COL + col;
			AddFlowerHint(ind, type, group);
		}
		else
		{
			same = false;
		}
	}
	k = row - 1;
	same = true;
	while (k >= 0 && same)
	{

		int ctype = _character_arr[k][col].GetType();
		int state = _character_arr[k][col].GetState();

		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = k*FD_NUM_COL + col;
			AddFlowerHint(ind, type, group);
		}
		else
		{
			same = false;
		}
	}
	k = col + 1;
	same = true;
	while (k < FD_NUM_COL && same)
	{
		int ctype = _character_arr[row][k].GetType();
		int state = _character_arr[row][k].GetState();

		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = row*FD_NUM_COL + k;
			AddFlowerHint(ind, type, group);
		}
		else
		{
			same = false;
		}
	}
	k = col - 1;
	same = true;
	while (k >= 0 && same)
	{
		int ctype = _character_arr[row][k].GetType();
		int state = _character_arr[row][k].GetState();

		if ((state == CHARACTER_STATE_NORMAL || state == CHARACTER_STATE_EMPTY) && ctype == type)
		{
			int ind = row*FD_NUM_COL + k;
			AddFlowerHint(ind, type, group);
		}
		else
		{
			same = false;
		}
	}
}
void FDGame::ActiveFlowerHint(int group)
{
	for (int i = 0; i < FD_NUM_COL*FD_NUM_ROW; i++)
	{
		if (_flower_hint[i]._group != group)
		{
			continue;
		}
		if (_flower_hint[i]._index != -1)
		{
			int row = _flower_hint[i]._index / FD_NUM_COL;
			int col = _flower_hint[i]._index % FD_NUM_COL;
			if (_character_arr[row][col]._anim.GetCurrentAnim() != 3)
			{
				_character_arr[row][col]._anim.SetAnim(3, false);
			}
			else
			{
				if (_character_arr[row][col]._anim.IsCurrentAnimStop())
				{
					_character_arr[row][col]._anim.SetAnim(3, false);
				}
			}
		}
	}
}

void FDGame::AddEnergy(int energy)
{
	return;
	_energy_total += energy;
	if (_energy_total >= _power_up_icon[POWER_UP_TYPE_CREATE_GROUP]._energy_max + 10)
	{
		_energy_total = FD_ENERGY_TOTAL_MAX;
	}
	if (_energy_total<0)
	{
		_energy_total = 0;
	}
}