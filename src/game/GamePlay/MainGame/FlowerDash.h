#include "../../GameCore/PineGame.h"

#ifndef __PineEntertainment__FD__
#define __PineEntertainment__FD__

#include <stdio.h>

class FDGame : public PineGame
{
	CGame*_game;
	Graphic*_graphic;
public:
	FDGame();

	

	//--------------------IMPLEMENT GAMEPLAY
	int _loading_step;
	void UpdateAndRender();
	void Init();
	//--------------------Logo--------------------//
	bool _optimize_data;
	bool _auto_loading;
	void UpdateLogo();
	void RenderLogo();
	CAnimObject _anim_loading;
	//CGameDesign _game_design;
	//--------------------Game Play--------------------//
#define GAMEPLAY_LEVEL_EASY			0
#define GAMEPLAY_LEVEL_EASY1		1
#define GAMEPLAY_LEVEL_EASY2		2
#define GAMEPLAY_LEVEL_MEDIUM		3
#define GAMEPLAY_LEVEL_MEDIUM1		4
#define GAMEPLAY_LEVEL_MEDIUM2		5
#define GAMEPLAY_LEVEL_MEDIUM3		6
#define GAMEPLAY_LEVEL_HARD			7
#define GAMEPLAY_LEVEL_HARD1		8
#define GAMEPLAY_LEVEL_HARD2		9
#define GAMEPLAY_LEVEL_HARD3		10

#define GAMEPLAY_BUTTON_NUM		1
	CCharacter _character_arr[FD_NUM_ROW][FD_NUM_COL];

#define GAMEPLAY_BUTTON_PAUSE			0

	Button _gameplay_button[GAMEPLAY_BUTTON_NUM];
	void GamePlayRender(float x, float y, int opac);
	void GamePlayUpdate();
	bool GamePlayUpdateTouch();

	void InterfaceRender(float x, float y);
	int _cube_num_slot_ready;
	bool _cube_sur_finding;
	INT_TIME _hight_score;
	int _game_score;
	bool _new_hight_score;
	float _scale_time;
	Pine_Date_Time _game_time;
	int _count_frame_randomcube;
	int _game_easy_percent;
	int _game_hard_percent;
	int _game_level;
	CUser _user;
	CCharacter _character_arr_fake[FD_NUM_ROW * 4];
	void ResetCharacterSize();
	void AddCharacterFake(float x, float y, int type, int size, int timehold, float scale, int statescale);
	void ResetCharacterFake();
	void RenderCharacterFake(float x, float y, int opac);
	void UpdateCharacterFake();
	void FireCharacterFake(float x, float y);
	void FindFlower2x2();
	void FindFlower3x3();
	void FillSizeFlower2x2(int row, int col);
	void FillSizeFlower3x3(int row, int col);
	bool HasFlower2x2(int row,int col);
	bool HasFlower3x3(int row, int col);
	void CreateFlower2x2(int row, int col);
	void CreateFlower3x3(int row, int col);
	bool _flower_has_new;
	int _particle_count_frame;
	bool _is_need_find_fl_group;
	CDrawableManager _draw_manager;
	CCharacter _flower_render_ingame[FD_FLOWER_NUM_TYPE];
	int _flower_render_index[FD_FLOWER_NUM_TYPE];
	void FlowerRenderInit();
	void FlowerRenderSort(int type);
	void FlowerRenderAddCollected(int id);
	CScrollNumber _game_score_interface;
	int _hint_num_ready;
	bool _hint_has_ready;
	int _hint_group_num;
	int _hint_group_active;
	struct FLowerHint
	{
		int _index;
		int _group;
	};
	FLowerHint _flower_hint[FD_NUM_ARR];
	void ResetFlowerHint();
	void ResetFlowerHint(int group);
	void FindFlowerHint();
	void AddFlowerHint(int index, int type, int group);
	void AddFlowerHint(int index, int group);
	void ActiveFlowerHint(int group);
	int _hint_frame_countdown;
	int _character_size[FD_NUM_ROW][FD_NUM_COL];
	float _interface_time_scrollx;
	float _interface_time_scrollx_speed;
	float _interface_time_scrollx_tar;
	int _interface_weather_opac;
	int _interface_weather_opac_night;

