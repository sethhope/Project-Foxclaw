#include "stdlib.h"
#include "Game/Framework.h"

using namespace FCE;

int main()
{
	FRAMEWORK* framework = new FRAMEWORK();
	if(framework->init()!= 0)
	{
		return 1;
	}
	//TODO: do the timing thing
	while(framework->run == true)
	{
		if(framework->update()!=0)
		{
			return 2;
		}
		if(framework->render()!=0)
		{
			return 3;
		}
	}
	delete framework;
	return 0;
}
