#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
//extern "C" void InitADManager();
//extern "C" void InitADBanner();
//extern "C" void InitTakePicture();
//extern "C" void takeAPicture();
//extern "C" void LoadImageToPngFile(const char* url,const char *file_name);
extern "C" void AuthTwitter();
extern "C" void CaptureScreenshot();
extern "C" void PostScreenToTwitter();
extern "C" void charboostShowVideoReward(const char* title);
extern "C" void ShowAdmobFullScreen();
extern "C" void OS_InitCaptureViewVideo();
extern "C" void OS_ScreenCaptureVideoStart();
extern "C" void OS_ScreenCaptureVideoStop();
extern "C" int OS_GetCaptureVideoState();
extern "C" void OS_SetCaptureVideoState(int state);
extern "C" void OS_VideoPostSocial(const char* feed);
extern "C" void OS_ShowWebPage(const char* url, bool isOffline);
extern "C" bool checkServerRealIsAble();
extern "C" void RequestRegisterNotificationSetting();
extern "C" void OS_DeviceAskPushNotify();

#define LOGO_STATE_DEFAULT			0
#define LOGO_STATE_INIT_INFO		1
#define LOGO_STATE_CHECK_NETWORK	2
#define LOGO_STATE_SYNC_SAVE		3
#define LOGO_STATE_LOAD_FROM_SV		4
#define LOGO_STATE_SYNC_TO_SV		5
#define LOGO_STATE_FB_INIT          6

#define LOGO_STATE_LOAD_RES			50
#define LOGO_STATE_END				150

void FDGame::UpdateLogo()
{
	if (_sub_state == k_INIT)
	{
		GAME()->_fb_just_init = false;
		//_user._hadLoginFB = false;
		GAME()->_friendManager.SetInitGame(GAME());
#if defined(MAC_OS)

#else
		GAME()->_friendManager.InitTopPlayer();
#endif
		//_user._play_game_num = 0;
		_stateAppDownload = 0;
		GAME()->_freeTypeText.Init(10, 6);
		Device::InitDeviceMotion(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, k_SCREEN_WIDTH, k_SCREEN_HEIGHT);
		//GAME()->_image.CreateImageFromPNGinBundle(G(), "icon.png");
		//_game_design.Home.SheetSleep.int32Value.PrintListValue();
		//_game_design.Transit.SheetBy_Bicycle.int32Value.PrintListValue();
		_loading_step = 0;
		_auto_loading = true;
		_game_time.setNullDay();
		CGame::_this->_isNeedOptimize = true;
		PDEBUG("##### logo init");
		_game_design.General_design.Load();
		PDEBUG("##### logo init");
		_game_easy_percent = CMath::RANDOM(5, 20);
		_game_hard_percent = CMath::RANDOM(1, 10);
		_new_hight_score = false;
		GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_EXPLOSION_01, true, !CGame::_this->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_WEARTHER_01, true, CGame::_this->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_POWERUP_01, true, !CGame::_this->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_SLASH_SCREEN_01, true, CGame::_this->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_LOADING_02, true, !CGame::_this->_isNeedOptimize);
		GAME()->_anim_waiting.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_LOADING_02), SPRITEID_HUD_LOADING_02);
		GAME()->_anim_waiting.SetAnim(0, true);
		GAME()->_anim_waiting.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 - 180);
		AddEffectLight();
		_particle_lighting.Init(0, 0);
		_particle_lighting.Active(0, 0, 0, 0, 1, 1, 20);
#if ADD_ADVERT
		/*while(!ADVERT().IsServerProssingComplete())
		{
		;
		}
		ADVERT().SetOpen(PINE_ADVERT_SCROLL_TYPE);*/