	CPowerUp _power_up_icon[POWER_UP_NUM_MAX];
	float _energy_total_current;
	float _energy_total_target;

	void PowerUpReset();
	void PowerUpInit();
	void PowerUpRender(float x,float y);
	void PowerUpUpdate();
	void PowerUpUpdateTouch();
	void EatFlowerType(int type);
	void SortFlower();
	int _powerup_create_group_time;
	int _powerup_eat_random_type;
	int _sun_frame_index;
	int _sun_frame_smile;
	CAnimObject _sun_glow_anim;
	CAnimObject _sun_glow_anim1;
	bool _has_effect_sunrise;
	int _effect_sunrise_opac;
	bool _effect_sunrise_max;
	float _sunrise_rotate;
	float _sunrise_scale;
	int _sunrise_frame_render1;
	int _sunrise_frame_render2;
	float _sunrise_mky;

	CDialogManager _dialogMng;
	void ProcessingDialogYesNo(int context, int index);
	int _dialog_process_type;
	void ProcessingDialogOK(int context);
	int _energy_total;
	int _energy_total_max;
	void AddEnergy(int energy);
	int _use_power_up_num;
	int _play_game_num_times;
    int _play_game_num_show_video;
    int _result_delay_showad;
    
    CPushNotification _push_notify_manager;
	bool _has_started;
	int _combo_touch_time;
	int _combo_num;
	int _combo_num_max;
	bool _fever_mode_active;
	int _fever_mode_time_bonus;
	int _fever_mode_time;
	int _fever_mode_state;
	float _fever_bg_scale;
	float _fever_bg_scale_speed;
	float _fever_text_speed;
	float _fever_text_scrollx;
	EffectExplo _fever_particle;
	void FeverModdeInit();
	void FeverModeActive();
	void FeverModeRender(float x, float y, int opac);
	void FeverModeUpdate();
	void FeverModeDisable();
	void SuperBlastActive();
	int _fever_glow_rotate;
	float _fever_glow_scale;
	float _fever_field_opac;
	int _fever_field_opac_tar;
	int _fever_field_opac_hold;
	float _fever_field_opac_speed;
	int _scoreX_level;
	INT_TIME _num_flower_collected;
	void FlowerCollectedAdd(int num);
	void FlowerCollectedInit();
	int _fever_last_index;
#define FEVER_MODE_NUM_ANIM			14
	CAnimObject _fever_mode_anim1[FEVER_MODE_NUM_ANIM];
	CAnimObject _fever_mode_anim2[FEVER_MODE_NUM_ANIM];
	void FeverModeAnimInit();
	void FeverModeAnimRender(float x,float y);
	void FeverModeAnimUpdate();
	void FeverModeAnimForceEnd();
	void FeverModeAnimActive();
	int _fever_mode_anim_state;
	struct Farmer
	{
		float _x, _y;
		float _speed;
		float _targetx;
		int _state;
		int _id;
		CAnimObject _anim;
		void Init(int id);
		void Render(float x, float y);
		void Update();
		void Active(int id);
		void ForceEnd();
	};
#define FD_FARMER_NUM		5
	Farmer _farmer_render[FD_FARMER_NUM];
	void FarmerInit();
	void FarmerRender(float x, float y, int opac);
	void FarmerUpdate();
	void FarmerActive();
#define HURRY_UP_TYPE_NORMAL		0
#define HURRY_UP_TYPE_TIMEUP		1
	int _timer_render_current;
	int _timer_render_target;
	float _farmer_scale;
	float _farmer_text_scale;
	int _farmer_level_up_state;
	int _fever_mode_level;
	int _farmer_render_level;
	EffectExplo _farmer_particle;
	void HurryUpInit();
	void HurryUpActive(int type);
	void HurryUpRender(float x, float y, int opac);
	void HurryUpUpdate();
	bool _hurry_up_active;
	int _hurry_up_time;
	int _hurry_up_type;
	int _hurry_up_state;
	float _hurry_up_bg_scale;
	float _hurry_up_bg_scale_speed;
	float _hurry_up_text_speed;
	float _hurry_up_text_scrollx;
	CAnimObject _hurry_up_anim;
	void TimeUpRender(float x, float y);
	void TimeUpInit();
	void TimeUpUpdate();
	void TimeUpActive();
	struct EffectFire
	{
		float _x, _y;
		CAnimObject _anim;
		int _state;
		int _size;
		int _spriteid;
		int _time_hold;
		void Init();
		void Active(int sprite, int anim, int size, float x, float y, int timehold);
		void Render(float x, float y);
		void Update();
	};
	EffectFire _effect_fire[FD_NUM_ARR];
	void EffectFireInit();
	void EffectFireRender(float x, float y);
	void EffectFireActive(int sprite, int anim, int size, float x, float y, int timehold);
	void EffectFireUpdate();
	//--------------------Ext Function--------------------//
	void FirstFillCube();
	void CubeGroupRandom(int type, int num);
	void FillAllCube(bool isinit);
	void DropAllCube(bool isinit);
	int GetColNumEmpty(int col);
	bool FindCubeSur(int index,int type, int type_find, bool fevermode);
	int GetIndexHasNull();
	void CubeRender(float x, float y, int opac);
	void CubeUpdate();
	void CubeInit();
	bool CubeUpdateTouch();
	void CubeRandom();
	void ResetCubeReadyQueue();
	void AddCubeReadyToQueue(int index);
	void FireCubeQueue();
	void DisableCubeQueue();
	int _character_queue[FD_NUM_ARR];
	void FindAndSetFlowerGroup();
	void GetArrMove(int dir, int r, int c, int &or1 , int &oc1);
	int GetNumFlowerType(int type);
	int GetCubeAroundType(int col, int row);
	bool _has_tutorial_completed;
	struct SBFire
	{
		int _index;
		int _time_hold;
		void Init();
		void Active(int index, int timehold);
		void Update();
	};
	SBFire _sb_fire[FD_NUM_ARR];
	void SuperBlastFireInit();
	void SuperBlastFireAdd(int index, int timehold);
	void SuperBlastFireUpdate();
	bool SuperBlastFiring();
	int _screen_shake_x;
	int _screen_shake_y;
	//--------------------Game Effect Func--------------------//
#define PARTICLE_FIREUP_NUM				200
	struct ParticleFireUp
	{
		float _x[PARTICLE_FIREUP_NUM], _y[PARTICLE_FIREUP_NUM];
		float _speedx[PARTICLE_FIREUP_NUM],_speedy[PARTICLE_FIREUP_NUM];
		float _opac[PARTICLE_FIREUP_NUM];
		float _opac_speed[PARTICLE_FIREUP_NUM];
		int _frameid[PARTICLE_FIREUP_NUM];
		int _state[PARTICLE_FIREUP_NUM];
		float _rotate[PARTICLE_FIREUP_NUM];
		float _rotate_speed[PARTICLE_FIREUP_NUM];
		int _hold_time[PARTICLE_FIREUP_NUM];
		float _scale[PARTICLE_FIREUP_NUM];
		float _scale_speed[PARTICLE_FIREUP_NUM];
		bool _isloop;
		int _spriteid;
		int _num;
		int _width;
		int _height;
		float _main_speedx, _main_speedy;
		int _main_opac;
		int _main_opac_speed;
		float _centerx, _centery;
		void Init(int spriteid, bool isloop = true);
		void Active(float x, float y, float spx, float spy, int opac, float opacsp, float rosp, int frameindex, int numframe, int num, int width, int height);
		void Render(float x, float y);
		void Update();
		void ForceEnd();
	};
	ParticleFireUp _fever_fireup_bot;
	ParticleFireUp _fever_fireup_left;
	ParticleFireUp _fever_fireup_right;
	struct EffectLight
	{
		float _x, _y;
		int _dir;
		float _opac;
		float _speed;
		float _speed_opac;
		void Init(float x, float y, int opac, float speedopac, float speed);
		void Render(float x, float y, int opac);
		void Update();
	};
#define EFFECT_LIGHT_NUM_MAX		10
	EffectLight _effect_lighting[EFFECT_LIGHT_NUM_MAX];
	void AddEffectLight();
	void RenderEffectLight(float x, float y, int opac);
	void UpdateEffectLight();
#define PARTICLE_LIGHT_NUM_MAX		100
	struct ParticleLight
	{
		float _x[PARTICLE_LIGHT_NUM_MAX], _y[PARTICLE_LIGHT_NUM_MAX];
		float _opac[PARTICLE_LIGHT_NUM_MAX];
		float _speed[PARTICLE_LIGHT_NUM_MAX];
		float _speed_opac[PARTICLE_LIGHT_NUM_MAX];
		float _scale[PARTICLE_LIGHT_NUM_MAX];
		float _speedx[PARTICLE_LIGHT_NUM_MAX];
		float _speedy[PARTICLE_LIGHT_NUM_MAX];
		float _max_speedx[PARTICLE_LIGHT_NUM_MAX];
		int _frame_id[PARTICLE_LIGHT_NUM_MAX];
		int _state[PARTICLE_LIGHT_NUM_MAX];
		int _num;
		void Init(float speedopac, float speed);
		void Render(float x, float y, int opac);
		void Update();
		void AddParticleLight(float x, float y, float speedopac, float speed, int frame, int numframe);
		void RenderParticleLight(float x, float y, int opac);
		void UpdateParticleLight();
		void Active(float x, float y, float speedopac, float speed, int frame, int numframe, int num);
	};

