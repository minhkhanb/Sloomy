#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdio.h>
#include <PineEngine.h>
PE_USING_ENGINE;

extern "C" void ServerADVERTStart();
extern "C" void PurchaseProduct(const char *indentify);
extern "C" bool isChartboostAvailable();
extern "C" void DataSeverStart();



void CGame::OnScreenChange(PineSize size)
{
	///////////////////////////////////////////
	// INIT CONTEXT
	///////////////////////////////////////////

	//define support version
#if defined(_WINDOWS_PHONE8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1280_768, Orientation::Portrait },
		{ SupportVersion::RESVER_800_480, Orientation::Portrait }
	};
	int support_version_num = 2;
#elif defined(_WIN8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1366_768, Orientation::Portrait }
	};
	int support_version_num = 1;
#elif defined(MAC_OS)
	ResVersion support_version[] = {
		/*{ SupportVersion::RESVER_960_640, Orientation::Portrait },
		{ SupportVersion::RESVER_1024_768, Orientation::Portrait },
		{ SupportVersion::RESVER_1136_640, Orientation::Portrait },*/
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait }
	};
	int support_version_num = 1;
#elif defined(ANDROID_OS)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait }
	};
	int support_version_num = 1;
#else //windows
	ResVersion support_version[] = {

		{ SupportVersion::RESVER_1334_750, Orientation::Portrait },
	};
	int support_version_num = 1;
#endif
	//detect device version
	if (support_version_num > 0)
	{
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
		ResVersion curr_ver = GetAppropriateVersion(dvsize, support_version, support_version_num);
		Alignment game_align = { HorizontalAlign::Center, VerticalAlign::Middle };
		AlignmentContext(game_align, dvsize, curr_ver, true, _HCENTER);
		_current_version = VERSION_750x1334;
	}
#if defined(MAC_OS)
	{
		ResVersion device_support_version[] = {
			{ SupportVersion::RESVER_960_640, Orientation::Portrait },
			{ SupportVersion::RESVER_1024_768, Orientation::Portrait },
			{ SupportVersion::RESVER_1136_640, Orientation::Portrait },
			{ SupportVersion::RESVER_800_480, Orientation::Portrait },
			{ SupportVersion::RESVER_1280_768, Orientation::Portrait },
		};
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };

		_device._version = (GetAppropriateVersion(dvsize, device_support_version, 5)).Version;
	}
#endif
}
void CGame::InitEx()
{
	InitBuff(UNZIP_BUFFER_LENGTH_HIGH);

	_main_game = new FDGame();
	_main_game->Init();
	_main_game->SetState(k_GS_LOGO);
	_main_game->_game_id = APPLE_ID;
	_current_game = _main_game;
	FDGame* game = (FDGame*)(GAME()->_current_game);
	game->_hight_score = 0;
	_serverThread._gcSocial = NULL;
	game->_free_gift_time.setNullDay();
	game->_has_tutorial_completed = false;
	_icloud_state = 0;
#ifdef MAC_OS
	DataSeverStart();
#if ADD_ADVERT
	ServerADVERTStart();
#endif
#endif
	//_pineStore.AddNewProduct("Sloomy_NoAds");

}

void CGame::ReleaseEx()
{

}

void CGame::PauseEx()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	game->_push_notify_manager.PushNotification();
	if (game->_current_state == k_GS_PLAYGAME)
	{
		if (!game->_ingame_menu_active)
		{
			game->IngameMenuInit();
			game->_ingame_menu_active = true;
			game->_ingame_menu_scroll_state = 1;
		}

		game->_dialogMng.ForceClose();
		game->_has_tutorial_completed = true;
	}
}
void CGame::ResumeEx()
{
	FDGame *game = (FDGame*)GAME()->_current_game;

}
bool CGame::OnBackKeyPress() {
	//Implement function when 
	return false;
}
void CGame::UpdateAndRender()
{
	G()->ClearClip();
	G()->SetOpacity(100);
	if (!LoadGameForceground())
	{
		return;
	}
	_main_game->UpdateAndRender();
#if defined (ANDROID_OS)
	UpdateAds();
#endif
}



void CGame::UpdateTouch(int xx, int yy, int touch_state, int touch_id, int tap_count)
{
	/*switch (_GameState)
	{

	}*/
}



