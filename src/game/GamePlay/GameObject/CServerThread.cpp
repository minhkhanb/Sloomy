#include "pch.h"
#include "../../GameCore/CGame.h"
#include <PineEngine.h>
#include <string>
#include <iostream>

PE_USING_ENGINE;

extern "C" bool OS_IsLogginFacebook();

#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
extern "C" void getUniqueIDForDevice(char * uid);
extern "C" int AsynCheckAccountServer(PineServiceHandle* handle, const char* account_id);
extern "C" void AsynCreateNewAccount(PineServiceHandle* handle);
extern "C" void OS_AsynGetAvatarURLFB(PineServiceHandle* handle, PineAccountInfo* acc_info);
extern "C" void AsynSyncSaveFromServer(PineServiceHandle* handle);
extern "C" void AsynSyncSaveToServer(PineServiceHandle* handle);
extern "C" void DownloadAvatar(PineServiceHandle* handle,const char* url, const char *filename, float w, float h);
#else
extern "C" int GetServerSaveStatus();
extern "C" bool SynSaveFromServer();
extern "C" void SynSaveToServer();
extern "C" void OS_DeleteFile(const char *file_name);
extern "C" void getUniqueIDForDevice(char * uid);
extern "C" bool SetServerTime(Pine_Date_Time &time);
extern "C" bool checkServerRealIsAble();
extern "C" bool OS_DownloadFile(const char* url, const char* file_name,int time_out);
#endif
extern "C" bool GetInvitedId(char *invitedId);
extern "C" bool GetInvitionFriends();
extern "C" int InputInvitetionCode(char *inviteCode);
extern "C" bool ExitsFile(const char *file_name);
extern "C" std::string     OS_GetFacebookToken();

typedef Pine::Social::Facebook::CApplication CFacebookApplication;
typedef Pine::Social::Facebook::CSession CFacebookSession;
typedef Pine::Social::Facebook::CScoreItem CFacebookScoreItem;

bool CServerThread::AddCommand(enumTypeCommand typeCommand)
{
  
	for (int i = 0; i < MAX_COMMAND_IN_THREAD; i++)
	{
		if (_command[i]._stateCommand == enumStateCommand::COMMAND_STATE_NONE)
		{
			_command[i]._stateCommand = enumStateCommand::COMMAND_STATE_INIT;
			_command[i]._typeCommand = typeCommand;
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
			if (typeCommand == CServerThread::COMMAND_TYPE_SYN_SAVE_TO_SERVER){
				_svc_syn_save_to_server.reset();
#if defined(BUILD_SUPPORT)
				GAME()->_support_color = 0xFF00FF00;
#endif
			}
#endif
			return true;
		}
	}

	return false;
}

bool CServerThread::AlreadyHasCommand(enumTypeCommand typeCommand)
{
    for (int i = 0; i < MAX_COMMAND_IN_THREAD; i++)
    {
        if (_command[i]._typeCommand == typeCommand)
        {
            return true;
        }
    }
    return false;
}

void CServerThread::ClearAllCommand()
{
    return;
   for (int i = 0; i < MAX_COMMAND_IN_THREAD; i++)
   {
       _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_NONE;
       _command[i]._typeCommand = enumTypeCommand::COMMAND_TYPE_NONE;
   }
}

void CServerThread::EndCommand(int i)
{
	_command[i]._typeCommand = enumTypeCommand::COMMAND_TYPE_NONE;
    _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_NONE;
	
}

bool CServerThread::IsSynSaveComplete()
{
    if (_isNetworkAvailable)
    {
        if (_isSynSaveComplete)
        {
            return true;
        }
    }
    return false;
}

bool CServerThread::IsLoadAllFriendCompelete()
{

    /*if (!IsSynSaveComplete())
    {
        return false;
    }*/
    if (/*!_isInitGC ||*/ !_isInitFB)
    {
        return false;
    }
    
    /*if (_isInitFB && !_isLoadFriendFBComplete)
    {
        return false;
    }*/
    
    return true;
}