	ParticleLight _particle_lighting;
	ParticleLight _particle_leaft;
	int _particle_leaft_add_trigger;

#define PARTICLE_RAIN_NUM_MAX		20
	struct ParticleRain
	{
		float _x, _y;
		float _cenx, _ceny;
		float _speed;
		float _scalex;
		float _scaley;
		float _opac;
		float _speed_opac;
		int _state;
		int _aoe;
		void Init();
		void Active(float x, float y, float scalex, float scaley, int aoe, float speed);
		void Render(float x, float y, int opac);
		void Update();
		void ForceEnd();
	};
	ParticleRain _particle_rain_s1[PARTICLE_RAIN_NUM_MAX];
	ParticleRain _particle_rain_s2[PARTICLE_RAIN_NUM_MAX];
	ParticleRain _particle_rain_s3[PARTICLE_RAIN_NUM_MAX];
	ParticleRain _particle_rain_s4[PARTICLE_RAIN_NUM_MAX];
	ParticleRain _particle_rain_s5[PARTICLE_RAIN_NUM_MAX];
	ParticleRain _particle_rain_s6[PARTICLE_RAIN_NUM_MAX];

	CScrollNumber _score_interface[FD_FLOWER_NUM_TYPE];
	void InitParticleRain();
	void RenderParticleRain(float x, float y, int opac);
	void UpdateParticleRain();
	int _time_plus;
#define EFFECT_POINT_NUM_MAX		100
#define EFFECT_POINT_TYPE_NORMAL		0
#define EFFECT_POINT_TYPE_REGULAR		1
#define EFFECT_POINT_TYPE_BIG			2
#define EFFECT_POINT_TYPE_TIME_PLUS		3
#define EFFECT_POINT_TYPE_COMBO			4
#define EFFECT_POINT_TYPE_FEVER			5
#define EFFECT_POINT_TYPE_SUPER_BLAST	6
#define EFFECT_POINT_TYPE_MISS			7
	struct EffectPoint
	{
		float _x, _y;
		float _scroll_y;
		float _speed;
		int _opac;
		int _opac_hight_light;
		int _opac_hight_light_max;
		int _state;
		int _type;
		int _flower_id;
		int _value;
		void Init();
		float _scale;
		float _speed_scale;
		int _rotate;
		void Active(int type,int floid, float x, float y, int val, float speed);
		void Render(float x, float y, int opac);
		void Update();
	};
	EffectPoint _effect_point[EFFECT_POINT_NUM_MAX];
	void EffectPointInit();
	void EffectPointAdd(int type, int floid, float x,float y, int val, float speed);
	void EffectPointRender(float x,float y,int opac);
	void EffectPointUddate();
#define EFFECT_EXPLOSION_NUM_MAX			120
	EffectExplo _effect_explosion[EFFECT_EXPLOSION_NUM_MAX];
	void EffectExplosionInit();
	void EffectExplosionAdd(int type, float x, float y, float speed, float speedscale, int num,int frame, int numframe, int timehold,int size=0);
	void EffectExplosionRender(float x,float y,int opac);
	void EffectExplosionUpdate();
	struct EffectEvaporate
	{
		float _x, _y;
		float _scale;
		float _speed_scale;
		CAnimObject _anim;
		int _state;
		int _opac;
		int _speed_opac;
		int _timehold;
		void Init(int anim);
		void Active(float x, float y, float speedscale, float speedopac,int timehold);
		void Render(float x, float y,int opac);
		void Update();
	};
#define EFFECT_EVAPORATE_NUM_MAX		64
	EffectEvaporate _effect_eva[EFFECT_EVAPORATE_NUM_MAX];
	
