#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

CUser::CUser()
{
}

CUser::~CUser()
{
}
void CUser::Init()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	sprintf(_userName, "%s","user_name");
	sprintf(_userID, "%s","user_id");
    sprintf(_user_fb_id, "%s","fb_id");
    sprintf(_user_fb_name, "%s","fb_name");
    sprintf(_avatar_URL, "%s","avatar_url");
	_free_gift_num = 0;
	_sound_enable = true;
	_music_enable = true;
	_notify_enable = false;
	_free_gift_collected = 0;
    _save_cloud_version = 0;
	SetTimeFreeGift();
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		_flower_arr[i].Init(i, 0, 0);
	}
	game->PowerUpInit();
	_playgame_times = 0;
    _has_removed_ads = false;
    _hadLoginFB = false;
	_play_game_num = 1;
}
void CUser::UserDefault()
{
	_version = GAME_CURRENT_VERSION;
	Init();
    //GAME()->DefaultICloud();
}
void CUser::UserDataSave()
{
	SaveToBuffer();

#if defined(MAC_OS) || defined(_WINDOWS_PHONE8)
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
#else

	FILE * stream = NULL;

	stream = fopen(SAVE_FILE_NAME, "wb");

	if (stream != NULL)
	{
		fwrite((void *)_buffer, sizeof(BYTE), MAX_SAVE_BUFFER_CACHE, stream);
	}

	if (stream != NULL)
	{
		fclose(stream);
	}
    
#endif
    
}
void CUser::UserDataLoad()
{
	int saved = 0;
	Buffer* buff = OS_LoadAppData(SAVE_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_buffer, buff->Data(), saved);
		delete buff;
	}
	printf("Load data saved = %d", saved);
	if (saved == 0)
	{
		UserDefault();
		UserDataSave();
	}
	else
	{
		LoadFromBuffer();
		if (_version < SAVE_CURRENT_VERSION)
		{
			//xu ly syn server
			UserDataSave();
		}
        
	}
#if GAME_TEST
    GAME()->ResetValueICloud();
#endif
}
void CUser::LoadFromBuffer()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	_version = GAME()->GetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION);
	for (int i = 0; i < 128; i++)
	{
		_userName[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_NAME + i);
		_userID[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_ID + i);
	}
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		int id = GAME()->GetInt32At(_buffer, SAVE_FORMAT_FLOWER_ID + i * 4);
		int level = GAME()->GetInt32At(_buffer, SAVE_FORMAT_FLOWER_LEVEL + i * 4);
		int xp = GAME()->GetInt32At(_buffer, SAVE_FORMAT_FLOWER_XP + i * 4);
		_flower_arr[i].Init(id, level, xp);
	}
	game->_hight_score = GAME()->GetInt64At(_buffer, SAVE_FORMAT_GAME_HIGHT_SCORE);
	_free_gift_num = GAME()->GetInt16At(_buffer, SAVE_FORMAT_GAME_FREE_GIFT_NUM);
	_free_gift_time = GAME()->GetInt64At(_buffer, SAVE_FORMAT_GAME_FREE_GIFT_TIME);
	INT_TIME fg_time = _free_gift_time - time(NULL);
	if (fg_time > 0)
	{
		game->_free_gift_time.setDateBYSecond(fg_time);
	}
	else
	{
		game->_free_gift_time.setNullDay();
	}
	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		int pu_num = 0;
		pu_num = GAME()->GetInt16At(_buffer, SAVE_FORMAT_POWER_UP_NUM + i * 2);
		game->_power_up_icon[i].Init(i, 0, pu_num, 0, 0);
	}
	_sound_enable = GAME()->GetByteAt(_buffer, SAVE_FORMAT_SOUND_ENABLE);
	_music_enable = GAME()->GetByteAt(_buffer, SAVE_FORMAT_MUSIC_ENABLE);
	_notify_enable = GAME()->GetByteAt(_buffer, SAVE_FORMAT_NOTIFY_ENABLE);
	_free_gift_collected = GAME()->GetInt16At(_buffer, SAVE_FORMAT_FREE_GIFT_COLLECTED);
	_save_cloud_version = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION);
	_playgame_times = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_GAMEPLAY_TIME);
    _has_removed_ads = GAME()->GetByteAt(_buffer, SAVE_FORMAT_SAVE_REMOVE_ADS);
    _hadLoginFB = GAME()->GetByteAt(_buffer, SAVE_FORMAT_SAVE_HAS_FB_LOGIN);
    
	for (int i = 0; i < SAVE_CHARACTER_NAME_LENGTH_CATCH; i++)
	{
		_userNameUnicode[i] = GAME()->GetInt32At(_buffer, SAVE_FORMAT_USER_NAME_UNICODE + i * 4);
	}
	_userNameUnicodeLenght = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_NAME_LENGHT_UNICODE);
	_play_game_num = GAME()->GetInt32At(_buffer, SAVE_FORMAT_USER_PLAY_NUM);
    //game->PowerUpInit();
    if(_version < GAME_CURRENT_VERSION_4)
    {
		_playgame_times = 0;
        _version = GAME_CURRENT_VERSION;
    }
    if(_version < GAME_CURRENT_VERSION_5)
    {
        _has_removed_ads = false;
        _hadLoginFB = false;
        _version = GAME_CURRENT_VERSION;
    }
	if (_version < GAME_CURRENT_VERSION_7)
	{
		_playgame_times = 0;
		_version = GAME_CURRENT_VERSION;
	}
	if (_version < GAME_CURRENT_VERSION_9)
	{
		_version = GAME_CURRENT_VERSION;
		_play_game_num = 1;
	}
}
void CUser::SaveToBuffer()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
    _save_cloud_version++;
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION, _version);
	for (int i = 0; i < 128; i++)
	{
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_NAME + i, _userName[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_ID + i, _userID[i]);
	}
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		int id = _flower_arr[i]._id;
		int level = _flower_arr[i]._level;
		int xp = _flower_arr[i]._xp;
		GAME()->SetInt32At(_buffer, SAVE_FORMAT_FLOWER_ID + i * 4,id);
		GAME()->SetInt32At(_buffer, SAVE_FORMAT_FLOWER_LEVEL + i * 4,level);
		GAME()->SetInt32At(_buffer, SAVE_FORMAT_FLOWER_XP + i * 4,xp);
	}
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_GAME_HIGHT_SCORE, game->_hight_score);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_GAME_FREE_GIFT_NUM, _free_gift_num);
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_GAME_FREE_GIFT_TIME, _free_gift_time);
	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		GAME()->SetInt16At(_buffer, SAVE_FORMAT_POWER_UP_NUM + i * 2, game->_power_up_icon[i]._num);
	}
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_SOUND_ENABLE, _sound_enable);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_MUSIC_ENABLE, _music_enable);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_NOTIFY_ENABLE, _notify_enable);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_FREE_GIFT_COLLECTED, _free_gift_collected);
    GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION, _save_cloud_version);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_GAMEPLAY_TIME, _playgame_times);
    GAME()->SetByteAt(_buffer, SAVE_FORMAT_SAVE_REMOVE_ADS,_has_removed_ads);
    GAME()->SetByteAt(_buffer, SAVE_FORMAT_SAVE_HAS_FB_LOGIN,_hadLoginFB);
	for (int i = 0; i < SAVE_CHARACTER_NAME_LENGTH_CATCH; i++)
	{
		GAME()->SetInt32At(_buffer, SAVE_FORMAT_USER_NAME_UNICODE + i * 4, _userNameUnicode[i]);
	}
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_NAME_LENGHT_UNICODE, _userNameUnicodeLenght);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_USER_PLAY_NUM, _play_game_num);
}
void CUser::SetUserID(char* userid)
{
	sprintf(_userID, "%s", userid);
}
const char* CUser::GetUserID()
{
	return _userID;
}
void CUser::SetUserName(char* username)
{
	sprintf(_userName, "%s", username);
}
const char* CUser::GetName()
{
	return _userName;
}
void CUser::SetFBID(char* userid)
{
    sprintf(_user_fb_id, "%s", userid);
}
const char* CUser::GetFBID()
{
    return _user_fb_id;
}
void CUser::SetFBName(char* username)
{
    sprintf(_user_fb_name, "%s", username);
}
const char* CUser::GetFBName()
{
    return _user_fb_name;
}
void CUser::SetAvatarURL(char* url)
{
    sprintf(_avatar_URL, "%s", url);
}
const char* CUser::GetAvatarURL()
{
    return _avatar_URL;
}

