#ifndef _C_USER_H_
#define _C_USER_H_



#include "../../Lib/DataType/CType.h"

#define SAVE_FILE_NAME									"FDGame.pine"

#define SAVE_BACKUP_FILE_NAME							"FDGameBK.pine"


#define SAVE_VERSION_FIRST_VERSION						0
#define SAVE_CURRENT_VERSION							0

#define GAME_CURRENT_VERSION_1							1
#define GAME_CURRENT_VERSION_2							2//1/4/2016
#define GAME_CURRENT_VERSION_3							3//12/4/2016
#define GAME_CURRENT_VERSION_4							4//26/4/2016
#define GAME_CURRENT_VERSION_5							5//26/4/2016
#define GAME_CURRENT_VERSION_6							6//28/4/2016
#define GAME_CURRENT_VERSION_7							7//11/5/2016
#define GAME_CURRENT_VERSION_8							7//20/5/2016
#define GAME_CURRENT_VERSION_9                          8//24/5/2016
#define GAME_CURRENT_VERSION							GAME_CURRENT_VERSION_8

#define SAVE_FORMAT_CURRENT_VERSION						0
#define SAVE_FORMAT_USER_ID					(SAVE_FORMAT_CURRENT_VERSION+8)
#define SAVE_FORMAT_USER_NAME				(SAVE_FORMAT_USER_ID + 128)

#define SAVE_FORMAT_FLOWER_ID				(SAVE_FORMAT_USER_NAME + 128)
#define SAVE_FORMAT_FLOWER_LEVEL			(SAVE_FORMAT_FLOWER_ID + 4*FD_FLOWER_NUM_TYPE)
#define SAVE_FORMAT_FLOWER_XP				(SAVE_FORMAT_FLOWER_LEVEL + 4*FD_FLOWER_NUM_TYPE)

#define SAVE_FORMAT_GAME_HIGHT_SCORE		(SAVE_FORMAT_FLOWER_XP + 4*FD_FLOWER_NUM_TYPE)
#define SAVE_FORMAT_GAME_FREE_GIFT_TIME		(SAVE_FORMAT_GAME_HIGHT_SCORE + 8)
#define SAVE_FORMAT_GAME_FREE_GIFT_NUM		(SAVE_FORMAT_GAME_FREE_GIFT_TIME + 8)
#define SAVE_FORMAT_POWER_UP_NUM			(SAVE_FORMAT_GAME_FREE_GIFT_NUM + 2)
#define SAVE_FORMAT_MUSIC_ENABLE			(SAVE_FORMAT_POWER_UP_NUM + 2*POWER_UP_NUM_MAX)
#define SAVE_FORMAT_SOUND_ENABLE			(SAVE_FORMAT_MUSIC_ENABLE + 1)
#define SAVE_FORMAT_NOTIFY_ENABLE			(SAVE_FORMAT_SOUND_ENABLE + 1)
#define SAVE_FORMAT_FREE_GIFT_COLLECTED		(SAVE_FORMAT_NOTIFY_ENABLE + 1)
#define SAVE_FORMAT_SAVE_CLOUD_VERSION		(SAVE_FORMAT_FREE_GIFT_COLLECTED + 2)
#define SAVE_FORMAT_SAVE_GAMEPLAY_TIME		(SAVE_FORMAT_SAVE_CLOUD_VERSION + 4)
#define SAVE_FORMAT_SAVE_REMOVE_ADS         (SAVE_FORMAT_SAVE_GAMEPLAY_TIME + 4)
#define SAVE_FORMAT_SAVE_HAS_FB_LOGIN       (SAVE_FORMAT_SAVE_REMOVE_ADS + 1)
#define SAVE_FORMAT_USER_NAME_UNICODE       (SAVE_FORMAT_SAVE_HAS_FB_LOGIN + 1)
#define SAVE_FORMAT_USER_NAME_LENGHT_UNICODE    (SAVE_FORMAT_USER_NAME_UNICODE + SAVE_CHARACTER_NAME_LENGTH_CATCH*4)

#define SAVE_FORMAT_USER_PLAY_NUM			(SAVE_FORMAT_USER_NAME_LENGHT_UNICODE + 2)

#define SAVE_FORMAT_END						(SAVE_FORMAT_USER_PLAY_NUM + 2)
#define SAVE_FORMAT_LENGTH					(SAVE_FORMAT_END)

#define SAVE_MAX_BUFFER				10000
#define MAX_SAVE_BUFFER_CACHE		10000

class CUser
{
public:
	CUser();
	~CUser();

	char _buffer[SAVE_MAX_BUFFER];
	INT64 _version;

	void Init();
	void UserDefault();
	void UserDataSave();
	void UserDataLoad();
	void LoadFromBuffer();
	void SaveToBuffer();
	void SetUserID(char* userid);
	const char* GetUserID();
	void SetUserName(char* username);
	const char* GetName();
	CFlower _flower_arr[FD_FLOWER_NUM_TYPE];
	void FlowerInit(int id, int level, int xp);
	int FlowerGetLevel(int id);
	void FlowerAddXP(int type,int xp);
	INT_TIME _free_gift_time;
	int _free_gift_num;
	int _free_gift_collected;
	
	void SetTimeFreeGift();
	bool _music_enable;
	bool _sound_enable;
	bool _notify_enable;
    
    void SetFBID(char* userid);
    const char* GetFBID();
    void SetFBName(char* username);
    const char* GetFBName();
    void SetAvatarURL(char* url);
    const char* GetAvatarURL();
    int _save_cloud_version;
	int _playgame_times;
    bool _has_removed_ads;
    PineImage _avatar;
    int _avatarState;
    bool _hadLoginFB;
    INT32 _userNameUnicode[SAVE_CHARACTER_NAME_LENGTH_CATCH];
    int _userNameUnicodeLenght;
    char _user_fb_id[128];
    char _user_fb_name[128];
	int _play_game_num;
private:
	char _userName[128];
	char _userID[128];
    
    char _avatar_URL[1024];
};


#endif

