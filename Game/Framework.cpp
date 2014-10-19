#include "Framework.h"
using namespace FCE;

FRAMEWORK::FRAMEWORK()
{
	
}

FRAMEWORK::~FRAMEWORK()
{
	log->logData("Framework Closed");
	device->drop();
	delete log;
}

int FRAMEWORK::init()
{
	run = true;
	log = new LOGGER();
	log->init("EngineLog.txt");
	log->debugData("Logger initialized");
	log->logData("Fox Claw Engine by Jcam Technologies");
	log->logData("Engine build", BUILD);
	log->logData("Platform", PLAT);
	log->logData("Creating Graphics Driver");
/*
	SIrrlichtCreationParameters para;
	para.DriverType = video::EDT_DIRECT3D9;
	para.HighPrecisionFPU = true;
	para.WindowSize = core::dimension2d<u32>(SCREENWIDTH, SCREENHEIGHT);
	log->logData("Trying Direct3D 9");
	device = createDeviceEx(para);
	if(device == 0)
	{
		log->logData("Trying Direct3D 8");
		para.DriverType = video::EDT_DIRECT3D8;
		device = createDeviceEx(para);
		if(device == 0)
		{
			log->logData("Trying OPENGL");
			para.DriverType = video::EDT_OPENGL;
			device = createDeviceEx(para);
			if(device == 0)
			{
				log->logData("Trying Software Renderer");
				para.DriverType = video::EDT_SOFTWARE;
				device = createDeviceEx(para);
				if(device == 0)
				{
					log->logData("Failed to create driver");
					return 1;
				}
			}
		}
	}
*/
	log->logData("Trying OpenGL");
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 32, false, true);
	if(!device)
	{
		log->logData("Trying Software Renderer");
		device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(800, 600), 32, false, false);
		if(!device)
		{
			log->logData("Could not create video driver");
			return 1;
		}
	}
	device->setWindowCaption(L"FoxClaw Engine by Jcam Technologies");
	
	log->logData("Getting driver");
	driver = device->getVideoDriver();
	log->logData("Getting manager");
	manager = device->getSceneManager();
	log->logData("Getting GUI");
	gui = manager->getGUIEnvironment();
	log->logData("Creating scene");
	scene = new SCENE(log, device);
	scene->init();
	manager->addCameraSceneNodeFPS(0, 100.0f, 0.01f);
	log->logData("Scene created");
	return 0;
}

int FRAMEWORK::update()
{
	run = device->run();
	scene->update();
	return 0;
}

int FRAMEWORK::render()
{
	if(device->isWindowActive())
	{
		driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
		scene->render();
		driver->endScene();
		int fps = driver->getFPS();
        core::stringw str = L"FoxClaw Engine by Jcam Technologies [";
        str += driver->getName();
        str += "] FPS:";
        str += fps;
        device->setWindowCaption(str.c_str());
	}else
	{
		device->yield();
	}
	return 0;
}
