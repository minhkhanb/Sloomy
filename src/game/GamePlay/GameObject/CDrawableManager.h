#if !_DRAWABLE_MANAGER_H
#define _DRAWABLE_MANAGER_H (1)
#define DRAWABLE_POOL_MAX (200)


class CAnimObject;
class CDrawableManager
{
	CAnimObject* _anim_list_static[DRAWABLE_POOL_MAX];
	float _posX[DRAWABLE_POOL_MAX];
	float _posY[DRAWABLE_POOL_MAX];
	int _ids[DRAWABLE_POOL_MAX];
	float _scale[DRAWABLE_POOL_MAX];
	int _num;
public: 
	
	void Add(CAnimObject* object, float posX, float posY, float scale);
	void Update();
	void Render(float x,float y);
	void Init();
};


#endif