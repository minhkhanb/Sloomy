#define FD_NUM_ROW		8
#define FD_NUM_COL		8
#define FD_NUM_ARR		(FD_NUM_ROW*FD_NUM_COL)

#define GAME_TEST		0

#define FD_SLOT_WIDTH	90
#define FD_ACTIVE_NUM_MIN		3
#define FD_TIME_A_GAME			(1*60)
#define FD_FLOWER_NUM_TYPE				6

#define FD_GENERAL_DESIGN_TIME_BONUS		0
#define FD_GENERAL_DESIGN_SUN_LEVEL			1

#define FD_GENERAL_DESIGN_XSCORE_PARAM		1
#define FD_GENERAL_DESIGN_XTIME_PARAM		3
#define FD_GENERAL_DESIGN_XENERGY_PARAM		11
#define FD_GENERAL_DESIGN_FEVER_TRIGGER		12
#define FD_GENERAL_DESIGN_COMBO_TIME		13
#define FD_GENERAL_DESIGN_FEVER_TIME		14
#define FD_GENERAL_DESIGN_BLAST_TRIGGER		15
#define FD_GENERAL_DESIGN_FEVER_MODE_TIME	16

#define FD_FLOWER_SORT_TYPE_NUM			0
#define FD_FLOWER_SORT_TYPE_ID			1

#define FD_FIND_SUR_TYPE_NORMAL			0
#define FD_FIND_SUR_TYPE_TOUCH			1
#define FD_FIND_SUR_TYPE_HINT			2

#define POWER_UP_NUM_MAX			3
#define POWER_UP_TYPE_CREATE_GROUP		1
#define POWER_UP_TYPE_SORT				2
#define POWER_UP_TYPE_EAT_RANDOM		0

#define POWER_UP_SORT_LEFT_TOP			0
#define POWER_UP_SORT_LEFT_BOT			1
#define POWER_UP_SORT_RIGHT_TOP			2
#define POWER_UP_SORT_RIGHT_BOT			3
#define POWER_UP_SORT_FREE				4

#define DIALOG_TYPE_FREE_GIFT_PU1			0
#define DIALOG_TYPE_FREE_GIFT_PU2			1
#define DIALOG_TYPE_FREE_GIFT_PU3			2
#define DIALOG_TYPE_NOTIFY_POPUP			3
#define DIALOG_TYPE_ICLOUD_CONFIRM			4
#define DIALOG_TYPE_ICLOUD_SYNC_TO			5
#define DIALOG_TYPE_TUTORIAL				6
#define DIALOG_TYPE_NETWORK_ERROR			7
#define DIALOG_TYPE_RATE_US					8
#define DIALOG_TYPE_FOLLOW_TW				9


#define FD_ENERGY_TOTAL_MAX			500
#define TIMES_PLAY_GAME_SHOW_ADD		1
#define MENU_EXT_HEIGHT					400

#define SAVE_CHARACTER_NAME_LENGTH_CATCH			32
#define PARAM_DELAY_REQUEST_FRIEND                  (24)
#define FACEBOOK_APP_ID                     "453252721546775"
#define FACEBOOK_GET_AVATAR_SIZE            @"https://graph.facebook.com/%@/?fields=picture.type(large)"
#define FACEBOOK_GET_AVATAR_SIZE_TOKEN      @"https://graph.facebook.com/%@/?fields=picture.type(large)&access_token=%@"
#define FACEBOOK_REQUEST_GET_ME             @"me?fields=id,name,picture.type(large)"

#define FD_COMBO_TOUCH_TIME				((((FDGame*)CGame::_this->_current_game)->GetDesignGeneral(FD_GENERAL_DESIGN_COMBO_TIME))*60)
#define FD_FEVER_MODE_TRIGGER			((((FDGame*)CGame::_this->_current_game)->GetDesignGeneral(FD_GENERAL_DESIGN_FEVER_TRIGGER)))
#define FD_FEVER_MODE_COMBO_TIME		((((FDGame*)CGame::_this->_current_game)->GetDesignGeneral(FD_GENERAL_DESIGN_FEVER_TIME))*60)
#define FD_BLAST_MODE_TRIGGER			((((FDGame*)CGame::_this->_current_game)->GetDesignGeneral(FD_GENERAL_DESIGN_BLAST_TRIGGER)))
#define FD_FEVER_MODE_TIME				((((FDGame*)CGame::_this->_current_game)->GetDesignGeneral(FD_GENERAL_DESIGN_FEVER_MODE_TIME))*60)
//Lam update
#define NEW_DT()			(1000.0f/FPS_MAX)

