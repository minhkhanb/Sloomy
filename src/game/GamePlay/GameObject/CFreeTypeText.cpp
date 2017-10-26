#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../../GameCore/Os_Functions.h"

CFreeTypeText::CFreeTypeText()
{
	_text = NULL;
}

void CFreeTypeText::Init(int size, int charspace)
{
	PineFreeType::InitFreeType();
	ResourceItem item = OS_GetResourceItem("ArialUnicodeMS.ttf");
	_pen = PineFreeType::LoadPen(item);
	_pen->setCharSize(size);
	_pen->setSpaceSize(charspace);
	_cvas = PineFreeTypeCanvas::create(G(), _pen, 256);

	_text = _cvas->createNewText();
}

void CFreeTypeText::SetText(INT32 *arrayInt, int length, int lenRender)
{
	int len = length;
	if (lenRender !=-1)
	{
		if (lenRender < length)
		{
			len = lenRender;

			//check AI Render test
			int count = 0;
			for (int i = len - 1; i >= 0; i--)
			{
				if (arrayInt[i] == 32)
				{
					break;
				}
				else
				{
					count++;
				}
			}
			if (count == 1) //kiem tra loai bo 1 ky cuoi VD:lenRender = 6 "Quoc Nguyen" => "Quoc N" => "Quoc"
			{
				len--;
			}
			else //kiem tra con 2 ky tu cuoi
			{
				if (len + 3 > length) //VD:lenRender = 6 "Quoc Nguy" => "Quoc N" => "Quoc Nguy"
				{
					len = length;
				}
				else 
				{
					bool hasSpace = false;
					for (int i = 0; i < 3; i++)
					{
						if (arrayInt[len + i] == 32)
						{
							hasSpace = true;
							break;
						}
					}
					if (hasSpace) //VD:lenRender = 6 "Stephen Ho" => "Stephe" => "Stephen"
					{
						count = 0;
						for (int i = 0; i < 3; i++)
						{
							if (arrayInt[len + i] == 32)
							{
								break;
							}
							else
							{
								count++;
							}
						}
						len += count;
					}
					else //VD:lenRender = 7 "Quoc Nguyen" => "Quoc Ng" => "Quoc Ng..."
					{
						for (int i = 0; i < 3; i++)
						{
							arrayInt[len + i] = '.';
						}
						len += 3;
					}
				}
			}

			//xu ly khoan trang cuoi
			for (int i = len - 1; i >= 0; i--)
			{
				if (arrayInt[i] == 32)
				{
					len--;
				}
				else
				{
					break;
				}
			}
		}
	}

	if (_text != NULL)
	{
		SAFE_DEL(_text);
	}
    _text = _cvas->createNewText();
    _text->Init(arrayInt, len);
}

void CFreeTypeText::Render(float x, float y,int anchor)
{
	_text->Render(x, y,anchor);
}

void CFreeTypeText::RenderPage(float x, float y, int width,int anchor)
{
    _text->RenderPage(x, y,width,anchor);
}