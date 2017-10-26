#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
extern "C" bool ExitsFile(const char *file_name);
extern "C" int deCryptBuffer(char * bufferInput, int length, char * bufferOut, const char * passWord);

#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON
#undef _DEC_BUTTON_NUMS


#define _DEC_BUTTON_NUMS				(BUTTON_INVITE_NUMS)
#define _DEC_BUTTON_ARRAY				_button
#define _DEC_ANIM_BUTTON_BEGIN			0
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_INVITE_FRIEND

void CFriendManager::SetInitGame(CGame *game)
{
	_game = game;
	_gp = game->_g;
	_stateFriend = 0;
};

void CFriendManager::SynForNewFriendVersion(int fromVersion)
{
	for (int i = fromVersion; i <= FRIEND_VERSION; i++)
	{
		NewDataFriendVersion(i);
	}
	_versionFriend = FRIEND_VERSION;
}

void CFriendManager::NewDataFriendVersion(int version)
{
	switch (version)
	{
	case FRIEND_VERSION_0:
		break;	
	}
}
void CFriendManager::DataDefault()
{
	
	_versionFriend = FRIEND_VERSION;
    _numCurrentFriend = 0;
    
#ifdef _WINDOWS
	for (int i = 0; i < 10; i++)
	{
		AccountInfo acc;
		sprintf(acc._id, "GC_%d", i + 1);

		acc._userNameUnicode[0] = 'P';
		acc._userNameUnicode[1] = 'l';
		acc._userNameUnicode[2] = 'a';
		acc._userNameUnicode[3] = 'y';
		acc._userNameUnicode[4] = 'e';
		acc._userNameUnicode[5] = 'r';
		acc._userNameUnicode[6] = ' ';
		acc._userNameUnicode[7] = i + 1 + 48;
		acc._userNameUnicodeLenght = 8;
		AddFriend(acc);
	}
#endif
}
bool CFriendManager::DataLoad()
{
	bool ret = false;
	int saved = 0;

	char *buffer = NULL;

	Buffer* buff = OS_LoadAppData(SAVE_FRIEND_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();

		if (saved > 0)
		{
			buffer = new char[saved];
			memcpy(buffer, buff->Data(), saved);
		}
		delete buff;
	}

	_avatarFriendLoaded = 0;
	_avatarTopLoaded = 0;
	if (saved > 0)
	{
		ret = true;

		_versionFriend = GAME()->GetInt16At(buffer, SAVE_FRIEND_VERSION);
        _numCurrentFriend = GAME()->GetInt16At(buffer, SAVE_FRIEND_NUM_CURRENT_FRIEND);
		
		_friendList = new AccountInfo[_numCurrentFriend];
		int index = SAVE_FRIEND_BEGIN_ID;
		for (int i = 0; i < _numCurrentFriend; i++)
		{
			for (int c = 0; c < SOCIAL_ID_STRING_LENGTH; c++)
			{
				_friendList[i]._id[c] = GAME()->GetCharAt(buffer, index + SAVE_FRIEND_ID + c);
			}

			for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
			{
				_friendList[i]._userNameUnicode[c] = GAME()->GetInt32At(buffer, index + SAVE_FRIEND_NAME + c * 4);
			}

			_friendList[i]._userNameUnicodeLenght = GAME()->GetByteAt(buffer, index + SAVE_FRIEND_NAME_LENGTH);		
			index += SAVE_FRIEND_LENGTH;
		}
		if (_versionFriend < FRIEND_VERSION)
		{
			SynForNewFriendVersion(_versionFriend + 1);
			DataSave();
		}
	}
	else
	{
		DataDefault();		
		DataSave();
	}
	SAFE_DEL_ARRAY(buffer);

	printf("\n length load friend = %d", saved);
	return ret;
}
void CFriendManager::DataSave()
{
	int length = SAVE_FRIEND_BEGIN_ID + _numCurrentFriend * SAVE_FRIEND_LENGTH;

	printf("\n length save friend = %d", length);
	char *buffer = new char[length];
	GAME()->SetInt16At(buffer, SAVE_FRIEND_VERSION, _versionFriend);
	GAME()->SetInt16At(buffer, SAVE_FRIEND_NUM_CURRENT_FRIEND, _numCurrentFriend);
	int index = SAVE_FRIEND_BEGIN_ID;
	for (int i = 0; i < _numCurrentFriend; i++)
	{
		for (int c = 0; c < SOCIAL_ID_STRING_LENGTH; c++)
		{
			GAME()->SetCharAt(buffer, index + SAVE_FRIEND_ID + c, _friendList[i]._id[c]);
		}

		for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
		{
			GAME()->SetInt32At(buffer, index + SAVE_FRIEND_NAME + c * 4, _friendList[i]._userNameUnicode[c]);
		}

		GAME()->SetByteAt(buffer, index+ SAVE_FRIEND_NAME_LENGTH, _friendList[i]._userNameUnicodeLenght);
		index += SAVE_FRIEND_LENGTH;
	}

	OS_SaveAppData(SAVE_FRIEND_FILE_NAME, buffer, length, false);

#ifdef _WINDOWS

#else
	SAFE_DEL_ARRAY(buffer);
#endif
	
}