	void EvaporateInit();
	void EvaporateAdd(float x, float y, float speedscale, float opac, int timehold);
	void EvaporateRender(float x,float y,int opac);
	void EvaporateUpdate();
	EffectEvaporate _effect_uneva[EFFECT_EVAPORATE_NUM_MAX];
	void UnEvaporateInit();
	void UnEvaporateAdd(float x, float y, float speedscale, float opac, int timehold);
	void UnEvaporateRender(float x, float y, int opac);
	void UnEvaporateUpdate();
#define PARTICLE_FLY_TO_NUM_MAX		50
#define PARTICLE_FLY_TO_TYPE_SUNFLOWER		0
#define PARTICLE_FLY_TO_TYPE_FLOWER			1
	struct ParticleFlyTo
	{
		float _x[PARTICLE_FLY_TO_NUM_MAX], _y[PARTICLE_FLY_TO_NUM_MAX];
		float _tarx[PARTICLE_FLY_TO_NUM_MAX], _tary[PARTICLE_FLY_TO_NUM_MAX];
		float _speed[PARTICLE_FLY_TO_NUM_MAX];
		float _speed_delta[PARTICLE_FLY_TO_NUM_MAX];
		float _speed_deltax[PARTICLE_FLY_TO_NUM_MAX];
		int _count_frame_free_fly[PARTICLE_FLY_TO_NUM_MAX];
		int _state[PARTICLE_FLY_TO_NUM_MAX];
		float _scale[PARTICLE_FLY_TO_NUM_MAX];
		int _state_main;
		int _num;
		int _value;
		int _type;
		void Init();
		void Render(float x, float y);
		void Update();
		void Active(float x, float y, int num,float tarx,float tary, float speed, int value, int type);
	};
	ParticleFlyTo _particle_time_plus[FD_NUM_ARR];
	void ParticleFlyToInit();
	void ParticleFlyToActive(float x, float y, int num, float tarx, float tary, float speed, int value, int type);
	void ParticleFlyToRender(float x, float y);
	void ParticleFlyToUpdate();
#define PARTICLE_FIRE_WORK_NUM_MAX		200
#define PARTICLE_FIRE_WORK_TYPE_DEFAULT			0
#define PARTICLE_FIRE_WORK_TYPE_CIRCLE			1
#define PARTICLE_FIRE_WORK_TYPE_COLOR			2
	struct EffectFireWork
	{
		float _x[PARTICLE_FIRE_WORK_NUM_MAX], _y[PARTICLE_FIRE_WORK_NUM_MAX];
		float _scale[PARTICLE_FIRE_WORK_NUM_MAX];
		float _scale_speed[PARTICLE_FIRE_WORK_NUM_MAX];
		float _speed[PARTICLE_FIRE_WORK_NUM_MAX];
		float _angle[PARTICLE_FIRE_WORK_NUM_MAX];
		COLOR _color[PARTICLE_FIRE_WORK_NUM_MAX];
		float _rotate[PARTICLE_FIRE_WORK_NUM_MAX];
		float _speed_rotate[PARTICLE_FIRE_WORK_NUM_MAX];
		int _opac[PARTICLE_FIRE_WORK_NUM_MAX];
		int _opac_speed[PARTICLE_FIRE_WORK_NUM_MAX];
		int _spriteid;
		int _frameid[PARTICLE_FIRE_WORK_NUM_MAX];
		int _state;
		int _type;
		int _num;
		bool _is_loop;
		float _speed_main;
		float _cenx, _ceny;
		int _frameid_main;
		void Init(int spriteid,int frameid,int type = 0,bool loop = true);
		void Active(float x,float y,float scale,float speedscale,float rotatespeed,float opacspeed,float speed,int num,int numframe = 1);
		void Render(float x, float y, int opac);
		void Update();
		void ForceEnd();
	};
	EffectFireWork _congra_fire_work1;
	EffectFireWork _congra_fire_work2;

