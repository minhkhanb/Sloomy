#ifndef CDIALOG_MANAGER_H
#define CDIALOG_MANAGER_H
#include "CDialog.h"
#define MAX_DIALOG_NUMS	(20)
class CDialogManager
{
public:
	int _opac;
	CDialog _dialog[MAX_DIALOG_NUMS];
	void Update();
	void Render(float x, float y);
	void Add(int contexrt,int type,int typetransaction, const char* title, const char* body, float x, float y);
	bool isOpenDialog();
	void ForceClose();
};
#endif