void CFriendManager::CopyAccInfo(AccountInfo *accCopy, AccountInfo *accTarget, bool newFriend)
{
	//accTarget = accCopy;
	for (int c = 0; c < SOCIAL_ID_STRING_LENGTH; c++)
	{
		accTarget->_id[c] = accCopy->_id[c];
	}

	for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
	{
		accTarget->_userNameUnicode[c] = accCopy->_userNameUnicode[c];
	}
	accTarget->_userNameUnicodeLenght = accCopy->_userNameUnicodeLenght;

    sprintf(accTarget->_guidID, "%s", accCopy->_guidID);
	sprintf(accTarget->_avatarUrl, "%s", accCopy->_avatarUrl);
    sprintf(accTarget->_userName, "%s", accCopy->_userName);
	accTarget->_avatarState = accCopy->_avatarState;
	accTarget->_rank = accCopy->_rank;
	accTarget->_score = accCopy->_score;
}
void CFriendManager::CopyTopPlayer(AccountInfo accCopy, AccountInfo &accTarget)
{
	//accTarget = accCopy;
	sprintf(accTarget._id, "%s", accCopy._id);
	for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
	{
		accTarget._userNameUnicode[c] = accCopy._userNameUnicode[c];
        if (c < accCopy._userNameUnicodeLenght)
        {
            printf("\n %d : %d", c, accCopy._userNameUnicode[c]);
        }
	}
	accTarget._userNameUnicodeLenght = accCopy._userNameUnicodeLenght;
	sprintf(accTarget._avatarUrl, "%s", accCopy._avatarUrl);
    sprintf(accTarget._userName, "%s", accCopy._userName);
	accTarget._avatarState = accCopy._avatarState;
	accTarget._rank = accCopy._rank;
	accTarget._score = accCopy._score;
    
    //accTarget._avatar = accCopy._avatar;
}
AccountInfo *CFriendManager::GetFriend(const char* id)
{
	for (int i = 0; i < _numCurrentFriend; i++)
	{
		if (strcmp(id, _friendList[i]._id) == 0)
		{
			return &_friendList[i];
		}
	}
	return NULL;
}
void CFriendManager::AddFriend(AccountInfo account)
{
	int indexAccount = -1;
	for (int i = 0; i < _numCurrentFriend; i++)
	{
		if (strcmp(account._id, _friendList[i]._id) == 0)
		{
			indexAccount = i;
			break;
		}
	}
    
    printf("\n f_indexAccount = %d",indexAccount);
	if (indexAccount != -1)//da ton tai cap nhat thong tin friend
	{
        account._avatarState = _friendList[indexAccount]._avatarState;
		CopyAccInfo(&account, &_friendList[indexAccount], false);
	}
	else // add friend
	{
		AccountInfo *tempList = new AccountInfo[_numCurrentFriend + 1];
        
		for (int i = 0; i < _numCurrentFriend; i++)
		{
			CopyAccInfo(&_friendList[i], &tempList[i], false);
		}
        account._avatarState = AVATAR_STATE_NONE;
		CopyAccInfo(&account, &tempList[_numCurrentFriend], true);
		_numCurrentFriend++;
#ifdef _WINDOWS

#else
		SAFE_DEL_ARRAY(_friendList);
#endif
		_friendList = tempList;
	}
}
void CFriendManager::AddTopPlayer(AccountInfo account)
{
    //printf("\n _numTopPlayer = %d",_numTopPlayer);
	if (_numTopPlayer < MAX_TOP_PLAYER)
	{
		int indexAccount = -1;
		for (int i = 0; i < _numTopPlayer; i++)
		{
			if (strcmp(account._id, _topPlayer[i]._id) == 0)
			{
				indexAccount = i;
				break;
			}
		}
        
        printf("\n t_indexAccount = %d",indexAccount);
		if (indexAccount != -1)
		{
            account._avatarState = _topPlayer[indexAccount]._avatarState;
			CopyTopPlayer(account, _topPlayer[indexAccount]);
		}
		else
		{
            account._avatarState = AVATAR_STATE_NONE;
			CopyTopPlayer(account, _topPlayer[_numTopPlayer]);
			_numTopPlayer++;
		}

		//sort score
		for (int i = 0; i<_numTopPlayer - 1; i++)
		{
			for (int j = i + 1; j<_numTopPlayer; j++)
			{
				if (_topPlayer[i]._score < _topPlayer[j]._score)
				{
					AccountInfo res = _topPlayer[i];
					_topPlayer[i] = _topPlayer[j];
					_topPlayer[j] = res;
				}
			}
		}

#ifdef _WINDOWS
		for (int i = 0; i<_numTopPlayer; i++)
		{
			_topPlayer[i]._rank = i + 1;
		}
#endif
	}
    
    
}
//

extern "C" char* OS_GetResourceFilePath(const char* filename);
void CFriendManager::UpdateCreateAvatarFriend()
{
#if ADD_FACEBOOK
    FDGame *game = (FDGame*)GAME()->_current_game;
	if (GAME()->_serverThread.IsLoadAllFriendCompelete())
	{
		for (int i = 0; i < _numCurrentFriend; i++)
		{
			if (_friendList[i]._avatarState == AVATAR_STATE_LOADING_BINARY)
			{
				char fileName[64];
#ifdef _WINDOWS
				sprintf(fileName, "%s","ava.png");
#else
				sprintf(fileName, USER_AVATAR_FILE_NAME, _friendList[i]._id);
#endif

				_friendList[i]._avatar.SetTextureMaxSize(64, 64);
                _friendList[i]._avatarMaskState =AVATAR_STATE_LOADING_BINARY;
				if (_friendList[i]._avatar.CreateImageFromPNGinDocument(_gp, fileName) > 0)
				{
					_friendList[i]._avatarState = AVATAR_STATE_INIT_SUCCESS;
                    
                    char file_mask[512];
                    char file_source[512];
                    sprintf(file_mask, "%s", OS_GetResourceFilePath("avatar_mask"));
                    sprintf(file_source, "%s", OS_GetDocumentFilePath(fileName));
                    
                    if (_friendList[i]._avatarMask.CreateImageWithMixImage(_gp, file_mask, file_source))
                    {
                        _friendList[i]._avatarMaskState = AVATAR_STATE_INIT_SUCCESS;
                    }
                    else
                    {
                        _friendList[i]._avatarMaskState = AVATAR_STATE_INIT_FAILED;
                    }
				}
				else
				{
					_friendList[i]._avatarState = AVATAR_STATE_INIT_FAILED;
				}
			}
		}
        
        if (game->_user._avatarState == AVATAR_STATE_LOADING_BINARY)
        {
            char fileName[64];
            sprintf(fileName, USER_AVATAR_FILE_NAME, game->_user.GetFBID());
            
            game->_user._avatar.SetTextureMaxSize(128, 128);
            if (game->_user._avatar.CreateImageFromPNGinDocument(_gp, fileName) > 0)
            {
                game->_user._avatarState = AVATAR_STATE_INIT_SUCCESS;
            }
            else
            {
                game->_user._avatarState = AVATAR_STATE_INIT_FAILED;
            }
        }
	}
#endif
}
void CFriendManager::UpdateDownloadAvatar()
{
#if ADD_FACEBOOK
	for (int i = 0; i < _numCurrentFriend; i++)
	{
        
		if (_friendList[i]._avatarState == AVATAR_STATE_NONE)
		{
			_friendList[i]._avatarState = AVATAR_STATE_SAVE_AS;
			char filename[256];

			sprintf(filename, USER_AVATAR_FILE_NAME, _friendList[i]._id);
            printf("\n _friendList[i]._avatarUrl = %s",_friendList[i]._avatarUrl);
			if (strlen(_friendList[i]._avatarUrl) > 0)
			{
				PineSocial::DownloadAvatar(_friendList[i]._avatarUrl, filename, 2*USER_AVATAR_WW, 2*USER_AVATAR_WW);
			}
#if defined (MAC_OS)
			if (ExitsFile(filename))
			{
				_friendList[i]._avatarState = AVATAR_STATE_LOADING_BINARY;
			}
			else
			{
				_friendList[i]._avatarState = AVATAR_STATE_INIT_FAILED;
			}
#endif
		}		
	}
#endif
}
void CFriendManager::UnloadMemForAvatar()
{
#if ADD_FACEBOOK
    for (int i = 0; i < _numCurrentFriend; i++)
    {
        if (_friendList[i]._avatarState == AVATAR_STATE_INIT_SUCCESS)
        {
            _friendList[i]._avatarState = AVATAR_STATE_LOADING_BINARY;
            _friendList[i]._avatar.FreeTexture(_gp);
        }
    }
#endif
}