	struct CCloud
	{
		float _x, _y;
		float _tarx, _tary;
		float _speed;
		int _frameid;
		int _state;
		int _layer;
		void Init();
		void Add(int frameid, float x, float y, float tarx, float tary, float speed);
		void Render(float x, float y);
		void Update();
	};
#define EFFECT_CLOUD_NUM_MAX		10
	CCloud _effect_cloud[EFFECT_CLOUD_NUM_MAX];
	void CloudInit();
	void CloudAddNew(int frameid, float x, float y, float tarx, float tary, float speed);
	void CloudRender(float x, float y, int layer);
	void CloudUpdate();
	int _effect_cloud_frame_add;
	struct CBoard
	{
		float _x, _y;
		int _opac;
		int _main_time;
		int _time_hold;
		int _frameid;
		int _state;
		
		void setPosY(float y);

		void Init(float x,float y);
		void Render(float x, float y);
		void Update();
		void Active(int timehold, int frameid, int numframe);
		void ForceEnd();
	};
	CBoard _board_light[FD_NUM_ARR];
	void BoardInit();
	void BoardSetBeginPos();
	void BoardRender(float x, float y);
	void BoardActive();
	void BoardUpdate();
	void BoardForceEnd();
	int _board_light_type_active;
	//--------------------Game Design--------------------//
#define CHARACTER_NUM_MAX			20
	static const int _character_sprite[CHARACTER_NUM_MAX];
	int GetDesignBeginning(int id, int param);
	int GetDesignFlowerPercent(int id, int param);
	int GetDesignSpawPercent(int id, int param);
	int GetDesignFlowerLevel(int level, int param);
	float GetDesignGeneral(int id);
	int GetDesignFreeGiftTime(int id, int param);
	int GetDesignFreeGiftRW(int id, int param);
	int GetDesignScoreMutiply(int id, int param);
	int GetDesignFeverModeTime(int id, int param);
	//--------------------Game Result--------------------//
	void ResultRender(float x, float y, int opac);
	void ResultUpdate();
	bool ResultUpdateTouch();
	void ResultUpdateTouchFreeGift();
	int _result_fill_opac;
	bool _result_isclose;
	int _result_touch_id;
    bool _want_to_retry;
	CGameDesign _game_design;
#define RESULT_NUM_BUTTON               6
#define RESULT_BUTTON_FB_SHARE          0
#define RESULT_BUTTON_LEADERBOARD       1
#define RESULT_BUTTON_RETRY             2
#define RESULT_BUTTON_WATCH_ADS         3
#define RESULT_BUTTON_LOGIN_FB          4
#define RESULT_BUTTON_SETTING			5

#define RESULT_MENU_EXT_NUM_BUTTON      10
#define RESULT_MENU_EXT_SOUND			0
#define RESULT_MENU_EXT_MUSIC			1
#define RESULT_MENU_EXT_CONTACT			2
#define RESULT_MENU_EXT_RATE			3
#define RESULT_MENU_EXT_LIKE_FB         4
#define RESULT_MENU_EXT_LIKE_TW			5
#define RESULT_MENU_EXT_CREDITS         6
#define RESULT_MENU_EXT_SYN_CLOUD       7
#define RESULT_MENU_EXT_REMOVE_ADS		8
#define RESULT_MENU_EXT_RESTORE			9