void CServerThread::UpdateTimeSynSaveToserver()
{
#if !GAME_SUPPORT
	FDGame*game = (FDGame*)(GAME()->_current_game);
    //if (IsLoadAllFriendCompelete())
    if (IsSynSaveComplete())
    {
        if (_timeDelaySynSaveToServer > 0)
        {
            _timeDelaySynSaveToServer -= NEW_DT();
            //printf("\n _timeDelaySynSaveToServer = %lld",_timeDelaySynSaveToServer);
            if (_timeDelaySynSaveToServer <= 0)
            {
#if defined (MAC_OS)
				//GAME()->_serverThread._isNetworkAvailable = checkServerRealIsAble();
				if (GAME()->_serverThread._isNetworkAvailable)
				{
					_timeDelaySynSaveToServer = CMath::RANDOM(TIME_DELAY_SYNSAVE_TO_SERVER, TIME_DELAY_SYNSAVE_TO_SERVER1);
					AddCommand(enumTypeCommand::COMMAND_TYPE_CHECK_STATUS);
					_isSynSaveComplete = false;
				}
#endif
            }
        }
    }
#endif
}

void CServerThread::GoToLoadingMain()
{
    FDGame*game = (FDGame*)(GAME()->_current_game);
    //reset server Thread
    for (int i = 0; i < MAX_COMMAND_IN_THREAD; i++)
    {
        _command[i]._typeCommand = enumTypeCommand::COMMAND_TYPE_NONE;
        _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_NONE;
    }
    
    _isNetworkAvailable = false;
    _isInitGC = false;
    _isLoadFriendGCComplete = false;
    _isInitFB = false;
    _isLoadFriendFBComplete = false;
    _isSynSaveComplete = false;
    _idStringLogin[0] = '\0';
    _userNameLogin[0] = '\0';
    _isFristCheckStatus = true;
    game->SetState(k_GS_LOGO);
    //game

}
/*wp8 android rule
COMMAND_TYPE_INIT_FB
	COMMAND_TYPE_CHECK_STATUS
		COMMAND_TYPE_CHECK_STATUS (STATE_PROCESSING) - CREATE NEW ACC
			
		COMMAND_TYPE_SYN_SAVE_TO_SERVER

		COMMAND_TYPE_SYN_SAVE_FROM_SERVER

		COMMAND_TYPE_SWAP_SAVE_FROM_SERVER

	COMMAND_TYPE_NETWORK_ERROR

*/
/**
    COMMAND_TYPE_INIT_GC
        COMMAND_TYPE_NETWORK_ERROR
        COMMAND_TYPE_CHECK_STATUS
            COMMAND_TYPE_SYN_SAVE_TO_SERVER
                COMMAND_TYPE_INIT_FB
            COMMAND_TYPE_SYN_SAVE_FROM_SERVER
                COMMAND_TYPE_INIT_FB
            COMMAND_TYPE_SWAP_SAVE_FROM_SERVER
                //reset
 
 
    COMMAND_TYPE_INIT_FB
        COMMAND_TYPE_LOAD_GC_FRIEND
            COMMAND_TYPE_LOAD_FB_FRIEND
                COMMAND_TYPE_CHECK_GC_ID_IN_SERVER
                    COMMAND_TYPE_CHECK_FB_ID_IN_SERVER
 
**/
extern "C" void OS_AdBoxDownloadFile();
void CServerThread::ServerRun()
{
	FDGame*game = (FDGame*)(GAME()->_current_game);
    
#if defined (MAC_OS)
	while (GAME() != NULL && !GAME()->_isExit)
#endif
	{
		for (int i = 0; i < MAX_COMMAND_IN_THREAD; i++)
		{			
			//PDEBUG("command %d\n_command[i]._stateCommand = %d", i, _command[i]._stateCommand );
			if (_command[i]._stateCommand == enumStateCommand::COMMAND_STATE_INIT)
			{
                //printf("\n_command[%d]._typeCommand = %d",i,_command[i]._typeCommand);

				switch (_command[i]._typeCommand)
				{
                    case CServerThread::COMMAND_TYPE_INIT_GC:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#if defined(BUILD_SUPPORT)

#endif
#else 
#if defined(MAC_OS)
                            if (_gcSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS || _gcSocial->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT)
                            {
                                _isInitGC = true;
                                sprintf(_userNameLogin, "%s", _gcSocial->GetUserName());
                                _gcSocial->GetUserIdString(_gcSocial->GetIdString(), _idStringLogin);
                                
								((PineGameCenterController*)_gcSocial)->LoadMyScore();

                                printf("\n already SAVE : id = %s, userName = %s ", game->_user.GetUserID(), game->_user.GetName());
                                EndCommand(i);
                                _isNetworkAvailable = true;
#if !GAME_SUPPORT
                                if(_isNetworkAvailable)
                                {
									if(strcmp(game->_user.GetUserID(),"user_id") != 0)
									{
										if(strcmp(game->_user.GetUserID(), _idStringLogin) != 0)
										{
											game->_user.UserDefault();
										}
									}
                                    AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_CHECK_STATUS);
                                }
                                else
                                {
									if(strcmp(game->_user.GetUserID(),"user_id") != 0)
									{
										if(strcmp(game->_user.GetUserID(), _idStringLogin) != 0)
										{
											game->_user.UserDefault();
											char name[128];
											char userid[128];
											sprintf(userid, "%s", _idStringLogin);
											sprintf(name, "%s", _userNameLogin);
											game->_user.SetUserID(userid);
											game->_user.SetUserName(name);
											game->_user.UserDataSave();
											game->SetState(k_GS_LOGO);
										}
									}
                                    EndCommand(i);
                                    AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_NETWORK_ERROR);
                                }
#else
                                sprintf(_idStringLogin, "%s",SUPPORT_GC_ID);
                                if(strcmp(game->_user.GetUserID(),SUPPORT_GC_ID) != 0)
                                {
                                    
#if !SUPPORT_CHECK_SAVE
                                    AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_SYN_SAVE_FROM_SERVER);
#endif
                                }
                                else
                                {
                                
#if SUPPORT_CHECK_SAVE
                                AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_SYN_SAVE_TO_SERVER);
#endif
                                }
                                
#endif
                            }
                            else if (_gcSocial->GetFlag() == SOCIAL_FLAG_ACCESS_DENINE)
                            {
                                printf("\n 00 GC denine");
                                EndCommand(i);
                                AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_NETWORK_ERROR);
                            }
