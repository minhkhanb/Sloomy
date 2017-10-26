#include "pch.h"
#include "../../GameCore/CGame.h"
extern "C" void	GoToReviewInItune();
extern "C" void SendMailSupport(const char *body, const char *title);

void FDGame::GamePlayRender(float x, float y, int opac)
{
	x += _screen_shake_x;
	y += _screen_shake_y;
	if (_interface_weather_opac < 100)
	{
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 11, 0, 0);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 0, 0, 0);
	}
	
	if (_interface_weather_opac > 0)
	{
		G()->SetOpacity(_interface_weather_opac);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 12, 0, 0);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 1, 0, 0);
	}
	
	if (_interface_weather_opac_night > 0)
	{
		G()->SetOpacity(_interface_weather_opac_night);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 13, 0, 0);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 2, 0, 0);
	}
	G()->SetOpacity(opac);
	if (_interface_weather_opac < 100)
	{
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->DrawFrame(G(), 0, 0, -(GAME()->reduce));	//:todo---move -y background board
	}

	if (_interface_weather_opac > 0)
	{
		G()->SetOpacity(_interface_weather_opac);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->DrawFrame(G(), 1, 0, 0);
	}
	
	if (_interface_weather_opac_night > 0)
	{
		G()->SetOpacity(_interface_weather_opac_night);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->DrawFrame(G(), 2, 0, 0);
	}
	if (_fever_field_opac > 0)
	{
		G()->SetColor(0xb50f1d24);
		G()->SetOpacity(_fever_field_opac);
		//printf("\n _fever_field_opac  %f", _fever_field_opac);
		G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height, true);
		G()->SetOpacity(opac);
	}



	//printf("\n _interface_weather_opac = %d,night = %d ", _interface_weather_opac, _interface_weather_opac_night);

	if (_interface_weather_opac_night % 30 == 0 && _interface_weather_opac_night >= -120 && _has_started
		&& !_hurry_up_active)
	{
		if (_particle_leaft._num == 0 && _fever_mode_state == 0)
		{
			HurryUpActive(HURRY_UP_TYPE_NORMAL);
		}
		if (_particle_leaft_add_trigger < _interface_weather_opac_night)
		{
			_particle_leaft_add_trigger = _interface_weather_opac_night;
			if (_particle_leaft._num < 15)
			{
				_particle_leaft.AddParticleLight(0, 0, 0, CMath::RANDOM(6, 12), 3, 3);
			}
			else
			{
				for (int i = 0; i < (_particle_leaft._num / 20) + 1; i++)
				{
					_particle_leaft.AddParticleLight(0, 0, 0, CMath::RANDOM(6, 12), 3, 3);
				}
			}
		}
	}
	/*if (_fever_field_opac > 0)
	{
		G()->SetOpacity(_fever_field_opac);
		{
			G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
			GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->DrawFrame(G(), 3, 0, 0);
			G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
		}
	}*/
	G()->SetOpacity(opac);
	FeverModeAnimRender(x, y);
	BoardRender(x, y);
	EffectFireRender(x, y);
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 41, 0, 0);
	float per = 0;
	if (!_game_time.isNullDay())
	{
		per = ((_game_time.GetTotalMiliSecond()*1.0f) / (FD_TIME_A_GAME*1000.0f));
	}
	int clipw = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFrameWidth(44);
	_timer_render_target = per*clipw;
	if (_timer_render_current < _timer_render_target)
	{
		_timer_render_current += 2;
		if (_timer_render_current >= _timer_render_target)
		{
			_timer_render_current = _timer_render_target;
		}
	}
	else if (_timer_render_current > _timer_render_target)
	{
		_timer_render_current -= 1;
		if (_timer_render_current <= _timer_render_target)
		{
			_timer_render_current = _timer_render_target;
		}
	}
	else
	{

	}
	int mkx = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleX(44, 0);
	int mky = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterY(44, 0); 
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 43, x, y- (GAME()->reduce));	//:todo---move -y background board
	G()->SetClip(mkx, 0, _timer_render_current, _screen_height); 
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 44, x, y- (GAME()->reduce));	//:todo---move -y background board
	if (_interface_weather_opac > 0)
	{
		G()->SetOpacity(_interface_weather_opac);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 55, x, y- (GAME()->reduce));	//:todo---move -y background board
	}
	
	if (_interface_weather_opac_night > 0)
	{
		G()->SetOpacity(_interface_weather_opac_night);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 56, x, y - (GAME()->reduce));	//:todo---move -y background board
	}
	/*if (_fever_field_opac > 0)
	{
		G()->SetOpacity(_fever_field_opac);
		GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 58, x, y);
	}*/
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 57, x, y - (GAME()->reduce));	//:todo---move -y background board
	G()->ClearClip();
	G()->SetOpacity(opac);
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 45, x + mkx + _timer_render_current, y + mky - (GAME()->reduce));	//:todo---move -y background board
	int sec = 0;
	if (!_game_time.isNullDay())
	{
		sec = _game_time.GetTotalSecond();
	}
	int char_space = GET_SPRITE(SPRITEID_FONT_TIMELINE_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_TIMELINE_01)->_char_spacing = 2;
	GET_SPRITE(SPRITEID_FONT_TIMELINE_01)->DrawAnsiTextFormat(G(), x + k_SCREEN_WIDTH / 2, y + mky - (GAME()->reduce), _HCENTER | _VCENTER, "%02d", sec); //:todo---move -y background board
	GET_SPRITE(SPRITEID_FONT_TIMELINE_01)->_char_spacing = char_space;
	CloudRender(x, y, 0);
	InterfaceRender(0, 0);
	CloudRender(x, y, 1);

	if (!_game_time.isNullDay())
	{
		sec = _game_time.GetTotalSecond();
	}
	if (sec < 9 && sec > 0)
	{
		if (_scale_time == 0)
		{
			PLAY_SFX(SFX_TIME_COUTING);
		}
		if (_scale_time < 1.0f)
		{
			_scale_time += 0.1f;
		}
		else
		{
			_scale_time = 1.0f;
		}
		G()->SetScale(1.0f, _scale_time, k_SCREEN_WIDTH / 2 + x, y + 150);
		GET_SPRITE(SPRITEID_FONT_COUNTDOWN_01)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2 + x, y + 150, _HCENTER | _VCENTER, "%d", sec);
		G()->ClearScale();

	}
	_draw_manager.Init();
	CubeRender(x, y, opac);
	RenderCharacterFake(x, y, opac);
	_draw_manager.Render(x, y- (GAME()->reduce));	//:todo---move -y background board	
	//PowerUpRender(x, y);
	EvaporateRender(x, y, opac);
	UnEvaporateRender(x, y, opac);
	int lifht_opac = _interface_weather_opac;
	if (lifht_opac < 0)
	{
		lifht_opac = 0;
	}
	_particle_lighting.Render(x + _point_o.X, y + _point_o.Y, 100 - lifht_opac);
	//RenderEffectLight(x + _point_o.X, y + _point_o.Y, 100 - lifht_opac);
	EffectExplosionRender(x, y, opac);

	EffectPointRender(x, y, opac);
	ParticleFlyToRender(x, y);
	for (int i = 0; i < GAMEPLAY_BUTTON_NUM; i++)
	{
		_gameplay_button[i].Render(G());
		_gameplay_button[i].Update();
	}
	//_butter_fly.Render(x, y, opac);
	if (_has_effect_sunrise && !_ingame_menu_active)
	{
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		G()->SetOpacity(_effect_sunrise_opac*0.12f);
		G()->SetColor(0xffffffff);
		//G()->FillFullScreen(true);
		G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height, true);
		int mx = GET_SPRITE(SPRITEID_EFFECT_LIGHT_01)->GetFModuleCenterX(0, 0);
		int my = GET_SPRITE(SPRITEID_EFFECT_LIGHT_01)->GetFModuleCenterY(0, 0);
		G()->SetOpacity(_effect_sunrise_opac*1.0f);
		G()->SetRotate(_sunrise_rotate, mx, my + _sunrise_mky);
		G()->SetScale(_sunrise_scale, _sunrise_scale, mx, my + _sunrise_mky);
		GET_SPRITE(SPRITEID_EFFECT_LIGHT_01)->DrawFrame(G(), _sunrise_frame_render1, 0, 0 + _sunrise_mky);
		GET_SPRITE(SPRITEID_EFFECT_LIGHT_01)->DrawFrame(G(), _sunrise_frame_render2, 0, 0 + _sunrise_mky);
		G()->ClearScale();
		G()->ClearRotate();
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
		G()->SetOpacity(opac);
	}
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 16, x + _point_o.X, y);
	int score_x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleX(16, 0) + 6;
	int score_y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleY(16, 0);
	_game_score_interface.Render(score_x + x + _point_o.X, score_y + y + 100, _LEFT, 2);
	_game_score_interface.Update();
	FeverModeRender(x, y, opac);
	HurryUpRender(x, y, opac);
	FarmerRender(x, y - (GAME()->farmerReduce), opac);	//:todo--move -y background farmer
	_particle_leaft.Render(x + _point_o.X, y + _point_o.Y, opac);
	IngameRender(x, y, opac);
	//CreditRender(x, y, opac);
}
void FDGame::GamePlayUpdate()
{
	if (_sub_state == k_INIT)
	{

#if defined MAC_OS
		PineAdBox::SetSkipAdvert();
#endif
#if defined (MAC_OS)
#if ADD_FACEBOOK
		/*if(_user._hadLoginFB)
		{
			GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_SCORE);
		}*/

#endif
#endif
		//_hight_score = 0;

		_particle_leaft_add_trigger = -1000;
		_fever_mode_level = 0;
		_fever_mode_time_bonus = 0;
		_scale_time = 0;
		int paramcombo = CGameDesign::FileGeneral_design::CSheetfever_mode::combo;
		_combo_num_max = GetDesignFeverModeTime(_fever_mode_level, paramcombo); 
		EffectFireInit();
		CloudInit();
		//_butter_fly.Init(SPRITEID_HUD_BUTTERFLY_01, CMath::RANDOM(0, 2));
		//_butter_fly.Active(0, 0, 2, 1.0f, 0.01f);
		int clipw = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFrameWidth(44);
		_timer_render_target = clipw;
		_timer_render_current = _timer_render_target;
		_time_plus = 0;
		ResetCharacterSize();
		ResetCharacterFake();
		_energy_total_current = 0;
		_energy_total_target = 0;
		_has_started = false;
		_use_power_up_num = 0;
		_energy_total = 0;
		//PowerUpReset();
		_sun_frame_index = 14;
		_sun_frame_smile = 0;
		_powerup_create_group_time = 0;
		ParticleFlyToInit();
		UnEvaporateInit();
		EvaporateInit();
		EffectExplosionInit();
		EffectPointInit();
		AddEffectLight();
		_particle_lighting.Init(0, 0);
		_particle_lighting.Active(0, 0, 0, 0, 1, 1, 20);
		_particle_leaft.Init(0, 0);

		CreditInit();
		//ResetCharacterFake();
		_game_time.setDateBYSecond(FD_TIME_A_GAME);
		//_game_time.setDateBYSecond(10);
		_game_score = 0;
		_game_score_interface.Init(SPRITEID_FONT_SCORE_INGAME_01, _game_score, 40);
		ResetFlowerHint();
		CubeInit();
		FillAllCube(true);
		ResetCubeReadyQueue();
		_interface_time_scrollx = 0;
		_interface_time_scrollx_tar = 0;
		_interface_weather_opac = 1;
		_interface_weather_opac_night = 1;
		_interface_time_scrollx_speed = 0.4f;
		_sub_state = k_LOOP;
		_count_frame_randomcube = CMath::RANDOM(5, 8);
		FlowerRenderInit();
		_hint_frame_countdown = 3 * FPS_MAX;
		_powerup_eat_random_type = CHARACTER_TYPE_NULL;
		for (int i = 0; i < GAMEPLAY_BUTTON_NUM; i++)
		{
			_gameplay_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
			_gameplay_button[i].SetAnim(SPRITEID_HUD_PAUSE_01, 0 + i * 4);
			_gameplay_button[i].SetPos(0 - _point_o.X, _point_o.Y);
		}

		_sun_glow_anim.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_BACKGROUND_01), SPRITEID_HUD_BACKGROUND_01);
		_sun_glow_anim.SetAnim(0, true);
		_sun_glow_anim.SetPos(0, 0);
		_sun_glow_anim1.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_BACKGROUND_01), SPRITEID_HUD_BACKGROUND_01);
		_sun_glow_anim1.SetAnim(1, true);
		_sun_glow_anim1.SetPos(0, 0);
		IngameMenuInit();
		_user.UserDataSave();
		_has_effect_sunrise = false;
		_effect_sunrise_opac = 0;
		_effect_sunrise_max = false;
		FeverModdeInit();
		FlowerCollectedInit();
		FarmerInit();
		FarmerActive();
		HurryUpInit();
		BoardInit();
		if (_user._music_enable != GAME()->_musicEnable)
		{
			GAME()->_musicEnable = _user._music_enable;
		}
		if (_user._sound_enable != GAME()->_soundEnable)
		{
			GAME()->_soundEnable = _user._sound_enable;
		}
		GAME()->LoadMusic(BGM_INGAME);
		GAME()->PlayCurrentMusic(-1);
		
		//printf("\n GAME()->_advert.IsAvailable() = %d,%d",GAME()->_advert.IsAvailable(),_user._play_game_num);
