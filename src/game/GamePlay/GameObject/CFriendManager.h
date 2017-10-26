#if !CFRIEND_LIST_H
#define CFRIEND_LIST_H 1
#define SAVE_FRIEND_FILE_NAME "friend.pine"

#define FRIEND_VERSION_0					0
#define FRIEND_VERSION						FRIEND_VERSION_0

#define SAVE_FRIEND_VERSION						0
#define SAVE_FRIEND_NUM_CURRENT_FRIEND			(SAVE_FRIEND_VERSION + 2)
#define SAVE_FRIEND_BEGIN_ID					(SAVE_FRIEND_NUM_CURRENT_FRIEND + 2)


#define SAVE_FRIEND_ID							(0)
#define SAVE_FRIEND_NAME						(SAVE_FRIEND_ID + SOCIAL_ID_STRING_LENGTH)
#define SAVE_FRIEND_NAME_LENGTH                 (SAVE_FRIEND_NAME + SAVE_CHARACTER_NAME_LENGTH_CATCH * 4)
#define SAVE_FRIEND_LENGTH						(SAVE_FRIEND_NAME_LENGTH + 1)




#define USER_AVATAR_WW						(100)
#define USER_AVATAR_HH						(174)

#define MAX_TOP_PLAYER								50
#define MAX_CATCH_TOP_PLAYER						10
#define MAX_TIME_CATCH								10 //minit
#define CATCH_FILE_NAME								"topplayer.pine"

#define TOP_PLAYER_VERSION_0					0
#define TOP_PLAYER_VERSION						TOP_PLAYER_VERSION_0

#define CATCH_TOP_PLAYE_VERSION						0
#define CATCH_LEVEL									(CATCH_TOP_PLAYE_VERSION + 2)
#define CATCH_NUM_TOP								(CATCH_LEVEL + 2*MAX_CATCH_TOP_PLAYER)
#define CATCH_TIME									(CATCH_NUM_TOP + 2*MAX_CATCH_TOP_PLAYER)							
#define CATCH_TOP_PLAYER_USER_ID					(CATCH_TIME + 8*MAX_CATCH_TOP_PLAYER)
#define CATCH_TOP_PLAYER_RANK						(CATCH_TOP_PLAYER_USER_ID + (64*MAX_TOP_PLAYER)*MAX_CATCH_TOP_PLAYER)
#define CATCH_TOP_PLAYER_SCORE						(CATCH_TOP_PLAYER_RANK + (2*MAX_TOP_PLAYER)*MAX_CATCH_TOP_PLAYER)

#define SAVE_CATH_LENGHT							(CATCH_TOP_PLAYER_SCORE + (4*MAX_TOP_PLAYER)*MAX_CATCH_TOP_PLAYER)

struct AccountInfo
{
	char _id[SOCIAL_ID_STRING_LENGTH];
    char _guidID[512];
	INT32 _userNameUnicode[SAVE_CHARACTER_NAME_LENGTH_CATCH];
	int _userNameUnicodeLenght;

	PineImage _avatar,_avatarMask;
	BYTE _avatarState,_avatarMaskState;
	char _avatarUrl[1024];
    char _userName[128];
	INT32 _score;
    int _rank;
	//effect
	BYTE _state;
	float _offX;	
};
struct  CatchTopPlayer
{
	UINT64 _timeCatch;
	int _levelCath;
	int _numTop;
};
class CFriendManager
{
	CGame *_game;
	Graphic *_gp;
public:
	CFriendManager()
    {
#if ADD_FACEBOOK
		_avatarState = AVATAR_STATE_NONE;       
#endif
    };

	char *_buffer;


	void SetInitGame(CGame *game);
	PineImage _avatar;
	BYTE _avatarState;
	int _versionFriend, _versionTopPlayer;
	int _numCurrentFriend, _numTopPlayer;
    int _avatarFriendLoaded;
	int _avatarTopLoaded;

	AccountInfo *_friendList;
	AccountInfo _topPlayer[MAX_TOP_PLAYER];
	int _numTopServer;
	AccountInfo *GetFriend(const char* id);
	void AddFriend(AccountInfo account);
	static void CopyAccInfo(AccountInfo *accCopy, AccountInfo *accTarget, bool newFriend);
	static void CopyTopPlayer(AccountInfo accCopy, AccountInfo &accTarget);

    void AddTopPlayer(AccountInfo acc);
    int GetIndexFriendFromTop(AccountInfo acc);

	
	//friend save
	bool DataLoad();
	void DataSave();	
	void DataDefault();
	void SynForNewFriendVersion(int fromVersion);
	void NewDataFriendVersion(int version);


	//catch top player
	char _bufferTop[SAVE_CATH_LENGHT];
	CatchTopPlayer _catchTopPlayer[MAX_CATCH_TOP_PLAYER];
	void UpdateTimeCatchTop();
	int HasCatchTop(int level);
	int HasMinTimeCatch();
	int CanInputCatch(int level);
	bool CheckFullCatch();
	void InputCatch(int level);
	bool OutputCatch(int level);
	void ClearCatch(int level);
	void LoadCatch();
	void SaveCatch();
	void DefaultCatch();

	void SynForNewTopVersion(int fromVersion);
	void NewDataTopVersion(int version);
	void UpdateCreateAvatarFriend();
	void UpdateDownloadAvatar();
    void UnloadMemForAvatar();

	bool _isShowFriend;
	bool _isCompleteLoadTop;
	bool _isRenderTopPlayer;
	CAnimObject _animWifi;
	CAnimObject _animNavigationLeft, _animNavigationRight;
	int _opaLeft, _opcaRight;
	int _myRankInLevel;
	int _stateShareFriend;

	void InitTopPlayer();
	void UpdateTopPlayer();
	void RenderTopPlayer(Button buttonConnect, float x, float y, PineMatrix4 tranf);
	void UpdateScroll();

	int _oldMyRank;
	int GetRankTop(const char* id);
	AccountInfo *GetFriendAffterMe();
	AccountInfo *GetMeInTop();
	
	int CheckNewRank(int rank);
	int GetIndexRank(int rank);
	void SortFriend();
	int GetRank(char* fbid);
    int GetScore(char* fbid);
	float _touchDragX;
	float _offsetDragX;
	float _scrollX;
	CScrollBar _scrollBarX;
	int _scroll_bar_touch_id;

	//invite friend
#define BUTTON_INVITE_NUMS				2
#define BUTTON_INVITE_CLOSE				0
#define BUTTON_INVITE_REFRESH			1

	void UpdateScrollFriend();
	bool UpdateTouchFriend();
	bool UpdateTouchFriendList();
	float _pointScrollY;
	float _pointScrollH;

	float _touchDragY;
	float _offsetDragY;
	float _scrollY;
	CScrollBar _scrollBarY;

	int _stateFriend,_opa;
	float _offX, _zoom;

	int _numFriend;

    char _fbFriendID[128];
    const char* GetFBFriendID(const char* idFriend);

};

#endif
