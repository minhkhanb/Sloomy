#ifndef _C_BUTTERFLY_H_
#define _C_BUTTERFLY_H_
class CButterFly
{
public:
	CButterFly();
	~CButterFly();
	float _x, _y;
	float _fly_y;
	float _tarx, _tary;
	float _speed;
	float _scale;
	float _scale_speed;
	CAnimObject _anim;
	int _state;
	int _animid;
	int _count_frame_change_orien;
	int _spriteid;
	void Init(int spriteid, int anim);
	void Active(float x, float y, float speed, float scale, float scalespeed);
	void Render(float x, float y, int opac);
	void Update();
	bool UpdateTouch();
private:

};

#endif