#if defined (ANDROID_OS)
		if (!GAME()->_has_show_ads_banner)
			GAME()->ShowBannerAds();
#endif 
	}
#if defined (ANDROID_OS)
	if (!GAME()->_has_show_ads_banner)
	{
		if (GAME()->_time_count_for_show_ads_banner > 0)
		{
			GAME()->_time_count_for_show_ads_banner--;
		}
		if (GAME()->_time_count_for_show_ads_banner == 0)
		{
			GAME()->ShowBannerAds();
		}
	}
#endif
#if defined MAC_OS
	PineAdBox::shouldAppPopup = false;
#endif
#if ADD_ADVERT
	if (GAME()->_advert.HasPopup())
	{
		return;
	}
#endif
	_screen_shake_x = 0;
	_screen_shake_y = 0;
	HurryUpUpdate();

	BoardUpdate();
	CreditUpdate();
	if (_credit_render_active)
	{
		return;
	}
	IngameUpdate();
	//_butter_fly.Update();
	if (_ingame_menu_active /*|| _dialogMng.isOpenDialog()*/)
	{
		return;
	}
	EffectFireUpdate();
	FeverModeUpdate();
	FarmerUpdate();

	if (_has_effect_sunrise)
	{
		if (_effect_sunrise_max)
		{
			if (_effect_sunrise_opac > 0)
			{
				_effect_sunrise_opac -= 2;
				if (_effect_sunrise_opac <= 0)
				{
					_has_effect_sunrise = false;
					_effect_sunrise_max = false;
				}
			}
		}
		else
		{
			if (_effect_sunrise_opac <= 120)
			{
				_effect_sunrise_opac += 2;
				if (_effect_sunrise_opac >= 120)
				{
					_effect_sunrise_opac = 120;

					_effect_sunrise_max = true;
				}
			}
		}
		if (_sunrise_scale > 0)
		{
			_sunrise_scale -= 0.001f;
		}
		_sunrise_rotate += 0.1f;
		if (_sunrise_rotate >= 360)
		{
			_sunrise_rotate -= 360;
		}
	}
	if (_interface_time_scrollx > _interface_time_scrollx_tar)
	{
		_interface_time_scrollx -= _interface_time_scrollx_speed;
		if (_interface_time_scrollx <= _interface_time_scrollx_tar)
		{
			_interface_time_scrollx = _interface_time_scrollx_tar;
		}
	}
	else if (_interface_time_scrollx < _interface_time_scrollx_tar)
	{
		_interface_time_scrollx += _interface_time_scrollx_speed;
		if (_interface_time_scrollx >= _interface_time_scrollx_tar)
		{
			_interface_time_scrollx = _interface_time_scrollx_tar;
		}
	}
	if (GAME()->_purchase_procesing_opac > 0)
	{
		return;
	}
	if (!_game_time.isNullDay() && _has_started && !_hurry_up_active && _fever_mode_state == 0)
	{
		int totaltime = _game_time.GetTotalMiliSecond();
		if (_game_time.UpdateCountDown())
		{
			_game_time.setNullDay();
			/*if (_hight_score < _game_score)
			{
			_hight_score = _game_score;
			_new_hight_score = true;
			}*/
			if (!_fever_mode_active && _fever_mode_time_bonus <= 0)
			{
				HurryUpActive(HURRY_UP_TYPE_TIMEUP);
			}
			else
			{
				_fever_mode_time_bonus = 120;
			}
		}
		else
		{
			_interface_time_scrollx_tar = 180 - 180 * (totaltime / (FD_TIME_A_GAME*1000.0f));
			if (_interface_time_scrollx_tar < 0)
			{
				_interface_time_scrollx_tar = 0;
			}
			_interface_weather_opac = /*100 -*/ 2000 * ((18000 - totaltime) / (FD_TIME_A_GAME*1000.0f));
			_interface_weather_opac_night = /*100 -*/ 7000 * ((7000 - totaltime) / (FD_TIME_A_GAME*1000.0f));
		}
	}
	/*if (_count_frame_randomcube <= 0)
	{
		_count_frame_randomcube = CMath::RANDOM(5, 8);
		CubeRandom();
	}*/
	_effect_cloud_frame_add--;
	if (_interface_weather_opac_night >= 100)
	{
		_effect_cloud_frame_add = 1;
	}
	if (_effect_cloud_frame_add <= 0)
	{
		_effect_cloud_frame_add = CMath::RANDOM(60, 300);
		int frame = CMath::RANDOM(49, 51);
		int cloudx = -200;
		int cloudy = CMath::RANDOM(0, 15) * 10;
		float speed = CMath::RANDOM(8, 20) / 10.0f;
		CloudAddNew(frame, cloudx, cloudy, _screen_width*1.5, cloudy, speed);
	}
	CloudUpdate();
	_sun_glow_anim1.UpdateAnimObject();
	_sun_glow_anim.UpdateAnimObject();
	FillAllCube(false);
	CubeUpdate();
	//PowerUpUpdate();
	UpdateCharacterFake();
	//UpdateEffectLight();
	_particle_lighting.Update();
	_particle_leaft.Update();
	UnEvaporateUpdate();
	EvaporateUpdate();
	EffectExplosionUpdate();
	EffectPointUddate();
	_sun_frame_index = 14;
	ParticleFlyToUpdate();
	if (_powerup_create_group_time > 0)
	{
		_powerup_create_group_time--;
		if (_powerup_create_group_time % 20 == 0)
		{
			int rand_group = CMath::RANDOM(0, 9);
			int row = CMath::RANDOM(0, FD_NUM_ROW - 3);
			int col = CMath::RANDOM(0, FD_NUM_COL - 3);
			if (rand_group <= 6)
			{
				CreateFlower2x2(row, col);
			}
			else
			{
				CreateFlower3x3(row, col);
			}
			//CubeRandom();
			FindAndSetFlowerGroup();
		}
		if (_powerup_create_group_time <= 0)
		{
			ResetFlowerHint();
		}
	}
	if (_cube_sur_finding)
	{
		_cube_sur_finding = false;
		for (int i = GAME()->_num_touches - 1; i >= 0; i--)
		{
			if (TOUCHS(i)->_state != TOUCH_UP)
			{
				_cube_sur_finding = true;
			}
		}
		return;
	}
	bool has_touch_screen = false;
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		int x = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleX(0, 0);
		int y = GET_SPRITE(SPRITEID_HUD_BACKGROUND_02)->GetFModuleY(0, 0) + 44;
		int w = k_SCREEN_WIDTH;
		int h = w;
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (_power_up_icon[POWER_UP_TYPE_CREATE_GROUP]._has_actived)
				{
					_powerup_create_group_time = 300;
					_power_up_icon[POWER_UP_TYPE_CREATE_GROUP].ActivedFunc(false);
					has_touch_screen = true;
					TOUCHS(i)->_state = TOUCH_DRAG;
					_power_up_icon[POWER_UP_TYPE_CREATE_GROUP].ActiveScale(1.0f, 1.1f, 1.0f, 0.1f, 4);
				}
				else if (_power_up_icon[POWER_UP_TYPE_SORT]._has_actived)
				{
					SortFlower();
					_power_up_icon[POWER_UP_TYPE_SORT].ActivedFunc(false);
					has_touch_screen = true;
					TOUCHS(i)->_state = TOUCH_DRAG;
					_power_up_icon[POWER_UP_TYPE_SORT].ActiveScale(1.0f, 1.1f, 1.0f, 0.1f, 4);
				}
			}
		}
		else
		{
			/*if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				for (int i = 0; i < POWER_UP_NUM_MAX; i++)
				{
					if (_power_up_icon[i]._has_actived)
					{
						_power_up_icon[i].ActivedFunc(true);
						_power_up_icon[i].ActiveScale(1.0f, 1.2f, 0.8f, 0.1f, 4);
						has_touch_screen = true;
					}
				}
				if (has_touch_screen)
				{
					TOUCHS(i)->_state = TOUCH_DRAG;
				}
			}*/
		}
	}
	if (_hurry_up_active)
	{
		if (_hurry_up_type == HURRY_UP_TYPE_TIMEUP)
		{
			if (_hurry_up_state == 4)
			{
				SetState(k_GS_RESULT);

			}
			return;
		}
	}
	if (_game_time.isNullDay())
	{
		//printf("\n _fever_mode_time_bonus = %d", _fever_mode_time_bonus);
		if (!_fever_mode_active)
		{
			_fever_mode_time_bonus--;
			if (_fever_mode_time_bonus == 0)
			{
				HurryUpActive(HURRY_UP_TYPE_TIMEUP);
			}
			//_fever_mode_time_bonus = 0;
			if (_fever_mode_time_bonus <= 0)
			{
				return;
			}
		}

	}
	if (_dialogMng.isOpenDialog() || !_has_tutorial_completed)
	{
		return;
	}
	if (!has_touch_screen)
	{
		CubeUpdateTouch();
	}
	_hint_frame_countdown--;
	if (_hint_frame_countdown <= 0)
	{
		FindFlowerHint();
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
		if (_hint_has_ready)
		{
			if (_hint_group_active == -1)
			{
				_hint_group_active = CMath::RANDOM(0, _hint_group_num - 1);
			}
			ActiveFlowerHint(_hint_group_active);
		}
	}
	if (!GamePlayUpdateTouch() && !has_touch_screen)
	{
		//PowerUpUpdateTouch();
	}


}
bool FDGame::GamePlayUpdateTouch()
{
	bool press = false;
	bool ret = false;
	int index = -1;
	for (int i = 0; i < GAMEPLAY_BUTTON_NUM; i++)
	{
		if (_gameplay_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_gameplay_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case GAMEPLAY_BUTTON_PAUSE:
				IngameMenuInit();
				_ingame_menu_active = true;
				_ingame_menu_scroll_state = 1;
				break;
			default:
				break;
			}
			if (_gameplay_button[index]._state != BUTTON_STATE_LOCK && _gameplay_button[index]._state != BUTTON_STATE_HIDE)
			{
				_gameplay_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < GAMEPLAY_BUTTON_NUM; touch++)
		{
			int x = _gameplay_button[touch].TZ_GetX() - 30;
			int y = _gameplay_button[touch].TZ_GetY() - 30;
			int w = _gameplay_button[touch].TZ_GetWidth() + 60;
			int h = _gameplay_button[touch].TZ_GetHeight() + 60;
			Button *button = &_gameplay_button[touch];
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

void FDGame::InterfaceRender(float x, float y)
{
	int my = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleY(0, 19);
	float moon_delta = _interface_time_scrollx - 170;
	if (moon_delta < 0)
	{
		moon_delta = 0;
	}
	int moon_posy = 700 - (_interface_time_scrollx + moon_delta*moon_delta * 5);
	//printf("\n posy = %d,_interface_time_scrollx = %f", moon_posy, _interface_time_scrollx);
	if (moon_posy <= 115)
	{
		moon_posy = 115;
	}
	float suny_delta = _interface_time_scrollx - 140;
	if (suny_delta < 0)
	{
		suny_delta = 0;
	}
	//printf("\n suny_delta = %f", suny_delta*suny_delta);
	float sun_posy = _interface_time_scrollx + suny_delta*(suny_delta*0.65f);
	G()->SetClip(_point_o.X, 0, _screen_width, my);

	if (_sun_frame_smile > 0)
	{
		_sun_frame_smile--;
		_sun_frame_index = 17;
	}
	else
	{
		_sun_frame_index = 14;
	}
	_sunrise_mky = -sun_posy;
	if (_sunrise_mky <= 0)
	{
		_sunrise_mky = 0;
	}
	if (_fever_glow_scale > 0)
	{
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		G()->SetScale(_fever_glow_scale, _fever_glow_scale, x + k_SCREEN_WIDTH / 2, y + 170 - sun_posy);
		G()->SetRotate(_fever_glow_rotate, x + k_SCREEN_WIDTH / 2, y + 170 - sun_posy);
		GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 3, x + k_SCREEN_WIDTH / 2, y + 170 - sun_posy);
		G()->ClearRotate();
		G()->ClearScale();
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
	_sun_glow_anim.SetPos(x + k_SCREEN_WIDTH / 2, y + 170 - sun_posy);
	_sun_glow_anim1.SetPos(x + k_SCREEN_WIDTH / 2, y + 170 - sun_posy);
	_sun_glow_anim.DrawAnimObject(G());
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), _sun_frame_index, x + k_SCREEN_WIDTH / 2, y + 170 - sun_posy);
	_sun_glow_anim1.DrawAnimObject(G());
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 15, x + k_SCREEN_WIDTH / 2, y + moon_posy);
	G()->ClearClip();

}
void FDGame::PowerUpInit()
{

	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		int num = 2;
		if (i != 0)
		{
			num = 1;
		}
		_power_up_icon[i].Init(i, 0, num, 0, 0);
	}
}
void FDGame::PowerUpReset()
{
	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		_power_up_icon[i].Reset();
	}
}
void FDGame::PowerUpRender(float x, float y)
{
	/*int energy_max = _power_up_icon[POWER_UP_TYPE_CREATE_GROUP]._energy_max;
	int mh = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFrameHeight(6);
	int cliph = ((_energy_total*1.0f) / (energy_max*1.0f))*mh;
	if (cliph >= mh)
	{
		cliph = mh;
	}
	_energy_total_target = cliph;
	if (_energy_total_current < _energy_total_target)
	{
	_energy_total_current += 2;
	if (_energy_total_current >= _energy_total_target)
	{
		_energy_total_current = _energy_total_target;
	}
	}
	if (_energy_total_current > _energy_total_target)
	{
		_energy_total_current -= 2;
		if (_energy_total_current <= _energy_total_target)
		{
			_energy_total_current = _energy_total_target;
		}
	}

	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 3, x + 0, y + 0);

	int mx = 0;
	int my = 0;
	int clipx = 0;
	int clipy = 0;
	mx = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterX(65, 0);
	my = GET_SPRITE(SPRITEID_HUD_POWERUP_01)->GetFModuleCenterY(65, 0);

	G()->SetClip(clipx, clipy + my + mh / 2 - _energy_total_current, k_SCREEN_WIDTH, _energy_total_current);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 6, x + mx, y + my);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	G()->ClearClip();
	float dis1 = CMath::ABSOLUTE_VALUE((mh / 2.0f) - _energy_total_current);
	float scale = 1.0f - (dis1 / (mh / 2.0f));
	if (scale <= 0)
	{
		scale = 0;
	}
	if (scale < 1.0f)
	{
		scale *= ((2.0f - scale));
	}
	if (scale >= 0.85f)
	{
		scale = 0.85f;
	}
	G()->SetScale(scale, scale, x + mx, y + my + mh / 2 - _energy_total_current);
	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 7, x + mx, y + my + mh / 2 - _energy_total_current);
	G()->ClearScale();
	int spacing = GET_SPRITE(SPRITEID_FONT_POWER_UP_02)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_POWER_UP_02)->_char_spacing = 2;
	GET_SPRITE(SPRITEID_FONT_POWER_UP_02)->DrawAnsiTextFormat(G(), mx + x + 10, my + y, _HCENTER | _VCENTER, "%d :", _energy_total);
	GET_SPRITE(SPRITEID_FONT_POWER_UP_02)->_char_spacing = spacing;
	GET_SPRITE(SPRITEID_HUD_POWERUP_01)->DrawFrame(G(), 65, x + 0, y + 0);*/
	///*if (_energy_total_current >= mh)
	//{
	//	if (!_just_full)
	//	{
	//		_just_full = true;
	//		_effect_explo.Active(0, mx, my, 7.0f, 0.04f, 30, 54, 1, 10);
	//		_anim_energy_full.SetAnim(24, false);

	//	}
	//}*/
	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		_power_up_icon[i].Render(x, y);
	}
}
void FDGame::PowerUpUpdate()
{
	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		_power_up_icon[i].Update();
	}
}
void FDGame::PowerUpUpdateTouch()
{
	int index_active = -1;
	for (int i = 0; i < POWER_UP_NUM_MAX; i++)
	{
		if (_power_up_icon[i].UpdateTouch())
		{
			/*if (!_power_up_icon[i]._has_actived)
			{
				_power_up_icon[i].ActivedFunc(true);
				_power_up_icon[i].ActiveScale(1.0f, 1.2f, 0.8f, 0.1f, 4);
			}*/
			index_active = i;
			break;
		}
	}

	if (index_active != -1)
	{
		//printf("\n index_active = %d", index_active);
		for (int i = 0; i < POWER_UP_NUM_MAX; i++)
		{
			if (_power_up_icon[i]._has_actived && i != index_active)
			{
				_power_up_icon[i].ActivedFunc(true);
				_power_up_icon[i].ActiveScale(1.0f, 1.1f, 1.0f, 0.1f, 4);
			}
		}
	}
}

