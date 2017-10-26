#ifndef _C_FLOWER_H_
#define _C_FLOWER_H_

class CFlower
{
public:
	CFlower();
	~CFlower();
	int _level;
	int _xp;
	int _id;
	int _xp_need;
	int _xp_collected;
	int _xp_max;
	CAnimObject _anim;
	void Init(int id, int level, int xp);
	void LevelUp();
	void Render(float x, float y);
	void Update();
	void AddXP(int xp);
private:

};

#endif