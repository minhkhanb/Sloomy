#ifndef CDIALOG_H
#define CDIALOG_H

#define DIALOGTYPE_YESNO (1<<0)
#define DIALOGTYPE_OK	(1<<1)

#define STATE_NOTUSE	(0)
#define STATE_OPEN		(1)
#define STATE_NORMAL	(2)
#define STATE_CLOSE		(3)

#define ZOOM_IN_OUT_TRANSACTION (0)
#define OPACITY_TRANSACTION (1)



#include "../GameObject/MenuSystem.h"

class CDialog
{
	Button _yesno[2];
	Button _ok;
	int _context;
	int _state;
	float _x;
	float _y;
	int _type;
	char _title[256];
	char _body[256];
	int	_typetransaction;
	
	float _scale;
	float _cenX;
	float _cenY;

	float _opa;
	

public:
	CDialog();
	void Init(int context,int type,int typetransaction, const char* title, const char* body, float x, float y);
	void Update();
	void Render(float x,float y);
	int GetState();
	void SetState(int state);
	void UpdateZoomTransaction();
	void UpdateOpacityTransaction();
	bool UpdateTouchYesNo();
	bool UpdateTouchOK();
	int _main_opac;
};
#endif