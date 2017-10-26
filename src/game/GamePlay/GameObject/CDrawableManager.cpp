#include "pch.h"
#include "../../GameCore/CGame.h"

void CDrawableManager::Add(CAnimObject* object, float posX, float posY, float scale)
{
	_anim_list_static[_num] = object;
	_posX[_num] = posX;
	_posY[_num] = posY;
	_scale[_num] = scale;
	int index = _num;
	for(int i = 0; i < _num; i++)
	{
		int node = _ids[i];
		if(posY < _posY[node])
		{
            index = i;
			if(!(_posY[node] == posY && _posX[node] < posX))
			{
				index = i;
				break;
			}
		}
	}
	//printf("\n type[%d] = %d",_num, type);
	for(int i = _num-1; i >= index; i--)
	{
		_ids[i+1] = _ids[i];
		
	}
	_ids[index] = _num;
	_num++;
	
}
void CDrawableManager::Update()
{
}
void CDrawableManager::Render(float x, float y)
{
	FDGame *game = (FDGame*)GAME()->_current_game;
	float posX = 0;
	float posY = 0;
	int index = 0;
	//printf("\n_num  = %d",_num);
	for(int i = 0; i < _num; i++)
	{
		 index = _ids[i];
		 posX = _posX[index] + x;
		 posY = _posY[index] + y;
		 _anim_list_static[index]->SetPos(posX, posY);
		 G()->SetScale(_scale[index], _scale[index], posX, posY);
		 _anim_list_static[index]->DrawAnimObject(G());
		 G()->ClearScale();
	}
}
void CDrawableManager::Init()
{
	_num = 0;
}