	Button _menu_ext[RESULT_MENU_EXT_NUM_BUTTON];
	Button _result_button[RESULT_NUM_BUTTON];
	CAnimObject _free_gift_anim;
	CAnimObject _result_anim;
	Pine_Date_Time _free_gift_time;
	CScrollNumber _result_your_score;
	CScrollNumber _result_hight_score;
	int _menu_ext_touch_id;
	int _free_gift_index;
	float _result_fill_scrolly;
	int _result_scroll_state;
	int _result_button_scrolly;
	float _result_button_scroll_speed;
	int _result_interface_scrolly;
	float _result_interface_scroll_speed;
#define EFFECT_FIRE_STAR_NUM_MAX			10
	EffectFireWork _effect_fire_star[EFFECT_FIRE_STAR_NUM_MAX];
	void EffectFireStarInit();
	void EffectFireStarAdd(float x, float y, float scale, float speedscale, float rotatespeed, float opacspeed, float speed, int num);
	void EffectFireStarRender(float x, float y, int opac);
	void EffectFireStarUpdate();
	int _count_frame_add_fire_star;
	float _result_new_score_scale;
	int _result_new_score_state;

	void GiftInit();
	void GiftRender(float x, float y);
	void GiftUpdate();
	void GiftActive();
	int _gift_render_opac;
	int _gift_render_state;
	float _gift_render_scale;
	float _gift_render_x;
	float _gift_render_y;
	CAnimObject _gift_render_anim;
	int _gift_glow_opac;
	float _gift_glow_scale;
	float _gift_glow_rotate;
	EffectFireWork _gift_particle;
	void MenuExtRender(float x, float y, int opac);
	void MenuExtUpdate();
	bool MenuExtUpdateTouch();
	void MenuExtInit();
	void MenuExtActive();
	int _menu_ext_scroll_y;
	float _menu_ext_scroll_speed;
	int _menu_ext_scroll_state;
	bool _menu_ext_isclose;
	bool _menu_ext_active;
    int _menu_ext_touch_index;
	//--------------------Game Credits--------------------//
#define CREDIT_BUTTON_NUM			1
#define CREDIT_BUTTON_OK			0
	Button _credit_button[CREDIT_BUTTON_NUM];
	bool _credit_render_active;
	bool _credit_is_close;
	int _credit_render_opac;
	void CreditInit();
	void CreditRender(float x, float y, int opac);
	void CreditUpdate();
	bool CreditUpdateTouch();
	int _credit_scroll_y;
	float _credit_scroll_speed;
	int _credit_scroll_state;

#define FB_SHARE_TYPE_SCORE		0
#define FB_SHARE_TYPE_RANK		1
	int _share_score_state;
	int _share_score_opac;
	int _share_score_type;
	void ShareScoreInit();
	void ShareScoreRender(float x, float y, int opac);
	void ShareScoreUpdate();
	//--------------------Game Ingame Menu--------------------//
	void IngameRender(float x, float y, int opac);
	void IngameUpdate();
	bool IngameUpdateTouch();
	void IngameMenuInit();
	int _ingame_menu_fill_opac;
	bool _ingame_menu_isclose;
	bool _ingame_menu_active;
	bool _ingame_menu_retry;
	bool _ingame_menu_to_result;
	int _ingame_menu_scroll_state;
	int _ingame_menu_scrolly;
	float _ingame_menu_scroll_speed;
#define INGAME_MENU_NUM_BUTTON			4//12
#define INGAME_MENU_BUTTON_RESUME			0
#define INGAME_MENU_BUTTON_QUIT				1
#define INGAME_MENU_BUTTON_RETRY			2
#define INGAME_MENU_BUTTON_LEADERBOARD		3
#define INGAME_MENU_BUTTON_MUSIC			4
#define INGAME_MENU_BUTTON_SOUND			5
#define INGAME_MENU_BUTTON_RATE_US			6
#define INGAME_MENU_BUTTON_FB_LIKE			7
#define INGAME_MENU_BUTTON_TW_LIKE			8
#define INGAME_MENU_BUTTON_CREDITS			9
#define INGAME_MENU_BUTTON_CLOUD			10
#define INGAME_MENU_BUTTON_CONTACT			11