#endif
		_anim_loading.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_SLASH_SCREEN_01), SPRITEID_HUD_SLASH_SCREEN_01);
		_anim_loading.SetAnim(0, true);
		_anim_loading.SetPos(0, 0);
		_sub_state = k_LOOP;
		GAME()->_frame_auto_stop = 10000;
		_play_game_num_times = 0;
		_play_game_num_show_video = 0;

		/*#if defined (MAC_OS)
				OS_DeviceAskPushNotify();
		#endif*/
#if defined (ANDROID_OS)
#if defined (INITIAL_ADMOD)
		GAME()->InitAds();
#endif
#endif
	}
	else if (_sub_state == k_LOOP)
	{
		GAME()->_frame_auto_stop -= NEW_DT();
		//printf("\n AcellX = %f, AcellY = %f, AcellZ = %f", Device::GetDeviceMotionX(),Device::GetDeviceMotionY(),Device::GetDeviceMotionZ());
		for (int i = 0; i < GAME()->_num_touches; i++)
		{
			if (TOUCHS(i)->_flag && TOUCHS(i)->_state == TOUCH_UP)
			{
				Device::SetCurrentBalance();
				//CGame::_this->ClearAllTouch();
			}
		}
		PDEBUG("##### _loading_step = %d", _loading_step);
		switch (_loading_step)
		{
		case LOGO_STATE_DEFAULT:
			_user.UserDataLoad();
			_loading_step = LOGO_STATE_INIT_INFO;
			break;
		case LOGO_STATE_INIT_INFO:
#ifdef MAC_OS
			if (GAME()->_serverThread._gcSocial == NULL)
			{
				GAME()->_serverThread._gcSocial = new PineGameCenterController();
			}
			GAME()->_serverThread._gcSocial->Init();
			GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_INIT_GC);
			if (!PineSocial::IsDeviceHacked())
			{
				PineSocial::LoadIAP();
				_loading_step = LOGO_STATE_CHECK_NETWORK;
			}
			else
			{
				_loading_step = LOGO_STATE_CHECK_NETWORK;
			}
#else
			_loading_step = LOGO_STATE_CHECK_NETWORK;
#endif
			break;
		case LOGO_STATE_CHECK_NETWORK:
#if defined (MAC_OS)
#if ADD_FACEBOOK
			GAME()->_serverThread._isNetworkAvailable = GAME()->_device.IsNetworkAvailable();
			if (strcmp(_user.GetFBID(), "fb_id") != 0 && _user._hadLoginFB)
			{
				/*if (GAME()->_serverThread._fbSocial == NULL)
				{
					GAME()->_serverThread._fbSocial = new PineFacebookController();
				}

				GAME()->_serverThread._fbSocial->Init();*/
				if (GAME()->_serverThread._isNetworkAvailable)
				{
					GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_INIT_FB);
				}

			}
#endif
#endif


			_loading_step = LOGO_STATE_LOAD_RES;
			break;
		case 51:
			for (int i = SPRITEID_CHARACTER_LILY; i <= SPRITEID_CHARACTER_VIOLET; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, !CGame::_this->_isNeedOptimize);
			}
			break;
		case 52:
		{
			int sprite_start = SPRITEID_HUD_BACKGROUND_01;
			int sprite_end = SPRITEID_EFFECT_EXPLOSION_01;
			for (int i = sprite_start; i <= sprite_end; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, CGame::_this->_isNeedOptimize);
			}
		}
		break;
		case 53:
			for (int i = SPRITEID_CHARACTER_LILY_02; i <= SPRITEID_CHARACTER_VIOLET_02; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, !CGame::_this->_isNeedOptimize);
			}
			break;
		case 54:
			for (int i = SPRITEID_CHARACTER_LILY_03; i <= SPRITEID_CHARACTER_FARMER_01; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, !CGame::_this->_isNeedOptimize);
			}
			break;
		case 55:
		{
			int sprite_start = SPRITEID_FONT_POINT_INGAME_01;
			int sprite_end = SPRITEID_FONT_MULTIPLY_01;
			for (int i = sprite_start; i <= sprite_end; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, !CGame::_this->_isNeedOptimize);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_FACEBOOKSCORE_01, true, !CGame::_this->_isNeedOptimize);
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_NAME_FACEBOOK_01, true, !CGame::_this->_isNeedOptimize);
		}
		break;
		case 56:
			/*for (int i = SPRITEID_FONT_COST_AQUIRE; i <= SPRITEID_FONT_TIMING; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, !CGame::_this->_isNeedOptimize);
			}*/

			break;
		case 80:
			for (int i = SFX_BUTTON; i <= SFX_BEATING_FRIEND; i++)
			{
				GAME()->LoadSFX(i);
			}
			break;
		case LOGO_STATE_END:
			SetState(k_GS_PLAYGAME);


