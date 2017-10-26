 #include "pch.h"
#include "../../GameCore/CGame.h"
extern "C" void RequestRegisterNotificationSetting();
extern "C" void	GoToReviewInItune();
extern "C" void SendMailSupport(const char *body, const char *title);
extern "C" void PurchaseProduct(const char *indentify);
extern "C" void IapRestore();
extern "C" bool OS_IsNetworkAvailable();

void FDGame::ResultRender(float x, float y, int opac)
{
	G()->SetColor(0xf50f1d24);
	G()->FillRect(_point_o.X, _point_o.Y + _result_fill_scrolly, _screen_width, _screen_height - _result_fill_scrolly, true);
	//G()->SetOpacity(_result_fill_opac);
	G()->SetOpacity(opac);
	_result_anim.SetPos(0, -_result_interface_scrolly);
	_result_anim.DrawAnimObject(G());
	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 0, x, y - _result_interface_scrolly);
	float mx = k_SCREEN_WIDTH / 2;//GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(0, 0);
	float my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(41, 0) + 110;
	_result_your_score.Render(mx + x, my + y - _result_interface_scrolly, _HCENTER | _VCENTER,8);
	_result_your_score.Update();
    if(_result_delay_showad<=0 && !_result_isclose && _result_fill_opac >= 100)
    {
        //printf("\n _user._has_removed_ads = %d",_user._has_removed_ads);
		if (_play_game_num_times >= TIMES_PLAY_GAME_SHOW_ADD && !_user._has_removed_ads)
		{

#if defined (MAC_OS)
            if(GAME()->_device.IsNetworkAvailable())
            {
                /*if (!GAME()->_advert.HasPopup() && GAME()->_advert.IsAvailable() && _user._play_game_num >= 2)
                {
                    GAME()->_advert.Init();
                    _stateAppDownload = 1;
                    _play_game_num_times = 0;
                    _play_game_num_show_video = 0;
                }
                else*/
                if(_play_game_num_show_video >= 5)
                {
                    if (PineAdBox::AdsAvailableNum > 0)
                    {
                        int adboxtype = PineAdBox::HasVideoAds();
                        if (adboxtype >= 0)
                        {
                            PineAdBox::ShowVideoAds(adboxtype);
                        }
                        _play_game_num_times = 0;
                        _play_game_num_show_video = 0;
                    }
                }
                else
                {
                    if (PineAdBox::AdsAvailableNum > 0)
                    {
                        PineAdBox::SetReactiveAdvert();
                        PineAdBox::ShowAdvertFullScreen();
                        _play_game_num_times = 0;
                    }
                }
                
            }
            
#endif
            
        }
    }
	my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(0, 1) + 35;
	_result_hight_score.Render(mx + x, my + y - _result_interface_scrolly, _HCENTER | _VCENTER,8);
	_result_hight_score.Update();
	if (_new_hight_score)
	{
		GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 41, x, y - _result_interface_scrolly);
		if (_result_hight_score.GetValueCurrent() == _result_hight_score.GetValueTarget() && _game_score == _hight_score)
		{
			if (_count_frame_add_fire_star == -1)
			{
				_result_new_score_state = 1;
			}
			_count_frame_add_fire_star++;
			if (_count_frame_add_fire_star>=40)
			{
				_count_frame_add_fire_star = 0;
				int posx = CMath::RANDOM(-10, 10) * 10 + mx + x;
				int posy = CMath::RANDOM(-3, 3) * 10 + y + GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(41, 0) + 50;
				EffectFireStarAdd(posx, posy, 0, 0.015f, 1, 1, 2.8f, 15);
			}
		}
	}
	else
	{
		GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 42, x, y - _result_interface_scrolly);
	}
    mx = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(40, 0);
    my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(40, 0);
	G()->SetScale(_result_new_score_scale, _result_new_score_scale, mx, my); 
    GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 40, 0, 0);
    G()->ClearScale();
	//GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 1, x, y);
	int spacing = 0;
	//for (int i = 0; i < FD_FLOWER_NUM_TYPE-1; i++)
	//{
	//	float posx = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(0, 3 + i);
	//	float posy = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(0, 3 + i) ;
	//	_flower_render_ingame[_flower_render_index[i]].Render(x + posx, y + posy - 62, _result_fill_opac);
	//	_score_interface[i].Render(posx + x, posy + y, _HCENTER | _VCENTER,2);
	//	_score_interface[i].Update();
	//	spacing = GET_SPRITE(SPRITEID_FONT_LEVEL)->_char_spacing;
	//	GET_SPRITE(SPRITEID_FONT_LEVEL)->_char_spacing = 2;
	//	GET_SPRITE(SPRITEID_FONT_LEVEL)->DrawAnsiTextFormat(G(), posx + x, posy + y - 185, _HCENTER | _VCENTER, "SCORE x %d", _user._flower_arr[_flower_render_index[i]]._level + 1);
	//	GET_SPRITE(SPRITEID_FONT_LEVEL)->_char_spacing = spacing;
	//	int clipx = posx + x - 50;
	//	float xp_cur = _user._flower_arr[_flower_render_index[i]]._xp*1.0f;
	//	float xp_max = _user._flower_arr[_flower_render_index[i]]._xp_max*1.0f;
	//	int clipw = 98* (xp_cur / xp_max);
	//	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 15, posx + x, posy + y - 160);
	//	G()->SetClip(clipx, 0, clipw, k_SCREEN_HEIGHT);
	//	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 16, posx + x, posy + y - 160);
	//	G()->ClearClip();
	//	/*if (_score_interface[i]._value_cur_delta > 0)
	//	{
	//		_user._flower_arr[_flower_render_index[i]].AddXP(_score_interface[i]._value_cur_delta);
	//	}*/
	//	if (xp_cur >= xp_max)
	//	{
	//		_user._flower_arr[_flower_render_index[i]].LevelUp();
	//		_flower_render_ingame[_flower_render_index[i]].SetScaleState(CHARACTER_SCALE_STATE_OUT);
	//		_flower_render_ingame[_flower_render_index[i]]._scale = 0.8f;
	//		_flower_render_ingame[_flower_render_index[i]]._scale_max = 1.4f;
	//	}
	//}
	//if (_gift_render_state <= 0)
	//{
	//	_free_gift_anim.DrawAnimObject(G());
	//	//GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 17,  x,  y );
	//	if (!_free_gift_time.isNullDay())
	//	{
	//		int h = _free_gift_time.getHour();
	//		int m = _free_gift_time.getMinute();
	//		int s = _free_gift_time.getSecond();
	//		if (h>0)
	//		{
	//			mx = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(17, 0);
	//			my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleY(17, 0) + 152;
	//			spacing = GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing;
	//			GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing = 2;
	//			GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->DrawAnsiTextFormat(G(), mx + x, my + y, _HCENTER, "%02d:%02d:%02d", h, m, s);
	//			GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing = spacing;
	//		}
	//		else
	//		{
	//			mx = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(17, 0);
	//			my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleY(17, 0) + 152;
	//			spacing = GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing;
	//			GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing = 2;
	//			GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->DrawAnsiTextFormat(G(), mx + x, my + y, _HCENTER, "%02d:%02d", m, s);
	//			GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing = spacing;
	//		}
	//		if (_free_gift_anim.GetCurrentAnim() != 20)
	//		{
	//			_free_gift_anim.SetAnim(20, true);
	//		}
	//	}
	//	else
	//	{
	//		if (_free_gift_anim.GetCurrentAnim() != 21)
	//		{
	//			_free_gift_anim.SetAnim(21, true);
	//		}
	//		mx = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(17, 0);
	//		my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleY(17, 0) + 152;
	//		spacing = GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing;
	//		GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing = 2;
	//		GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->DrawAnsiTextFormat(G(), mx + x, my + y, _HCENTER, "READY");
	//		GET_SPRITE(SPRITEID_FONT_TIME_GIFT_01)->_char_spacing = spacing;
	//	}
	//}
	
	if (_user._hadLoginFB && GAME()->_device.IsNetworkAvailable())
	{
		if (_result_button[RESULT_BUTTON_LOGIN_FB]._state != BUTTON_STATE_HIDE)
		{
			_result_button[RESULT_BUTTON_LOGIN_FB].SetState(BUTTON_STATE_HIDE);
		}
        
        GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 123, x, y + _result_button_scrolly);
        _fb_loading_rotate += 3;
        if (_fb_loading_rotate >= 360)
        {
            _fb_loading_rotate -= 360;
        }
        if (GAME()->_friendManager._numTopPlayer <= 0)
        {
            my = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(123, 0);
            GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 125, x + k_SCREEN_WIDTH / 2, my + y + _result_button_scrolly);
            G()->SetRotate(-_fb_loading_rotate, x + k_SCREEN_WIDTH / 2, my + y + _result_button_scrolly);
            GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 124, x + k_SCREEN_WIDTH / 2, my + y + _result_button_scrolly);
            G()->ClearRotate();
        }
        
	}
	else
	{
		
		if (_result_button[RESULT_BUTTON_LOGIN_FB]._state == BUTTON_STATE_HIDE)
		{
			_result_button[RESULT_BUTTON_LOGIN_FB].SetState(BUTTON_STATE_NORMAL);
		}return;
		GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 35, x, y + _result_button_scrolly);
	}
	
    //RenderParticleRain(x, y, _result_fill_opac);
	//G()->SetOpacity(_result_fill_opac);
	
	for (int i = 0; i < RESULT_NUM_BUTTON; i++)
	{
		if (_result_button[i]._state != BUTTON_STATE_HIDE)
		{
			int btx = 0;
			int bty = 0;
			/*if (i != RESULT_BUTTON_WATCH_ADS && i != RESULT_BUTTON_LOGIN_FB)
			{
				bty = 60;
			}*/
			/*if (i == RESULT_BUTTON_SETTING)
			{
				btx = -_point_o.X;
			}*/
			_result_button[i].SetPos(btx, bty + _result_button_scrolly - (GAME()->fbButtonReduce));
			_result_button[i].Render(G());
			_result_button[i].Update();
		}
	}
	EffectFireStarRender(x, y, opac);
	//GiftRender(x, y);
	G()->SetOpacity(opac);
    if(_user._hadLoginFB && GAME()->_device.IsNetworkAvailable())
    {
        PineMatrix4 tranf;
        tranf = CMath::Matrix4Scale2D(1, 1, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2 - 88 + _result_button_scrolly);
        GAME()->_friendManager.RenderTopPlayer(_result_button[0], 0, k_SCREEN_HEIGHT - 99 + _result_button_scrolly, tranf);
    }
	
	MenuExtRender(x,y,opac);
	CreditRender(x, y, opac);
	RankUpRender(x, y, opac);
	ShareScoreRender(x, y, opac);
    
}
void FDGame::ResultUpdate()
{
	if (_sub_state == k_INIT)
	{

		_fb_loading_rotate = 0;
		_rank_up_active = false;
        //_game_score += 5000;
#if defined (MAC_OS)
#if ADD_FACEBOOK
        if(_user._hadLoginFB && GAME()->_device.IsNetworkAvailable())
        {
            GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_POST_FB_SCORE);
        }
#endif
#endif
        //printf("\n num player: %d",GAME()->_friendManager._numTopPlayer);
		int min = -(GAME()->_friendManager._numTopPlayer - 5) * 150;
		int max = _point_o.X;
		if (min >= _point_o.X)
		{
			min = _point_o.X;
		}
        GAME()->_friendManager._scrollBarX._min = min;
		GAME()->_friendManager._scrollBarX.Init(min, max, 2, 1.3f);
		GAME()->_friendManager._scrollBarX.SetState(1);
		GAME()->_friendManager._scrollBarX._current = _point_o.X;
        _result_delay_rankup_render = 20;
        _result_delay_showad = 30;
		_result_scroll_state = 0;
		_result_interface_scroll_speed = 70;
		_result_button_scroll_speed = 50;
		_result_button_scrolly = _screen_height / 2 + 30;
		_result_interface_scrolly = _screen_height / 2;
		InitParticleRain();
		for (int i = 0; i < RESULT_NUM_BUTTON; i++)
		{
			_result_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
			_result_button[i].SetAnim(SPRITEID_HUD_RESULT_01, i * 4);
			_result_button[i].SetPos(0, 0);
			if (i < RESULT_BUTTON_WATCH_ADS)
			{
				_result_button[i].SetPos(0, 60);
			}
			if (i == RESULT_BUTTON_SETTING)
			{
				_result_button[i].SetPos(0 - _point_o.X, 0);
			}
		}
		//_result_button[RESULT_BUTTON_LOGIN_FB].SetState(BUTTON_STATE_HIDE);
		_result_button[RESULT_BUTTON_WATCH_ADS].SetState(BUTTON_STATE_HIDE);
		_result_fill_scrolly = 0;// k_SCREEN_HEIGHT;
		ShareScoreInit();
		_has_effect_sunrise = false;
		_result_your_score.Init(SPRITEID_FONT_YOURSCORE_01, 0, 10);
		_result_hight_score.Init(SPRITEID_FONT_BESTSCORE_01, _hight_score, 10);
		_particle_count_frame = 60;
		//FlowerRenderSort(FD_FLOWER_SORT_TYPE_ID);
		//for (int i = 0; i < FD_FLOWER_NUM_TYPE; i++)
		//{
		//	_score_interface[i].Init(SPRITEID_FONT_POINT_01, _flower_render_ingame[_flower_render_index[i]]._xp_collected, 30);
		//	//_flower_render_ingame[_flower_render_index[i]]._xp_collected = _user._flower_arr[_flower_render_index[i]]._xp_need;
		//}
		
		_result_fill_opac = 0;
		_result_isclose = false;
		_result_touch_id = -1;
		_sub_state = k_LOOP;
		/*_free_gift_anim.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_RESULT_01), SPRITEID_HUD_RESULT_01);
		_free_gift_anim.SetAnim(20, true);
		_free_gift_anim.SetPos(0, 0);*/
		_result_anim.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_RESULT_01), SPRITEID_HUD_RESULT_01);
		_result_anim.SetAnim(26, true);
		_result_anim.SetPos(0, 0);
		EffectFireStarInit();
		_result_new_score_scale = 0;
		_result_new_score_state = 0;
		_new_hight_score = false;
		if (_game_score > _hight_score)
		{
            _hight_score = _game_score;
			_new_hight_score = true;
			PLAY_SFX(SFX_RESULT_NEW_HIGHSCORE);
		}
		else
		{
			PLAY_SFX(SFX_RESULT);
		}
		_user._playgame_times++;
        _play_game_num_times++;
        _play_game_num_show_video++;
		MenuExtInit();
		_user.UserDataSave();
        GAME()->AutoSyncICloud();

		GAME()->LoadMusic(BGM_MAINMENU_RESULT);
		GAME()->PlayCurrentMusic(-1);
		/*if (!GAME()->IsCurrentMusicPlaying())
		{
			GAME()->StopCurrentMusic();
			GAME()->LoadMusic(BGM_MAINMENU_RESULT);
			GAME()->PlayCurrentMusic(-1);
		}
		else
		{

			GAME()->StopCurrentMusic();
			GAME()->LoadMusic(BGM_MAINMENU_RESULT);
			GAME()->PlayCurrentMusic(-1);
		}*/
		//GiftInit();
	}
	//printf("\n 00 _result_button_scrolly = %d", _result_button_scrolly);
	switch (_result_scroll_state)
	{
	case 0:
		if (_result_interface_scrolly > -150)
		{
			if (_result_interface_scrolly <= -50)
			{
				_result_interface_scroll_speed *= 0.5f;
				if (_result_interface_scroll_speed < 5)
				{
					_result_interface_scroll_speed = 5;
				}
			}
			//printf("\n 000 speed = %f - scrolly = %f", _result_interface_scroll_speed, _result_interface_scrolly);
			_result_interface_scrolly -= _result_interface_scroll_speed;
			if (_result_interface_scrolly <= -150)
			{
				_result_scroll_state = 1;
				_result_interface_scroll_speed = 20;
			}
		}
		break;
	case 1:
		if (_result_interface_scrolly < 0)
		{
			if (_result_interface_scrolly >= -70)
			{
				_result_interface_scroll_speed *= 0.7f;
			}
			if (_result_interface_scroll_speed <= 0.2f)
			{
				_result_interface_scroll_speed = 0.2f;
			}
			//printf("\n 111 _result_interface_scroll_speed = %f", _result_interface_scroll_speed);
			_result_interface_scrolly += _result_interface_scroll_speed;
			if (_result_interface_scrolly >= 0)
			{
				//_result_scroll_state = 3;
				_result_interface_scrolly = 0;
			}
		}
		if (_result_button_scrolly > -100)
		{
			if (_result_button_scrolly <= -20)
			{
				_result_button_scroll_speed *= 0.6f;
				if (_result_button_scroll_speed < 5)
				{
					_result_button_scroll_speed = 5;
				}
			}
			_result_button_scrolly -= _result_button_scroll_speed;
			if (_result_button_scrolly <= -100)
			{
				_result_scroll_state = 3;
				_result_button_scroll_speed = 20;
			}
		}
		break;
	case 2:
		if (_result_button_scroll_speed >= 50)
		{
			_result_button_scroll_speed = 50;
		}
		if (_result_button_scrolly > -100)
		{
			if (_result_button_scrolly <= -20)
			{
				_result_button_scroll_speed *= 0.6f;
				if (_result_button_scroll_speed < 5)
				{
					_result_button_scroll_speed = 5;
				}
			}
			_result_button_scrolly -= _result_button_scroll_speed;
			if (_result_button_scrolly <= -100)
			{
				_result_scroll_state = 3;
				_result_button_scroll_speed = 20;
			}
		}
		break;
	case 3:
		if (_result_button_scrolly < 0)
		{
			if (_result_button_scrolly >= -60)
			{
				_result_button_scroll_speed *= 0.65f;
			}
			if (_result_button_scroll_speed <= 0.2f)
			{
				_result_button_scroll_speed = 0.2f;
			}
			_result_button_scrolly += _result_button_scroll_speed;
			if (_result_button_scrolly >= 0)
			{
				_result_scroll_state = 4;
				_result_button_scrolly = 0;
			}
		}
		break;
	case 4:
		//_result_scroll_state = 0;
		break;
	default:
		break;
	}
    int min = -(GAME()->_friendManager._numTopPlayer - 5) * 150;
    if (min >= _point_o.X)
    {
        min = _point_o.X;
    }
    GAME()->_friendManager._scrollBarX._min = min;
	if (_result_button_scrolly > 0)
	{
		return;
	}
	if (_result_fill_scrolly > 0)
	{
		_result_fill_scrolly -= 70;
		if (_result_fill_scrolly <= 0)
		{
			_result_fill_scrolly = 0;
		}
		return;
	}
	switch (_result_new_score_state)
	{
	case 0:
		break;
	case 1:
		if (_result_new_score_scale < 1.2f)
		{
			_result_new_score_scale += 0.08f;
			if (_result_new_score_scale >= 1.2f)
			{
				_result_new_score_state = 2;
			}
		}
		break;
	case 2:
		if (_result_new_score_scale > 0.9f)
		{
			_result_new_score_scale -= 0.08f;
			if (_result_new_score_scale <= 0.9f)
			{
				_result_new_score_state = 3;
			}
		}
		break;
	case 3:
		if (_result_new_score_scale < 1.0f)
		{
			_result_new_score_scale += 0.01f;
			if (_result_new_score_scale >= 1.0f)
			{
				_result_new_score_state = 4;
			}
		}
		break;
	case 4:
		if (_result_new_score_scale < 1.05f)
		{
			_result_new_score_scale += 0.005f;
			if (_result_new_score_scale >= 1.05f)
			{
				_result_new_score_state = 5;
			}
		}
		break;
	case 5:
		if (_result_new_score_scale > 0.95f)
		{
			_result_new_score_scale -= 0.005f;
			if (_result_new_score_scale <= 0.95f)
			{
				_result_new_score_state = 4;
			}
		}
		break;
	default:
		break;
	}
	//GiftUpdate();
    //UpdateParticleRain();
	//_free_gift_anim.UpdateAnimObject();
	CreditUpdate();
	if (_credit_render_active)
	{
		return;
	}
    if(_result_your_score.GetValueCurrent() == _result_your_score.GetValueTarget() && !_dialogMng.isOpenDialog())
    {
        _result_delay_rankup_render--;
		if (!_rank_up_active && _result_delay_rankup_render <= 0)
		{
            _result_delay_showad--;
			if (_result_delay_showad == 29)
			{
#if defined (MAC_OS)
#if ADD_FACEBOOK
            if(_user._hadLoginFB && GAME()->_device.IsNetworkAvailable())
            {
                RankUpInit();
				char fbid[128];
				sprintf(fbid, "%s", _user.GetFBID());
				int rank = GAME()->_friendManager.GetRank(fbid);
                int my_index = GAME()->_friendManager.GetIndexRank(rank);
                GAME()->_friendManager._topPlayer[my_index]._score = (int)_hight_score;
				GAME()->_friendManager.SortFriend();
				int my_rank = GAME()->_friendManager.CheckNewRank(rank);
				if (my_rank >= 0)
				{
					int f_index = GAME()->_friendManager.GetIndexRank(my_rank + 1);
					float x = k_SCREEN_WIDTH / 2 - 140;
					float y = k_SCREEN_HEIGHT / 2 - 120;
					int rank = GAME()->_friendManager._topPlayer[f_index]._rank-1;
					char url[1024];
					sprintf(url, "%s", GAME()->_friendManager._topPlayer[f_index]._id);
					_friend_rank.Init(x, y, rank, false, GAME()->_friendManager._topPlayer[f_index]._avatar, GAME()->_friendManager._topPlayer[f_index]._score, GAME()->_friendManager._topPlayer[f_index]._avatarState);
                    printf("\n f_id:%s", GAME()->_friendManager._topPlayer[f_index]._id);
					for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
					{
						_friend_rank._userNameUnicode[c] = GAME()->_friendManager._topPlayer[f_index]._userNameUnicode[c];
						if (c < GAME()->_friendManager._topPlayer[f_index]._userNameUnicodeLenght)
						{
							printf("\n %d : %d", c, GAME()->_friendManager._topPlayer[f_index]._userNameUnicode[c]);
						}
					}
					_friend_rank._userNameUnicodeLenght = GAME()->_friendManager._topPlayer[f_index]._userNameUnicodeLenght;
                    sprintf(_friend_rank._name, "%s", GAME()->_friendManager._topPlayer[f_index]._userName);
					x = k_SCREEN_WIDTH / 2 + 140;
					y = k_SCREEN_HEIGHT / 2 + 120;
					rank = my_rank+1;
					my_index = GAME()->_friendManager.GetIndexRank(my_rank);
                    sprintf(url, "%s", GAME()->_friendManager._topPlayer[my_index]._id);
					_my_rank.Init(x, y, rank, true, GAME()->_friendManager._topPlayer[my_index]._avatar, (int)_hight_score, GAME()->_friendManager._topPlayer[my_index]._avatarState);
					for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
					{
						_my_rank._userNameUnicode[c] = GAME()->_friendManager._topPlayer[my_index]._userNameUnicode[c];
						if (c < GAME()->_friendManager._topPlayer[my_index]._userNameUnicodeLenght)
						{
							printf("\n %d : %d", c, GAME()->_friendManager._topPlayer[my_index]._userNameUnicode[c]);
						}
					}
                    sprintf(_my_rank._name, "%s", GAME()->_friendManager._topPlayer[my_index]._userName);
					_my_rank._userNameUnicodeLenght = GAME()->_friendManager._topPlayer[my_index]._userNameUnicodeLenght;
					_rank_up_active = true;
					_rank_up_particle.Active(k_SCREEN_WIDTH / 2, _point_o.Y - 10, _screen_width, 1.5f, 0.03f, 4, 40);
                }
			}
#endif 
#elif defined(ANDROID_OS)
#else
				int rank = GAME()->_friendManager.GetRank("GC_4");
				RankUpInit();
				GAME()->_friendManager._topPlayer[rank]._score = 700;
				GAME()->_friendManager.SortFriend();
				int my_rank = GAME()->_friendManager.CheckNewRank(rank);
				my_rank = 2;
				if (my_rank >= 0)
				{
					int f_index = GAME()->_friendManager.GetIndexRank(my_rank - 1);
					float x = k_SCREEN_WIDTH / 2 - 140;
					float y = k_SCREEN_HEIGHT / 2 - 120;
					int rank = GAME()->_friendManager._topPlayer[f_index]._rank;
					char url[1024];
					sprintf(url, "%s", "url");
					_friend_rank.Init(x, y, rank, false, GAME()->_friendManager._topPlayer[f_index]._avatar, GAME()->_friendManager._topPlayer[f_index]._score, GAME()->_friendManager._topPlayer[f_index]._avatarState);
					for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
					{
						_friend_rank._userNameUnicode[c] = GAME()->_friendManager._topPlayer[f_index]._userNameUnicode[c];
						if (c < GAME()->_friendManager._topPlayer[f_index]._userNameUnicodeLenght)
						{
							printf("\n %d : %d", c, GAME()->_friendManager._topPlayer[f_index]._userNameUnicode[c]);
						}
					}
					_friend_rank._userNameUnicodeLenght = GAME()->_friendManager._topPlayer[f_index]._userNameUnicodeLenght;
					x = k_SCREEN_WIDTH / 2 + 140;
					y = k_SCREEN_HEIGHT / 2 + 120;
					rank = my_rank;
					int my_index = GAME()->_friendManager.GetIndexRank(my_rank);
					_my_rank.Init(x, y, rank, true, _user._avatar, _hight_score, GAME()->_friendManager._topPlayer[my_index]._avatarState);
					for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
					{
						_friend_rank._userNameUnicode[c] = GAME()->_friendManager._topPlayer[my_index]._userNameUnicode[c];
						if (c < GAME()->_friendManager._topPlayer[my_index]._userNameUnicodeLenght)
						{
							printf("\n %d : %d", c, GAME()->_friendManager._topPlayer[my_index]._userNameUnicode[c]);
						}
					}
					_friend_rank._userNameUnicodeLenght = GAME()->_friendManager._topPlayer[my_index]._userNameUnicodeLenght;
					_rank_up_active = true;
					_rank_up_particle.Active(k_SCREEN_WIDTH / 2, _point_o.Y - 10, _screen_width, 1.5f, 0.03f, 4, 40);
				}
#endif
			}
			
		}
    }
	RankUpUpdate();
	MenuExtUpdate();
	ShareScoreUpdate();
	EffectFireStarUpdate();
	_result_anim.UpdateAnimObject();

	if (!_result_isclose)
	{
		if (_result_fill_opac < 100)
		{
			_result_fill_opac += 10;
			if (_result_fill_opac >= 100)
			{
				_result_fill_opac = 100;
				_result_your_score.StartTo(_game_score);
				if (_new_hight_score)
				{
					_result_hight_score.StartTo(_hight_score);
				}
            }
		}
	}
	else
	{
		if (_result_fill_opac >0)
		{
			//printf("\n _result_fill_opac = %d", _result_fill_opac);
			_result_fill_opac -= 8;
			if (_result_fill_opac <= 0)
			{
				SetState(k_GS_PLAYGAME);
#if defined (MAC_OS)
#if ADD_FACEBOOK
				if (_user._hadLoginFB && GAME()->_device.IsNetworkAvailable())
				{
					GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_SCORE);
				}
#endif
#endif
			}
		}
	}
	bool isend_init = true;
	//for (int i = 0; i < FD_FLOWER_NUM_TYPE-1; i++)
	//{
	//	_flower_render_ingame[_flower_render_index[i]].Update();
	//	
	//	if (_flower_render_ingame[_flower_render_index[i]]._scale_state == CHARACTER_SCALE_STATE_NORMAL && _flower_render_ingame[_flower_render_index[i]]._scale >= 1.0f)
	//	{
	//		
	//		if (_flower_render_ingame[_flower_render_index[i]]._xp_collected <= 0)
	//		{
	//			continue;
	//		}
	//		float posx = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(0, 3+i);
	//		float posy = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(0, 3+i) - 176;
	//		switch (i)
	//		{
	//		case 0:
	//			for (int j = 0; j < PARTICLE_RAIN_NUM_MAX;j++)
	//			{
	//				if (_particle_rain_s1[j]._state == 0)
	//				{
	//					_particle_rain_s1[j].Active(posx, posy, 0.5f, 0.5f, 50, 2);
	//					
	//				}
	//			}
	//			
	//			break;
	//		case 1:
	//			for (int j = 0; j < PARTICLE_RAIN_NUM_MAX; j++)
	//			{
	//				if (_particle_rain_s2[j]._state == 0)
	//				{
	//					_particle_rain_s2[j].Active(posx, posy, 0.5f, 0.5f, 50, 2);
	//				}
	//			}
	//			break;
	//		case 2:
	//			for (int j = 0; j < PARTICLE_RAIN_NUM_MAX; j++)
	//			{
	//				if (_particle_rain_s3[j]._state == 0)
	//				{
	//					_particle_rain_s3[j].Active(posx, posy, 0.5f, 0.5f, 50, 2);
	//				}
	//			}
	//			break;
	//		case 3:
	//			for (int j = 0; j < PARTICLE_RAIN_NUM_MAX; j++)
	//			{
	//				if (_particle_rain_s4[j]._state == 0)
	//				{
	//					_particle_rain_s4[j].Active(posx, posy, 0.5f, 0.5f, 50, 2);
	//				}
	//			}
	//			break;
	//		case 4:
	//			for (int j = 0; j < PARTICLE_RAIN_NUM_MAX; j++)
	//			{
	//				if (_particle_rain_s5[j]._state == 0)
	//				{
	//					_particle_rain_s5[j].Active(posx, posy, 0.5f, 0.5f, 50, 2);
	//				}
	//			}
	//			break;
	//		case 5:
	//			for (int j = 0; j < PARTICLE_RAIN_NUM_MAX; j++)
	//			{
	//				if (_particle_rain_s6[j]._state == 0)
	//				{
	//					_particle_rain_s6[j].Active(posx, posy, 0.5f, 0.5f, 50, 2);
	//				}
	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		isend_init = false;
	//	}
	//	
	//}
	//if (isend_init)
	//{
	//	_particle_count_frame--;
	//	if (_particle_count_frame <= 0)
	//	{
	//		for (int i = 0; i < FD_FLOWER_NUM_TYPE - 1; i++)
	//		{
	//			int scroll_type = _score_interface[i].GetType();
	//			if (scroll_type == SCROLL_TYPE_END && _score_interface[i].GetValueCurrent() == 0)
	//			{

	//				//_score_interface[i].SetValue(_user._flower_arr[_flower_render_index[i]]._xp_need);
	//				_score_interface[i].SetValue(_flower_render_ingame[_flower_render_index[i]]._xp_collected);

	//			}
	//			int xp_render = _user._flower_arr[_flower_render_index[i]]._xp_need - _flower_render_ingame[_flower_render_index[i]]._xp_collected;
	//			if (scroll_type == SCROLL_TYPE_END || scroll_type == SCROLL_TYPE_NONE)
	//			{
	//				if (xp_render <= 0)
	//				{
	//					//_user._flower_arr[_flower_render_index[i]].AddXP(_user._flower_arr[_flower_render_index[i]]._xp_need);
	//					_flower_render_ingame[_flower_render_index[i]]._xp_collected -= _user._flower_arr[_flower_render_index[i]]._xp_need;
	//					xp_render = 0;
	//				}
	//				else
	//				{
	//					//_user._flower_arr[_flower_render_index[i]].AddXP(_flower_render_ingame[_flower_render_index[i]]._xp_collected);
	//					_flower_render_ingame[_flower_render_index[i]]._xp_collected = 0;
	//				}
	//				_score_interface[i].StartTo(_flower_render_ingame[_flower_render_index[i]]._xp_collected);
	//			}
	//		}
	//	}
	//}
	if (_result_fill_opac >= 100 && _result_your_score.GetValueCurrent() == _result_your_score.GetValueTarget())
	{
		if (!ResultUpdateTouch() )
		{
			if (GAME()->_purchase_procesing_opac <= 0 && !_dialogMng.isOpenDialog() )
			{
				for (int i = GAME()->_num_touches - 1; i >= 0; i--)
				{
					int x = _point_o.X;
					int y = _point_o.Y;
					int w = _screen_width;
					int h = k_SCREEN_HEIGHT - MENU_EXT_HEIGHT + 60;
					if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h) )
					{
						if (TOUCHS(i)->_state == TOUCH_DOWN && _menu_ext_active)
						{
							if (_menu_ext_touch_id == -1)
							{
								if (!_menu_ext_isclose)
								{
									_menu_ext_isclose = true;
								}
								_menu_ext_touch_id = TOUCHS(i)->_id;
							}
						}
						else if (TOUCHS(i)->_state == TOUCH_UP)
						{
							if (_menu_ext_touch_id == TOUCHS(i)->_id)
							{
								_menu_ext_touch_id = -1;
							}
						}
					}
					else
					{
						if (TOUCHS(i)->_state == TOUCH_UP)
						{
							if (_menu_ext_touch_id == TOUCHS(i)->_id)
							{
								_menu_ext_touch_id = -1;
							}
						}
					}
				}
				
			}
			if (!_menu_ext_active)
			{
				GAME()->_friendManager.UpdateScroll();
			}
			
			//ResultUpdateTouchFreeGift();
		}
	}
    //if (_user._free_gift_collected == 6)
	if(_user._playgame_times == 6)
    {
        if (!_user._notify_enable)
        {
//#if defined (MAC_OS)
//			RequestRegisterNotificationSetting();
//#endif

            /*if (!_dialogMng.isOpenDialog())
            {
                char text[128];
                sprintf(text, "Do you want us to notify when the Treasure is ready?");
                _dialogMng.Add(DIALOG_TYPE_NOTIFY_POPUP, DIALOGTYPE_YESNO, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
            }*/
        }
    }
	if (_user._playgame_times == 5)
	{
		if (!_dialogMng.isOpenDialog())
		{
			char text[128];
			sprintf(text, "Do you love Sloomy enough to give us stars?");
			_dialogMng.Add(DIALOG_TYPE_RATE_US, DIALOGTYPE_YESNO, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
		}
	}
	if (_user._playgame_times == 11)
	{
		if (!_dialogMng.isOpenDialog())
		{
			char text[128];
			sprintf(text, "Would you like to follow us on Twitter?");
			_dialogMng.Add(DIALOG_TYPE_FOLLOW_TW, DIALOGTYPE_YESNO, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
		}
	}
	if(_user._hadLoginFB && GAME()->_device.IsNetworkAvailable())
    {
        if(GAME()->_friendManager._numTopPlayer > 0)
        {
            char fbid[128];
            sprintf(fbid, "%s", _user.GetFBID());
            int score = GAME()->_friendManager.GetScore(fbid);
            if(score != _hight_score && score > 0)
            {
                int rank = GAME()->_friendManager.GetRank(fbid);
                int my_index = GAME()->_friendManager.GetIndexRank(rank);
                if(my_index >= 0)
                {
                    GAME()->_friendManager._topPlayer[my_index]._score = (int)_hight_score;
                }
                
            }
        }
        
    }

}
void FDGame::ResultUpdateTouchFreeGift()
{
	if (!_free_gift_time.isNullDay())
	{
		return;
	}
	if (_dialogMng.isOpenDialog() || _gift_render_state > 0)
	{
		return;
	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		int x = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleX(17, 0) - 20;
		int y = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleY(17, 0) - 20;
		int w = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFrameWidth(17) + 40;
		int h = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFrameHeight(17) + 40;
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				PLAY_SFX(SFX_BUTTON);
				TOUCHS(i)->_state = TOUCH_DRAG;
				GiftActive();
				
			}
		}
	}
}
bool FDGame::ResultUpdateTouch()
{
	bool press = false;
	bool ret = false;
	int index = -1;
	if (_dialogMng.isOpenDialog() || _gift_render_state > 0 || _menu_ext_active || _menu_ext_touch_id != -1 || GAME()->_purchase_procesing_opac > 0 || _rank_up_active)
	{
		return false;
	}
#if defined (MAC_OS)
    if ((PineAdBox::showAdState == SHOW_AD_PROCESSING || PineAdBox::showAdState == SHOW_AD_COMPLETED) && _want_to_retry)
    {
        if( PineAdBox::showAdState != SHOW_AD_COMPLETED)
        {
            return  false;
        }
    }
#endif
    for (int i = 0; i < RESULT_NUM_BUTTON; i++)
	{
		if (_result_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_result_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case RESULT_BUTTON_FB_SHARE:
				_share_score_state = 1;
				_share_score_opac = 0;
				_share_score_type = FB_SHARE_TYPE_SCORE;
				break;
			case RESULT_BUTTON_LEADERBOARD:
				GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_GC;
#if defined (MAC_OS)
                    if ((GAME()->_serverThread._gcSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS || GAME()->_serverThread._gcSocial->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT) && GAME()->_serverThread._gcSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
                    {
                        GAME()->_serverThread.AddCommand(CServerThread::COMMAND_TYPE_SUBMIT_LEADERBOARD);
                    }
                    PineGameCenterController::ShowLeaderBoard("Sloomy_01");
#endif
				break;
			case RESULT_BUTTON_RETRY:
                {
                    _result_isclose = true;
                    _want_to_retry = true;
                }
				break;
			case RESULT_BUTTON_WATCH_ADS:
				break;
			case RESULT_BUTTON_LOGIN_FB:
#if defined (MAC_OS)
                    if(GAME()->_device.IsNetworkAvailable())
                    {
                        GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_INIT_FB);
                    }
                    else
                    {
                        if (!_dialogMng.isOpenDialog())
                        {
                            char text[128];
                            sprintf(text, "Network required!");
                            _dialogMng.Add(DIALOG_TYPE_NETWORK_ERROR, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
                        }
                    }
                    
#endif
				break;
			case RESULT_BUTTON_SETTING:
				//MenuExtInit();
				MenuExtActive();
				break;
			default:
				break;
			}
			if (_result_button[index]._state != BUTTON_STATE_LOCK && _result_button[index]._state != BUTTON_STATE_HIDE)
			{
				_result_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}
		
	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < RESULT_NUM_BUTTON; touch++)
		{
			if (_result_button[touch]._state == BUTTON_STATE_LOCK || _result_button[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}
            int w = _result_button[touch].TZ_GetWidth();
            int h = _result_button[touch].TZ_GetHeight();
			int x = _result_button[touch].TZ_GetX();
			int y = _result_button[touch].TZ_GetY();
			Button *button = &_result_button[touch];
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
						PLAY_SFX(SFX_BUTTON);
					}
					else
					{
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
					}
					else
					{
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
		}
		
	}
	return press;
}

void FDGame::FlowerRenderSort(int type)
{
	switch (type)
	{
	case FD_FLOWER_SORT_TYPE_NUM:
	{
		int max = 0;
		for (int i = 0; i < FD_FLOWER_NUM_TYPE - 1; i++)
		{
			max = _flower_render_ingame[_flower_render_index[i]]._xp_collected;
			for (int j = i + 1; j < FD_FLOWER_NUM_TYPE; j++)
			{
				int index = _flower_render_index[j];
				if (max < _flower_render_ingame[index]._xp_collected)
				{
					int temp = _flower_render_index[i];
					max = _flower_render_ingame[index]._xp_collected;
					_flower_render_index[i] = _flower_render_index[j];
					_flower_render_index[j] = temp;
					
				}
			}
		}	
	}
		break;
	case FD_FLOWER_SORT_TYPE_ID:
	{
		int temp = _flower_render_index[0];
		_flower_render_index[0] = _flower_render_index[FD_FLOWER_NUM_TYPE - 1];
		_flower_render_index[FD_FLOWER_NUM_TYPE - 1] = temp;
	}
		
		break;
	default:
		break;
	}
	for (int i = 0; i < FD_FLOWER_NUM_TYPE - 1; i++)
	{
		_flower_render_ingame[_flower_render_index[i]]._disable_hold_time = 10 + i * 8;
        
	}
}



void FDGame::InitParticleRain()
{
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX;i++)
	{
		_particle_rain_s1[i].Init();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s2[i].Init();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s3[i].Init();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s4[i].Init();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s5[i].Init();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s6[i].Init();
	}
}
void FDGame::RenderParticleRain(float x, float y, int opac)
{
	
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		if (_score_interface[0].GetType() == SCROLL_TYPE_END )
		{
			_particle_rain_s1[i].ForceEnd();
		}
		_particle_rain_s1[i].Render(x, y, opac);
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		if (_score_interface[1].GetType() == SCROLL_TYPE_END)
		{
			_particle_rain_s2[i].ForceEnd();
		}
		_particle_rain_s2[i].Render(x, y, opac);
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		if (_score_interface[2].GetType() == SCROLL_TYPE_END)
		{
			_particle_rain_s3[i].ForceEnd();
		}
		_particle_rain_s3[i].Render(x, y, opac);
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		if (_score_interface[3].GetType() == SCROLL_TYPE_END)
		{
			_particle_rain_s4[i].ForceEnd();
		}
		_particle_rain_s4[i].Render(x, y, opac);
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		if (_score_interface[4].GetType() == SCROLL_TYPE_END)
		{
			_particle_rain_s5[i].ForceEnd();
		}
		_particle_rain_s5[i].Render(x, y, opac);
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		if (_score_interface[5].GetType() == SCROLL_TYPE_END)
		{
			_particle_rain_s6[i].ForceEnd();
		}
		_particle_rain_s6[i].Render(x, y, opac);
	}
}
void FDGame::UpdateParticleRain()
{
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s1[i].Update();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s2[i].Update();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s3[i].Update();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{
		_particle_rain_s4[i].Update();
	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{

		_particle_rain_s5[i].Update();

	}
	for (int i = 0; i < PARTICLE_RAIN_NUM_MAX; i++)
	{

		_particle_rain_s6[i].Update();

	}
}
void FDGame::CreditInit()
{
	for (int i = 0; i < CREDIT_BUTTON_NUM; i++)
	{
		_credit_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
		_credit_button[i].SetAnim(SPRITEID_HUD_CONGRATULATION_01, i * 4);
		_credit_button[i].SetPos(k_SCREEN_WIDTH/2, k_SCREEN_HEIGHT*2/3 - 70);
	}
	_credit_render_opac = 0;
	_credit_render_active = false;
	_credit_is_close = false;
	_credit_scroll_state = 0;
	_credit_scroll_speed = 70;
	_credit_scroll_y = _screen_height;
}
void FDGame::CreditRender(float x, float y, int opac)
{
	if (!_credit_render_active)
	{
		return;
	}
	G()->SetOpacity(_credit_render_opac);
	G()->SetColor(0xf90f1d24);
	//G()->FillFullScreen(true);
	G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height,true);
	GET_SPRITE(SPRITEID_HUD_CONGRATULATION_01)->DrawFrame(G(), 5, x, y - _credit_scroll_y);
	
	for (int i = 0; i < CREDIT_BUTTON_NUM; i++)
	{
		_credit_button[i].SetPos(x + k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT * 2 / 3 + 170 + y - _credit_scroll_y);
		_credit_button[i].Render(G());
		_credit_button[i].Update();
	}
	G()->SetOpacity(opac);
}
void FDGame::CreditUpdate()
{
	if (!_credit_render_active)
	{
		return;
	}
	if (_credit_is_close)
	{
		if (_credit_render_opac > 0)
		{
			_credit_render_opac -= 5;
			if (_credit_render_opac <= 0)
			{
				_credit_render_opac = 0;
				_credit_render_active = false;
			}
		}
		_credit_scroll_y += _credit_scroll_speed;
		_credit_scroll_speed *= 1.1f;
	}
	else
	{
		if (_credit_render_opac <= 100)
		{
			_credit_render_opac += 20;
			if (_credit_render_opac >= 100)
			{
				_credit_render_opac = 100;
			}
		}
		switch (_credit_scroll_state)
		{
		case 0:
			break;
		case 1:
			if (_credit_scroll_y > -100)
			{
				if (_credit_scroll_y <= 0)
				{
					_credit_scroll_speed *= 0.55f;
					if (_credit_scroll_speed <= 2)
					{
						_credit_scroll_speed = 2;
					}
				}
				//printf("\n _credit_scroll_speed = %f", _credit_scroll_speed);
				_credit_scroll_y -= _credit_scroll_speed;
				if (_credit_scroll_y <= -100)
				{
					_credit_scroll_state = 2;
					//_credit_scroll_y = 640;
					_credit_scroll_speed = 20;
				}
			}
			break;
		case 2:
			if (_credit_scroll_y < 0)
			{
				int speed = (CMath::ABSOLUTE_VALUE(_credit_scroll_y));

				if (speed <= 50)
				{
					_credit_scroll_speed *= 0.67f;
				}
				if (_credit_scroll_speed <= 0.2)
				{
					_credit_scroll_speed = 0.2f;
				}
				//printf("\n _credit_scroll_speed = %f", _credit_scroll_speed);
				_credit_scroll_y += _credit_scroll_speed;
				if (_credit_scroll_y >= 0)
				{
					_credit_scroll_state = 3;
					_credit_scroll_y = 0;
					_credit_scroll_speed = 50;
				}
			}
			break;
		case 3:
			break;
		default:
			break;
		}
	}
	if (_credit_render_opac >=100)
	{
		CreditUpdateTouch();
	}
}
bool FDGame::CreditUpdateTouch()
{
	bool press = false;
	bool ret = false;
	int index = -1;
	for (int i = 0; i < CREDIT_BUTTON_NUM; i++)
	{
		if (_credit_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_credit_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case CREDIT_BUTTON_OK:
				_credit_is_close = true;
				break;
			default:
				break;
			}
			if (_credit_button[index]._state != BUTTON_STATE_LOCK && _credit_button[index]._state != BUTTON_STATE_HIDE)
			{
				_credit_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < CREDIT_BUTTON_NUM; touch++)
		{
			int x = _credit_button[touch].TZ_GetX();
			int y = _credit_button[touch].TZ_GetY();
			int w = _credit_button[touch].TZ_GetWidth();
			int h = _credit_button[touch].TZ_GetHeight();
			Button *button = &_credit_button[touch];
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						PLAY_SFX(SFX_BUTTON);
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;

						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
			}
		}

	}
	return press;
}

