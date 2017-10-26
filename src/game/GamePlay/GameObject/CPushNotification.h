
#ifndef _PUSH_NOTIFICATION_H
#define _PUSH_NOTIFICATION_H

#define MAX_PUSH_NOTIFICATION		100


struct Notification
{
	char _text[1024];
	INT_TIME _timePush;
	BYTE _type;
};


class  CPushNotification
{
public:

	int _numNotification;
	enum NotificationType
	{
		NOTIFICATION_FREE_GIFT,
		NOTIFICATION_COME_BACK_1DAYS,
        NOTIFICATION_COME_BACK_2DAYS,
		NOTIFICATION_COME_BACK_5DAYS,
		NOTIFICATION_COME_BACK_10DAYS,
	}; 
	Notification _notifiction[MAX_PUSH_NOTIFICATION];
	void AddNotification(BYTE type);
	void PushNotification();
	bool HasNotification(BYTE type);
	INT_TIME GetMaxTimePush(BYTE type);
	void Init(){ _numNotification = 0; };
    void Reset();
};

#endif


