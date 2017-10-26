#include "pch.h"
#include "../../GameCore/CGame.h"

#if defined(MAC_OS)
extern "C" void OS_BasicPostSocial(const char* feed);
extern "C" bool isChartboostAvailable();
extern "C" void IapRestore();
extern "C" bool OS_GC_HasOpenning();
extern "C" void RequestRegisterNotificationSetting();
extern "C" void	GoToReviewInItune();
#else
extern "C" void OS_BasicPostSocial(const char* feed) {};
extern "C" bool isChartboostAvailable() { return true; };
extern "C" bool OS_GC_HasOpenning() { return false; };

#endif
FDGame::FDGame()
{
	
}
void FDGame::Init()
{

	_game = GAME();
	_graphic = G();
	PineGame::Init();

	//port game//
	_point_o = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_LEFT | ANCHOR_TOP);
	_point_right_bottom = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	_screen_width = _point_right_bottom.X - _point_o.X;
	_screen_height = _point_right_bottom.Y - _point_o.Y;
	//printf("\n _point_o.X: %f, _point_o.Y: %f, point_right_bottom.X: %f, point_right_bottom.Y: %f", _point_o.X, _point_o.Y, _point_right_bottom.X, _point_right_bottom.Y);
	//printf("\n ScaleOutX: %f, ScaleOutY: %f, scaleX: %f, scaleY: %f, ScaleOut: %f, TranslateX: %f, TranslateY: %f", GAME()->_game_context.ScaleOutX, GAME()->_game_context.ScaleOutY, GAME()->_game_context.ScaleX, GAME()->_game_context.ScaleY, GAME()->_game_context.ScaleOut, GAME()->_game_context.TranslateX, GAME()->_game_context.TranslateY);
	GAME()->_typeOtimizeDrawModule = true;
	////port game////
	_congra_fire_work1.Init(SPRITEID_EFFECT_EXPLOSION_01, 53, PARTICLE_FIRE_WORK_TYPE_COLOR);
	_congra_fire_work2.Init(SPRITEID_EFFECT_EXPLOSION_01, 53, PARTICLE_FIRE_WORK_TYPE_COLOR);
#if defined (ANDROID_OS)
	GAME()->_has_show_ads_banner = false;
	GAME()->resetBoard = false;
	GAME()->reduce = 40;
	GAME()->farmerReduce = 80;
	GAME()->fbButtonReduce = 100;
#else
	GAME()->reduce = 0;
	GAME()->farmerReduce = 0;
	GAME()->fbButtonReduce = 0;
#endif
}