void FDGame::ShareScoreInit()
{
	_share_score_state = 0;
	_share_score_opac = 0;
	_share_score_type = FB_SHARE_TYPE_SCORE;
}
void FDGame::ShareScoreRender(float x, float y, int opac)
{
	if (_share_score_state == 0)
	{
		return;
	}
	x += k_SCREEN_WIDTH / 2;
	y += k_SCREEN_HEIGHT / 2;
	G()->SetOpacity(_share_score_opac);
	G()->SetColor(0xee0f1d24);
	//G()->FillFullScreen(true);
	G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height,true);
	switch (_share_score_type)
	{
	case FB_SHARE_TYPE_SCORE:
		GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->DrawFrame(G(), 0, x, y);
		break;
	case FB_SHARE_TYPE_RANK:
	{
		int stringX = 0;
		int stringY = 0;
		int spacing = 0;
		GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->DrawFrame(G(), 1, x, y);
		int mx = GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->GetFModuleCenterX(1, 5);
		int my = GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->GetFModuleCenterY(1, 5)+1;
		
#if defined(MAC_OS)
		if(_my_rank._avatar_state == AVATAR_STATE_INIT_SUCCESS)
        {
            float im_scale = 200.0f/_friend_rank._avatar.GetWidth();
            G()->SetScale(im_scale, im_scale, mx + x - 101, my + y - 101);
            _my_rank._avatar.DrawImageWithRect(G(), mx + x - 101, my + y - 101);
            G()->ClearScale();
        }
        else
        {
            G()->SetScale(2.0f, 2.0f, mx + x, my + y);
            GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 126, mx + x, my + y+5);
            G()->ClearScale();
        }

        GAME()->_freeTypeText.SetText(_my_rank._userNameUnicode, _my_rank._userNameUnicodeLenght, 6);
        stringX = mx + x;
        stringY = my + y - 170;
        G()->SetScale(1.2f, 1.2f, stringX, stringY);
        GAME()->_freeTypeText.Render(stringX, stringY, _HCENTER);
        G()->ClearScale();
        spacing = GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing;
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = 3;
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->DrawAnsiTextFormat(G(), stringX, stringY + 30, _HCENTER | _VCENTER, "#%d", _my_rank._rank);
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = spacing;
        spacing = GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing;
        GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = 5;
        GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->DrawAnsiTextFormat(G(), mx + x, my + y + 155, _HCENTER | _VCENTER, "%d", _my_rank._score);
        GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = spacing;
        //GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->DrawFrame(G(), 2, x, y);
#else
		G()->SetColor(0xffff0000);
		G()->FillRect(mx - 100 + x, my - 100 + y, 200, 200);
#endif

		mx = GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->GetFModuleCenterX(1, 6)+1;
		my = GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->GetFModuleCenterY(1, 6)-2;
#if defined(MAC_OS)
        if(_friend_rank._avatar_state == AVATAR_STATE_INIT_SUCCESS)
        {
            float im_scale = 200.0f/_friend_rank._avatar.GetWidth();
            G()->SetScale(im_scale, im_scale, mx + x - 101, my + y - 101);
            _friend_rank._avatar.DrawImageWithRect(G(), mx + x - 101, my + y - 101);
            G()->ClearScale();
        }
        else
        {
            G()->SetScale(2.0f, 2.0f, mx + x, my + y);
            GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 126, mx + x, my + y+5);
            G()->ClearScale();
        }
