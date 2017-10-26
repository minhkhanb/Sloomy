#include "pch.h"
#include "../../GameCore/CGame.h"

CFriendList::CFriendList()
{
}

CFriendList::~CFriendList()
{
}

void CFriendList::Init(float x, float y, char name, char url, int score)
{
	_x = x;
	_y = y;
	sprintf(_name, "%s", name);
	sprintf(_avatar_url, "%s", url);
	_score = score;
}
void CFriendList::Render(float x, float y, int opac)
{

}
void CFriendList::Update()
{

}