#endif
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_INIT_FB:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
														
#else
                        UpdateStateInitTypeInitFB(i);
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_NETWORK_ERROR:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
#else
                        _isNetworkAvailable = false;
                        _isInitGC = false;
                        _isLoadFriendGCComplete = false;
                        _isInitFB = false;
                        _isLoadFriendFBComplete = false;
#endif
						EndCommand(i);
                        break;
                    case CServerThread::COMMAND_TYPE_CHECK_STATUS:
#if defined(BUILD_SUPPORT)

#else
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
#else
#if defined (MAC_OS)
                    {
                        EndCommand(i);
                    }
#endif
#endif
#endif
					
                        break;
                    case CServerThread::COMMAND_TYPE_SWAP_SAVE_FROM_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
#else
#if defined (MAC_OS)
                        /*if (SynSaveFromServer())
                        {
                            char name[128];
                            sprintf(name, "%s", _userNameLogin);
                            game->_user.SetUserName(name);
                            //getUniqueIDForDevice(game->_user._identityString);
                            game->_user.LoadFromBuffer();
                            game->_user.UserDataSave();
                            OS_DeleteFile(SAVE_BACKUP_FILE_NAME);
                            game->SetState(k_GS_LOGO);
                            
                        }*/
                        EndCommand(i);
                        GoToLoadingMain();
#endif
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_SYN_SAVE_FROM_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
#else
#if defined (MAC_OS)
                        /*if (SynSaveFromServer())
                        {
                            char name[128];
                            sprintf(name, "%s", _userNameLogin);
                            game->_user.SetUserName(name);
                            //getUniqueIDForDevice(game->_user._identityString);
                            game->_user.LoadFromBuffer();
                            game->_user.UserDataSave();
                            OS_DeleteFile(SAVE_BACKUP_FILE_NAME);
                            game->SetState(k_GS_LOGO);
                        }*/
                        EndCommand(i);
                        if (_isFristCheckStatus)
                        {
                            _isFristCheckStatus = false;
                        }
                        _isSynSaveComplete = true;
#endif
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_SYN_SAVE_TO_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)

#if defined(BUILD_SUPPORT)
								//notify
#else

#endif

#else
#if defined (MAC_OS)
                        printf("\n sync save");
                        //SynSaveToServer();
                        EndCommand(i);
                        if (_isFristCheckStatus)
                        {
                            _isFristCheckStatus = false;
                        }
                        _isSynSaveComplete = true;
#endif
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_LOAD_GC_FRIEND:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else

#endif
                        break;
                    case CServerThread::COMMAND_TYPE_LOAD_FB_FRIEND:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
#else
                        if (_isInitFB)
                        {
                            _fbSocial->LoadFriends();
                            _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_PROCESSING;
                        }
                        else
                        {
                            EndCommand(i);
                        }
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_CHECK_GC_ID_IN_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
                        
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_CHECK_FB_ID_IN_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						

#else
                        
#endif
                        
                        break;
                    case CServerThread::COMMAND_TYPE_DOWNLOAD_AVATAR:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
					