void CFriendManager::InitTopPlayer()
{
    FDGame *game = (FDGame*)GAME()->_current_game;
	_isShowFriend = false;
     _isRenderTopPlayer = true;
	 _stateShareFriend = 0;
	 _numTopPlayer = 0;
	 _scroll_bar_touch_id = -1;
	for (int i = 0; i < MAX_TOP_PLAYER; i++)
	{
		_topPlayer[i]._state = 0;
		_topPlayer[i]._offX = k_SCREEN_WIDTH;
	}
	/*_animWifi.SetAnimSprite(GET_SPRITE(_game->_user._spriteIDHudResultComplete), game->_user._spriteIDHudResultComplete);
	_animWifi.SetAnim(10, true);
	
	_animNavigationLeft.SetAnimSprite(GET_SPRITE(_game->_user._spriteIDHudResultComplete), _game->_user._spriteIDHudResultComplete);
	_animNavigationLeft.SetAnim(8, true);

	_animNavigationRight.SetAnimSprite(GET_SPRITE(_game->_user._spriteIDHudResultComplete), _game->_user._spriteIDHudResultComplete);
	_animNavigationRight.SetAnim(9, true);*/

	_opaLeft = 100;
	_opcaRight = 100;


	if (!game->_user._hadLoginFB)
	{
		_isShowFriend = true;
	}

	/*if (_game->_serverThread.IsLoadAllFriendCompelete())
	{
		if (HasCatchTop(_game->_user._currentLevelPlay) != -1)
		{			
			if(OutputCatch(_game->_user._currentLevelPlay))
            {
                _isRenderTopPlayer = true;
				_isShowFriend = true;
            }
		}
		else
		{	
			if (!_isRenderTopPlayer)
			{
				if (_game->_user._levelMap[_game->_user._currentLevelPlay]._score > 0)
				{
					_isRenderTopPlayer = true;				
					
					AccountInfo myInfo;
					sprintf(myInfo._id, "%s", _game->_user._userID);

					for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
					{
						myInfo._userNameUnicode[c] = _game->_user._userNameUnicode[c];
					}
					myInfo._userNameUnicodeLenght = _game->_user._userNameUnicodeLenght;
					sprintf(myInfo._avatarUrl, "%s", _game->_user._avatarURL);
#ifdef _WINDOWS

#else
					myInfo._rank = 1;
#endif
	
					myInfo._score = _game->_user._levelMap[_game->_user._currentLevelPlay]._score;
					_game->_friendManager.AddTopPlayer(myInfo);									
				}
				else
				{
					_isRenderTopPlayer = false;
				}
			}			
			_isCompleteLoadTop = false;
			_game->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_GET_TOP_PLAYER);
		}
	}*/

	_scrollX = 0;

#ifdef _WINDOWS
	
		for (int i = 0; i < 5; i++)
		{
			AccountInfo acc;
			sprintf(acc._id, "GC_%d", i + 1 * 2);

			acc._userNameUnicode[0] = 'P';
			acc._userNameUnicode[1] = 'l';
			acc._userNameUnicode[2] = 'a';
			acc._userNameUnicode[3] = 'y';
			acc._userNameUnicode[4] = 'e'; //" ki tu xuong dong"
			acc._userNameUnicode[5] = 'r';
			acc._userNameUnicode[6] = ' ';
			acc._userNameUnicode[7] = '1'+ i;

			acc._userNameUnicodeLenght = 8;			
			acc._score = i*100 + 100;
			AddTopPlayer(acc);
		}
	//InputCatch(_game->_user._currentLevelPlay);
		int min = -(_numTopPlayer - 5) * 150;
		int max = 0;
		if (min >= 0)
		{
			min = 0;
		}
	_scrollBarX.Init(min, max, 2, 1.3f);
	_scrollBarX.SetState(1);	
#endif
}
void CFriendManager::UpdateTopPlayer()
{

	if (_numTopPlayer > 3)
	{
		int BeginX = 0 + _scrollX;
		int WW = _numTopPlayer * 150;

		if (BeginX >= -100)
		{
			int offX = 100 - CMath::ABSOLUTE_VALUE(BeginX <= 0 ? BeginX : 0);
			_opcaRight = 100 - 100* offX/ 100.0f;
		}
		else
		{
			_opcaRight = 100;
		}

		if (CMath::ABSOLUTE_VALUE(BeginX) <= WW - 450 - 100)
		{
			_opaLeft = 100;
		}
		else
		{
			int offX = 100 - (WW - 450 - CMath::ABSOLUTE_VALUE(BeginX <= 0 ? BeginX : 0));
			_opaLeft = 100 - 100 * offX / 100.0f;
		}
	}

	/*if (_stateTable >= 4 && _game->_GameState == k_GS_RESULT_COMPLETE)
	{
		switch (_stateShareFriend)
		{
		case 1:
			if (_game->_friendManager.GetMeInTop() != NULL &&  _game->_friendManager.GetFriendAffterMe() != NULL)
			{
				if (_game->_friendManager.GetMeInTop()->_avatarState == AVATAR_STATE_INIT_SUCCESS
					&& _game->_friendManager.GetFriendAffterMe()->_avatarState == AVATAR_STATE_INIT_SUCCESS)
				{
					_game->_sharePicture.InitShare(CSharePicture::enumTypeShare::SHARE_FRIEND);
					_stateShareFriend = 2;
				}
			}		
			break;
		}
	}*/
}
void CFriendManager::RenderTopPlayer(Button buttonConnect, float x, float y, PineMatrix4 tranf)
{
    FDGame *game = (FDGame*)GAME()->_current_game;
		//GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(_gp, 32, x, y);
		if (_isRenderTopPlayer)
		{
			int beginX = x + _scrollX + 75;
			
			float clipX = x + game->_point_o.X;
			float clipY = y- USER_AVATAR_HH/2 - 45;
			float clipW = game->_screen_width;
			float clipH = k_SCREEN_HEIGHT;

			//_game->SetTranformRect(tranf, clipX, clipY, clipW, clipH, clipX, clipY, clipW, clipH);
			_gp->SetClip(clipX, clipY, clipW, clipH);

			PineMatrix4 scale;
			for (int i = 0; i < _numTopPlayer; i++)
			{
				
				int stringX = beginX;
				int stringY = y + 15;
								
				scale = CMath::Matrix4Scale2D(85 / (float)(USER_AVATAR_WW), 85 / (float)(USER_AVATAR_HH), stringX, stringY);
				//_gp->SetTransform(CMath::Matrix4ToParam(scale));
				BYTE avatarState = AVATAR_STATE_INIT_FAILED;
				PineImage *image = NULL;
#ifdef MAC_OS
#if ADD_FACEBOOK
				

				if (GetFriend(_topPlayer[i]._id) != NULL)
				{					
					avatarState = GetFriend(_topPlayer[i]._id)->_avatarState;					
					image = &GetFriend(_topPlayer[i]._id)->_avatar;
                    _topPlayer[i]._avatar = GetFriend(_topPlayer[i]._id)->_avatar;
				}
				else if(strcmp( _topPlayer[i]._id,game->_user.GetFBID()) == 0)
				{
					avatarState = game->_user._avatarState;
					image = &game->_user._avatar;
                    _topPlayer[i]._avatar =game->_user._avatar;
				}

				
                
                _topPlayer[i]._avatarState = avatarState;
                
#endif
#else
				
#endif
				//_gp->SetTransform(CMath::Matrix4ToParam(tranf));

				//GET_SPRITE(_game->_user._spriteIDHudResultComplete)->DrawFrame(_gp, 17, beginX, y);
				//rank
                int frame_index = 32;
                if (i % 2 == 0)
                {
                    frame_index = 33;
                    
                }
                GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(_gp, frame_index, beginX, y );
				
                if (avatarState == AVATAR_STATE_INIT_SUCCESS && image != NULL)
                {
                    float im_scale = 100.0f/image->GetWidth();
                    G()->SetScale(im_scale, im_scale, stringX-USER_AVATAR_WW/2, stringY-USER_AVATAR_WW/2);
                    //GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(_gp, 126, beginX, y);
                    image->DrawImageWithRect(_gp, stringX-USER_AVATAR_WW/2, stringY-USER_AVATAR_WW/2);
                    G()->ClearScale();
                }
                else
                {
					GET_SPRITE(SPRITEID_HUD_RESULT_01)->DrawFrame(_gp, 126, beginX, y+ 15);
                }

                stringX = beginX;
				stringY = y + 85;
				scale = CMath::Matrix4Scale2D(0.35, 0.35, stringX, stringY);
				scale = CMath::Matrix4Multiply(scale, tranf);
				//_gp->SetTransform(CMath::Matrix4ToParam(scale));
                int spacing = GET_SPRITE(SPRITEID_FONT_FACEBOOKSCORE_01)->_char_spacing;
                GET_SPRITE(SPRITEID_FONT_FACEBOOKSCORE_01)->_char_spacing = 3;
				GET_SPRITE(SPRITEID_FONT_FACEBOOKSCORE_01)->DrawAnsiTextFormat(_gp, stringX, stringY, _HCENTER | _VCENTER, "%d", _topPlayer[i]._score);
                GET_SPRITE(SPRITEID_FONT_FACEBOOKSCORE_01)->_char_spacing = spacing;
                /*spacing = GET_SPRITE(SPRITEID_FONT_NAME_FACEBOOK_01)->_char_spacing;
                GET_SPRITE(SPRITEID_FONT_NAME_FACEBOOK_01)->_char_spacing = 3;
                GET_SPRITE(SPRITEID_FONT_NAME_FACEBOOK_01)->DrawAnsiTextFormat(_gp, stringX, stringY, _HCENTER | _VCENTER, "#%d", _topPlayer[i]._rank);
                GET_SPRITE(SPRITEID_FONT_NAME_FACEBOOK_01)->_char_spacing = spacing;*/
                
                char text_rank[10];
                int lenght = sprintf(text_rank, "#%d", _topPlayer[i]._rank);
                int rank_uni[64];
                for(int i = 0; i< lenght;i++)
                {
                    rank_uni[i] = text_rank[i];
                }
				stringX = beginX;
				stringY = y - 72;
				scale = CMath::Matrix4Scale2D(0.9f, 0.9f, stringX, stringY);
				scale = CMath::Matrix4Multiply(scale, tranf);
				_gp->SetTransform(CMath::Matrix4ToParam(scale));
                _game->_freeTypeText.SetText(rank_uni, lenght, 6);
                G()->SetScale(0.75f,0.75f, stringX, stringY + 32);
                _game->_freeTypeText.Render(stringX, stringY + 32, _HCENTER);
                G()->ClearScale();
                G()->SetScale(0.75f,0.75f, stringX, stringY);
				_game->_freeTypeText.SetText(_topPlayer[i]._userNameUnicode, _topPlayer[i]._userNameUnicodeLenght, 6);
				_game->_freeTypeText.Render(stringX, stringY, _HCENTER);
                G()->ClearScale();
				_gp->SetTransform(CMath::Matrix4ToParam(tranf));
				beginX += 150;
			}
			_gp->ClearClip();

			if (_numTopPlayer > 3)
			{
				_gp->SetOpacity(_opaLeft);
				_animNavigationLeft.SetPos(x,y);
				_animNavigationLeft.DrawAnimObject(_gp);
				_animNavigationLeft.UpdateAnimObject();

				_gp->SetOpacity(_opcaRight);
				_animNavigationRight.SetPos(x, y);
				_animNavigationRight.DrawAnimObject(_gp);
				_animNavigationRight.UpdateAnimObject();
				_gp->SetOpacity(100);
			}
		}
		else
		{
			if (buttonConnect._state == BUTTON_STATE_NORMAL || buttonConnect._state == BUTTON_STATE_LOCK)
			{
				buttonConnect.SetPos(x, y);
				buttonConnect.Render(_gp);
			}
			else
			{
				_animWifi.SetPos(x, y);
				_animWifi.DrawAnimObject(_gp);
				_animWifi.UpdateAnimObject();
			}
			buttonConnect.Update();
		}
}
void CFriendManager::UpdateScroll()
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	if (_numTopPlayer > 3)
	{
		//printf("\n _scrollX = %f", _scrollX);
		_scrollX = _scrollBarX.GetCurrent();
		//printf("\n 111 _scrollX = %f", _scrollX);
		_scrollBarX.Update();
		for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
		{
			int x = game->_point_o.X;
			int y = k_SCREEN_HEIGHT - USER_AVATAR_HH;
			int w = game->_screen_width;
			int h = USER_AVATAR_HH;
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_scroll_bar_touch_id == -1)
					{
						_offsetDragX = 0;
						_touchDragX = TOUCHS(i)->x;
						_scrollBarX.Set();
						_scrollBarX.SetState(0);
						_scroll_bar_touch_id = TOUCHS(i)->_id;
					}
					
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (_scroll_bar_touch_id == TOUCHS(i)->_id)
					{
						_offsetDragX = TOUCHS(i)->x - _touchDragX;
						_touchDragX = TOUCHS(i)->x;
						_scrollBarX.AddOffset(_offsetDragX);
						_scrollBarX.ValidateRangeExt();
						_scrollBarX.ForceToTarget();
						_scrollBarX.SetState(1);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (_scroll_bar_touch_id == TOUCHS(i)->_id)
					{
						if (!_scrollBarX._collided)
						{
							_scrollBarX.End();
						}
						_scrollBarX.SetState(2);
						_scroll_bar_touch_id = -1;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (_scroll_bar_touch_id == TOUCHS(i)->_id)
					{
						if (!_scrollBarX._collided)
						{
							_scrollBarX.End();
						}
						_scrollBarX.SetState(2);
						_scroll_bar_touch_id = -1;
					}
				}
			}
		}
	}
    else
    {
        //CGame::_this->ClearAllTouch();
    }
}

