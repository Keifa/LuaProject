#include "LuaHandler.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	LuaHandler lh;
	lh.start();

	std::system("pause");
	return 0;
}