#else

                        UpdateStateInitTypeDownloadAvatar(i);

#endif
                        
                        break;
                    case COMMAND_TYPE_LOAD_FB_SCORE:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
#if defined (MAC_OS)
                        UpdateStateInitTypeLoadFBScore(i);
#endif
#endif
                        
                        break;
                    case COMMAND_TYPE_POST_FB_SCORE:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
#if defined (MAC_OS)
                        UpdateStateInitTypePostFBScore(i);
#endif
#endif
                        break;
					case CServerThread::COMMAND_TYPE_DOWNLOAD_AVATAR_VISIT_FRIEND:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
						
#endif
                        
						break;
                    case CServerThread::COMMAND_TYPE_GENERAL_TEXTURE_AVATAR:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
                        
#endif
                        
                        break;
                        
                    case CServerThread::COMMAND_TYPE_VISIT_RANDOM:
                    {
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
						
#endif
                        //char idTmp[128];
#if defined (MAC_OS)
                        
#endif
                        
                    }
                        
                        break;
                        
                    case CServerThread::COMMAND_TYPE_VISIT_FRIENDLIST:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
						
#endif
#if defined (MAC_OS)
                        EndCommand(i);
#endif
                        
                        break;
                    case CServerThread::COMMAND_TYPE_LOAD_PK_DATA:
					{
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)

#endif
					}break;
					case CServerThread::COMMAND_TYPE_SUBMIT_LEADERBOARD:
#if defined (MAC_OS)
                        PineGameCenterController::SubmitMyScore("Sloomy_01", game->_hight_score);
						EndCommand(i);
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_CHECK_SERVER_NETWORK:
                        printf("\n check server network");
#if defined (MAC_OS)
                        /*game->_offline_reward_collected = OFFLINE_RW_COLLECTED_TYPE_WAITING;
                        if (_gcSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS || _gcSocial->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT)
                        {
                            _gcSocial->GetUserIdString(_gcSocial->GetIdString(), _idStringLogin);
                        }
                        _isNetworkAvailable = SetServerTime(GAME()->_server_time);
                        if(GAME()->_processing_view_type == PROCESSING_VIEW_CHECK_NETWORK)
                        {
                            GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
                        }
                        printf("\n check server network 111, _isNetworkAvailable = %d",_isNetworkAvailable);
                        if(_isNetworkAvailable)
                        {
                            AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_CHECK_STATUS);
                        }
                        else
                        {
                            game->_offline_reward_collected = OFFLINE_RW_COLLECTED_TYPE_NONE;
                        }
                        printf("\n check server network 222, game->_offline_reward_collected = %d",game->_offline_reward_collected);
                        GAME()->_server_time.printDate();*/
                        EndCommand(i);
#endif
						break;
                        case CServerThread::COMMAND_TYPE_GET_INVITATION_CODE:
#if defined (MAC_OS)
                        /*if(GetInvitedId(game->_user._ivitation_code))
                        {
							game->_user._user_revision = 1;
                            game->_user.UserDataSave();
                            if(game->_user._invitation_reward_current > game->_user._invitation_num)
                            {
                                game->_user._invitation_reward_current = game->_user._invitation_num;
                            }
                        }*/
#endif
                        //GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
                        EndCommand(i);
                        break;
                    case CServerThread::COMMAND_TYPE_POST_INVITATION_CODE:
                    {
#if defined (MAC_OS)
                        /*int ret = InputInvitetionCode(game->_ivitation_code_input);
						GAME()->_processing_view_type = PROCESSING_VIEW_TYPE_NONE;
                        if(ret == 1)
                        {
                            char text[256];
                            sprintf(text, "%s", "Your code has been submitted");
                            game->_dialogMng.Add(DIALOG_CONTEXT_INVITATION_CONFIRM, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "Congratulations", text, 0, 0);
                        }
                        else if(ret == 2)
                        {
                            char text[256];
                            sprintf(text, "%s", "Invalid Code!");
                            game->_dialogMng.Add(DIALOG_CONTEXT_INVITATION_MESSAGE, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "ERROR", text, 0, 0);
                        }
                        else
                        {
                            char text[256];
                            sprintf(text, "%s", "Network connection is not available!");
                            game->_dialogMng.Add(DIALOG_CONTEXT_INVITATION_MESSAGE, DIALOGTYPE_OK, ZOOM_IN_OUT_TRANSACTION, "ERROR", text, 0, 0);
                        }*/
#endif
                    }
                        EndCommand(i);
                        break;
                    case CServerThread::COMMAND_TYPE_GET_NUM_INVITATION:
