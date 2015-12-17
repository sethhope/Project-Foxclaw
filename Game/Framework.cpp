#include "Framework.h"
using namespace FCE;

FRAMEWORK::FRAMEWORK()
{
	//Nothing yet...
}

FRAMEWORK::~FRAMEWORK()
{
	log->logData("Framework Closed");

	delete scene;
	delete log;
	delete config;
	device->drop(); //Drop the irrlicht device
}

int FRAMEWORK::init()
{
	run = true; // Set the run variable.
	log = new LOGGER(); //create the logger
	device = createDevice(video::EDT_NULL);
	//initialize the logger to EngineLog.txt
	log->init((std::string)(device->getFileSystem()->getAbsolutePath("EngineLog.txt").c_str()));
	log->setDebug(ALL);
	//log header data.
	log->debugData(MINOR, "Logger initialized");
	log->logData("Fox Claw Engine by Jcam Technologies");
	log->logData("Engine build", VERSION_FULLVERSION_STRING);
	log->logData("Engine Version", VERSION_STATUS);
	log->logData("Platform", PLAT);
	log->logData("Loading config", (std::string)(device->getFileSystem()->getAbsolutePath("config.xml").c_str()));
	config = new Config(device, log);
	config->loadConfig((std::string)(device->getFileSystem()->getAbsolutePath("config.xml").c_str()));
	for(std::map<std::string, f32>::iterator it = config->data.begin(); it != config->data.end(); it++)
    {
        log->logData(it->first.c_str(), it->second);
    }
	log->logData("Creating Graphics Driver");
	bool fullScreen = config->data["fullscreen"];
	bool vSync = config->data["vsync"];
	u32 d = config->data["driver"];
	video::E_DRIVER_TYPE dType;
	if(d == 1)
        dType = video::EDT_DIRECT3D9;
    if(d == 2)
        dType = video::EDT_OPENGL;
    if(d == 3)
        dType = video::EDT_BURNINGSVIDEO;
    if(d == 4)
        dType = video::EDT_NULL;
    if(dType!=NULL)
    {
        log->logData("Trying driver", d);
        log->logData("1=DX9, 2=OGL, 3=SOFTWARE, 4=NULL");
        SIrrlichtCreationParameters p;
        p.DriverType = dType;
        p.WindowSize = core::dimension2d<u32>(config->data["width"], config->data["height"]);
        p.Bits = (u8)32;
        p.Fullscreen = fullScreen;
        p.Stencilbuffer = true;
        p.Vsync = vSync;
        p.EventReceiver = &receiver;
        p.HighPrecisionFPU = true;
        device = createDeviceEx(p);
        if(!device)
        {
            if(!device)
            {
                log->logData("Trying Software Renderer");
                device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(config->data["width"], config->data["height"]), 16, fullScreen, false, false, &receiver);
                if(!device)
                {
                    //Failed to create driver
                    log->logData("Could not create video driver");
                    return 1;
                }
            }
        }
    }else
    {
        log->logData("Trying OpenGL");
        device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(config->data["width"], config->data["height"]), 32, fullScreen, false, vSync, &receiver);
        if(!device)
        {
            //OpenGL failed. Try software renderer
            log->logData("Trying Software Renderer");
            device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(config->data["width"], config->data["height"]), 16, fullScreen, false, false, &receiver);
            if(!device)
            {
                //Failed to create driver
                log->logData("Could not create video driver");
                return 1;
            }
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
	scene = new SCENE(log, device, config);
	scene->init(receiver);
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
	run = !scene->exit;
	log->debugData(EXTRA, "Updating GUI");
	receiver.callers = scene->callers;
	std::vector<GUI*>::iterator it = scene->callers.begin();
	while(it < scene->callers.end())
	{
		if((*it)->guiCaller == 0)
		{
			it = scene->callers.erase(it);
			log->debugData(MAJOR, "Removing gui object");
		}
		else
		{
			it++;
		}
	}
	receiver.callers=scene->callers;
	it = scene->guiObjects.begin();
	while(it < scene->guiObjects.end())
	{
		if((*it)->guiCaller == 0)
		{
			it = receiver.callers.erase(it);
			it--;
			log->debugData(MAJOR, "Removing gui object");
		}
		else
		{
			bool exists = false;
			for(std::vector<GUI*>::iterator ite = receiver.callers.begin(); ite < receiver.callers.end(); ite++)
			{
				if((*it) == (*ite))
				{
					exists = true;
				}
			}
			if(!exists)
			{
				log->debugData(MAJOR, "Added GUI to Receiver");
				receiver.addGUI((*it));
			}
			it++;
		}
	}

	scene->callers = receiver.callers;
	scene->guiObjects.clear();
	return 0;
}

int FRAMEWORK::render()
{
	//check if window is active
	if(device->isWindowActive())
	{
		//begin scene rendering. Clear screen to black first.
		driver->beginScene(true, true, video::SColor(255, 50, 50, 50));
		//render the scene
		scene->render();
		//finish rendering
		driver->endScene();
		//This last block is adding the FPS to the window header.
		u32 fps = driver->getFPS();
        core::stringw str = L"FoxClaw Engine by Jcam Technologies ";
        str += VERSION_STATUS;
        str += "[";
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