void FDGame::UpdateAndRender()
{
	GAME()->setUseOptimizeDrawModule(true);
	//update
	switch (_current_state)
	{
	case k_GS_LOGO:
		//if (!PineSocial::HasloggingSystem)
	{
		UpdateLogo();
	}
	RenderLogo();
	break;
	case k_GS_PLAYGAME:
		GamePlayUpdate();
		GamePlayRender(0, 0, 100);
		break;
	case k_GS_RESULT:
		ResultUpdate();
		GamePlayRender(0, 0, 100);
		ResultRender(0, 0, 100);
		break;
	default:
		break;
	}
	_dialogMng.Update();
	if (_current_state > k_GS_LOGO)
	{
		if (!_free_gift_time.isNullDay())
		{
			if (_free_gift_time.UpdateCountDown())
			{
				_free_gift_time.setNullDay();
			}
		}
		if (_dialogMng.isOpenDialog())
		{
			
			_dialogMng.Render(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		}
		_congra_fire_work1.Update();
		_congra_fire_work2.Update();
        GAME()->_friendManager.UpdateCreateAvatarFriend();
#if defined (MAC_OS)
        if(GAME()->_device.IsNetworkAvailable())
        {
            bool pinead_pop;
            pinead_pop = true;
            if(_current_state == k_GS_PLAYGAME && _has_started && !_ingame_menu_active)
            {
                pinead_pop = false;
            }
            if (!GAME()->_advert.HasPopup() && GAME()->_advert.IsAvailable() && _user._play_game_num <= 0 && pinead_pop)
            {
                GAME()->_advert.Init();
                _stateAppDownload = 1;
                _play_game_num_times = 0;
                _play_game_num_show_video = 0;
            }
        }
#endif
    }
#if defined (MAC_OS)
	if ((_new_hight_score /*&& GAME()->_serverThread._isSynSaveComplete*/) && _current_state == k_GS_PLAYGAME)
	{
		//printf("\n ready init gc success");
		if ((GAME()->_serverThread._gcSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS || GAME()->_serverThread._gcSocial->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT) && GAME()->_serverThread._gcSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
		{
			//printf("\n init gc success");
			GAME()->_serverThread.AddCommand(CServerThread::COMMAND_TYPE_SUBMIT_LEADERBOARD);
			_new_hight_score = false;
		}
	}
#endif
    if(!GAME()->_device.IsNetworkAvailable())
    {
        _user._hadLoginFB = false;
    }
#if defined (MAC_OS)
	if(GAME()->_purchase_procesing_opac > 0)
    {
        GAME()->RenderLoadingPurchase(0,0);
    }
	if (!OS_GC_HasOpenning())
	{
		if (GAME()->_processing_view_type == PROCESSING_VIEW_TYPE_GC)
		{
			GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
		}
	}
	if (PineSocial::HasloggingSystem)
	{
		GAME()->_purchase_procesing_opac += 3;
		if (GAME()->_purchase_procesing_opac > 100)
		{
			GAME()->_purchase_procesing_opac = 100;
		}
	}
	else
		if (GAME()->_is_purchase_procesing || GAME()->_processing_view_type != PROCESSING_VIEW_TYPE_NONE)
		{
			GAME()->_purchase_procesing_opac += 3;
			if (GAME()->_purchase_procesing_opac > 100)
			{
				GAME()->_purchase_procesing_opac = 100;
			}
		}
		else
		{
			GAME()->_purchase_procesing_opac -= 10;
			if (GAME()->_purchase_procesing_opac <= 0)
			{
				GAME()->_purchase_procesing_opac = 0;
				GAME()->_loading_count_frame = 0;
			}
		}
#else
#endif 
#if ADD_ADVERT
	if (GAME()->_advert.HasPopup())
	{
		//G()->SetScale(GAME()->_game_context.ScaleOutX, GAME()->_game_context.ScaleOutY, GAME()->_game_context.ScaleX, GAME()->_game_context.ScaleY);
		GAME()->_advert.Render();
		//G()->ClearScale();
		GAME()->_advert.Update();

	}

	
    if(_stateAppDownload == 1 && CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_DONE)
    {
        _stateAppDownload = 0;
        CGame::_this->_advert.SetDownloadCompete();
    }
    if(CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL && _stateAppDownload == 1)
    {
        _stateAppDownload = 0;
        _user._play_game_num = 1;
    }
#endif
	GAME()->setUseOptimizeDrawModule(false);
}

//Lam --- Port function


void FDGame::MeasureButton(Button* button)
{
	PinePoint btpos;
	btpos.X = button->_posX + button->TZ_GetX();
	btpos.Y = button->_posY + button->TZ_GetY();
	float btn_width = button->TZ_GetWidth();
	float btn_height = button->TZ_GetHeight();
	btpos = GAME()->_game_context.MeasureDensity(btpos, btn_width, btn_height);
	button->_posX = btpos.X - button->TZ_GetX();
	button->_posY = btpos.Y - button->TZ_GetY();
}

void FDGame::MeasureButton(Button* button, int anchor)
{
	PinePoint btpos;
	btpos.X = button->_posX + button->TZ_GetX();
	btpos.Y = button->_posY + button->TZ_GetY();


	if ((anchor & ANCHOR_RIGHT) != 0) {
		btpos.X = btpos.X - k_SCREEN_WIDTH;
	}

	if ((anchor & ANCHOR_BOTTOM) != 0) {
		btpos.Y = btpos.Y - k_SCREEN_HEIGHT;
	}

	btpos = GAME()->_game_context.MeasureAnchor(btpos, anchor);
	button->_posX = btpos.X - button->TZ_GetX();
	button->_posY = btpos.Y - button->TZ_GetY();
}
void FDGame::MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal) {
	PinePoint btpos;
	btpos = GAME()->_game_context.MeasureAnchor(posx, 0, anchor_horizonal);
	float button_fixed_x = button->GetFixX();
	button->_posX = btpos.X - button_fixed_x;
}
void FDGame::MeasureButtonY(float posy, Button* button, BYTE anchor_vertical) {
	PinePoint btpos;
	btpos = GAME()->_game_context.MeasureAnchor(posy, 0, anchor_vertical);
	float button_fixed_y = button->GetFixY();
	button->_posY = btpos.Y - button_fixed_y;
}
PinePoint FDGame::MeasureFrame(CSprite* sprite, UINT32 frameid) {
	float framex = sprite->GetFModuleX(frameid, 0);
	float framey = sprite->GetFModuleY(frameid, 0);
	float framew = sprite->GetFModuleWidth(frameid, 0);
	float frameh = sprite->GetFModuleHeight(frameid, 0);
	return GAME()->_game_context.MeasureDensity(framex, framey, framew, frameh);
}
void FDGame::MeasureAnim(CAnimObject* obj) {
	obj->CalculateCurrentAFrame();
	int fid = obj->_anim_sprite->GetFrameIDOfAFrame(obj->_current_aframes);
	PineRectangle bound = obj->_anim_sprite->GetFmoduleBound(fid, 0);
	PinePoint anim_pos = obj->_anim_sprite->GetAFramePos(obj->_current_aframes);
	PinePoint point = GAME()->_game_context.MeasureDensity(bound.X + anim_pos.X, bound.Y + anim_pos.Y, bound.W, bound.H);
	obj->_x = obj->_x + point.X - bound.X - anim_pos.X;
	obj->_y = obj->_y + point.Y - bound.Y - anim_pos.Y;

}

PinePoint FDGame::MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley)
{
	float framex = sprite->GetFModuleX(frameid, 0);
	float framey = sprite->GetFModuleY(frameid, 0);
	float framew = sprite->GetFModuleWidth(frameid, 0);
	float frameh = sprite->GetFModuleHeight(frameid, 0);
	float scale = scalex > scaley ? scalex : scaley;
	PinePoint point = GAME()->_game_context.MeasureDensity(framex - framex * (scale - 1), framey - framey * (scale - 1), framew * scale, frameh * scale);
	point.X -= framex;
	point.Y -= framey;
	return point;
}