#if defined (MAC_OS)
                        //GetInvitionFriends();
						
#endif
                        EndCommand(i);
                        break;
                    default:
                        break;
                    
				}
			}
			else if (_command[i]._stateCommand == enumStateCommand::COMMAND_STATE_PROCESSING)
			{
				switch (_command[i]._typeCommand)
				{
                case CServerThread::COMMAND_TYPE_INIT_GC:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
					

#else
#if defined (MAC_OS)
                    if (_gcSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS || _gcSocial->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT)
                    {
                        _isInitGC = true;
						sprintf(_userNameLogin, "%s", _gcSocial->GetUserName());
						_gcSocial->GetUserIdString(_gcSocial->GetIdString(), _idStringLogin);

						((PineGameCenterController*)_gcSocial)->LoadMyScore();

						printf("\n already SAVE : id = %s, userName = %s ", game->_user.GetUserID(), game->_user.GetName());
						EndCommand(i);

						if(GAME()->_serverThread._isNetworkAvailable)
						{
							if(strcmp(game->_user.GetUserID(), _idStringLogin) != 0)
							{
								game->_user.UserDefault();
							}
							AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_CHECK_STATUS);
						}
						else
						{
							if (strcmp(game->_user.GetUserID(), _idStringLogin) != 0)
							{
								game->_user.UserDefault();
								char name[128];
								char userid[128];
								sprintf(userid, "%s", _idStringLogin);
								sprintf(name, "%s", _userNameLogin);
								game->_user.SetUserID(userid);
								game->_user.SetUserName(name);
								game->_user.UserDataSave();
								game->SetState(k_GS_LOGO);
							}
							EndCommand(i);
							AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_NETWORK_ERROR);
						}
                    }
                    else if (_gcSocial->GetFlag() == SOCIAL_FLAG_ACCESS_DENINE)
                    {
                        printf("\n GC denine");
                        EndCommand(i);
                        AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_NETWORK_ERROR);
                    }
#endif
#endif
                    break;
                case CServerThread::COMMAND_TYPE_INIT_FB:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
					
#if defined(BUILD_SUPPORT)
											GAME()->_support_network_color = 0xFFFF0000;
#else
                        
#endif
                        
#if defined(BUILD_SUPPORT)
								GAME()->_support_network_color = 0xFFFF0000;
#else
								
#endif
							
#else//MAC_OS
                        UpdateStateProcessingTypeInitFB(i);
#endif//OS detected
                    break;
				case CServerThread::COMMAND_TYPE_CHECK_STATUS:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)

#endif
					break;
				case CServerThread::COMMAND_TYPE_LOAD_GC_FRIEND:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
					
#endif
					break;
				case CServerThread::COMMAND_TYPE_LOAD_FB_FRIEND:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)

#else
					
#endif
					break;
				case CServerThread::COMMAND_TYPE_CHECK_GC_ID_IN_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
					
#endif
					break;	
				case CServerThread::COMMAND_TYPE_CHECK_FB_ID_IN_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
					
					
#else
					
#endif
					break;
				case CServerThread::COMMAND_TYPE_VISIT_RANDOM:{
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
					
					
#endif
				}break;
				case CServerThread::COMMAND_TYPE_LOAD_PK_DATA:{
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
					
#endif
				}break;
                    case CServerThread::COMMAND_TYPE_CHECK_SERVER_NETWORK:
#if defined (MAC_OS)
                        //_isNetworkAvailable = SetServerTime(GAME()->_server_time);
                        EndCommand(i);
#endif
                        break;
                    case CServerThread::COMMAND_TYPE_DOWNLOAD_AVATAR:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
                        
#else
                        
                        UpdateStateInitTypeDownloadAvatar(i);
                        
#endif
                        
                        break;
                    case COMMAND_TYPE_LOAD_FB_SCORE:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
#if defined (MAC_OS)
                        UpdateStateInitTypeLoadFBScore(i);
#endif
#endif
                        
                        break;
                    case COMMAND_TYPE_POST_FB_SCORE:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)
#else
#if defined (MAC_OS)
                        UpdateStateInitTypePostFBScore(i);
#endif
#endif
                        break;
                    default:
					break;
				}
			}			
		}
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) //|| defined(ANDROID_OS)

#else
		CGame::_this->Sleep(20);
#endif
		
	}
}


void CServerThread::VisitPeople(bool IsRandom)
{
    if(IsRandom)
    {
        
    }
    else
    {
        
    }
}

