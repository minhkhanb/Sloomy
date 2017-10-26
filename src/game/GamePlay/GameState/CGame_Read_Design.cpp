#include "pch.h"
#include "../../GameCore/CGame.h"

const int FDGame::_character_sprite[CHARACTER_NUM_MAX]=
{
	SPRITEID_CHARACTER_SUNFLOWER,
	SPRITEID_CHARACTER_SUNFLOWER_02,
	SPRITEID_CHARACTER_SUNFLOWER_03,
	SPRITEID_CHARACTER_LILY,
	SPRITEID_CHARACTER_LILY_02,
	SPRITEID_CHARACTER_LILY_03,
	SPRITEID_CHARACTER_VIOLET,
	SPRITEID_CHARACTER_VIOLET_02,
	SPRITEID_CHARACTER_VIOLET_03,
	SPRITEID_CHARACTER_LOTUS,
	SPRITEID_CHARACTER_LOTUS_02,
	SPRITEID_CHARACTER_LOTUS_03,
	SPRITEID_CHARACTER_ROSE,
	SPRITEID_CHARACTER_ROSE_02,
	SPRITEID_CHARACTER_ROSE_03,
	SPRITEID_CHARACTER_ORCHILD,
	SPRITEID_CHARACTER_ORCHILD_02,
	SPRITEID_CHARACTER_ORCHILD_03,

};

int FDGame::GetDesignBeginning(int id, int param)
{
	return _game_design.General_design.SheetBeginning.int32Value.getValue(id, param);
}
int FDGame::GetDesignFlowerPercent(int id, int param)
{
	return _game_design.General_design.SheetPercent_Spawn.int32Value.getValue(id, param);
}
int FDGame::GetDesignSpawPercent(int id, int param)
{
	return _game_design.General_design.SheetFlower_Respawn.int32Value.getValue(id, param);
}
int FDGame::GetDesignFlowerLevel(int level, int param)
{
	if (level < 0 )
	{
		return 0;
	}
	/*if (level >= _game_design.General_design.SheetFlower_Design.int32Value._totalData)
	{
		level = _game_design.General_design.SheetFlower_Design.int32Value._totalData - 1;
	}*/
	return _game_design.General_design.SheetFlower_Design.int32Value.getValue(level, param);
}
float FDGame::GetDesignGeneral(int id)
{
	return _game_design.General_design.SheetGeneral.floatValue.getValue(id,0);
}
int FDGame::GetDesignFreeGiftTime(int id, int param)
{
	return _game_design.General_design.SheetTreasure_Time.int32Value.getValue(id, param);
}
int FDGame::GetDesignFreeGiftRW(int id, int param)
{
	return _game_design.General_design.SheetTreasure_Item.int32Value.getValue(id, param);
}

int FDGame::GetDesignScoreMutiply(int id, int param)
{
	return _game_design.General_design.SheetScore.int32Value.getValue(id, param);
}

int FDGame::GetDesignFeverModeTime(int id, int param)
{
	return _game_design.General_design.Sheetfever_mode.int32Value.getValue(id, param);
}