	Button _ingame_menu_button[INGAME_MENU_NUM_BUTTON];


#define RANK_UP_NUM_BUTTON		2
#define RANK_UP_BUTTON_SHARE	0
#define RANK_UP_BUTTON_CLOSE	1
	Button _rank_up_button[RANK_UP_NUM_BUTTON];
    int _result_delay_rankup_render;
	int _rankup_scroll_state;
	int _rankup_scrolly;
	float _rankup_scroll_speed;
	void RankUpInit();
	void RankUpActive();
	void RankUpRender(float x, float y, int opac);
	void RankUpUpdate();
	bool RankUpUpdateTouch();
	int _rank_up_render_opac;
	bool _rank_up_active;
	bool _rank_up_is_close;
	struct FriendRank
	{
		float _x, _y;
		float _tarx, _tary;
		float _tarx1, _tary1;
		PineImage _avatar;
		int _rank;
		float _speed;
		float _speed_max;
		int _state;
		bool _rank_up;
        int _score;
		int _avatar_state;
        char _name[128];
		INT32 _userNameUnicode[SAVE_CHARACTER_NAME_LENGTH_CATCH];
		int _userNameUnicodeLenght;
		void Init(float x, float y, int rank, bool up, PineImage avatar,int score, int avatarstate);
		void Render(float x, float y, int opac);
		void Update();
		void SetRank(int rank);
		void Run(float tarx, float tary, float tarx1, float tary1);
		void Stop();
	};
	int _stateAppDownload;
	int _fb_loading_rotate;
	FriendRank _my_rank;
	FriendRank _friend_rank;
	//--------------------PORT GAME--------------------//
	PinePoint _point_o, _point_right_bottom;
	void MeasureButton(Button* button);
	void MeasureButton(Button* button, int anchor);
	void MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal);
	void MeasureButtonY(float posy, Button* button, BYTE anchor_vertical);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid);
	void MeasureAnim(CAnimObject* obj);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor);
	float _screen_width;
	float _screen_height;
	float _port_scale_x;
	float _port_scale_y;
	//////////////////////PORT GAME/////////
    
    char _textToPostSocial[256];
#define NUM_PARTICLE_COLOR			100
	struct ParticleColor
	{
		int _num;
		float _x_center;
		float _y_center;
		int _width;
		float _speed_main;
		float _x[NUM_PARTICLE_COLOR], _y[NUM_PARTICLE_COLOR];
		float _scale_max[NUM_PARTICLE_COLOR];
		float _scale[NUM_PARTICLE_COLOR];
		float _speed_scale[NUM_PARTICLE_COLOR];
		float _speed[NUM_PARTICLE_COLOR];
		int _dir[NUM_PARTICLE_COLOR];
		float _speed_dir[NUM_PARTICLE_COLOR];
		bool _scale_out[NUM_PARTICLE_COLOR];
		COLOR _color[NUM_PARTICLE_COLOR];
		int _state[NUM_PARTICLE_COLOR];
		int _opac[NUM_PARTICLE_COLOR];
		int _opac_speed[NUM_PARTICLE_COLOR];
		void Init();
		void Active(float x, float y, int width, float speed, float speedscale,int speedopac, int num);
		void Render(float x, float y, int opac);
		void Update();
		void ForceEnd();
		
	};
	ParticleColor _rank_up_particle;
	
};

#endif  // __PineEntertainment__FD__ //