void CUser::FlowerInit(int id, int level, int xp)
{
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		_flower_arr[i].Init(id, level, xp);
	}
}
int CUser::FlowerGetLevel(int id)
{
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		if (_flower_arr[i]._id == id)
		{
			return _flower_arr[i]._level;
		}
	}
	return 0;
}
void CUser::FlowerAddXP(int type, int xp)
{
	for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
	{
		if (_flower_arr[i]._id == type)
		{
			_flower_arr[i]._xp_collected += xp;
			while (_flower_arr[i]._xp_collected >= _flower_arr[i]._xp_need)
			{
				_flower_arr[i].LevelUp();
				_flower_arr[i]._xp_collected -= _flower_arr[i]._xp_need;
				printf("\n level up type = %d",type);
			}
		}
	}
}

void CUser::SetTimeFreeGift()
{
	
	FDGame *game = (FDGame*)GAME()->_current_game;
	//_free_gift_num = CMath::RANDOM(0, game->_game_design.General_design.SheetTreasure_Time.int32Value._totalData - 1);
	int param = CGameDesign::FileGeneral_design::CSheetTreasure_Time::time;
	int max_time = game->GetDesignFreeGiftTime(game->_game_design.General_design.SheetTreasure_Time.int32Value._totalData - 1, param) * 60;
	int time_off = time(NULL) - _free_gift_time;
	int index = _free_gift_num - 1;
	if (index < 0)
	{
		index = game->_game_design.General_design.SheetTreasure_Time.int32Value._totalData - 1;
	}
	int time_design = game->GetDesignFreeGiftTime(index, param) * 60;
	time_off += time_design;
	if (time_off >= max_time)
	{
		_free_gift_num = 0;
	}
	int ds_time = game->GetDesignFreeGiftTime(_free_gift_num, param) * 60;
	_free_gift_time = time(NULL) + ds_time;
	_free_gift_num++;
	if (_free_gift_num >= game->_game_design.General_design.SheetTreasure_Time.int32Value._totalData)
	{
		_free_gift_num = 0;
	}

	_free_gift_collected++;
	game->_free_gift_time.setDateBYSecond(ds_time);
    UserDataSave();
}
