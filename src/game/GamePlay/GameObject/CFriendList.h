#ifndef _C_FRIEND_LIST_
#define _C_FRIEND_LIST_
class CFriendList
{
public:
	CFriendList();
	~CFriendList();
	float _x, _y;
	char _name[128];
	char _avatar_url[256];
	int _score;
	void Init(float x, float y, char name, char url, int score);
	void Render(float x, float y, int opac);
	void Update();
private:

};


#endif