int CFriendManager::GetRankTop(const char* id)
{
	for (int i = 0; i < _numTopPlayer; i++)
	{
		if(strcmp(_topPlayer[i]._id, id) == 0)
		{
			return _topPlayer[i]._rank;
		}
	}
	return 1000;
}
AccountInfo *CFriendManager::GetFriendAffterMe()
{
    FDGame *game = (FDGame*)GAME()->_current_game;
	for (int i = 0; i < _numTopPlayer; i++)
	{
		if (strcmp(_topPlayer[i]._id, game->_user.GetUserID()) == 0)
		{
			if (i+1 < _numTopPlayer)
			{
				return &_topPlayer[i + 1];
			}
		}
	}
	return NULL;
}
AccountInfo *CFriendManager::GetMeInTop()
{
    FDGame *game = (FDGame*)GAME()->_current_game;
	for (int i = 0; i < _numTopPlayer; i++)
	{
		if (strcmp(_topPlayer[i]._id, game->_user.GetUserID()) == 0)
		{			
			return &_topPlayer[i];
		}
	}
	return NULL;
}

/////catch
void CFriendManager::UpdateTimeCatchTop()
{
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		if (_catchTopPlayer[i]._levelCath != -1)
		{
			if (time(NULL) - _catchTopPlayer[i]._timeCatch >= MAX_TIME_CATCH * 60)
			{
				_catchTopPlayer[i]._levelCath = -1;
				SaveCatch();
			}
		}
	}
}
bool CFriendManager::CheckFullCatch()
{
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		if (_catchTopPlayer[i]._levelCath == -1)
		{
			return false;
		}
	}
	return true;
}
int CFriendManager::HasMinTimeCatch()
{
	UINT64 min = 9999999999999999999;
	int index = -1;
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		if (_catchTopPlayer[i]._levelCath != -1)
		{
			if (_catchTopPlayer[i]._timeCatch < min)
			{
				min =  _catchTopPlayer[i]._timeCatch;
				index = i;
			}
		}
	}
	return index;
}
int CFriendManager::HasCatchTop(int level)
{
	bool hasCatch = false;
	int index = -1;
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		if (_catchTopPlayer[i]._levelCath == level)
		{
			hasCatch = true;
			index = i;
			break;
		}
	}
	if (hasCatch)
	{
		if (time(NULL) - _catchTopPlayer[index]._timeCatch >= MAX_TIME_CATCH*60)
		{
			index = -1;
		}		
	}
	return index;
}
void CFriendManager::InputCatch(int level)
{
	bool addCatch = false;
	int index = -1;
	if (HasCatchTop(level) == -1)
	{
		if (CheckFullCatch())
		{
			index = HasMinTimeCatch();
			if (index != -1)
			{
				addCatch = true;
			}
		}
		else
		{
			addCatch = true;
			for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
			{
				if (_catchTopPlayer[i]._levelCath == -1)
				{
					index = i;
					break;
				}
			}
		}		
	}	

	if (addCatch)
	{		
		printf("\n input \n");
		_catchTopPlayer[index]._numTop = _numTopPlayer;
		_catchTopPlayer[index]._levelCath = level;
		_catchTopPlayer[index]._timeCatch = time(NULL);


		int beginID = CATCH_TOP_PLAYER_USER_ID + (64 * MAX_TOP_PLAYER)*index;
		int beginRank = CATCH_TOP_PLAYER_RANK + (2 * MAX_TOP_PLAYER)*index;
		int beginScore = CATCH_TOP_PLAYER_SCORE + (4 * MAX_TOP_PLAYER)*index;
		for (int f = 0; f < _numTopPlayer; f++)
		{
			//user ID
			for (int c = 0; c < 64; c++)
			{
				_game->SetCharAt(_bufferTop, beginID + c, _topPlayer[f]._id[c]);
			}
			beginID += 64;

			//rank
			_game->SetInt16At(_bufferTop, beginRank, _topPlayer[f]._rank);
			beginRank += 2;

			//score
			_game->SetInt32At(_bufferTop, beginScore, _topPlayer[f]._score);
			beginScore += 4;

			printf("\n rank = %d, score = %d", _topPlayer[f]._rank, _topPlayer[f]._score);
		}		
		SaveCatch();
	}
}
int CFriendManager::GetIndexFriendFromTop(AccountInfo acc)
{
    for (int j = 0; j < _numCurrentFriend; j++)
    {
        if (strcmp(acc._id, _friendList[j]._id) == 0)
        {
            return j;
        }
    }
    return -1;
}
bool CFriendManager::OutputCatch(int level)
{
#if ADD_FACEBOOK
    FDGame *game = (FDGame*)GAME()->_current_game;
	int index = HasCatchTop(level);
	if (index != -1)
	{
		_numTopPlayer = GAME()->GetInt16At(_bufferTop, CATCH_NUM_TOP + index * 2);
        if (_numTopPlayer > 0)
        {
            int beginID = CATCH_TOP_PLAYER_USER_ID + (64 * MAX_TOP_PLAYER)*index;
            int beginRank = CATCH_TOP_PLAYER_RANK + (2 * MAX_TOP_PLAYER)*index;
            int beginScore = CATCH_TOP_PLAYER_SCORE + (4 * MAX_TOP_PLAYER)*index;
            for (int f = 0; f < _numTopPlayer; f++)
            {
                //user ID
                for (int c = 0; c < 64; c++)
                {
                    _topPlayer[f]._id[c] = _game->GetCharAt(_bufferTop, beginID + c);
                }
                beginID += 64;
                
                _topPlayer[f]._avatarState = AVATAR_STATE_NONE;
                if (strcmp(_topPlayer[f]._id, game->_user.GetUserID()) == 0)
                {
                    for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
                    {
                        _topPlayer[f]._userNameUnicode[c] = game->_user._userNameUnicode[c];
                    }
                    _topPlayer[f]._userNameUnicodeLenght = game->_user._userNameUnicodeLenght;
                    sprintf(_topPlayer[f]._avatarUrl, "%s", game->_user.GetAvatarURL());
                }
                else
                {
                    int indexF = GetIndexFriendFromTop(_topPlayer[f]);
                    if (indexF != -1)
                    {
                        CopyTopPlayer(_friendList[indexF], _topPlayer[f]);
                    }
                }            
                
                //rank
                _topPlayer[f]._rank = _game->GetInt16At(_bufferTop, beginRank);
                beginRank += 2;
                
                //score
                _topPlayer[f]._score = _game->GetInt32At(_bufferTop, beginScore);
                beginScore += 4;
                
                printf("\n id = %s, rank = %d, score = %d", _topPlayer[f]._id,_topPlayer[f]._rank, _topPlayer[f]._score);
            }            
            return true;
        }
        else
        {
            return false;
        }
	}
#endif
    return false;
}
void CFriendManager::ClearCatch(int level)
{
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		if (_catchTopPlayer[i]._levelCath == level)
		{
			_catchTopPlayer[i]._levelCath = -1;
			_catchTopPlayer[i]._timeCatch = 0;
			_catchTopPlayer[i]._numTop = 0;
			break;
		}
	}	
}
void CFriendManager::LoadCatch()
{
	int saved = 0;
	Buffer* buff = OS_LoadAppData(CATCH_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_bufferTop, buff->Data(), saved);
		delete buff;
	}

	if (saved == 0)
	{
		DefaultCatch();
		SaveCatch();
	}
	else
	{
		_versionTopPlayer = GAME()->GetInt16At(_bufferTop, CATCH_TOP_PLAYE_VERSION);
		for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
		{
			_catchTopPlayer[i]._levelCath = GAME()->GetInt16At(_bufferTop, CATCH_LEVEL + i * 2);
			_catchTopPlayer[i]._numTop = GAME()->GetInt16At(_bufferTop, CATCH_NUM_TOP + i * 2);
			_catchTopPlayer[i]._timeCatch = GAME()->GetInt64At(_bufferTop, CATCH_TIME + i * 8);
		}

		if (_versionTopPlayer < TOP_PLAYER_VERSION)
		{
			SynForNewTopVersion(_versionTopPlayer + 1);
			SaveCatch();
		}
	}
}
void CFriendManager::SaveCatch()
{
	printf("\n SAVE_CATH_LENGHT = %d", SAVE_CATH_LENGHT);
	GAME()->SetInt16At(_bufferTop, CATCH_TOP_PLAYE_VERSION, _versionTopPlayer);
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		GAME()->SetInt16At(_bufferTop, CATCH_LEVEL + i * 2, _catchTopPlayer[i]._levelCath);
		GAME()->SetInt16At(_bufferTop, CATCH_NUM_TOP + i * 2, _catchTopPlayer[i]._numTop);
		GAME()->SetInt64At(_bufferTop, CATCH_TIME + i * 8, _catchTopPlayer[i]._timeCatch);
	}	

	OS_SaveAppData(CATCH_FILE_NAME, _bufferTop, SAVE_CATH_LENGHT, false);
}
void CFriendManager::DefaultCatch()
{
	for (int i = 0; i < MAX_CATCH_TOP_PLAYER; i++)
	{
		_catchTopPlayer[i]._levelCath = -1;
		_catchTopPlayer[i]._timeCatch = 0;
		_catchTopPlayer[i]._numTop = 0;
	}
}