void CGame::PurchaseIAP(const char* iapID, enumIAPResponseStatus status)
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	printf("\n IAP id = %s, success = %d", iapID, status);
	if (status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS)
	{
		if (strcmp(iapID, "Sloomy_NoAds") == 0)
		{
			game->_user._has_removed_ads = true;
		}
		if (_processing_view_type == PROCESSING_VIEW_TYPE_IAP)
		{
			_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
		}
		_is_purchase_procesing = false;
	}
	else if (status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_FAIL)
	{
		if (_processing_view_type == PROCESSING_VIEW_TYPE_IAP)
		{
			_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
		}
		_is_purchase_procesing = false;
	}
	else if (status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_END_RESTORE)
	{
		if (_processing_view_type == PROCESSING_VIEW_TYPE_IAP)
		{
			_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
		}
		_is_purchase_procesing = false;
		//printf("\n end restore");
	}
	else if (status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE)
	{
		if (_processing_view_type == PROCESSING_VIEW_TYPE_IAP)
		{
			_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
		}
		game->_user._has_removed_ads = true;
		//printf("\n restore success");
		PineSocial::ShowMessageBox("Your purchases have been restore!", "Thank you for your support");
		_is_purchase_procesing = false;
	}
	else if (status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_FAIL_RESTORE)
	{
		if (_processing_view_type == PROCESSING_VIEW_TYPE_IAP)
		{
			_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
		}
		_is_purchase_procesing = false;
	}
}
void callback(ECloudKeyValueChangeReason reason, std::vector<std::string> keys)
{
	switch (reason)
	{
	case ECloudKeyValueChangeReason::AccountChange:
		PDEBUG("ICloud Change reason Account Change\n");
		GAME()->_icloud_state = 1;
		break;
	case ECloudKeyValueChangeReason::InitialSyncChange:
		PDEBUG("ICloud Change reason InitialSyncChange\n");
		GAME()->_icloud_state = 2;
		break;
	case ECloudKeyValueChangeReason::QuotaViolationChange:
		PDEBUG("ICloud Change reason QuotaViolationChange\n");
		GAME()->_icloud_state = 3;
		break;
	case ECloudKeyValueChangeReason::ServerChange:
		PDEBUG("ICloud Change reason ServerChange\n");
		GAME()->_icloud_state = 4;
		break;
	}

	for (int i = 0; i < keys.size(); i++)
	{
		PDEBUG("key change:%s\n", keys[i].c_str());
	}
}

void CGame::AutoSyncICloud()
{

#if defined(MAC_OS)
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);

	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
	}
	else {
		PDEBUG(" auto sync icloud\n");
		if (_icloud_state == 0) {
			service->synchronize();
			int server_state = CheckCanPushDataSave();
			PDEBUG(" auto icloud success server_state = %d\n", server_state);
			if (server_state == ICLOUD_STATE_READY_SYNS || server_state == ICLOUD_STATE_OLD_VERSION)
			{
				SaveToICloud();
				PDEBUG(" auto sync icloud success\n");
			}
		}
	}
#endif
}
void CGame::CheckICloud()
{
	// return;
	FDGame *game = (FDGame*)GAME()->_current_game;
#if defined(MAC_OS)
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);

	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
	}
	else {
		PDEBUG("icloud success\n");
		if (_icloud_state == 0) {
			service->synchronize();
			int server_state = CheckCanPushDataSave();
			PDEBUG("icloud success server_state = %d\n", server_state);
			if (server_state == ICLOUD_STATE_NEW_VERSION) {
				if (!game->_dialogMng.isOpenDialog())
				{
					char text[128];
					sprintf(text, "You have an saved progress on iCloud.\nDo you want to sync?");
					game->_dialogMng.Add(DIALOG_TYPE_ICLOUD_CONFIRM, DIALOGTYPE_YESNO, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
				}

			}
			else
			{
				if (SaveToICloud())
				{
					if (!game->_dialogMng.isOpenDialog())
					{
						char text[128];
						sprintf(text, "your data has been saved");
						game->_dialogMng.Add(DIALOG_TYPE_ICLOUD_SYNC_TO, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "", text, 0, 0);
					}
				}
			}
		}
	}
#endif
}