void CServerThread::UpdateStateInitTypeDownloadAvatar(int i)
{
#if defined(MAC_OS)
    FDGame *game = (FDGame*)GAME()->_current_game;
    
#if ADD_FACEBOOK
    GAME()->_friendManager.UpdateDownloadAvatar();
    if (game->_user._avatarState == AVATAR_STATE_NONE)
    {
        game->_user._avatarState = AVATAR_STATE_SAVE_AS;
        char filename[256];
        
        sprintf(filename, USER_AVATAR_FILE_NAME, game->_user.GetFBID());
        if (strlen(game->_user.GetAvatarURL()) > 0)
        {
            PineSocial::DownloadAvatar(game->_user.GetAvatarURL(), filename, USER_AVATAR_WW, USER_AVATAR_WW);
        }
        if (ExitsFile(filename))
        {
            game->_user._avatarState = AVATAR_STATE_LOADING_BINARY;
        }
        else
        {
            game->_user._avatarState = AVATAR_STATE_INIT_FAILED;
        }
    }
    AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_GET_TOP_CURRENT);
#endif
#endif
    EndCommand(i);
}

void CServerThread::UpdateStateProcessingTypeInitFB(int i)
{
    FDGame *game = (FDGame*)GAME()->_current_game;
    if(!GAME()->_device.IsNetworkAvailable())
    {
        printf("\n network:%d",GAME()->_device.IsNetworkAvailable());
        return ;
    }
    _timeOutFB--;
    if (_timeOutFB <= 0)
    {
        //SetTimeOutRequest();
    }
#if ADD_FACEBOOK
#if defined(MAC_OS)
    if (_fbSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
    {
        if (_fbSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS)
        {
            _fbSocial->GetUserIdString(_fbSocial->GetUserScopeID(), game->_user._user_fb_id);
            _fbSocial->GetUserIdString(_fbSocial->GetUserScopeID(), _idLogin);
            
            char avatarURL[1024];
            sprintf(avatarURL, "%s", _fbSocial->GetURLAvatar());
            game->_user.SetAvatarURL(avatarURL);
            //Name facebook unicode
            for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
            {
                _userNameUnicode_FB[c] = _fbSocial->_unicodeUserName[c];
            }
            _userNameUnicodeLenght_FB = _fbSocial->_currentUnicodeUserNameLength;
            
            for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
            {
                game->_user._userNameUnicode[c] = _userNameUnicode_FB[c];
            }
            game->_user._userNameUnicodeLenght = _userNameUnicodeLenght_FB;
            
            _isInitFB = true;
            game->_user._hadLoginFB = true;
            printf("\n url = %s,fbid=%s",avatarURL,game->_user._user_fb_id);
            game->_user.UserDataSave();
            
            printf("\n new fb session");
            GAME()->_friendManager.InitTopPlayer();
            //GAME()->_friendManager._numTopPlayer = 0;
            _fb_session = std::shared_ptr<CFacebookSession>(new CFacebookSession);
            _fb_app = std::shared_ptr<CFacebookApplication>(new CFacebookApplication);
            _fb_app->AppId = FACEBOOK_APP_ID;
            _fb_token = OS_GetFacebookToken();
            _fb_session->setToken(_fb_token);
            _fb_game_service = Pine::Social::Facebook::v2_5::CGameService::get(_fb_app, _fb_session);
            
            GAME()->_fb_just_init = true;
            EndCommand(i);
            //AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_FRIEND);
            AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_SCORE);
            
        }
        else if (_fbSocial->GetFlag() == SOCIAL_FLAG_ACCESS_DENINE || _fbSocial->GetFlag() == SOCIAL_FLAG_NO_ACCOUNT_LOGIN)
        {
            //SetTimeOutRequest();
            printf("\n fb denine \n");
            EndCommand(i);
            game->_user._hadLoginFB = false;
        }
    }
#endif
#endif
}

