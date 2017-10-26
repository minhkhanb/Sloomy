#ifndef _C_POWERUP_
#define _C_POWERUP_
class CPowerUp
{
public:
	CPowerUp();
	~CPowerUp();
	float _x, _y;
	int _id;
	int _energy;
	int _energy_max;
	int _num;
	Button _button;
	void Reset();
	void Init(int id, int energy,int num, float x,float y);
	void Render(float x, float y);
	void Update();
	bool UpdateTouch();
	bool _has_actived;
	void ActivedFunc(bool cancel);
	float _scale;
	float _speed_scale;
	float _scale_max;
	float _scale_min;
	int _state_text_scale;
	void InitScale();
	void ActiveScale(float scale, float scalemax, float scalemin, float speedscale,int scalestate);
	void UpdateScale();
	float _clip_current;
	float _clip_target;
	bool _just_full;
	//EffectExplo _effect_explo;
	//CAnimObject _icon;
	CAnimObject _anim_energy_full;
private:

};


#endif