int CGame::CheckCanPushDataSave()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
#if defined(MAC_OS)
	int state = -10;
	if (!_device.IsNetworkAvailable())
	{
		state = ICLOUD_STATE_DISCONECT;
	}
	else
	{
		std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
		if (service == nullptr)
		{
			PDEBUG("icloud fail \n");
		}
		else {
			PDEBUG("icloud success\n");
			if (_icloud_state == 0) {
				service->synchronize();
				Sleep(100);
				std::vector<BYTE> data = service->readBytes("data");
				PDEBUG("icloud data.size() = %ld \n", data.size());
				if (data.size() > 0)
				{
					char buffer[SAVE_MAX_BUFFER];
					sprintf(buffer, "");
					for (int i = 0; i < SAVE_FORMAT_LENGTH; i++)
					{
						buffer[i] = data[i];
					}
					int save_version = 0;
					save_version = GAME()->GetInt32At(buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION);
					INT_TIME game_version = GAME()->GetInt64At(buffer, SAVE_FORMAT_CURRENT_VERSION);
					if (save_version < game->_user._save_cloud_version)
					{
						state = ICLOUD_STATE_OLD_VERSION;
					}
					else if (save_version == game->_user._save_cloud_version)
					{
						state = ICLOUD_STATE_READY_SYNS;
					}
					else if (save_version > game->_user._save_cloud_version)
					{
						state = ICLOUD_STATE_NEW_VERSION;
					}
				}
				else
				{
					state = ICLOUD_STATE_NODATA;
				}
			}
		}
	}
	return state;
#else
	return -10;
#endif
}

void CGame::ResetValueICloud()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
#if defined(MAC_OS)
	std::shared_ptr<Pine::Cloud::CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);

	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
	}
	else {
		PDEBUG(" reset icloud success\n");
		if (_icloud_state == 0) {
			service->synchronize();
			std::vector<BYTE> data = service->readBytes("data");
			{
				game->_user.UserDataSave();
				service->write("data", game->_user._buffer, SAVE_FORMAT_LENGTH);
				PDEBUG("\n\nicloud write data success\n\n");
			}
		}
	}
#endif
}

bool CGame::SaveToICloud()
{
	bool ret = false;
#if defined(MAC_OS)
	FDGame *game = (FDGame*)GAME()->_current_game;
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);

	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
		return ret;
	}
	else {
		PDEBUG(" save to icloud success\n");
		if (_icloud_state == 0) {
			service->synchronize();
			int server_state = CheckCanPushDataSave();
			if (server_state == ICLOUD_STATE_READY_SYNS || server_state == ICLOUD_STATE_OLD_VERSION ||
				server_state == ICLOUD_STATE_NODATA)
			{

				service->write("data", game->_user._buffer, SAVE_FORMAT_LENGTH);
				PDEBUG("\n\nicloud write data success\n\n");
				ret = true;
				GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
				std::vector<BYTE> data = service->readBytes("data");
				if (data.size() > 0) {
					char buf[SAVE_FORMAT_LENGTH];
					for (int i = 0; i < SAVE_FORMAT_LENGTH; i++)
					{
						buf[i] = data[i];
					}
					int save_version = GAME()->GetInt32At(buf, SAVE_FORMAT_SAVE_CLOUD_VERSION);
					INT_TIME score = GAME()->GetInt64At(buf, SAVE_FORMAT_GAME_HIGHT_SCORE);
					INT_TIME score_buf = GAME()->GetInt64At(game->_user._buffer, SAVE_FORMAT_GAME_HIGHT_SCORE);
					PDEBUG("\n\n sync to save_version = %d,score = %lld,score_buf=%lld", save_version, score, score_buf);
					PDEBUG("\n\n user save_version = %d, score = %lld", game->_user._save_cloud_version, game->_hight_score);
				}
			}

			/*std::vector<BYTE> bytes = { 1,2,3,4,5 };

			 if (service->write("test", (const char*)bytes.data(), bytes.size()))
			 {
			 PDEBUG("icloud write data success\n");
			 std::vector<BYTE> data = service->readBytes("test");

			 for (int i = 0; i< data.size(); i++)
			 {
			 PDEBUG("data: %d %d\n", i, data[i]);
			 }
			 }
			 else {
			 PDEBUG("icloud write data fail\n");
			 }*/
		}
	}

#endif
	return ret;
}

void CGame::DefaultICloud()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
#if defined(MAC_OS)
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);

	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
	}
	else {
		PDEBUG(" default icloud success\n");
		if (_icloud_state == 0) {
			service->synchronize();
			std::vector<BYTE> data = service->readBytes("data");
			if (data.size() > 0) {
				PDEBUG("\n\nicloud read data success data.size() = %ld\n\n", data.size());
				for (int i = 0; i < SAVE_FORMAT_LENGTH; i++)
				{
					game->_user._buffer[i] = data[i];
				}
				game->_user.LoadFromBuffer();
			}
			else
			{
				game->_user.SaveToBuffer();
				service->write("data", game->_user._buffer, SAVE_FORMAT_LENGTH);
				PDEBUG("\n\nicloud write data success\n\n");
			}
		}
	}