void FDGame::EatFlowerType(int type)
{
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].GetType() == type)
			{
				EffectExplosionAdd(type, _character_arr[i][j].GetPosX(), _character_arr[i][j].GetPosY(), 7.0f, 0.04f, 5, type * 3, 3, 15, CHARACTER_SIZE_BIG + 1);
				_character_arr[i][j].Fire(false);

			}
		}
	}
	ResetFlowerHint();
	/*FindFlowerHint();
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].GetState() == CHARACTER_STATE_WAITING)
			{
				_character_arr[i][j].SetState(CHARACTER_STATE_NORMAL);
			}
		}
	}*/
}
void FDGame::SortFlower()
{
	int arr_group[FD_FLOWER_NUM_TYPE];
	int arr_group_num[FD_FLOWER_NUM_TYPE];
	int num_group = 0;
	int count = 0;
	while (count < FD_FLOWER_NUM_TYPE)
	{
		arr_group[count] = -1;
		arr_group_num[count] = 0;
		bool has_group = false;
		for (int i = 0; i < FD_NUM_ROW; i++)
		{
			for (int j = 0; j < FD_NUM_COL; j++)
			{
				if (_character_arr[i][j].GetType() == count)
				{
					arr_group[count] = count;
					has_group = true;
					break;
				}
			}
			if (has_group)
			{
				break;
			}
		}
		arr_group_num[count] = GetNumFlowerType(arr_group[count]);
		count++;
	}
	num_group = count;
	int num = 0;
	int last_modifyr = 0;
	int last_modifyc = 0;
	int num_type = 0;
	int dir = 0;
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			//_character_arr[i][j].SetType(CHARACTER_TYPE_NULL);
			_character_arr[i][j].SetState(CHARACTER_STATE_WAITING);
		}
	}
	//for (int i = 0; i < num_group;i++)
	//{
	//	int type = arr_group[i];
	//	type = i;
	//	num_type = arr_group_num[i];
	//	if (i == 0)
	//	{
	//		last_modifyr = 0;
	//		last_modifyc = 0;
	//		_character_arr[last_modifyr][last_modifyc].Modify(type);
	//		_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
	//		dir = POWER_UP_SORT_LEFT_TOP;
	//	}
	//	else if (i == 1)
	//	{
	//		last_modifyr = 0;
	//		last_modifyc = FD_NUM_COL - 1;
	//		_character_arr[last_modifyr][last_modifyc].Modify(type);
	//		_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
	//		dir = POWER_UP_SORT_RIGHT_TOP;
	//	}
	//	else if (i == 2)
	//	{
	//		last_modifyr = FD_NUM_ROW - 1;
	//		last_modifyc = FD_NUM_COL - 1;
	//		_character_arr[last_modifyr][last_modifyc].Modify(type);
	//		_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
	//		dir = POWER_UP_SORT_RIGHT_BOT;
	//	}
	//	else if (i == 3)
	//	{
	//		last_modifyr = FD_NUM_ROW - 1;
	//		last_modifyc = 0;
	//		_character_arr[last_modifyr][last_modifyc].Modify(type);
	//		_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
	//		dir = POWER_UP_SORT_LEFT_BOT;
	//	}
	//	else
	//	{
	//		for (int r = 0; r < FD_NUM_ROW; r++)
	//		{
	//			for (int c = 0; c < FD_NUM_COL; c++)
	//			{
	//				if (_character_arr[r][c].GetState() == CHARACTER_STATE_WAITING)
	//				{
	//					last_modifyr = r;
	//					last_modifyc = c;
	//					_character_arr[last_modifyr][last_modifyc].Modify(type);
	//					_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
	//					break;
	//				}
	//			}
	//		}
	//		dir = POWER_UP_SORT_FREE;
	//	}
	//	num = 0;
	//	while (num <= num_type)
	//	{
	//		int newr = -1;
	//		int newc = -1;
	//		GetArrMove(dir,last_modifyr, last_modifyc, newr, newc);
	//		last_modifyr = newr;
	//		last_modifyc = newc;
	//		//printf("\n dir= %d last_modifyr = %d,last_modifyc=%d",dir, last_modifyr, last_modifyc);
	//		_character_arr[last_modifyr][last_modifyc].Modify(type);
	//		_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
	//		num++;
	//	}
	//}
	for (int i = 0; i < num_group; i++)
	{
		int type = arr_group[i];
		if (type == -1)
		{
			continue;
		}
		num_type = arr_group_num[i];
		bool has_null = false;
		for (int r = 0; r < FD_NUM_ROW; r++)
		{
			for (int c = 0; c < FD_NUM_COL; c++)
			{
				if (_character_arr[r][c].GetState() == CHARACTER_STATE_WAITING)
					//if (_character_arr[r][c].GetType() == CHARACTER_TYPE_NULL)
				{
					last_modifyr = r;
					last_modifyc = c;
					_character_arr[last_modifyr][last_modifyc].Modify(type);
					num_type--;
					_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
					has_null = true;
					break;
				}
			}
			if (has_null)
			{
				break;
			}
		}
		//printf("\n 0000 modifyr = %d,modifyc=%d,type=%d,num_type = %d", last_modifyr, last_modifyc, type, num_type);
		dir = POWER_UP_SORT_FREE;
		num = 0;
		while (num < num_type)
		{
			int newr = -1;
			int newc = -1;
			//printf("\n 000 modifyr = %d,modifyc=%d,type=%d", last_modifyr, last_modifyc, type);
			GetArrMove(dir, last_modifyr, last_modifyc, newr, newc);

			//printf("\n modifyr = %d,modifyc=%d,type=%d", last_modifyr, last_modifyc, type);
			if (newr != -1 && newr != -1)
			{
				last_modifyr = newr;
				last_modifyc = newc;
				_character_arr[last_modifyr][last_modifyc].Modify(type);
				_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);

			}
			num++;
			/*if (num < num_type && (newc1 != -1 || newr1 != -1))
			{
				_character_arr[last_modifyr][last_modifyc].Modify(type);
				_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
				last_modifyr = newr1;
				last_modifyc = newc1;
				num++;
			}*/
		}
	}
	for (int r = 0; r < FD_NUM_ROW; r++)
	{
		for (int c = 0; c < FD_NUM_COL; c++)
		{
			if (_character_arr[r][c].GetState() == CHARACTER_STATE_WAITING)
			{
				last_modifyr = r;
				last_modifyc = c;
				_character_arr[last_modifyr][last_modifyc].Modify(arr_group[num_group - 1]);
				num_type--;
				_character_arr[last_modifyr][last_modifyc].SetState(CHARACTER_STATE_NORMAL);
			}
		}
	}
	FindAndSetFlowerGroup();
}
void FDGame::GetArrMove(int dir, int r, int c, int &or1, int &oc1)
{
	bool arr_temp[4] = { false,false,false,false };
	int type = _character_arr[r][c].GetType();
	if (r - 1 >= 0)
	{
		if (_character_arr[r - 1][c].GetState() == CHARACTER_STATE_WAITING)
		{
			arr_temp[0] = true;//top
		}
	}
	if (c + 1 < FD_NUM_COL)
	{
		if (_character_arr[r][c + 1].GetState() == CHARACTER_STATE_WAITING)
		{
			arr_temp[1] = true;//right
		}
	}
	if (r + 1 < FD_NUM_ROW)
	{
		if (_character_arr[r + 1][c].GetState() == CHARACTER_STATE_WAITING)
		{
			arr_temp[2] = true;//bot
		}
	}
	if (c - 1 >= 0)
	{
		if (_character_arr[r][c - 1].GetState() == CHARACTER_STATE_WAITING)
		{
			arr_temp[3] = true;//left
		}
	}
	switch (dir)
	{
	case POWER_UP_SORT_LEFT_BOT://left-bot
	{
		if (arr_temp[1])
		{
			or1 = r;
			oc1 = c + 1;
			for (int i = r; i < FD_NUM_COL + 1; i++)
			{
				int type2 = _character_arr[i + 1][c + 1].GetState();
				//printf("\n rrrr type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i - 1][c].GetType() == type*/)
				{
					or1 = i + 1;
					oc1 = c + 1;
				}
			}
		}
		int rand = CMath::RANDOM(0, 1);
		bool change = true;
		if (or1 != -1 && oc1 != -1)
		{
			change = false;
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[0] && change)
		{
			or1 = r - 1;
			oc1 = c;
		}
		rand = CMath::RANDOM(0, 1);
		change = true;
		if (or1 != -1 && oc1 != -1)
		{
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[3])
		{
			or1 = r;
			oc1 = c - 1;
			for (int i = r; i < FD_NUM_COL - 1; i++)
			{
				int type2 = _character_arr[i + 1][c - 1].GetState();
				//printf("\n llll type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i- 1][c].GetType() == type*/)
				{
					or1 = i + 1;
					oc1 = c - 1;
				}
			}
		}
	}
	break;
	case POWER_UP_SORT_LEFT_TOP://left-top
	{
		if (arr_temp[1])
		{
			or1 = r;
			oc1 = c + 1;
			for (int i = r; i > 0; i--)
			{
				int type2 = _character_arr[i - 1][c + 1].GetState();
				//printf("\n rrrr type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i - 1][c].GetType() == type*/)
				{
					or1 = i - 1;
					oc1 = c + 1;
					arr_temp[0] = true;
				}
			}
		}
		int rand = CMath::RANDOM(0, 1);
		bool change = true;
		if (or1 != -1 && oc1 != -1)
		{
			change = false;
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[2] && change)
		{
			or1 = r + 1;
			oc1 = c;
		}
		rand = CMath::RANDOM(0, 1);
		change = true;
		if (or1 != -1 && oc1 != -1)
		{
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[3] /*&& change*/)
		{
			or1 = r;
			oc1 = c - 1;
			for (int i = r; i > 0; i--)
			{
				int type2 = _character_arr[i - 1][c - 1].GetState();
				//printf("\n llll type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i- 1][c].GetType() == type*/)
				{
					or1 = i - 1;
					oc1 = c - 1;
				}
			}
		}
	}
	break;
	case POWER_UP_SORT_RIGHT_TOP://right-top
	{
		if (arr_temp[3])
		{
			or1 = r;
			oc1 = c - 1;
			for (int i = r; i > 0; i--)
			{
				int type2 = _character_arr[i - 1][c - 1].GetState();
				//printf("\n rrrr type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i - 1][c].GetType() == type*/)
				{
					or1 = i - 1;
					oc1 = c - 1;
				}
			}
		}
		int rand = CMath::RANDOM(0, 1);
		bool change = true;
		if (or1 != -1 && oc1 != -1)
		{
			change = false;
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[2] && change)
		{
			or1 = r + 1;
			oc1 = c;
		}
		rand = CMath::RANDOM(0, 1);
		change = true;
		if (or1 != -1 && oc1 != -1)
		{
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[1])
		{
			or1 = r;
			oc1 = c + 1;
			for (int i = r; i > 0; i--)
			{
				int type2 = _character_arr[i + 1][c - 1].GetState();
				//printf("\n llll type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i- 1][c].GetType() == type*/)
				{
					or1 = i - 1;
					oc1 = c + 1;
				}
			}
		}
	}
	break;
	case POWER_UP_SORT_RIGHT_BOT://right-bot
	{
		if (arr_temp[3])
		{
			or1 = r;
			oc1 = c - 1;
			for (int i = r; i < FD_NUM_COL + 1; i++)
			{
				int type2 = _character_arr[i + 1][c - 1].GetState();
				//printf("\n rrrr type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i - 1][c].GetType() == type*/)
				{
					or1 = i + 1;
					oc1 = c - 1;
				}
			}
		}
		int rand = CMath::RANDOM(0, 1);
		bool change = true;
		if (or1 != -1 && oc1 != -1)
		{
			change = false;
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[2] && change)
		{
			or1 = r - 1;
			oc1 = c;
		}
		rand = CMath::RANDOM(0, 1);
		change = true;
		if (or1 != -1 && oc1 != -1)
		{
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[1])
		{
			or1 = r;
			oc1 = c + 1;
			for (int i = r; i < FD_NUM_COL - 1; i++)
			{
				int type2 = _character_arr[i + 1][c + 1].GetState();
				//printf("\n llll type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING /*&& _character_arr[i- 1][c].GetType() == type*/)
				{
					or1 = i + 1;
					oc1 = c + 1;
				}
			}
		}
	}
	break;
	case POWER_UP_SORT_FREE:
	{
		/*if (arr_temp[0])
		{
			or = r - 1;
			oc = c;
		}*/
		if (r == -1 || c == -1)
		{
			printf("\n drag");
		}
		if (arr_temp[1])
		{
			or1 = r;
			oc1 = c + 1;
			bool cache = false;
			for (int i = r; i > 0; i--)
			{
				int type2 = _character_arr[i - 1][c + 1].GetState();
				//printf("\n rrrr type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING && _character_arr[i - 1][c].GetType() == type)
				{
					or1 = i - 1;
					oc1 = c + 1;
					arr_temp[0] = true;
				}
			}
		}
		int rand = CMath::RANDOM(0, 1);
		bool change = true;
		if (or1 != -1 && oc1 != -1)
		{
			change = false;
			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[2] && change)
		{
			or1 = r + 1;
			oc1 = c;
			if (r + 1 == FD_NUM_COL - 1)
			{
				bool cache = false;
				for (int i = c; i > 0; i--)
				{
					int type2 = _character_arr[r + 1][i - 1].GetState();
					//printf("\n bbbb type2 = %d", type2);
					if (type2 == CHARACTER_STATE_WAITING && _character_arr[r][i - 1].GetType() == type)
					{
						or1 = r + 1;
						oc1 = i - 1;

					}
				}
			}

		}
		rand = CMath::RANDOM(0, 1);
		change = true;
		if (or1 != -1 && oc1 != -1)
		{
			/*if (arr_temp[0])
			{
				change = false;
			}*/

			if (rand == 0)
			{
				change = true;
			}
		}
		if (arr_temp[3] /*&& change*/)
		{
			or1 = r;
			oc1 = c - 1;
			bool cache = false;
			for (int i = r; i > 0; i--)
			{
				int type2 = _character_arr[i - 1][c - 1].GetState();
				//printf("\n llll type2 = %d", type2);
				if (type2 == CHARACTER_STATE_WAITING && _character_arr[i - 1][c].GetType() == type)
				{
					or1 = i - 1;
					oc1 = c - 1;
				}
			}
		}
	}
	break;
	default:
		break;
	}

}