void CFriendManager::SynForNewTopVersion(int fromVersion)
{
	for (int i = fromVersion; i <= TOP_PLAYER_VERSION; i++)
	{
		NewDataTopVersion(i);
	}
	_versionTopPlayer= TOP_PLAYER_VERSION;
}

void CFriendManager::NewDataTopVersion(int version)
{
	switch (version)
	{
	case TOP_PLAYER_VERSION_0:
		break;
	}
}

void CFriendManager::UpdateScrollFriend()
{
	if (_numFriend > 4)
	{
		_scrollY = _scrollBarY.GetCurrent();
		_scrollBarY.Update();

		float per = (-_scrollY / _pointScrollH);
		if (per <0)
		{
			per = 0;
		}
		if (per > 1)
		{
			per = 1;
		}

		_pointScrollY = per * 950;
		for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
		{
			if (TOUCHS(i)->_flag)
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					_offsetDragY = 0;
					CGame::_this->ClearAnotherTouch(i);
					_touchDragY = TOUCHS(i)->y;
					_scrollBarY.Set();
					_scrollBarY.SetState(0);
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					_offsetDragY = TOUCHS(i)->y - _touchDragY;
					_touchDragY = TOUCHS(i)->y;
					_scrollBarY.AddOffset(_offsetDragY);
					_scrollBarY.ValidateRangeExt();
					_scrollBarY.ForceToTarget();
					_scrollBarY.SetState(1);
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (!_scrollBarY._collided)
					{
						_scrollBarY.End();
					}
					_scrollBarY.SetState(2);
					//CGame::_this->ClearAllTouch();
				}
			}
		}
	}
    else
    {
        //CGame::_this->ClearAllTouch();
    }
}
bool CFriendManager::UpdateTouchFriend()
{
    return false;
	/*bool ret = false;
	int index = -1;
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
		if (_DEC_BUTTON_ARRAY[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_DEC_BUTTON_ARRAY[index].NeedAction())
		{
			switch (index)
			{
			case BUTTON_INVITE_CLOSE:
				_stateFriend = 5;
				break;

			case BUTTON_INVITE_REFRESH:
				_game->_isServerProssesing = true;
				_game->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_FRIEND);
				break;
			}

			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				if (_DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_HIDE)
				{
					_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
			_game->SetProcessingButton();
			CGame::_this->ClearAllTouch();
		}
		return true;
	}
	bool pressed = false;
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			for (int touch = 0; touch <_DEC_BUTTON_NUMS; touch++)
			{
				if (_DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_LOCK || _DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}


				int x = _DEC_BUTTON_ARRAY[touch].TZ_GetX() - 5;
				int y = _DEC_BUTTON_ARRAY[touch].TZ_GetY() - 5;
				int w = _DEC_BUTTON_ARRAY[touch].TZ_GetWidth() + 10;
				int h = _DEC_BUTTON_ARRAY[touch].TZ_GetHeight() + 10;
				Button *button = &_DEC_BUTTON_ARRAY[touch];
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					pressed = true;
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							_game->SetProcessingButton(TOUCHS(i)->_id);
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							_game->SetProcessingButton(TOUCHS(i)->_id);
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{

						button->SetState(BUTTON_STATE_ACTIVE);
						TOUCHS(i)->_flag = false;
					}
				}
				else
				{
					if (button->_state == BUTTON_STATE_SELECT)
					{
						for (int j = 0; j < _DEC_BUTTON_NUMS; j++)
						{
							if (_DEC_BUTTON_ARRAY[j]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[j]._state != BUTTON_STATE_HIDE)
							{
								_DEC_BUTTON_ARRAY[j].SetState(BUTTON_STATE_NORMAL);
							}
						}
						_game->SetProcessingButton();
						CGame::_this->ClearAllTouch();
					}
				}
			}
		}
	}
	if (pressed == false)
	{
		return false;
	}
	else
	{
		return true;
	}*/
}