#endif
}

void CGame::LoadFromICloud()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
#if defined(MAC_OS)
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);

	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
	}
	else {
		PDEBUG(" load icloud success\n");
		if (_icloud_state == 0) {
			service->synchronize();
			std::vector<BYTE> data = service->readBytes("data");
			if (data.size() > 0) {
				PDEBUG("\n\nicloud read data success\n\n");
				for (int i = 0; i < SAVE_FORMAT_LENGTH; i++)
				{
					game->_user._buffer[i] = data[i];
				}
				int save_version = GAME()->GetInt32At(game->_user._buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION);
				INT_TIME score = GAME()->GetInt64At(game->_user._buffer, SAVE_FORMAT_GAME_HIGHT_SCORE);
				PDEBUG("\n\n load save_version = %d,score = %lld", save_version, score);
				GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
				game->_user.LoadFromBuffer();
				PDEBUG("\n\n user save_version = %d, score = %lld", game->_user._save_cloud_version, game->_hight_score);
			}

			/*std::vector<BYTE> bytes = { 1,2,3,4,5 };

			 if (service->write("test", (const char*)bytes.data(), bytes.size()))
			 {
			 PDEBUG("icloud write data success\n");
			 std::vector<BYTE> data = service->readBytes("test");

			 for (int i = 0; i< data.size(); i++)
			 {
			 PDEBUG("data: %d %d\n", i, data[i]);
			 }
			 }
			 else {
			 PDEBUG("icloud write data fail\n");
			 }*/
		}
	}
#endif
}
#if defined(_WINDOWS)
void CGame::ProcessCommandLine(const char* cmd)
{
	
}
#endif
void CGame::CompleteSendRequest()
{}
#if defined (ANDROID_OS)
bool CGame::InitAds()
{
	PDEBUG("INIT ADS");
	_admob_manager.setUnitId("DONT_NEED");
	_admob_manager.prepareInterstitial();
	_admob_manager.prepareBanner();
	_admob_manager.prepareRewardedVideo();
	//_unityads_manager.prepareAds();
	//_unityads_manager.prepareVideo();
	//_adsVungle_manager.prepare();
	//_unityads_manager.prepareSkippableVideo();
	_on_show_video_ads = false;
	_admob_play_count = 0;
	_time_count_for_show_ads_banner = -1;
	return true;
}
void CGame::ShowAds()
{
	PDEBUG("\n call ShowAds");
	//_on_show_video_ads = true;
	//_is_waiting_for_reward = true;
	//_on_video_ads_reward = false;
	/*if (_unityads_manager.isVideoReady())
	{
	PDEBUG("SHOW UNITY VIDEO");
	_unityads_manager.showVideo();
	_admob_play_count = 0;
	}
	else if (_adsVungle_manager.isReady())
	{
	PDEBUG("SHOW VUNGLE ADS");
	_admob_play_count = 0;
	_adsVungle_manager.show();
	}
	else */if (_admob_manager.isInterstitialReady())
	{
		PDEBUG("SHOW _admob showInterstitial");
		_admob_manager.showInterstitial();
	}
}

void CGame::ShowBannerAds()
{
	PDEBUG(" call _admob banner: %d %d", _admob_manager.isBannerReady(), _admob_manager.isShowBanner());
	if (_admob_manager.isBannerReady() && !_admob_manager.isShowBanner())
	{
		PDEBUG("SHOW _admob banner");
		_admob_manager.showBanner();
		_has_show_ads_banner = true;
		if (!resetBoard) {
			FDGame* game = (FDGame*)(GAME()->_current_game);
			game->BoardSetBeginPos();
			resetBoard = true;
		}
		

	}
	else {
		_time_count_for_show_ads_banner = 2 * FPS_MAX;
	}
}
void CGame::UpdateAds()
{
	if (OS_IsNetworkAvailable())
	{
		//Admob
		PineAdmobAsync::enumAdmobUpdateResult result = _admob_manager.update();
		//PDEBUG("\n enumAdmobUpdateResult: %d", result);
		switch (result)
		{
		case PineAdmobAsync::enumAdmobUpdateResult::SHOW_VIDEO_SUCCESS:
			break;
		case PineAdmobAsync::enumAdmobUpdateResult::SHOW_VIDEO_FAIL:
			break;
		default:
			break;
		}
	}
}
#endif