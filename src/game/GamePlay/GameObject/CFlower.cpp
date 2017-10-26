#include "pch.h"
#include "../../GameCore/CGame.h"

CFlower::CFlower()
{
}

CFlower::~CFlower()
{
}
void CFlower::Init(int id, int level, int xp)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_id = id;
	_level = level;
	_xp = xp;
	int param = CGameDesign::FileGeneral_design::CSheetFlower_Design::Number_of_Flower;
	_xp_max = game->GetDesignFlowerLevel(_level, param) - game->GetDesignFlowerLevel(_level-1, param);
	if (_id == CHARACTER_TYPE_SUNFLOWER)
	{
		int ratio = game->GetDesignGeneral(FD_GENERAL_DESIGN_SUN_LEVEL);
		_xp_max *= (ratio / 100.0f);
	}
	_xp_need = _xp_max - _xp;
	_xp_collected = 0;
	printf("\n _id = %d,_xp_max = %d,level=%d", _id, _xp_max,_level);
}
void CFlower::Render(float x, float y)
{
	FDGame* game = (FDGame*)(GAME()->_current_game);
	/*_anim.SetPos(x,  y);
	G()->SetScale(_scale, _scale, x, y);
	_anim.DrawAnimObject(G());
	G()->ClearScale();
	G()->SetOpacity(opac);*/
}
void CFlower::Update()
{

}
void CFlower::LevelUp()
{
	if (_id == CHARACTER_TYPE_SUNFLOWER)
	{
		return;
	}
	FDGame* game = (FDGame*)(GAME()->_current_game);
	_level++;
	int param = CGameDesign::FileGeneral_design::CSheetFlower_Design::Number_of_Flower;
	_xp_max = game->GetDesignFlowerLevel(_level, param) - game->GetDesignFlowerLevel(_level - 1, param);
	if (_id == CHARACTER_TYPE_SUNFLOWER)
	{
		int ratio = game->GetDesignGeneral(FD_GENERAL_DESIGN_SUN_LEVEL);
		_xp_max *= (ratio / 100.0f);
	}
	_xp = 0;
	_xp_need = _xp_max - _xp;
}
void CFlower::AddXP(int xp)
{
	if (_id == CHARACTER_TYPE_SUNFLOWER)
	{
		return;
	}
	_xp += xp;
	_xp_need -= xp;
	if (_xp_need < 0)
	{
		_xp_need = 0;
		printf("\n game bug \n game bug\n game bug\n game bug\n game bug\n game bug\n game bug\n game bug");
	}
}