bool CFriendManager::UpdateTouchFriendList()
{
	bool ret = false;
    return ret;
	/*int index = -1;
	for (int i = 0; i < _numFriend; i++)
	{
		if (_typeFriend == CFriendManager::enumTypeFriend::HELP_FRIEND)
		{
			if (_friendRequest[i]._button._state == BUTTON_STATE_ACTIVE)
			{
				ret = true;
				index = i;
				break;
			}
		}
		else
		{
			if (_friendList[i]._button._state == BUTTON_STATE_ACTIVE)
			{
				ret = true;
				index = i;
				break;
			}
		}
	}
	if (ret)
	{
		if (_typeFriend == CFriendManager::enumTypeFriend::HELP_FRIEND)
		{
			if (_friendRequest[index]._button.NeedAction())
			{        
				_indexChooseFriend = index;
#ifdef _WINDOWS
				_game->_user.AddFriendHelp(_friendRequest[index]._id);
				RemoveRequest(_friendRequest[index]._id);
#else
				OS_SendRequest(GetFBFriendID(_friendRequest[index]._id), _game->_design.getTextDes(38), _game->_design.getTextDes(35));
#endif
                
				for (int i = 0; i < _numFriend; i++)
				{
					if (_friendRequest[i]._button._state != BUTTON_STATE_LOCK && _friendRequest[i]._button._state != BUTTON_STATE_HIDE)
					{
						_friendRequest[i]._button.SetState(BUTTON_STATE_NORMAL);
					}
				}
				_game->SetProcessingButton();
				CGame::_this->ClearAllTouch();
			}			
		}
		else
		{
			if (_friendList[index]._button.NeedAction())
			{
				_indexChooseFriend = index;			
				
#ifdef _WINDOWS
				_friendList[index]._button.SetState(BUTTON_STATE_HIDE);
				SetTimeReSend(_friendList[_indexChooseFriend]._id);
				AddFriendRequest(_friendList[_indexChooseFriend]);
#else
				OS_SendRequest(GetFBFriendID(_friendList[index]._id), _game->_design.getTextDes(37), _game->_design.getTextDes(34));
#endif
               
				_game->_user.DataSave();

				for (int i = 0; i < _numFriend; i++)
				{
					if (_friendList[i]._button._state != BUTTON_STATE_LOCK && _friendList[i]._button._state != BUTTON_STATE_HIDE)
					{
						_friendList[i]._button.SetState(BUTTON_STATE_NORMAL);
					}
				}
				_game->SetProcessingButton();
				CGame::_this->ClearAllTouch();
			}
		}
		return true;
	}
	
	bool pressed = false;
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			for (int touch = 0; touch <_numFriend; touch++)
			{
				if (_game->CheckHadTouchButton(TOUCHS(i)->_id))
				{
					continue;
				}
				Button *button; 
				int x, y, w, h;
				if (_typeFriend == CFriendManager::enumTypeFriend::HELP_FRIEND)
				{
					if (_friendRequest[touch]._button._state == BUTTON_STATE_LOCK || _friendRequest[touch]._button._state == BUTTON_STATE_HIDE)
					{
						continue;
					}
					button = &_friendRequest[touch]._button;
					x = _friendRequest[touch]._button.TZ_GetX() - 5;
					y = _friendRequest[touch]._button.TZ_GetY() - 5;
					w = _friendRequest[touch]._button.TZ_GetWidth() + 10;
					h = _friendRequest[touch]._button.TZ_GetHeight() + 10;
				}
				else
				{
					if (_friendList[touch]._button._state == BUTTON_STATE_LOCK || _friendList[touch]._button._state == BUTTON_STATE_HIDE)
					{
						continue;
					}
					button = &_friendList[touch]._button;
					x = _friendList[touch]._button.TZ_GetX() - 5;
					y = _friendList[touch]._button.TZ_GetY() - 5;
					w = _friendList[touch]._button.TZ_GetWidth() + 10;
					h = _friendList[touch]._button.TZ_GetHeight() + 10;
				}
				
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					pressed = true;
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							_game->SetProcessingButton(TOUCHS(i)->_id);
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							_game->SetProcessingButton(TOUCHS(i)->_id);
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{

						button->SetState(BUTTON_STATE_ACTIVE);
						TOUCHS(i)->_flag = false;
					}
				}
				else
				{
					if (button->_state == BUTTON_STATE_SELECT)
					{
						for (int j = 0; j < _numFriend; j++)
						{
							if (_typeFriend == CFriendManager::enumTypeFriend::HELP_FRIEND)
							{
								if (_friendRequest[j]._button._state != BUTTON_STATE_LOCK && _friendRequest[j]._button._state != BUTTON_STATE_HIDE)
								{
									_friendRequest[j]._button.SetState(BUTTON_STATE_NORMAL);
								}
							}
							else
							{
								if (_friendList[j]._button._state != BUTTON_STATE_LOCK && _friendList[j]._button._state != BUTTON_STATE_HIDE)
								{
									_friendList[j]._button.SetState(BUTTON_STATE_NORMAL);
								}
							}
						}
						_game->SetProcessingButton();
						CGame::_this->ClearAllTouch();
					}
				}
			}
		}
	}
	if (pressed == false)
	{
		return false;
	}
	else
	{
		return true;
	}*/
}


