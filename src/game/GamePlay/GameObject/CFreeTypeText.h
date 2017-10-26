#if !defined (_FREE_TYPE_TEXT_H)
#define _FREE_TYPE_TEXT_H

class CFreeTypeText
{
private:
	PineFreeTypeCanvas* _cvas;
	PineFreeTypePen* _pen;
	PineFreeTypeText* _text;
public:
	CFreeTypeText();

	void Init(int size, int charspace);

    void SetText(INT32 *arrayInt, int length, int lenRender = -1);
	void Render(float x, float y,int anchor=0);
    void RenderPage(float x, float y, int width,int anchor);
};

#endif