void CServerThread::UpdateStateInitTypeInitFB(int i)
{
#if defined(MAC_OS)
    FDGame *game = (FDGame*)GAME()->_current_game;
#if ADD_FACEBOOK
    if(!GAME()->_device.IsNetworkAvailable())
    {
        printf("\n network:%d",GAME()->_device.IsNetworkAvailable());
        return ;
    }
    if (!game->_user._hadLoginFB || _fb_game_service == nullptr)
    {
        if(_fbSocial == NULL)
        {
            _fbSocial = new PineFacebookController();
        }
        game->_user._hadLoginFB=false;
        _fbSocial->Init();
        _timeOutFB = 60 * 60;
        _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_PROCESSING;
        GAME()->_fb_just_init = false;
    }
    else
    {
        _timeOutFB--;
        if (_timeOutFB<=0)
        {
            //SetTimeOutRequest();
        }
        if (_fbSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
        {
            if (_fbSocial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS)
            {
                _fbSocial->GetUserIdString(_fbSocial->GetUserScopeID(), game->_user._user_fb_id);
                _fbSocial->GetUserIdString(_fbSocial->GetUserScopeID(), _idLogin);
                
                char avatarURL[1024];
                sprintf(avatarURL, "%s", _fbSocial->GetURLAvatar());
                game->_user.SetAvatarURL(avatarURL);
                //Name facebook unicode
                for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
                {
                    _userNameUnicode_FB[c] = _fbSocial->_unicodeUserName[c];
                }
                _userNameUnicodeLenght_FB = _fbSocial->_currentUnicodeUserNameLength;
                
                for (int c = 0; c < SAVE_CHARACTER_NAME_LENGTH_CATCH; c++)
                {
                    game->_user._userNameUnicode[c] = _userNameUnicode_FB[c];
                }
                game->_user._userNameUnicodeLenght = _userNameUnicodeLenght_FB;
                
                _isInitFB = true;
                game->_user._hadLoginFB = true;
                printf("\n url = %s,fbid=%s",avatarURL,game->_user._user_fb_id);
                game->_user.UserDataSave();
                
                printf("\n new fb session");
                GAME()->_friendManager.InitTopPlayer();
                //GAME()->_friendManager._numTopPlayer = 0;
                _fb_session = std::shared_ptr<CFacebookSession>(new CFacebookSession);
                _fb_app = std::shared_ptr<CFacebookApplication>(new CFacebookApplication);
                _fb_app->AppId = FACEBOOK_APP_ID;
                _fb_token = OS_GetFacebookToken();
                _fb_session->setToken(_fb_token);
                _fb_game_service = Pine::Social::Facebook::v2_5::CGameService::get(_fb_app, _fb_session);

                GAME()->_fb_just_init = true;
                EndCommand(i);
                AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_SCORE);
                //AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_FRIEND);
                
                
            }
            else if (_fbSocial->GetFlag() == SOCIAL_FLAG_ACCESS_DENINE || _fbSocial->GetFlag() == SOCIAL_FLAG_NO_ACCOUNT_LOGIN)
            {
                //SetTimeOutRequest();
                printf("\n fb denine \n");
                EndCommand(i);
                game->_user._hadLoginFB = false;
            }
        }
    }
#endif
#endif
}