PinePoint FDGame::MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor)
{
	PineRectangle bound = sprite->GetFrameBound(frameid);
	PinePoint btpos;
	if (anchor == ANCHOR_TOP)
	{
		btpos = GAME()->_game_context.MeasureAnchor(0, bound.Y, anchor);


	}

	return btpos;
}
//Lam End Port

void FDGame::ProcessingDialogYesNo(int context, int index)
{
	_dialog_process_type = index;
	switch (_dialog_process_type) 
	{
	case 0://yes
		switch (context)
		{
		case DIALOG_TYPE_NOTIFY_POPUP:
#if defined (MAC_OS)
			RequestRegisterNotificationSetting();
#endif
			_user._notify_enable = true;
			break;
        case DIALOG_TYPE_ICLOUD_CONFIRM:
            GAME()->LoadFromICloud();
            _user.UserDataSave();
            SetState(k_GS_LOGO);
            break;
		case DIALOG_TYPE_RATE_US:
#if defined (MAC_OS)
			GoToReviewInItune();
#endif
			_user._playgame_times++;
			break;
		case DIALOG_TYPE_FOLLOW_TW:
#if defined (MAC_OS)
			PineSocial::OpenURL(TWITTER_URL);
#endif
			_user._playgame_times++;
			break;
		default:
			break;
		}
		break;
	case 1://cancel
		switch (context)
		{
		case DIALOG_TYPE_NOTIFY_POPUP:
			_user._notify_enable = false;
            _user._free_gift_collected++;
			_user._playgame_times++;
			break;
		case DIALOG_TYPE_RATE_US:
			_user._playgame_times++;
			break;
		case DIALOG_TYPE_FOLLOW_TW:
			_user._playgame_times++;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	_user.UserDataSave();
}

void FDGame::ProcessingDialogOK(int context)
{
	
	switch (context)
	{
	case DIALOG_TYPE_FREE_GIFT_PU1:
	case DIALOG_TYPE_FREE_GIFT_PU2:
	case DIALOG_TYPE_FREE_GIFT_PU3:
	{
		int param_val = CGameDesign::FileGeneral_design::CSheetTreasure_Item::quantity;
		int value_rw = GetDesignFreeGiftRW(_free_gift_index, param_val);
		_power_up_icon[context]._num += value_rw;
		_user.SetTimeFreeGift();
		_congra_fire_work1.ForceEnd();
		_congra_fire_work2.ForceEnd();
		_gift_render_state = 6;
	}
		break;
	case DIALOG_TYPE_ICLOUD_SYNC_TO:
		break;
	case DIALOG_TYPE_TUTORIAL:
		_has_tutorial_completed = true;
		break;
	default:
		break;
	}
}
void CGame::RenderLoadingPurchase(float x, float y)
{
    FDGame *game = (FDGame*)GAME()->_current_game;
	G()->SetOpacity(_purchase_procesing_opac);
	G()->SetColor(0xf50f1d24);
	G()->FillRect(game->_point_o.X, game->_point_o.Y, game->_screen_width, game->_screen_height,true);
	GAME()->_anim_waiting.DrawAnimObject(G());
	GAME()->_anim_waiting.UpdateAnimObject();
	_loading_count_frame++;
	G()->SetOpacity(100);
}