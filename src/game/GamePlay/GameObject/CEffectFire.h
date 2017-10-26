#ifndef _EFFECT_FIRE_
#define _EFFECT_FIRE_

#define NUM_MAX_PARTICAL_FIRE			100

#define EFFECT_FIRE_STATE_NORMAL		0
#define EFFECT_FIRE_STATE_START			1
#define EFFECT_FIRE_STATE_END			2
#define EFFECT_FIRE_STATE_FREE			3

#include "../../Lib/DataType/CType.h"

class CEffectFire
{
public:
	CEffectFire();
	~CEffectFire();
	float _x[NUM_MAX_PARTICAL_FIRE], _y[NUM_MAX_PARTICAL_FIRE];
	float _speed_delta[NUM_MAX_PARTICAL_FIRE];
	float _speed_opac[NUM_MAX_PARTICAL_FIRE];
	float _speed_rotate[NUM_MAX_PARTICAL_FIRE];
	float _speed_scale[NUM_MAX_PARTICAL_FIRE];
	float _scale[NUM_MAX_PARTICAL_FIRE];
	float _angle[NUM_MAX_PARTICAL_FIRE];
	float _rotate[NUM_MAX_PARTICAL_FIRE];
	float _opac[NUM_MAX_PARTICAL_FIRE];
	float _speed[NUM_MAX_PARTICAL_FIRE];
	int _state[NUM_MAX_PARTICAL_FIRE];
	int _sprite;
	int _frame[NUM_MAX_PARTICAL_FIRE];
	int _status;
	int _num;
	bool _is_circle;
	float _scale_max;
	float _centerx, _centery;
	void Init(int sprite, int frame, int numframe, int num, float scale, bool circle,float min_speed);
	void Active(float centerx, float centery, float speed, float speedopac, float speedrot, float speedscale, float scalemax, float opac);
	void Render(float x, float y, int opac);
	void Update();
	void ForceEnd();
	bool IsFree();
	float _min_speed;

private:

};


#endif;