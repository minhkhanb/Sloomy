#ifndef _C_EFFECT_EXPLO_H_
#define _C_EFFECT_EXPLO_H_

#define PARTICLE_EXPLOSION_NUM_MAX		200
#define EXPLOSION_TYPE_DEFAULT		0
#define EXPLOSION_TYPE_FALL			1
#define EXPLOSION_TYPE_CIRCLE		2
class EffectExplo
{
public:
	EffectExplo();
	~EffectExplo();
	float _x[PARTICLE_EXPLOSION_NUM_MAX], _y[PARTICLE_EXPLOSION_NUM_MAX];
	int _opac[PARTICLE_EXPLOSION_NUM_MAX];
	int _frame[PARTICLE_EXPLOSION_NUM_MAX];
	float _scale[PARTICLE_EXPLOSION_NUM_MAX];
	float _speed[PARTICLE_EXPLOSION_NUM_MAX];
	float _speed_scale[PARTICLE_EXPLOSION_NUM_MAX];
	int _state[PARTICLE_EXPLOSION_NUM_MAX];
	int _angle[PARTICLE_EXPLOSION_NUM_MAX];
	int _time_hold[PARTICLE_EXPLOSION_NUM_MAX];
	float _scrolly[PARTICLE_EXPLOSION_NUM_MAX];
	float _speed_scroll[PARTICLE_EXPLOSION_NUM_MAX];
	int _spriteid;
	int _type;
	void Init(int spriteid,int type);
	int _num;
	int _size;
	CAnimObject _anim_effect_fire;
	void Active(int type, float x, float y, float speed, float speedscale, int num, int frame, int numframe, int timehold, int size = 0);
	void Render(float x, float y, int opac);
	void Update();
	bool IsEnd();
private:

};


#endif
