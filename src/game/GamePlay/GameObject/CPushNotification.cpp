#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

void CPushNotification::AddNotification(BYTE type)
{
    FDGame*game = (FDGame*)(GAME()->_current_game);
    INT_TIME no_time = 0;
	bool addPush = false;
	switch (type)
	{
	case CPushNotification::NOTIFICATION_FREE_GIFT:
			
           /* no_time = game->_user._free_gift_time - time(NULL);
            if(no_time > 10)
            {
                no_time += 60;
                addPush = true;
                sprintf(_notifiction[_numNotification]._text, "Your treasure is ready!");
            }*/
			
		break;
        case CPushNotification::NOTIFICATION_COME_BACK_1DAYS:
            no_time = 1*24*60*60;
            addPush = true;
            sprintf(_notifiction[_numNotification]._text, "Your flowers are ready to collect.");
			no_time += 60;
            break;
        case CPushNotification::NOTIFICATION_COME_BACK_2DAYS:
            no_time = 2*24*60*60;
            addPush = true;
            sprintf(_notifiction[_numNotification]._text, "Come back, your flowers miss you.");
			no_time += 60;
            break;
		case CPushNotification::NOTIFICATION_COME_BACK_5DAYS:
			no_time = 5 * 24 * 60 * 60;
			addPush = true;
			sprintf(_notifiction[_numNotification]._text, "Come back, your flowers need you now.");
			no_time += 60;
            break;
		case CPushNotification::NOTIFICATION_COME_BACK_10DAYS:
			no_time = 10 * 24 * 60 * 60;
			addPush = true;
			sprintf(_notifiction[_numNotification]._text, "Claim your 80k coins & 30 gems, a special relic for Free! Battle Now!");
			no_time += 60;
			break;
	}
	if (addPush)
	{
		_notifiction[_numNotification]._type = type;
		_notifiction[_numNotification]._timePush = no_time;
		//printf("\n Notification[%d]: %lld => %s -- type = %d", _numNotification, _notifiction[_numNotification]._timePush, _notifiction[_numNotification]._text,_notifiction[_numNotification]._type);
		_numNotification++;
	}	
}
bool CPushNotification::HasNotification(BYTE type)
{
	for (int i = 0; i < _numNotification; i++)
	{
		if (_notifiction[i]._type == type)
		{
			return true;
		}
	}
	return false;
}
INT_TIME CPushNotification::GetMaxTimePush(BYTE type)
{
	INT_TIME max = 0;
	for (int i = 0; i < _numNotification; i++)
	{
		if (_notifiction[i]._type == type)
		{
			if (max < _notifiction[i]._timePush)
			{
				max = _notifiction[i]._timePush;
			}
		}
	}
	return max;
}
void CPushNotification::PushNotification()
{
	_numNotification = 0;
	AddNotification(CPushNotification::NOTIFICATION_COME_BACK_1DAYS);
	AddNotification(CPushNotification::NOTIFICATION_COME_BACK_2DAYS);
	AddNotification(CPushNotification::NOTIFICATION_COME_BACK_5DAYS);

}

void CPushNotification::Reset()
{
    for (int i = 0; i < _numNotification; i++)
    {
        _notifiction[i]._timePush = 0;
        //_notifiction[i]._type = -1;
    }
    _numNotification = 0;
}