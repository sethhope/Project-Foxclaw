#include "Framework.h"
using namespace FCE;

FRAMEWORK::FRAMEWORK()
{
	//Nothing yet...
}

FRAMEWORK::~FRAMEWORK()
{
	log->logData("Framework Closed");
	//device->drop(); //Drop the irrlicht device
	delete scene;
	delete log;
}

int FRAMEWORK::init()
{
	run = true; // Set the run variable.
	log = new LOGGER(); //create the logger
	
	//initialize the logger to EngineLog.txt
	log->init("EngineLog.txt");
	log->setDebug(ALL);
	//log header data.
	log->debugData(MINOR, "Logger initialized");
	log->logData("Fox Claw Engine by Jcam Technologies");
	log->logData("Engine build", BUILD);
	log->logData("Platform", PLAT);
	log->logData("Creating Graphics Driver");
	//Create graphics driver
	//Check for opengl (Only OpenGL and software for now. DirectX may come later)
	log->logData("Trying OpenGL");
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(SCREENWIDTH, SCREENHEIGHT), 32, false, true, true, &receiver);
	if(!device)
	{
		//OpenGL failed. Try software renderer
		log->logData("Trying Software Renderer");
		device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(SCREENWIDTH, SCREENHEIGHT), 16, false, false, false, &receiver);
		if(!device)
		{
			//Failed to create driver
			log->logData("Could not create video driver");
			return 1;
		}
	}
	//set top of window
	device->setWindowCaption(L"FoxClaw Engine by Jcam Technologies");
	
	//get the driver, manager, and gui handler from irrlicht
	log->logData("Getting driver");
	driver = device->getVideoDriver();
	log->logData("Getting manager");
	manager = device->getSceneManager();
	log->logData("Getting GUI");
	gui = manager->getGUIEnvironment();
	log->logData("Creating scene");
	//Create a new scene object and initialize
	scene = new SCENE(log, device);
	scene->init();
	//add debug camera.
	log->logData("Scene created");
	return 0;
}

int FRAMEWORK::update()
{
	//check if device is running and set run variable
	run = device->run();
	//update the scene
	scene->update(receiver);
	return 0;
}

int FRAMEWORK::render()
{
	//check if window is active
	if(device->isWindowActive())
	{
		//begin scene rendering. Clear screen to black first.
		driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
		//render the scene
		scene->render();
		//finish rendering
		driver->endScene();
		if(receiver.KeyDown(irr::KEY_ESCAPE))
		{
			run = false;
		}
		//This last block is adding the FPS to the window header.
		int fps = driver->getFPS();
        core::stringw str = L"FoxClaw Engine by Jcam Technologies [";
        str += driver->getName();
        str += "] FPS:";
        str += fps;
        device->setWindowCaption(str.c_str());
	}else
	{
		//if window isn't active, pause rendering to save processing
		device->yield();
	}
	return 0;
}