#if defined (MAC_OS)
#if ADD_FACEBOOK
			/*if(_user._hadLoginFB)
			{
				GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_SCORE);
				GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_DOWNLOAD_AVATAR);
			}*/
#endif
#endif

			//GAME()->LoadFromICloud();
			break;
		default:
			break;
		}
		if (_loading_step >= LOGO_STATE_LOAD_RES)
		{
			_loading_step++;
		}
#if ADD_ADVERT
		/*if (!ADVERT().UpdateADVERT())
		{
		}*/
#endif
		UpdateEffectLight();
		_particle_lighting.Update();

	}
	else if (_sub_state == k_DESTROY)
	{
	}
}

void FDGame::RenderLogo()
{
	GET_SPRITE(SPRITEID_HUD_SLASH_SCREEN_01)->DrawFrame(G(), 0, 0, 0);
	GET_SPRITE(SPRITEID_HUD_SLASH_SCREEN_01)->DrawFrame(G(), 6, 0 - _point_o.X, 0);
	//_anim_loading.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 - 40);
	_anim_loading.DrawAnimObject(G());
	_anim_loading.UpdateAnimObject();
	/*int spacing = GET_SPRITE(SPRITEID_FONT_LOADING_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_LOADING_01)->_char_spacing = 2;
	GET_SPRITE(SPRITEID_FONT_LOADING_01)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 + 120, _HCENTER, "%d%%", _loading_step);
	GET_SPRITE(SPRITEID_FONT_LOADING_01)->_char_spacing = spacing;*/
	RenderEffectLight(_point_o.X, _point_o.Y, 100);
	_particle_lighting.Render(_point_o.X, _point_o.Y, 100);
}



void CGame::UnloadGameBackground()
{
	//printf("\nUnload Game Back ground");
	if (!_compete_backup_stock)
	{
		_step_force_loading = 0;
		BackupAnimStock();
		for (int i = 0; i < TOTAL_SPRITE_INGAME; i++)
		{
			if (_sprite_loaded[i])
			{
				//printf("\nUnload SpriteID = %d",i);
				UnloadSpriteVer(i);
				_sprite_loaded[i] = 1;
			}
		}
		_compete_backup_stock = true;
		printf("\nUnload background");
	}

}
bool CGame::LoadGameForceground()
{

	if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
	{
		return true;
	}

	_compete_backup_stock = false;
	//printf("\nloading force ground = %d",_step_force_loading);
	//G()->SetColor(0);
	//G()->FillRect(0,0,k_SCREEN_WIDTH,k_SCREEN_HEIGHT);


	if (_step_force_loading >= 10)
	{
		for (int i = 0; i < 100; i++)
		{

			if (_sprite_loaded[_step_force_loading - 10])
			{
				//printf("\n_step_force_loading-10 = %d",(_step_force_loading-10));
				LoadSpriteCurrentVer(_step_force_loading - 10, _sprite_antialias_loaded[_step_force_loading - 10], _sprite_optimized_loaded[_step_force_loading - 10]);
			}
			_step_force_loading++;
			if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
			{
				RestoreAnimStock();

				return true;
			}

		}
	}
	else
	{
		_step_force_loading++;
	}
	return false;
}