int FDGame::GetNumFlowerType(int type)
{
	int num = 0;
	for (int i = 0; i < FD_NUM_ROW; i++)
	{
		for (int j = 0; j < FD_NUM_COL; j++)
		{
			if (_character_arr[i][j].GetType() == type)
			{
				num++;
			}
		}
	}
	return num;
}
void FDGame::IngameMenuInit()
{
	_ingame_menu_scroll_state = 0;
	_ingame_menu_scrolly = _screen_height / 2;
	_ingame_menu_scroll_speed = 70;
	_ingame_menu_active = false;
	_ingame_menu_fill_opac = 0;
	_ingame_menu_isclose = false;
	_ingame_menu_retry = false;
	_ingame_menu_to_result = false;
	for (int i = 0; i < INGAME_MENU_NUM_BUTTON; i++)
	{
		int anim_start = 4;
		/*if (i >= INGAME_MENU_BUTTON_QUIT)
		{
			anim_start = 12;
		}*/
		_ingame_menu_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
		_ingame_menu_button[i].SetAnim(SPRITEID_HUD_PAUSE_01, anim_start + i * 4);
		_ingame_menu_button[i].SetPos(0, 0);
	}
	/*if (!_user._sound_enable)
	{
		_ingame_menu_button[INGAME_MENU_BUTTON_SOUND].SetAnim(SPRITEID_HUD_PAUSE_01, 44);
		_ingame_menu_button[INGAME_MENU_BUTTON_SOUND].SetPos(0, 0);
	}
	if (!_user._music_enable)
	{
		_ingame_menu_button[INGAME_MENU_BUTTON_MUSIC].SetAnim(SPRITEID_HUD_PAUSE_01, 40);
		_ingame_menu_button[INGAME_MENU_BUTTON_MUSIC].SetPos(0, 0);
	}*/
}
void FDGame::IngameRender(float x, float y, int opac)
{
	if (!_ingame_menu_active)
	{
		return;
	}
	G()->SetOpacity(_ingame_menu_fill_opac);
	G()->SetColor(0xf50f1d24);
	//G()->FillFullScreen(true);
	G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height, true);
	for (int i = 0; i < INGAME_MENU_NUM_BUTTON; i++)
	{
		_ingame_menu_button[i].SetPos(0, _ingame_menu_scrolly);
		_ingame_menu_button[i].Render(G());
		_ingame_menu_button[i].Update();

	}
	G()->SetOpacity(opac);
}
void FDGame::IngameUpdate()
{
	if (!_ingame_menu_active)
	{
		return;
	}

	if (!_ingame_menu_isclose)
	{
		if (_ingame_menu_fill_opac < 100)
		{
			_ingame_menu_fill_opac += 10;
		}
		switch (_ingame_menu_scroll_state)
		{
		case 0:
			break;
		case 1:
			if (_ingame_menu_scrolly > -120)
			{
				if (_ingame_menu_scrolly <= -20)
				{
					_ingame_menu_scroll_speed *= 0.6f;
					if (_ingame_menu_scroll_speed < 5)
					{
						_ingame_menu_scroll_speed = 5;
					}
				}
				//printf("\n 000 _ingame_menu_scroll_speed = %f", _ingame_menu_scroll_speed);
				_ingame_menu_scrolly -= _ingame_menu_scroll_speed;
				if (_ingame_menu_scrolly <= -120)
				{
					_ingame_menu_scroll_state = 2;
					_ingame_menu_scroll_speed = 20;
				}
			}
			break;
		case 2:
			if (_ingame_menu_scrolly < 0)
			{
				if (_ingame_menu_scrolly >= -60)
				{
					_ingame_menu_scroll_speed *= 0.65f;
				}
				if (_ingame_menu_scroll_speed <= 0.2f)
				{
					_ingame_menu_scroll_speed = 0.2f;
				}
				//printf("\n 111 _ingame_menu_scroll_speed = %f", _ingame_menu_scroll_speed);
				_ingame_menu_scrolly += _ingame_menu_scroll_speed;
				if (_ingame_menu_scrolly >= 0)
				{
					_ingame_menu_scroll_state = 3;
					_ingame_menu_scrolly = 0;
				}
				//_result_button_scrolly -= _result_button_scroll_speed;
			}
			break;
		case 3:

			break;
		case 4:

			break;
		case 5:
			//_result_scroll_state = 0;
			break;
		default:
			break;
		}
	}
	else
	{
		_ingame_menu_scrolly += 50;
		if (_ingame_menu_fill_opac > 0)
		{
			//printf("\n _result_fill_opac = %d", _result_fill_opac);
			_ingame_menu_fill_opac -= 10;
			if (_ingame_menu_fill_opac <= 0)
			{
				_ingame_menu_active = false;
				if (_ingame_menu_retry)
				{
					SetState(k_GS_PLAYGAME);
				}
				if (_ingame_menu_to_result)
				{
					SetState(k_GS_RESULT);
				}
			}
		}
	}
	IngameUpdateTouch();
}
bool FDGame::IngameUpdateTouch()
{
	bool press = false;
	bool ret = false;
	int index = -1;
	if (_dialogMng.isOpenDialog())
	{
		return false;
	}
	for (int i = 0; i < INGAME_MENU_NUM_BUTTON; i++)
	{
		if (_ingame_menu_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_ingame_menu_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case INGAME_MENU_BUTTON_RESUME:
				_ingame_menu_isclose = true;
				break;
			case INGAME_MENU_BUTTON_RETRY:
				_ingame_menu_retry = true;
				_ingame_menu_isclose = true;
				break;
			case INGAME_MENU_BUTTON_SOUND:
				_user._sound_enable = !_user._sound_enable;
				if (!_user._sound_enable)
				{
					_ingame_menu_button[INGAME_MENU_BUTTON_SOUND].SetAnim(SPRITEID_HUD_PAUSE_01, 44);
					_ingame_menu_button[INGAME_MENU_BUTTON_SOUND].SetPos(0, 0);
				}
				else
				{
					_ingame_menu_button[INGAME_MENU_BUTTON_SOUND].SetAnim(SPRITEID_HUD_PAUSE_01, 16);
					_ingame_menu_button[INGAME_MENU_BUTTON_SOUND].SetPos(0, 0);
				}

				break;
			case INGAME_MENU_BUTTON_MUSIC:
				_user._music_enable = !_user._music_enable;
				if (!_user._music_enable)
				{
					_ingame_menu_button[INGAME_MENU_BUTTON_MUSIC].SetAnim(SPRITEID_HUD_PAUSE_01, 40);
					_ingame_menu_button[INGAME_MENU_BUTTON_MUSIC].SetPos(0, 0);
				}
				else
				{
					_ingame_menu_button[INGAME_MENU_BUTTON_MUSIC].SetAnim(SPRITEID_HUD_PAUSE_01, 12);
					_ingame_menu_button[INGAME_MENU_BUTTON_MUSIC].SetPos(0, 0);
				}

				break;
			case INGAME_MENU_BUTTON_RATE_US:
#if defined (MAC_OS)
				GoToReviewInItune();
#endif
				break;
			case INGAME_MENU_BUTTON_FB_LIKE:
#if defined (MAC_OS)
				PineSocial::OpenURL(FACEBOOK_URL);
#endif
				break;
			case INGAME_MENU_BUTTON_TW_LIKE:
#if defined (MAC_OS)
				PineSocial::OpenURL(TWITTER_URL);
#endif
				break;
			case INGAME_MENU_BUTTON_CREDITS:
				CreditInit();
				_credit_render_active = true;
				break;
			case INGAME_MENU_BUTTON_CLOUD:
				GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_ICLOUD;
				//GAME()->ResetValueICloud();
				GAME()->CheckICloud();
				break;
			case INGAME_MENU_BUTTON_QUIT:
				_ingame_menu_isclose = true;
				_ingame_menu_to_result = true;
				//SetState(k_GS_RESULT);
				break;
			case INGAME_MENU_BUTTON_LEADERBOARD:
				GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_GC;
#if defined (MAC_OS)
				if ((GAME()->_serverThread._gcSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS || GAME()->_serverThread._gcSocial->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT) && GAME()->_serverThread._gcSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
				{
					GAME()->_serverThread.AddCommand(CServerThread::COMMAND_TYPE_SUBMIT_LEADERBOARD);
				}
				PineGameCenterController::ShowLeaderBoard("Sloomy_01");
#endif
				break;
			case INGAME_MENU_BUTTON_CONTACT:
#if defined (MAC_OS)
				char title[1024], body[1024];
				sprintf(title, "Sloomy's Support Request");
				sprintf(body, "%s", "");
				SendMailSupport(body, title);
#endif
				break;
			default:
				break;
			}
			if (_ingame_menu_button[index]._state != BUTTON_STATE_LOCK && _ingame_menu_button[index]._state != BUTTON_STATE_HIDE)
			{
				_ingame_menu_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < INGAME_MENU_NUM_BUTTON; touch++)
		{
			int x = _ingame_menu_button[touch].TZ_GetX();
			int y = _ingame_menu_button[touch].TZ_GetY();
			int w = _ingame_menu_button[touch].TZ_GetWidth();
			int h = _ingame_menu_button[touch].TZ_GetHeight();
			Button *button = &_ingame_menu_button[touch];
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

int FDGame::GetCubeAroundType(int col, int row)
{
	int type = CHARACTER_TYPE_NULL;
	int arr_type[9];
	int num = 0;
	for (int i = 0; i < 9; i++)
	{
		arr_type[i] = CHARACTER_TYPE_NULL;
	}
	int ctype = _character_arr[row][col + 1].GetType();
	int r = row;
	int c = col + 1;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	r = row + 1;
	c = col;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	r = row;
	c = col - 1;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	r = row - 1;
	c = col;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	/*r = row + 1;
	c = col + 2;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	r = row + 2;
	c = col + 2;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	r = row + 2;
	c = col + 1;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}
	r = row + 2;
	c = col;
	if (r >= 0 && r < FD_NUM_ROW && c >= 0 && c < FD_NUM_COL)
	{
		ctype = _character_arr[r][c].GetType();
		if (type != ctype)
		{
			arr_type[num] = ctype;
			num++;
		}
	}*/
	if (num > 0)
	{
		int rand = CMath::RANDOM(0, num - 1);
		type = arr_type[rand];
	}
	else
	{
		type = CMath::RANDOM(CHARACTER_TYPE_SUNFLOWER, CHARACTER_TYPE_ORCHILD);
	}
	return type;
}
void FDGame::FeverModdeInit()
{
	_combo_touch_time = FD_COMBO_TOUCH_TIME;
	_combo_num = 0;
	_fever_mode_active = false;
	_fever_mode_time = 0;
	_fever_last_index = -1;
	_fever_mode_state = 0;
	_fever_particle.Init(SPRITEID_HUD_FEVER_MODE_01, EXPLOSION_TYPE_DEFAULT);
	_fever_glow_scale = 0;
	_fever_glow_rotate = 0;
	_fever_field_opac = 0;
	_fever_field_opac_tar = 0;
	_fever_field_opac_hold = 0;
	_fever_field_opac_speed = 0;
	_fever_fireup_bot.Init(SPRITEID_HUD_FEVER_MODE_01);
	_fever_fireup_left.Init(SPRITEID_HUD_FEVER_MODE_01);
	_fever_fireup_right.Init(SPRITEID_HUD_FEVER_MODE_01);
	FeverModeAnimInit();
	SuperBlastFireInit();
}
void FDGame::FeverModeActive()
{
	_fever_mode_active = true;
	//_fever_mode_time = FD_FEVER_MODE_TIME;
	int paramtime = CGameDesign::FileGeneral_design::CSheetfever_mode::time;
	_fever_mode_time = GetDesignFeverModeTime(_fever_mode_level, paramtime) * 60;

	_fever_last_index = -1;
	_fever_mode_state = 1;
	_fever_bg_scale_speed = 0.05f;
	_fever_bg_scale = 0;
	_fever_text_scrollx = _screen_width / 2;
	_fever_text_speed = 5;
	_fever_glow_scale = 0;
	_fever_glow_rotate = 0;
	_fever_field_opac = 100;
	_fever_field_opac_tar = CMath::RANDOM(2, 20) * 5;
	_fever_field_opac_hold = CMath::RANDOM(1, 3) * 5;
	_fever_field_opac_speed = CMath::RANDOM(8, 14) / 2.0f;
	_fever_fireup_bot.Active(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT - 280, 0, -5.0f, 100, 2, 1, 6, 2, 20, k_SCREEN_WIDTH, 30);
	_fever_fireup_left.Active(-64, k_SCREEN_HEIGHT / 2, 0, -5.0f, 100, 2, 1, 6, 2, 7, 128, k_SCREEN_HEIGHT - 500);
	_fever_fireup_right.Active(k_SCREEN_WIDTH + 64, k_SCREEN_HEIGHT / 2, 0, -6.0f, 100, 2, 1, 6, 2, 7, 128, k_SCREEN_HEIGHT - 500);
	FeverModeAnimActive();
	GAME()->LoadMusic(BGM_INGAME_FEVER);
	GAME()->PlayCurrentMusic(-1);
	PLAY_SFX(SFX_ENTER_FEVERMODE);
	BoardActive();
	_fever_mode_level++;
	if (_fever_mode_level >= _game_design.General_design.Sheetfever_mode.int32Value._totalData)
	{
		_fever_mode_level = _game_design.General_design.Sheetfever_mode.int32Value._totalData - 1;
	}
}
void FDGame::FeverModeRender(float x, float y, int opac)
{
	float scalex = 1.0f;
	float scaley = 1.0f;
	switch (_fever_mode_state)
	{
	case 0:
		break;
	case 1:
		scaley = _fever_bg_scale;
		break;
	case 2:
		scaley = _fever_bg_scale;
		break;
	case 3:
		scalex = _fever_bg_scale * _fever_bg_scale;
		scaley = 1.0f;
		break;
	case 4:
		scalex = _fever_bg_scale *_fever_bg_scale *_fever_bg_scale *_fever_bg_scale *_fever_bg_scale * _fever_bg_scale* _fever_bg_scale* _fever_bg_scale;
		scaley = (2.0f - _fever_bg_scale);
		if (scaley > 1.0f)
		{
			scaley = 1.0f;
		}
		if (scaley <= 0)
		{
			scaley = 0;
		}
		break;
	case 5:
		scalex = 0;
		scaley = 0;

		break;
	default:
		break;
	}
	//printf("\n x:%f \n scalex:%f \n scaley:%f\n scalespeed:%f \n scrollspeed:%f", _fever_text_scrollx, scalex, scaley, _fever_bg_scale_speed, _fever_text_speed);
	if (_fever_mode_state > 0)
	{
		G()->SetScale(scalex, scaley, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 0, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
		G()->ClearScale();
		GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 1, x - _fever_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 2, x + _fever_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
	}

	/*_fever_fireup_bot.Render(x, y);
	_fever_fireup_left.Render(x, y);
	_fever_fireup_right.Render(x, y);*/
	_fever_particle.Render(x, y, opac);
}
void FDGame::FeverModeUpdate()
{
	_fever_fireup_bot.Update();
	_fever_fireup_left.Update();
	_fever_fireup_right.Update();
	_fever_particle.Update();
	FeverModeAnimUpdate();
	SuperBlastFireUpdate();

	if (_combo_touch_time > 0)
	{
		if (_fever_mode_state == 0 && _hurry_up_state == 0)
		{
			_combo_touch_time--;
		}
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

	}
	if (_fever_mode_active)
	{
		if (_fever_mode_time > 0)
		{
			if (_fever_mode_state == 0 && _hurry_up_state == 0)
			{
				_fever_mode_time--;
			}
			if (_fever_mode_time <= 0)
			{
				if (_game_time.GetTotalSecond() < 2)
				{
					_fever_mode_time_bonus = 120;
				}
				FeverModeDisable();
				GAME()->LoadMusic(BGM_INGAME);
				GAME()->PlayCurrentMusic(-1);
			}
		}
	}
	if (_combo_num >= _combo_num_max)
	{
		if (!_fever_mode_active)
		{
			FeverModeActive();
			//_fever_mode_time = FD_FEVER_MODE_TIME;
		}
		else
		{
			//_fever_mode_time--;
			/*if (_fever_mode_time<=0)
			{
				_fever_mode_active = false;
				_combo_num = 0;
			}*/

			if (_combo_num >= FD_BLAST_MODE_TRIGGER + _combo_num_max)
			{
				SuperBlastActive();
			}
		}
	}
	switch (_fever_mode_state)
	{
	case 0:
		break;
	case 1:
		_fever_text_scrollx -= _fever_text_speed;
		_fever_text_speed *= 1.2f;
		if (_fever_text_scrollx <= -30)
		{
			_fever_text_scrollx = -30;
			_fever_mode_state = 2;
			_fever_text_speed = 2;
			_fever_particle.Active(0, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, 15.0f, 0.05f, 40, 5, 4, 5);
		}
		_fever_bg_scale += _fever_bg_scale_speed;
		if (_fever_bg_scale >= 1.0f)
		{
			_fever_bg_scale = 1.0f;
		}
		_fever_bg_scale_speed *= 1.1f;
		break;
	case 2:
		_fever_text_scrollx += _fever_text_speed;
		_fever_text_speed *= 1.75f;
		if (_fever_text_scrollx >= 0)
		{
			_fever_text_scrollx = 0;
			_fever_mode_state = 3;
			_fever_bg_scale_speed = 0.02f;
		}
		if (_fever_bg_scale >= 1.0f)
		{
			_fever_bg_scale = 1.0f;
		}
		break;
	case 3:
		_fever_bg_scale_speed *= 1.05f;
		if (_fever_bg_scale_speed >= 0.05f)
		{
			_fever_mode_state = 4;
			_fever_text_speed = 0.1f;
			_fever_text_scrollx = 0;
		}
		_screen_shake_x = CMath::RANDOM(-5, 5);
		_screen_shake_y = CMath::RANDOM(-5, 5);
		break;
	case 4:
		_fever_text_scrollx += _fever_text_speed;
		_fever_text_speed *= 2.0f;
		if (_fever_text_scrollx >= _screen_width / 2)
		{
			//_fever_text_scrollx = 0;
			_fever_mode_state = 5;
		}
		_fever_bg_scale += _fever_bg_scale_speed;
		_fever_bg_scale_speed *= 1.075f;
		break;
	case 5:
		_fever_mode_state = 0;
		break;
	default:
		break;
	}
	if (!_fever_mode_active)
	{
		if (_fever_field_opac > 0)
		{
			_fever_field_opac -= 10;
		}
		if (_fever_glow_scale > 0)
		{
			_fever_glow_scale -= 0.1f;
			if (_fever_glow_scale <= 0)
			{
				_fever_glow_scale = 0;
			}
		}
		return;
	}

	/*if (_fever_field_opac < _fever_field_opac_tar)
	{
		_fever_field_opac += _fever_field_opac_speed;
		if (_fever_field_opac >= _fever_field_opac_tar)
		{
			_fever_field_opac = _fever_field_opac_tar;
			_fever_field_opac_hold = CMath::RANDOM(1, 3) * 5;
		}
	}
	else if (_fever_field_opac > _fever_field_opac_tar)
	{
		_fever_field_opac -= _fever_field_opac_speed;
		if (_fever_field_opac <= _fever_field_opac_tar)
		{
			_fever_field_opac = _fever_field_opac_tar;
			_fever_field_opac_hold = CMath::RANDOM(1, 3) * 5;
		}
	}
	else
	{
		_fever_field_opac_hold--;
		if (_fever_field_opac_hold <= 0)
		{
			_fever_field_opac_hold = 0;
			_fever_field_opac_tar = CMath::RANDOM(2, 20) * 5;
			_fever_field_opac_speed = CMath::RANDOM(8, 14) / 2.0f;
		}
	}*/
	if (_fever_glow_scale < 1.0f)
	{
		_fever_glow_scale += 0.07f;
		if (_fever_glow_scale >= 1.0f)
		{
			_fever_glow_scale = 1.0f;
		}
	}
	_fever_glow_rotate++;
	if (_fever_glow_rotate >= 360)
	{
		_fever_glow_rotate -= 360;
	}
}
void FDGame::SuperBlastActive()
{
	int row = _fever_last_index / FD_NUM_ROW;
	int col = _fever_last_index%FD_NUM_COL;
	for (int i = row - 2; i < row + 3; i++)
	{
		if (i < 0 || i >= FD_NUM_ROW)
		{
			continue;
		}
		for (int j = col - 2; j < col + 3; j++)
		{
			if (j < 0 || j >= FD_NUM_COL)
			{
				continue;
			}
			int index = i*FD_NUM_COL + j;
			//_character_arr[i][j].Fire();
			int time_hold = CMath::DISTANCE(_character_arr[row][col].GetPosX(), _character_arr[row][col].GetPosY(), _character_arr[i][j].GetPosX(), _character_arr[i][j].GetPosY()) / FD_SLOT_WIDTH;
			_character_arr[i][j]._disable_hold_time = time_hold * 5;
			SuperBlastFireAdd(index, time_hold * 12);
		}
	}
	_game_score += 500;
	EffectPointAdd(EFFECT_POINT_TYPE_BIG, 0, _character_arr[row][col].GetPosX(), _character_arr[row][col].GetPosY(), 500, 1.2f);
	EffectPointAdd(EFFECT_POINT_TYPE_SUPER_BLAST, 0, k_SCREEN_WIDTH / 2, 232, 0, 1.0f);
	FeverModeDisable();
	GAME()->LoadMusic(BGM_INGAME);
	GAME()->PlayCurrentMusic(-1);
	if (_game_time.GetTotalSecond() < 2)
	{
		_fever_mode_time_bonus = 120;
	}
	_fever_field_opac = 700;
	PLAY_SFX(SFX_SUPER_BLASH);

}
void FDGame::FeverModeDisable()
{
	if (_combo_num <= 0)
	{
		return;
	}
	_fever_fireup_bot.ForceEnd();
	_fever_fireup_left.ForceEnd();
	_fever_fireup_right.ForceEnd();
	_fever_mode_active = false;
	_combo_num = 0;
	FeverModeAnimForceEnd();
	BoardForceEnd();


	int paramcombo = CGameDesign::FileGeneral_design::CSheetfever_mode::combo;
	_combo_num_max = GetDesignFeverModeTime(_fever_mode_level, paramcombo);
}
void FDGame::FlowerCollectedAdd(int num)
{
	if (_scoreX_level >= _game_design.General_design.SheetScore.int32Value._totalData)
	{
		return;
	}
	_num_flower_collected += num;
	int param_numof = CGameDesign::FileGeneral_design::CSheetScore::number_of_flowers;
	int numofflower = GetDesignScoreMutiply(_scoreX_level - 1, param_numof);
	while (_num_flower_collected >= numofflower && _scoreX_level <= _game_design.General_design.SheetScore.int32Value._totalData)
	{
		_scoreX_level++;
		FarmerActive();
		_num_flower_collected -= numofflower;
		numofflower = GetDesignScoreMutiply(_scoreX_level - 1, param_numof);
	}
}
void FDGame::FlowerCollectedInit()
{
	_scoreX_level = 1;
	_num_flower_collected = 0;
}

void FDGame::FarmerInit()
{
	_farmer_scale = 1.0f;
	_farmer_text_scale = 1.0f;
	_farmer_level_up_state = 0;
	_farmer_render_level = _scoreX_level + 1;
	for (int i = 0; i < FD_FARMER_NUM; i++)
	{
		_farmer_render[i].Init(i);
	}
	_farmer_particle.Init(SPRITEID_HUD_FEVER_MODE_01, EXPLOSION_TYPE_DEFAULT);
}
void FDGame::FarmerRender(float x, float y, int opac)
{
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 42, x, y);
	int mkx = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterX(46, 0);
	int mky = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleY(46, 0) + GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFrameHeight(46);
	G()->SetScale(_farmer_scale, _farmer_scale, mkx, mky); 
	GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->DrawFrame(G(), 46, x, y); 
	mkx = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterX(46, 0);
	mky = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterY(46, 0); 
	float scale_text = _farmer_text_scale;
	if (_farmer_level_up_state <= 3)
	{
		scale_text = 1.0f;
	}

	if (scale_text > 1.0f)
	{
		//G()->SetScale(scale_text, scale_text, mkx, mky);
	}
	int char_space = GET_SPRITE(SPRITEID_FONT_MULTIPLY_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_MULTIPLY_01)->_char_spacing = 2;
	GET_SPRITE(SPRITEID_FONT_MULTIPLY_01)->DrawAnsiTextFormat(G(), x + mkx, y + mky, _HCENTER | _VCENTER, ":%d", _farmer_render_level); 
	GET_SPRITE(SPRITEID_FONT_MULTIPLY_01)->_char_spacing = char_space;
	G()->ClearScale(); 
	_farmer_particle.Render(x, y, opac); 
	for (int i = 0; i < FD_FARMER_NUM; i++)
	{
		_farmer_render[i].Render(x, y);
	}
}
void FDGame::FarmerUpdate()
{
	_farmer_particle.Update();
	switch (_farmer_level_up_state)
	{
	case 0:
		_farmer_scale = 1.0f;
		_farmer_text_scale = 1.0f;

		break;
	case 1:
		if (_farmer_scale < 1.5f)
		{
			_farmer_scale += 0.08f;
			if (_farmer_scale >= 1.5f)
			{
				_farmer_level_up_state = 2;
			}
		}
		break;
	case 2:
		if (_farmer_scale > 1.0f)
		{
			_farmer_scale -= 0.08f;
			if (_farmer_scale <= 1.0f)
			{
				_farmer_scale = 1.0f;
				_farmer_level_up_state = 3;
				_farmer_text_scale = 1.1f;
			}
		}
		break;
	case 3:
		_farmer_text_scale -= 0.02f;
		if (_farmer_text_scale <= 1.0f)
		{
			_farmer_level_up_state = 4;
			_farmer_text_scale = 1.0f;
		}
		break;
	case 4:
		if (_farmer_text_scale < 1.5f)
		{
			_farmer_text_scale += 0.05f;
			if (_farmer_text_scale >= 1.5f)
			{
				_farmer_level_up_state = 5;
			}
		}
		break;
	case 5:
		if (_farmer_text_scale > 1.0f)
		{
			_farmer_text_scale -= 0.05f;
			if (_farmer_text_scale <= 1.0f)
			{
				_farmer_text_scale = 1.0f;
				_farmer_level_up_state = 6;
			}
		}
		break;
	case 6:
		_farmer_level_up_state = 0;
		break;
	default:
		break;
	}
	for (int i = 0; i < FD_FARMER_NUM; i++)
	{
		_farmer_render[i].Update(); 
		if (_farmer_render[i]._state == 2)
		{
			if (_farmer_render[i]._x >= _screen_width + 150)
			{
				if (_farmer_render_level < _scoreX_level + 1)
				{
					_farmer_level_up_state = 1;
					_farmer_render_level = _scoreX_level + 1;
					int mkx = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterX(46, 0);
					int mky = GET_SPRITE(SPRITEID_HUD_BACKGROUND_01)->GetFModuleCenterY(46, 0);
					PLAY_SFX(SFX_SCORE_MULTIPLIER_UP);
					//_farmer_particle.Active(0, mkx, mky, 15.0f, 0.05f, 40, 4, 5, 5);
				}
			}
		}
	}
}
void FDGame::FarmerActive()
{
	for (int i = 0; i < FD_FARMER_NUM; i++)
	{
		if (_farmer_render[i]._id < _scoreX_level - 1)
		{
			_farmer_render[i].ForceEnd();
		}
	}
	for (int i = 0; i < FD_FARMER_NUM; i++)
	{
		if (_farmer_render[i]._state == 0)
		{
			_farmer_render[i].Active(_scoreX_level - 1);
			//printf("\n id[%d] = %d", i,_scoreX_level - 1);
			break;
		}
	}
}

void FDGame::FeverModeAnimInit()
{
	for (int i = 0; i < FEVER_MODE_NUM_ANIM; i++)
	{
		int mkx = GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->GetFModuleCenterX(31, i);
		int mky = GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->GetFModuleCenterY(31, i);
		_fever_mode_anim1[i].SetAnimSprite(GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01), SPRITEID_HUD_FEVER_MODE_01);
		_fever_mode_anim1[i].SetAnim(0, false);
		_fever_mode_anim1[i].SetPos(mkx, mky);
		mkx = GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->GetFModuleCenterX(32, i);
		mky = GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->GetFModuleCenterY(32, i);
		_fever_mode_anim2[i].SetAnimSprite(GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01), SPRITEID_HUD_FEVER_MODE_01);
		_fever_mode_anim2[i].SetAnim(3, true);
		_fever_mode_anim2[i].SetPos(mkx, mky);
	}
	_fever_mode_anim_state = 0;
}
void FDGame::FeverModeAnimRender(float x, float y)
{
	if (_fever_mode_anim_state == 0)
	{
		return;
	}
	for (int i = 0; i < FEVER_MODE_NUM_ANIM; i++)
	{
		_fever_mode_anim1[i].DrawAnimObject(G());
		_fever_mode_anim2[i].DrawAnimObject(G());
	}
}
void FDGame::FeverModeAnimUpdate()
{
	if (_fever_mode_anim_state == 0)
	{
		return;
	}
	for (int i = 0; i < FEVER_MODE_NUM_ANIM; i++)
	{
		_fever_mode_anim1[i].UpdateAnimObject();
		_fever_mode_anim2[i].UpdateAnimObject();
		if (_fever_mode_anim1[i].IsCurrentAnimStop())
		{
			if (_fever_mode_anim1[i].GetCurrentAnim() == 0)
			{
				_fever_mode_anim1[i].SetAnim(1, true);
			}
			else if (_fever_mode_anim1[i].GetCurrentAnim() == 2)
			{
				_fever_mode_anim_state = 0;
			}
		}
	}
}
void FDGame::FeverModeAnimForceEnd()
{
	if (_fever_mode_anim_state == 0 || _fever_mode_anim_state == 2)
	{
		return;
	}
	for (int i = 0; i < FEVER_MODE_NUM_ANIM; i++)
	{
		_fever_mode_anim1[i].SetAnim(2, false);
	}
	_fever_mode_anim_state = 2;
}

void FDGame::FeverModeAnimActive()
{
	_fever_mode_anim_state = 1;
	for (int i = 0; i < FEVER_MODE_NUM_ANIM; i++)
	{
		_fever_mode_anim2[i].SetAnim(3, true);
		_fever_mode_anim1[i].SetAnim(0, false);
	}
}

void FDGame::CloudInit()
{
	_effect_cloud_frame_add = CMath::RANDOM(0, 20);
	for (int i = 0; i < EFFECT_CLOUD_NUM_MAX; i++)
	{
		_effect_cloud[i].Init();
	}
}
void FDGame::CloudAddNew(int frameid, float x, float y, float tarx, float tary, float speed)
{
	for (int i = 0; i < EFFECT_CLOUD_NUM_MAX; i++)
	{
		if (_effect_cloud[i]._state == 0)
		{
			_effect_cloud[i].Add(frameid, x, y, tarx, tary, speed);
			break;
		}

	}
}
void FDGame::CloudRender(float x, float y, int layer)
{
	for (int i = 0; i < EFFECT_CLOUD_NUM_MAX; i++)
	{
		if (_effect_cloud[i]._layer == layer)
		{
			_effect_cloud[i].Render(x, y);
		}
	}
}
void FDGame::CloudUpdate()
{
	for (int i = 0; i < EFFECT_CLOUD_NUM_MAX; i++)
	{
		_effect_cloud[i].Update();
	}
}

void FDGame::HurryUpInit()
{
	_hurry_up_active = false;
	_hurry_up_state = 0;
	_hurry_up_bg_scale = 0;
	_hurry_up_bg_scale_speed = 0;
	_hurry_up_text_scrollx = 0;
	_hurry_up_text_speed = 0;
	_hurry_up_time = 0;
	_hurry_up_anim.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01), SPRITEID_HUD_FEVER_MODE_01);
	_hurry_up_anim.SetAnim(4, false);
	_hurry_up_anim.SetPos(0, k_SCREEN_HEIGHT / 2);
	_hurry_up_type = HURRY_UP_TYPE_NORMAL;
}
void FDGame::HurryUpActive(int type)
{
	_hurry_up_active = true;
	_hurry_up_state = 1;
	_hurry_up_bg_scale = 0;
	_hurry_up_bg_scale_speed = 0.05f;
	_hurry_up_text_scrollx = _screen_width / 2;
	_hurry_up_text_speed = 5;
	_hurry_up_time = 0;
	_hurry_up_anim.SetAnim(4, false);
	_hurry_up_type = type;
	if (_hurry_up_type == HURRY_UP_TYPE_NORMAL)
	{
		PLAY_SFX(SFX_HURRY_UP);
	}
	else
	{
		PLAY_SFX(SFX_TIMEOUT);
	}
}
void FDGame::HurryUpRender(float x, float y, int opac)
{
	float scalex = 1.0f;
	float scaley = 1.0f;
	switch (_hurry_up_state)
	{
	case 0:
		break;
	case 1:
		scaley = _hurry_up_bg_scale;
		break;
	case 2:
		scaley = _hurry_up_bg_scale;
		break;
	case 3:
		scalex = _hurry_up_bg_scale * _hurry_up_bg_scale;
		scaley = 1.0f;
		break;
	case 4:
		scalex = _hurry_up_bg_scale *_hurry_up_bg_scale *_hurry_up_bg_scale *_hurry_up_bg_scale *_hurry_up_bg_scale * _hurry_up_bg_scale* _hurry_up_bg_scale* _hurry_up_bg_scale;
		scaley = (2.0f - _hurry_up_bg_scale);
		if (scaley > 1.0f)
		{
			scaley = 1.0f;
		}
		if (scaley <= 0)
		{
			scaley = 0;
		}
		break;
	case 5:
		scalex = 0;
		scaley = 0;

		break;
	default:
		break;
	}
	//printf("\n x:%f \n scalex:%f \n scaley:%f\n scalespeed:%f \n scrollspeed:%f", _fever_text_scrollx, scalex, scaley, _fever_bg_scale_speed, _fever_text_speed);
	switch (_hurry_up_type)
	{
	case HURRY_UP_TYPE_NORMAL:
		if (_hurry_up_state > 0)
		{
			G()->SetScale(scalex, scaley, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 33, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			G()->ClearScale();
			GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 34, x - _hurry_up_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			//GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 2, x + _fever_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			_hurry_up_anim.SetPos(x + _hurry_up_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			_hurry_up_anim.DrawAnimObject(G());
		}
		break;
	case HURRY_UP_TYPE_TIMEUP:
		if (_hurry_up_state > 0)
		{
			G()->SetScale(scalex, scaley, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 39, x + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			G()->ClearScale();
			GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 40, x - _hurry_up_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
			GET_SPRITE(SPRITEID_HUD_FEVER_MODE_01)->DrawFrame(G(), 41, x + _hurry_up_text_scrollx + k_SCREEN_WIDTH / 2, y + k_SCREEN_HEIGHT / 2);
		}
		break;
	default:
		break;
	}

}
void FDGame::HurryUpUpdate()
{
	switch (_hurry_up_state)
	{
	case 0:
		break;
	case 1:
		_hurry_up_text_scrollx -= _hurry_up_text_speed;
		_hurry_up_text_speed *= 1.2f;
		if (_hurry_up_text_scrollx <= -30)
		{
			_hurry_up_text_scrollx = -30;
			_hurry_up_state = 2;
			_hurry_up_text_speed = 2;
			_fever_particle.Active(0, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, 15.0f, 0.05f, 40, 5, 4, 5);
		}
		_hurry_up_bg_scale += _hurry_up_bg_scale_speed;
		if (_hurry_up_bg_scale >= 1.0f)
		{
			_hurry_up_bg_scale = 1.0f;
		}
		_hurry_up_bg_scale_speed *= 1.1f;
		break;
	case 2:
		_hurry_up_text_scrollx += _hurry_up_text_speed;
		_hurry_up_text_speed *= 1.75f;
		if (_hurry_up_text_scrollx >= 0)
		{
			_hurry_up_text_scrollx = 0;
			_hurry_up_state = 3;
			_hurry_up_bg_scale_speed = 0.02f;
		}
		if (_hurry_up_bg_scale >= 1.0f)
		{
			_hurry_up_bg_scale = 1.0f;
		}
		break;
	case 3:
		if (_hurry_up_type == HURRY_UP_TYPE_NORMAL)
		{
			_hurry_up_anim.UpdateAnimObject();
			if (_hurry_up_anim.IsCurrentAnimStop())
			{
				_hurry_up_anim.SetAnim(4, true);
				_hurry_up_state = 4;
				_hurry_up_text_speed = 0.1f;
				_hurry_up_text_scrollx = 0;
			}
		}
		else
		{
			_hurry_up_bg_scale_speed *= 1.05f;
			if (_hurry_up_bg_scale_speed >= 2.0f)
			{
				_hurry_up_state = 4;
				_hurry_up_text_speed = 0.1f;
				_hurry_up_text_scrollx = 0;
			}
		}

		break;
	case 4:
		if (_hurry_up_type == HURRY_UP_TYPE_NORMAL)
		{
			_hurry_up_text_scrollx += _hurry_up_text_speed;
			_hurry_up_text_speed *= 2.0f;
			if (_hurry_up_text_scrollx >= _screen_width / 2)
			{
				//_hurry_up_text_scrollx = 0;
				_hurry_up_state = 5;
			}
			_hurry_up_bg_scale += _hurry_up_bg_scale_speed;
			_hurry_up_bg_scale_speed *= 1.075f;
		}
		break;
	case 5:
		HurryUpInit();
		//_hurry_up_state = 0;
		break;
	default:
		break;
	}
}

void FDGame::TimeUpRender(float x, float y)
{

}
void FDGame::TimeUpInit()
{

}
void FDGame::TimeUpUpdate()
{

}
void FDGame::TimeUpActive()
{

}