void CServerThread::UpdateStateInitTypeLoadFBScore(int i)
{
    FDGame *game = (FDGame*)GAME()->_current_game;
#if defined(MAC_OS)

#if ADD_FACEBOOK
    if(!GAME()->_device.IsNetworkAvailable() || !game->_user._hadLoginFB)
    {
        return ;
    }
    /*if (!game->_user._hadLoginFB || _fb_game_service == nullptr)
    {
        if(_fbSocial == NULL)
        {
            _fbSocial = new PineFacebookController();
        }
        
        _fbSocial->Init();
        _timeOutFB = 60 * 60;
        _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_PROCESSING;
    }
    else*/ if(game->_user._hadLoginFB)
    {
        printf("\n load fb friend score");
        
        
        //char token_str[128];
        //int lenght = sprintf(token_str, "%s", _fb_token.c_str());
        //PDEBUG("get score token:%s, lenght = %d\n", token.c_str(),lenght);
        try{
            std::vector<CFacebookScoreItem> list_score = _fb_game_service->getFriendsScore();
            if(list_score.size() > 1)
            {
                PDEBUG("friends score:\n");
            }
            for(int i = 0; i< list_score.size(); i++)
            {
                CFacebookScoreItem item = list_score[i];
                //PDEBUG("\n id:%s name:%s score:%d\n", item.Id.c_str(), std::string(item.UserName.begin(), item.UserName.end()).c_str(), item.Score);
                char avatar_url[1024];
                char url_download[1024];
                sprintf(url_download, "https://graph.facebook.com/%s/picture?width=200&height=200&access_token=%s",item.Id.c_str(),_fb_token.c_str());
                //sprintf(fileName, "%s",item.Id.c_str());
                //OS_DownloadFile(url_download,fileName,1);
                //sprintf(avatar_url, "%s", GAME()->_serverThread._fbSocial->GetUserAvartarURL(item.Id.c_str()));
                sprintf(avatar_url, "%s", url_download);
                AccountInfo userInfo;
                int lenght_id = sprintf(userInfo._id, "FB_%s", item.Id.c_str());
                
                int c = 0;
                std::wstring::iterator it = item.UserName.begin();
                while (it != item.UserName.end())
                {
                    userInfo._userNameUnicode[c++] = *it++;
                }
                
                userInfo._userNameUnicodeLenght = c;
                
                for (int j = c; j< SAVE_CHARACTER_NAME_LENGTH_CATCH; j++)
                {
                    userInfo._userNameUnicode[j] = '\0';
                }
                
                userInfo._userNameUnicodeLenght = c;
                //int lenght = sprintf(userInfo._userName, "%s", std::string(item.UserName.begin(), item.UserName.end()).c_str());
                for(int i = 0; i< userInfo._userNameUnicodeLenght;i++)
                {
                    //printf("\n %d : %d",i, userInfo._userNameUnicode[i]);
                    if(userInfo._userNameUnicode[i] < 32 || userInfo._userNameUnicode[i] > 126)
                    {
                        userInfo._userName[i] = '?';
                    }
                    else
                    {
                        userInfo._userName[i] = userInfo._userNameUnicode[i];
                        printf(" -> %c", userInfo._userName[i]);
                    }
                }
                //printf("\n fbid : %s",game->_user.GetFBID());
                
                userInfo._userName[userInfo._userNameUnicodeLenght] = '\0';
                //printf("\n userName[%d] = %s ",i,userInfo._userName);
                //printf("\n avatar_url: %s \n",avatar_url);
                sprintf(userInfo._avatarUrl, "%s", avatar_url);
                userInfo._rank = i + 1;
                userInfo._score = item.Score;
                //userInfo._score = CMath::RANDOM(100, 200);
                if(strcmp(userInfo._id,game->_user.GetFBID())==0)
                {
                    userInfo._score = (int)game->_hight_score;
                }
                //printf("\n lenght_id = %d ",lenght_id);
                //printf("\n _numTopPlayer = %d",GAME()->_friendManager._numTopPlayer);
                if(lenght_id > 3)
                {
                    GAME()->_friendManager.AddTopPlayer(userInfo);
                    GAME()->_friendManager.AddFriend(userInfo);
                }
                
                //printf("\n f_avatar_url = %s",avatar_url);
            
            }
            GAME()->_friendManager.SortFriend();
            if(GAME()->_fb_just_init)
            {
                AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_DOWNLOAD_AVATAR);
                GAME()->_fb_just_init = false;
            }
        }
        catch(Pine::Social::Facebook::CFacebookException fb_ex)
        {
            PDEBUG("error:%s\n", fb_ex.what());
        }catch(std::exception ex){
            PDEBUG("error:%s\n", ex.what());
            
        }
    }
#endif
#endif
    
    EndCommand(i);
    _is_create_service = true;
}
void CServerThread::UpdateStateProcessingTypeLoadFBScore(int i)
{
    
}
void CServerThread::UpdateStateInitTypePostFBScore(int i)
{
#if defined(MAC_OS)
#if ADD_FACEBOOK
    FDGame *game = (FDGame*)GAME()->_current_game;
    if(!GAME()->_device.IsNetworkAvailable() || !game->_user._hadLoginFB)
    {
        return ;
    }
    
    /*if (!game->_user._hadLoginFB || _fb_game_service == nullptr)
    {
        if(_fbSocial == NULL)
        {
            _fbSocial = new PineFacebookController();
        }
        
        _fbSocial->Init();
        _timeOutFB = 60 * 60;
        _command[i]._stateCommand = enumStateCommand::COMMAND_STATE_PROCESSING;
    }
    else*/ if(game->_user._hadLoginFB)
    {
        //_timeOutFB--;
        //_fb_game_service->putScore((int)game->_hight_score);
        if( _fb_game_service != nullptr)
        {
            try
            {

                //PDEBUG("delete score:%s\n", delete_status? "success": " fail");
                _fb_game_service->putScore((int)game->_hight_score);
                //AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_LOAD_FB_SCORE);
                //std::vector<CFacebookScoreItem> my_scores = _fb_game_service->getScore();

            }
            catch(Pine::Social::Facebook::CFacebookException ex)
            {
                PDEBUG("error:%s\n", ex.what());
            }
            catch(std::exception ex){
                PDEBUG("error:%s\n", ex.what());
                
            }
        }
        else
        {
            //not init facebook yet
        }

    }
#endif
#endif
    EndCommand(i);
}