/**
*	will be included in CGame.h
*	specific vars and methods for gameplay
*/

public:
// game state
	void UpdateLogo();
	void RenderLogo();
	
	
private:
	
public:
	

//Save System.
void GameDataLoad();
void GameDataSave();
void GameDataDefault();
void GameDataToBuffer();
void GameLoadDataFromBuffer();

BYTE red, green, blue, alpha;
#define GAME_FILE	"pinerecord.dat"

#define GAME_VERSION				100

#define GAME_DATA_VERSION			(0)

#define GAME_DATA_LENGTH				(GAME_DATA_VERSION + 4)



char _buffer_save[GAME_DATA_LENGTH];
int _game_version;
int _rate;

//PineHighScore _pineHighScore;

 long _sys_begin_second;
 long _sys_millisecond;
 long _game_millisecond;
 long _sys_second;

void UnloadGameBackground();
bool LoadGameForceground();

bool _needresume_when_playing;


void PurchaseIAP(const char* strID, enumIAPResponseStatus sucessed);

int  _frame_auto_stop;
PineImage _image;

PineGame *_main_game;
PineGame  *_feature_game;
PineGame    *_current_game;

bool _isNeedOptimize;
CServerThread _serverThread;
CFreeTypeText _freeTypeText;
CFriendManager _friendManager;
bool _fb_just_init;

void ResetValueICloud();
bool SaveToICloud();
int CheckCanPushDataSave();
void LoadFromICloud();
void DefaultICloud();
void CheckICloud();
void AutoSyncICloud();
int _icloud_state;
#define ICLOUD_STATE_NODATA      -2
#define ICLOUD_STATE_DISCONECT   -3
#define ICLOUD_STATE_OLD_VERSION -1
#define ICLOUD_STATE_NEW_VERSION 0
#define ICLOUD_STATE_READY_SYNS  1

#define PROCESSING_VIEW_TYPE_NONE       0
#define PROCESSING_VIEW_TYPE_GC         1
#define PROCESSING_VIEW_TYPE_ICLOUD     2
#define PROCESSING_VIEW_CHECK_NETWORK   3
#define PROCESSING_VIEW_TYPE_IAP        4

int _processing_view_type;
int _processing_time_out;
bool _is_purchase_procesing;
int _purchase_procesing_opac;
int _purchase_effect_rotate1;
int _purchase_effect_rotate2;
CAnimObject _anim_waiting;
int _loading_count_frame;
void RenderLoadingPurchase(float x, float y);
#if ADD_FACEBOOK
int _stateInviteFriend;
#endif
void CompleteSendRequest();
PineStore _pineStore;
//:todo--Admod-android
#if defined (ANDROID_OS)
//admod
PineAdmobAsync _admob_manager;
/*********************************/
bool InitAds();
void ShowAds();
void ShowBannerAds();
void UpdateAds();
/**********************************/
bool _on_show_video_ads;
bool _has_show_ads_banner;
bool resetBoard;

int _time_count_for_show_ads_banner;
int _admob_play_count;
/*********************************/
#define INITIAL_ADMOD
#endif
float reduce;
float farmerReduce;
float fbButtonReduce;

