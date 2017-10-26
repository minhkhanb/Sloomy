#ifndef _C_CHARACTER_H_
#define _C_CHARACTER_H_

#define CHARACTER_TYPE_NULL				-1

#define CHARACTER_TYPE_SUNFLOWER		0
#define CHARACTER_TYPE_LILY				1
#define CHARACTER_TYPE_VOILET			2
#define CHARACTER_TYPE_LOTUS			3
#define CHARACTER_TYPE_ROSE				4
#define CHARACTER_TYPE_ORCHILD			5



#define CHARACTER_STATE_NORMAL			0
#define CHARACTER_STATE_DISABLE			1
#define CHARACTER_STATE_ACTIVE			2
#define CHARACTER_STATE_WAITING			3
#define CHARACTER_STATE_MOVING			4
#define CHARACTER_STATE_FIRE			5
#define CHARACTER_STATE_EMPTY			6
#define CHARACTER_STATE_HIDE			7
#define CHARACTER_STATE_TOUCH_IT		8

#define CHARACTER_SCALE_STATE_HIDE			0
#define CHARACTER_SCALE_STATE_NORMAL		1
#define CHARACTER_SCALE_STATE_OUT			2
#define CHARACTER_SCALE_STATE_IN			3
#define CHARACTER_SCALE_STATE_TO_NORMAL		4


#define CHARACTER_SIZE_NORMAL			0
#define CHARACTER_SIZE_REGULAR			1
#define CHARACTER_SIZE_BIG				2


class CCharacter
{
public:
	CCharacter();
	~CCharacter();
	CAnimObject _anim;
	void Init(float x, float y);
	void CreateNew(int type, int timehold,float speedscale);
	void Modify(CCharacter *flower);
	void Modify(int type);
	void Render(float x, float y, int opac);
	void AddRender(float x, float y, int opac);
	void Update();
	void SetPos(float x, float y);
	void SetType(int type);
	void SetState(int state);
	int GetState();
	int GetType();
	float GetPosX();
	float GetPosY();
	int GetSpriteID();
	bool UpdateTouch();
	int _touch_id;
	void Fire(bool spblast);
	int _disable_hold_time;
	float _scale;
	float _scale_max;
	float _scale_min;
	int _scale_state;
	float _speed_scale;
	void SetScaleState(int state);
	int GetSize();
	void SetSize(int size);
	void SetSpriteID(int sprite);
	float _tarx,_tary;
	float _speed;
	void SetTar(float x, float y);
	float GetTarX();
	float GetTarY();
	int _xp_collected;
	int _xp;
	int _size_tar;
	void SetSizeTar(int size);
private:
	int _type;
	int _state;
	float _x, _y;
	int _spriteid;
	int _size;
};

#endif;