const char* CFriendManager::GetFBFriendID(const char* idFriend)
{
    int count = 0;
    for (int j = 3; j < strlen(idFriend); j++) {
        _fbFriendID[count] = idFriend[j];
        count++;
    }
    _fbFriendID[count] = '\0';
    return _fbFriendID;
}
int CFriendManager::CheckNewRank(int rank)
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	int index = -1;
	for (int i = 0; i < _numTopPlayer; i++)
	{
#if defined (MAC_OS)
		if (strcmp(_topPlayer[i]._id, game->_user.GetFBID()) == 0)
		{
			index = i;
			break;
		}
#else
		if (strcmp(_topPlayer[i]._id, "GC_4") == 0)
		{
			index = i;
			break;
		}
#endif
	}
	if (index != -1)
	{
		if (_topPlayer[index]._rank < rank)
		{
			return _topPlayer[index]._rank;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int CFriendManager::GetIndexRank(int rank)
{
	for (int i = 0; i < _numTopPlayer; i++)
	{
        printf("\n fb _topPlayer[%d]._rank : %d",i, _topPlayer[i]._rank);
		if (_topPlayer[i]._rank == rank)
		{
			return i;
		}
	}
	return -1;
}

void CFriendManager::SortFriend()
{
	int maxscore = _topPlayer[0]._score;
	for (int i = 0; i < _numTopPlayer - 1; i++)
	{
		for (int j = i + 1; j < _numTopPlayer; j++)
		{
			if (_topPlayer[i]._score <  _topPlayer[j]._score)
			{
				/*int rank = _topPlayer[i]._rank;
				_topPlayer[i]._rank = _topPlayer[j]._rank;
				_topPlayer[j]._rank = rank;*/
                AccountInfo temp;
                PineImage avatar;
                avatar = _topPlayer[i]._avatar;
                _topPlayer[i]._avatar = _topPlayer[j]._avatar;
                _topPlayer[j]._avatar = avatar;
                CopyTopPlayer(_topPlayer[i], temp);
                CopyTopPlayer(_topPlayer[j], _topPlayer[i]);
                CopyTopPlayer( temp,_topPlayer[j]);                
			}
            _topPlayer[j]._rank = j+1;
		}
        _topPlayer[i]._rank = i+1;
	}
}

int CFriendManager::GetRank(char* fbid)
{
	for (int i = 0; i < _numTopPlayer; i++)
	{
        //printf("\n fb id : %s",_topPlayer[i]._id);
		if (strcmp(_topPlayer[i]._id, fbid) == 0)
		{
			return _topPlayer[i]._rank;
		}
	}
	return -1;
}

int CFriendManager::GetScore(char* fbid)
{
    for (int i = 0; i < _numTopPlayer; i++)
    {
        //printf("\n fb id : %s",_topPlayer[i]._id);
        if (strcmp(_topPlayer[i]._id, fbid) == 0)
        {
            return _topPlayer[i]._score;
        }
    }
    return -1;
}