#else
		G()->SetColor(0xffff0000);
		G()->FillRect(mx - 100 + x, my - 100 + y, 200, 200);
#endif
        GAME()->_freeTypeText.SetText(_friend_rank._userNameUnicode, _friend_rank._userNameUnicodeLenght, 6);
        stringX = mx + x;
        stringY = my + y - 170;
        G()->SetScale(1.2f, 1.2f, stringX, stringY);
        GAME()->_freeTypeText.Render(stringX, stringY, _HCENTER);
        G()->ClearScale();
        spacing = GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing;
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = 3;
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->DrawAnsiTextFormat(G(), stringX, stringY + 30, _HCENTER | _VCENTER, "#%d", _friend_rank._rank);
        GET_SPRITE(SPRITEID_FONT_POP_UP_01)->_char_spacing = spacing;
        spacing = GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing;
        GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = 5;
        GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->DrawAnsiTextFormat(G(), mx + x, my + y + 155, _HCENTER | _VCENTER, "%d", _friend_rank._score);
        GET_SPRITE(SPRITEID_FONT_CONGRATULATION_01)->_char_spacing = spacing;
        GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->DrawFrame(G(), 2, x, y);
        
	}
		break;
	default:
		break;
	}
	
	int mx = GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->GetFModuleX(0, 4) ;
	int my = GET_SPRITE(SPRITEID_HUD_SHARE_FACEBOOK_01)->GetFModuleY(0, 4) + 10;
    if(_share_score_type == FB_SHARE_TYPE_RANK)
    {
        my += 25;
    }
	GET_SPRITE(SPRITEID_FONT_SCORE_FACEBOOK_01)->DrawAnsiTextFormat(G(), x + mx, y + my, _LEFT | _VCENTER, "%d", _hight_score);
	G()->SetOpacity(opac);
}
void FDGame::ShareScoreUpdate()
{
	if (_share_score_state == 0)
	{
		return;
	}
	switch (_share_score_state)
	{
	case 1:
		if (_share_score_opac < 100)
		{
			_share_score_opac += 5;
			if (_share_score_opac >= 100)
			{
				_share_score_state = 2;
				_share_score_opac = 100;
#if defined(MAC_OS)

				int mx = k_SCREEN_WIDTH/2 - 375 ;
				int my = k_SCREEN_HEIGHT/2 - 375;
                float offsetx = 0;
                if(GAME()->_device._version == SupportVersion::RESVER_1024_768)
                {
                    offsetx = 96;
                }
				PineSocial::SaveScreenToDocument("SloomyCapture", mx+offsetx, my, 750.0f-offsetx*2, 750.0f);
#endif
			}
		}
		break;
	case 2:
		if (_share_score_opac < 120)
		{
			_share_score_opac += 1;
			if (_share_score_opac >= 120)
			{
				_share_score_opac = 120;
				_share_score_state = 3;
			}
		}
		break;
	case 3:
		_share_score_opac -= 2;
		if (_share_score_opac <= 100)
		{
			if (_share_score_opac == 100)
			{
#if defined(MAC_OS)
                if(_share_score_type == FB_SHARE_TYPE_SCORE)
                {
                    sprintf(_textToPostSocial, "Beat my score! #Sloomy");
                }
                else
                {
                    
                    sprintf(_textToPostSocial, "Boom! I beat %s in #Sloomy",_friend_rank._name);
                    //sprintf(_textToPostSocial, "Boom! I beat %s in #Sloomy",_my_rank._name);
                    
                }
                //printf("\n _textToPostSocial = %s",_textToPostSocial);
				PineSocial::ShareImageWithURLDocument(_textToPostSocial, "SloomyCapture");
#endif
			}
			if (_share_score_opac <= 0)
			{
				_share_score_state = 0;
				_share_score_opac = 0;
			}
		}
		break;

	default:
		break;
	}
}
void FDGame::EffectFireStarInit()
{
	_count_frame_add_fire_star = -1;
	for (int i = 0; i < EFFECT_FIRE_STAR_NUM_MAX; i++)
	{
		_effect_fire_star[i].Init(SPRITEID_EFFECT_EXPLOSION_01, 54, PARTICLE_FIRE_WORK_TYPE_CIRCLE, false);
	}
}
void FDGame::EffectFireStarAdd(float x, float y, float scale, float speedscale, float rotatespeed, float opacspeed, float speed, int num)
{
	for (int i = 0; i < EFFECT_FIRE_STAR_NUM_MAX; i++)
	{
		if (_effect_fire_star[i]._state == 0)
		{
			_effect_fire_star[i].Active(x, y, scale, speedscale, rotatespeed, opacspeed, speed, num);
			break;
		}
	}
}
void FDGame::EffectFireStarRender(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_FIRE_STAR_NUM_MAX; i++)
	{
		_effect_fire_star[i].Render(x, y, opac);
	}
}
void FDGame::EffectFireStarUpdate()
{
	for (int i = 0; i < EFFECT_FIRE_STAR_NUM_MAX; i++)
	{
		_effect_fire_star[i].Update();
	}
}
void FDGame::GiftInit()
{
	_gift_render_opac = 100;
	_gift_render_state = 0;
	_gift_render_scale = 0.6f;
	_gift_render_x = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(17, 0);
	_gift_render_y = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(17, 0);
	_gift_render_anim.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_OPEN_GIFT_01), SPRITEID_HUD_OPEN_GIFT_01);
	_gift_render_anim.SetAnim(0, true);
	_gift_render_anim.SetPos(_gift_render_x, _gift_render_y);
	_gift_glow_opac = 0;
	_gift_glow_scale = 0;
	_gift_glow_rotate = 0;
	_gift_particle.Init(SPRITEID_HUD_OPEN_GIFT_01, 15);
}
void FDGame::GiftActive()
{
	_gift_render_state = 1;
	_gift_glow_opac = 0;
	_gift_glow_scale = 0;
	_gift_glow_rotate = 0;
	_gift_render_opac = 100;
	_gift_render_scale = 0.7f;
	_gift_render_x = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterX(17, 0);
	_gift_render_y = GET_SPRITE(SPRITEID_HUD_RESULT_01)->GetFModuleCenterY(17, 0);
	_gift_render_anim.SetAnim(0, true);
	_gift_render_anim.SetPos(_gift_render_x, _gift_render_y);
}
void FDGame::GiftRender(float x, float y)
{
	if (_gift_render_state == 0)
	{
		return;
	}
	G()->SetOpacity(_gift_render_opac);
	G()->SetColor(0xf50f1d24);
	G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height, true);
	G()->SetRotate(_gift_glow_rotate, _gift_render_x, _gift_render_y - 10);
	G()->SetOpacity(_gift_glow_opac);
	G()->SetScale(_gift_glow_scale, _gift_glow_scale, _gift_render_x, _gift_render_y - 10);
	GET_SPRITE(SPRITEID_HUD_OPEN_GIFT_01)->DrawFrame(G(), 0, _gift_render_x, _gift_render_y - 10);
	G()->ClearScale();
	G()->SetOpacity(100);
	G()->ClearRotate();
	G()->SetScale(_gift_render_scale, _gift_render_scale, _gift_render_x, _gift_render_y);
	_gift_render_anim.DrawAnimObject(G());
	G()->ClearScale();
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	_gift_particle.Render(x, y, _gift_render_opac);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
}
void FDGame::GiftUpdate()
{
	_gift_render_anim.SetPos(_gift_render_x, _gift_render_y);
	_gift_render_anim.UpdateAnimObject();
	_gift_particle.Update();
	switch (_gift_render_state)
	{
	case 0:
		break;
	case 1:
	{
		if (_gift_render_opac < 100)
		{
			_gift_render_opac += 5;
		}
		if (CHECK_POINT_IN_CIRCLE(_gift_render_x, _gift_render_y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, 20))
		{
			_gift_render_state = 2;
			_gift_render_x = k_SCREEN_WIDTH / 2;
			_gift_render_y = k_SCREEN_HEIGHT / 2;
			_gift_render_anim.SetAnim(1, false);
		}
		else
		{
			float angle = CMath::ANGLE(_gift_render_x, _gift_render_y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			_gift_render_x += 20 * CMath::COS(angle);
			_gift_render_y += 20 * CMath::SIN(angle);
		}
		float dis = CMath::DISTANCE(_gift_render_x, _gift_render_y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_gift_render_scale = 1.0f - (dis / 465.0f)*0.5f;
		if (_gift_render_scale >= 1.0f)
		{
			_gift_render_scale = 1.0f;
		}
	}
		break;
	case 2:
		if (_gift_render_anim.IsCurrentAnimStop())
		{
			_gift_render_state = 3;
			_gift_render_anim.SetAnim(2, false);
			_gift_particle.Active(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 - 20, 0, 0.02f, 1, 1, 1.5f, 80, 2);
		}
		break;
	case 3:
		if (_gift_render_anim.IsCurrentAnimStop())
		{
			_gift_render_state = 4;
			_gift_render_anim.SetAnim(3, true);
		}
		_gift_glow_scale += 0.1;
		if (_gift_glow_scale >= 1.4f)
		{
			_gift_glow_scale = 1.4f;
		}
		break;
	case 4:
		_gift_glow_opac += 3;
		if (_gift_glow_opac>= 160)
		{
			if (!_dialogMng.isOpenDialog())
			{
				_free_gift_index = CMath::RANDOM(0, _game_design.General_design.SheetTreasure_Item.int32Value._totalData - 1);
				int param_type = CGameDesign::FileGeneral_design::CSheetTreasure_Item::Power_up;
				int param_val = CGameDesign::FileGeneral_design::CSheetTreasure_Item::quantity;
				int type_rw = GetDesignFreeGiftRW(_free_gift_index, param_type);
				int value_rw = GetDesignFreeGiftRW(_free_gift_index, param_val);
				char text[256];
				sprintf(text, ":%d", value_rw);
				_dialogMng.Add(type_rw - 1, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
				_congra_fire_work1.Active(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, 0.2f, 0.01f, 1, 1, 6, 40);
				_congra_fire_work2.Active(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, 0.2f, 0.01f, 1, 1, 6, 20);
			}
			_gift_render_state = 5;
		}
		_gift_glow_scale += 0.1;
		if (_gift_glow_scale >= 1.4f)
		{
			_gift_glow_scale = 1.4f;
		}
		_gift_glow_rotate += 2;
		if (_gift_glow_rotate >= 360)
		{
			_gift_glow_rotate -= 360;
		}
		_gift_glow_scale += 0.1;
		if (_gift_glow_scale >= 1.4f)
		{
			_gift_glow_scale = 1.4f;
		}
		break;
	case 5:
		_gift_glow_scale += 0.1;
		if (_gift_glow_scale >= 1.4f)
		{
			_gift_glow_scale = 1.4f;
		}
		_gift_glow_rotate += 2;
		if (_gift_glow_rotate >= 360)
		{
			_gift_glow_rotate -= 360;
		}
		break;
	case 6:
		if (_gift_glow_scale > 0)
		{
			_gift_glow_scale -= 0.1f;
			if (_gift_glow_scale < 0)
			{
				_gift_glow_scale = 0;
			}
		}
		if (_gift_render_scale> 0)
		{
			_gift_render_scale -= 0.1f;
			if (_gift_render_scale < 0)
			{
				_gift_render_scale = 0;
				_gift_render_state = 0;
				_gift_particle.ForceEnd();
			}
		}
		if (_gift_render_opac > 0)
		{
			_gift_render_opac -= 10;
		}
		break;
	default:
		break;
	}
}

void FDGame::MenuExtRender(float x, float y, int opac)
{
	if (!_menu_ext_active)
	{
		return;
	}
	G()->SetOpacity(100);
	G()->SetColor(0xa90f1d24);
	G()->FillRect(_point_o.X,_point_o.Y,_screen_width,_screen_height,true);
	G()->SetColor(0xf5193b33);
	//G()->FillFullScreen(true);
	int fill_h = MENU_EXT_HEIGHT;
	G()->FillRect(_point_o.X, _point_o.Y + _screen_height - _menu_ext_scroll_y, _screen_width, fill_h, true);
	GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(G(), 110, 0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
	if (_user._has_removed_ads)
	{
		if (_menu_ext[RESULT_MENU_EXT_REMOVE_ADS]._state != BUTTON_STATE_LOCK)
		{
			_menu_ext[RESULT_MENU_EXT_REMOVE_ADS].SetState(BUTTON_STATE_LOCK);
		}
	}
	for (int i = 0; i < RESULT_MENU_EXT_NUM_BUTTON; i++)
	{
		int offsetx = 0;
		/*if (i == RESULT_MENU_EXT_HIDE)
		{
			offsetx = -_point_o.X;
		}*/
		_menu_ext[i].SetPos(0 + offsetx, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
		_menu_ext[i].Render(G());
		_menu_ext[i].Update();

	}
	G()->SetOpacity(opac);
}
void FDGame::MenuExtUpdate()
{
	if (!_menu_ext_active)
	{
		/*if (_result_button[RESULT_BUTTON_SETTING]._state == BUTTON_STATE_HIDE)
		{
			_result_button[RESULT_BUTTON_SETTING].SetState(BUTTON_STATE_NORMAL);
		}*/
		return;
	}
	else
	{
        //printf("\n GAME()->_statusBasicPostSocial = %d",GAME()->_statusBasicPostSocial);
        if(GAME()->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL && _menu_ext_touch_index == RESULT_MENU_EXT_CONTACT)
        {
            if(!_dialogMng.isOpenDialog())
            {
                char text[128];
                sprintf(text, "Error!\n\nPlease config your email first!");
                _dialogMng.Add(DIALOG_TYPE_NETWORK_ERROR, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
                GAME()->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_NONE;
                _menu_ext_touch_index = -1;
            }
        }
		if (!_menu_ext_isclose)
		{
			switch (_menu_ext_scroll_state)
			{
			case 0:
				break;
			case 1:
				if (_menu_ext_scroll_y < MENU_EXT_HEIGHT+100)
				{
					if (_menu_ext_scroll_y >= MENU_EXT_HEIGHT+50)
					{
						_menu_ext_scroll_speed *= 0.43f;
						if (_menu_ext_scroll_speed <= 2)
						{
							_menu_ext_scroll_speed = 2;
						}
					}
					printf("\n _menu_ext_scroll_speed = %f", _menu_ext_scroll_speed);
					_menu_ext_scroll_y += _menu_ext_scroll_speed;
					if (_menu_ext_scroll_y >= MENU_EXT_HEIGHT + 100)
					{
						_menu_ext_scroll_state = 2;
						//_menu_ext_scroll_y = 640;
						_menu_ext_scroll_speed = 15;
					}
				}
				break;
			case 2:
				if (_menu_ext_scroll_y > MENU_EXT_HEIGHT)
				{
					int speed = (CMath::ABSOLUTE_VALUE( _menu_ext_scroll_y - MENU_EXT_HEIGHT));
					
					if (speed <= 40)
					{
						_menu_ext_scroll_speed *= 0.68f;
					}
					if (_menu_ext_scroll_speed <= 0.2)
					{
						_menu_ext_scroll_speed = 0.2f;
					}
					printf("\n _menu_ext_scroll_speed = %f", _menu_ext_scroll_speed);
					_menu_ext_scroll_y -= _menu_ext_scroll_speed;
					if (_menu_ext_scroll_y <= MENU_EXT_HEIGHT)
					{
						_menu_ext_scroll_state = 3;
						_menu_ext_scroll_y = MENU_EXT_HEIGHT;
					}
				}
				break;
			case 3:
				break;
			default:
				break;
			}
			
		}
		else
		{
			if (_menu_ext_scroll_y > 0)
			{
				_menu_ext_scroll_y -= 50;
				if (_menu_ext_scroll_y <= 0)
				{
					_menu_ext_scroll_y = 0;
					_menu_ext_active = false;
				}
			}
		}
		/*if (_menu_ext_scroll_y < 300)
		{
			if (_result_button[RESULT_BUTTON_SETTING]._state!= BUTTON_STATE_HIDE)
			{
				_result_button[RESULT_BUTTON_SETTING].SetState(BUTTON_STATE_HIDE);
			}
		}*/
		if (!MenuExtUpdateTouch())
		{
		}
		if (_user._music_enable != GAME()->_musicEnable)
		{
			GAME()->_musicEnable = _user._music_enable;
		}
		if (_user._sound_enable != GAME()->_soundEnable)
		{
			GAME()->_soundEnable = _user._sound_enable;
		}
	}
}
bool FDGame::MenuExtUpdateTouch()
{
	if (_menu_ext_scroll_y < MENU_EXT_HEIGHT || _menu_ext_touch_id != -1 || GAME()->_purchase_procesing_opac > 0)
	{
		return false;
	}
	bool press = false;
	bool ret = false;
	int index = -1;
	if (_dialogMng.isOpenDialog())
	{
		return false;
	}
	for (int i = 0; i < RESULT_MENU_EXT_NUM_BUTTON; i++)
	{
		if (_menu_ext[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_menu_ext[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			/*case RESULT_MENU_EXT_HIDE:
				_menu_ext_isclose = true;
				break;*/
			case RESULT_MENU_EXT_SOUND:
				_user._sound_enable = !_user._sound_enable;
				if (!_user._sound_enable)
				{
					_menu_ext[RESULT_MENU_EXT_SOUND].SetAnim(SPRITEID_HUD_RESULT_01, 67);
					_menu_ext[RESULT_MENU_EXT_SOUND].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
				}
				else
				{
					_menu_ext[RESULT_MENU_EXT_SOUND].SetAnim(SPRITEID_HUD_RESULT_01, 27);
					_menu_ext[RESULT_MENU_EXT_SOUND].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
				}

				break;
			case RESULT_MENU_EXT_MUSIC:
				_user._music_enable = !_user._music_enable;
				GAME()->_musicEnable = _user._music_enable;
				if (!_user._music_enable)
				{
					GAME()->PauseCurrentMusic();
					_menu_ext[RESULT_MENU_EXT_MUSIC].SetAnim(SPRITEID_HUD_RESULT_01, 71);
					_menu_ext[RESULT_MENU_EXT_MUSIC].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
				}
				else
				{
					if (!GAME()->IsCurrentMusicPlaying())
					{
						GAME()->LoadMusic(BGM_MAINMENU_RESULT);
						GAME()->PlayCurrentMusic(-1);
					}
					else
					{
						GAME()->PlayCurrentMusic(-1);
					}
					_menu_ext[RESULT_MENU_EXT_MUSIC].SetAnim(SPRITEID_HUD_RESULT_01, 31);
					_menu_ext[RESULT_MENU_EXT_MUSIC].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
				}

				break;
			case RESULT_MENU_EXT_RATE:
#if defined (MAC_OS)
				GoToReviewInItune();
#endif
				break;
			case RESULT_MENU_EXT_LIKE_FB:
#if defined (MAC_OS)
				PineSocial::OpenURL(FACEBOOK_URL);
#endif
				break;
			case RESULT_MENU_EXT_LIKE_TW:
#if defined (MAC_OS)
				PineSocial::OpenURL(TWITTER_URL);
#endif
				break;
			case RESULT_MENU_EXT_CREDITS:
				CreditInit();
				_credit_render_active = true;
				_credit_scroll_state = 1;
				break;
			case RESULT_MENU_EXT_SYN_CLOUD:
				//GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_ICLOUD;
				//GAME()->ResetValueICloud();
#if defined (MAC_OS)
                    if(GAME()->_device.IsNetworkAvailable())
                    {
                        GAME()->CheckICloud();
                    }
                    else
                    {
                        if (!_dialogMng.isOpenDialog())
                        {
                            char text[128];
                            sprintf(text, "Network required!");
                            _dialogMng.Add(DIALOG_TYPE_NETWORK_ERROR, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
                        }
                    }
                    
#endif
				
				break;
			case RESULT_MENU_EXT_CONTACT:
#if defined (MAC_OS)
				char title[1024], body[1024];
				sprintf(title, "Sloomy's Support Request");
				sprintf(body, "%s", "");
				SendMailSupport(body, title);
                _menu_ext_touch_index = RESULT_MENU_EXT_CONTACT;
#endif
				break;
                case RESULT_MENU_EXT_REMOVE_ADS:
                    
#if defined (MAC_OS)
                    
                    if(GAME()->_device.IsNetworkAvailable())
                    {
                        PurchaseProduct("Sloomy_NoAds");
                        GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_IAP;
                    }
                    else
                    {
                        if (!_dialogMng.isOpenDialog())
                        {
                            char text[128];
                            sprintf(text, "Network required!");
                            _dialogMng.Add(DIALOG_TYPE_NETWORK_ERROR, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
                        }
                    }
                    
#endif
                    break;
                case RESULT_MENU_EXT_RESTORE:
#if defined (MAC_OS)
                    //PurchaseProduct("Sloomy_NoAds");
                    
                    if(GAME()->_device.IsNetworkAvailable())
                    {
                        IapRestore();
                        GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_IAP;
                    }
                    else
                    {
                        if (!_dialogMng.isOpenDialog())
                        {
                            char text[128];
                            sprintf(text, "Network required!");
                            _dialogMng.Add(DIALOG_TYPE_NETWORK_ERROR, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
                        }
                    }
#endif
                    break;
			default:
				break;
			}
			if (_menu_ext[index]._state != BUTTON_STATE_LOCK && _menu_ext[index]._state != BUTTON_STATE_HIDE)
			{
				_menu_ext[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < RESULT_MENU_EXT_NUM_BUTTON; touch++)
		{
            if(_menu_ext[touch]._state == BUTTON_STATE_LOCK || _menu_ext[touch]._state == BUTTON_STATE_HIDE)
            {
                continue;
            }
			int x = _menu_ext[touch].TZ_GetX();
			int y = _menu_ext[touch].TZ_GetY();
			int w = _menu_ext[touch].TZ_GetWidth();
			int h = _menu_ext[touch].TZ_GetHeight();
			Button *button = &_menu_ext[touch];
            
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						press = true;
						PLAY_SFX(SFX_BUTTON);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						//button->SetState(BUTTON_STATE_NORMAL);
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
			}
		}

	}
	return press;
}
void FDGame::MenuExtInit()
{
	_menu_ext_isclose = false;
	_menu_ext_scroll_y = 0;
	_menu_ext_active = false;
	_menu_ext_scroll_state = 0;
	_menu_ext_scroll_speed = 34;
	_menu_ext_touch_id = -1;
    _menu_ext_touch_index = -1;
	for (int i = 0; i < RESULT_MENU_EXT_NUM_BUTTON; i++)
	{
		int anim_start = 27;
		_menu_ext[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
		_menu_ext[i].SetAnim(SPRITEID_HUD_RESULT_01, anim_start + i * 4);
		_menu_ext[i].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
		/*if (i == RESULT_MENU_EXT_HIDE)
		{
			_menu_ext[i].SetPos(0 - _point_o.X, 0);
		}*/
	}
	if (!_user._sound_enable)
	{
		_menu_ext[RESULT_MENU_EXT_SOUND].SetAnim(SPRITEID_HUD_RESULT_01, 67);
		_menu_ext[RESULT_MENU_EXT_SOUND].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
	}
	if (!_user._music_enable)
	{
		_menu_ext[RESULT_MENU_EXT_MUSIC].SetAnim(SPRITEID_HUD_RESULT_01, 71);
		_menu_ext[RESULT_MENU_EXT_MUSIC].SetPos(0, MENU_EXT_HEIGHT - _menu_ext_scroll_y);
	}
    
}

void FDGame::MenuExtActive()
{
	_menu_ext_scroll_speed = 34;
	_menu_ext_active = true;
	_menu_ext_scroll_state = 1;
	_menu_ext_isclose = false;
	_menu_ext_scroll_y = 0;
	_menu_ext_touch_id = -1;
    _menu_ext_touch_index = -1;
}