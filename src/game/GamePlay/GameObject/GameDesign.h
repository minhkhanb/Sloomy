#ifndef _CGAME_DESIGN_H_
#define _CGAME_DESIGN_H_

#include "../../DataType/CType.h"

class DesignClassDataInt32
{
public:
	DesignClassDataInt32(){};

	int _beginByte;
	int *_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	int getValue(int row, int atribute);
};

class DesignClassDataInt64
{
public:
	DesignClassDataInt64(){};

	int _beginByte;
	INT64 *_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	INT64 getValue(int row, int atribute);
};

class DesignClassDataFloat
{
public:
	DesignClassDataFloat(){};

	int _beginByte;
	float *_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	float getValue(int row, int atribute);
};

class DesignClassDataString
{
public:
	DesignClassDataString(){};

	int _beginByte;
	char **_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	char *getValue(int row, int atribute);
};

class CGameDesign
{
public:
	CGameDesign(){};

	class FileGeneral_design
	{
		public:
		FileGeneral_design()
		{

		};

		void Load()
		{
			SheetGeneral.Load();
			Sheetfever_mode.Load();
			SheetPercent_Spawn.Load();
			SheetFlower_Respawn.Load();
			SheetFlower_Design.Load();
			SheetBeginning.Load();
			SheetTreasure_Time.Load();
			SheetTreasure_Item.Load();
			SheetScore.Load();

		};

		void UnLoad()
		{
			SheetGeneral.UnLoad();
			Sheetfever_mode.UnLoad();
			SheetPercent_Spawn.UnLoad();
			SheetFlower_Respawn.UnLoad();
			SheetFlower_Design.UnLoad();
			SheetBeginning.UnLoad();
			SheetTreasure_Time.UnLoad();
			SheetTreasure_Item.UnLoad();
			SheetScore.UnLoad();

		};

		class CSheetGeneral
		{
			public:
			DesignClassDataFloat floatValue;
			DesignClassDataString stringValue;

			CSheetGeneral()
			{
				floatValue._beginByte = 0;
				stringValue._beginByte = 76;

			};
			enum enumFloat
			{
				value = 0,

			};
			enum enumString
			{
				name = 0,

			};


			void Load()
			{
				floatValue.Load();
				stringValue.Load();

			};

			void UnLoad()
			{
				floatValue.UnLoad();
				stringValue.UnLoad();

			};
		};
		class CSheetfever_mode
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetfever_mode()
			{
				int32Value._beginByte = 631;

			};
			enum enumInt32
			{
				time = 0,
				combo = 1,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetPercent_Spawn
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetPercent_Spawn()
			{
				int32Value._beginByte = 679;

			};
			enum enumInt32
			{
				Sunflower = 0,
				Lily = 1,
				Violet = 2,
				Lotus = 3,
				Rose = 4,
				Orchid = 5,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetFlower_Respawn
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetFlower_Respawn()
			{
				int32Value._beginByte = 951;

			};
			enum enumInt32
			{
				Easy = 0,
				Medium = 1,
				Hard = 2,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetFlower_Design
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetFlower_Design()
			{
				int32Value._beginByte = 971;

			};
			enum enumInt32
			{
				Level = 0,
				Number_of_Flower = 1,
				Point = 2,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetBeginning
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetBeginning()
			{
				int32Value._beginByte = 3379;

			};
			enum enumInt32
			{
				Easy_1 = 0,
				Easy_2 = 1,
				Easy_3 = 2,
				Medium_1 = 3,
				Medium_2 = 4,
				Medium_3 = 5,
				Medium_4 = 6,
				Hard_1 = 7,
				Hard_2 = 8,
				Hard_3 = 9,
				Hard_4 = 10,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetTreasure_Time
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetTreasure_Time()
			{
				int32Value._beginByte = 3739;

			};
			enum enumInt32
			{
				ID = 0,
				time = 1,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetTreasure_Item
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetTreasure_Item()
			{
				int32Value._beginByte = 3795;

			};
			enum enumInt32
			{
				Power_up = 0,
				quantity = 1,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};
		class CSheetScore
		{
			public:
			DesignClassDataInt32 int32Value;

			CSheetScore()
			{
				int32Value._beginByte = 3827;

			};
			enum enumInt32
			{
				Level = 0,
				score = 1,
				number_of_flowers = 2,

			};


			void Load()
			{
				int32Value.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();

			};
		};

		CSheetGeneral SheetGeneral;
		CSheetfever_mode Sheetfever_mode;
		CSheetPercent_Spawn SheetPercent_Spawn;
		CSheetFlower_Respawn SheetFlower_Respawn;
		CSheetFlower_Design SheetFlower_Design;
		CSheetBeginning SheetBeginning;
		CSheetTreasure_Time SheetTreasure_Time;
		CSheetTreasure_Item SheetTreasure_Item;
		CSheetScore SheetScore;

	};

	FileGeneral_design General_design;

};

#endif