#include "Scene.h"

using namespace FCE;

SCENE::SCENE()
{
	//Should never be used
}

SCENE::SCENE(LOGGER* log, IrrlichtDevice* device)
{
	//Set init variables from constructor
	this->log = log;
 	this->device = device;
	gui = device->getGUIEnvironment();
	manager = device->getSceneManager();
	
}

SCENE::~SCENE()
{
	//drop the sound driver
	sound->drop();
}

void SCENE::init()
{
	
	//begin scene initialization
	log->logData("Initiating Scene");
	log->logData("Initializing Sound Engine");
	//create sound driver
	sound = createIrrKlangDevice();
	if(!sound)
	{
		//if driver fails to create, log it and return from initialization
		log->logData("Failed to create sound device");
		return;
	}
	//play test sound to test sound engine
	addSound("jingle.mp3", core::vector3df(0, 0, 0), false);
	//TODO:add splash screen
	
	//set initial values
	log->logData("Setting initial values");
	cameraPos = vec3df(0, 0, 0);
	cameraRot = vec3df(0, 0, 1);
	node = 0;
	lastID = 0;
	soundID = 0;
	manager->setShadowColor(video::SColor(150, 0, 0, 0));
	
	//load startup lua script
	log->logData("Loading startup script");
	//create new script
	mainScript = new SCRIPT(log);
	//set initial script values
	mainScript->init();
	//push the current scene as "MainScene" for use in lua
	luaW_push<SCENE>(mainScript->L, this);
	lua_setglobal(mainScript->L, "MainScene");
	//run mainScript
	mainScript->run("Scripts/startup.lua");
	log->logData("Loading scene objects");
	//run the init function within the startup script
	mainScript->runInit();
	log->logData("Loaded scene objects");
	log->logData("Finished initializing");
}

void SCENE::update()
{
	//set listener position for the sound manager
	sound->setListenerPosition( manager->getActiveCamera()->getAbsolutePosition(), manager->getActiveCamera()->getTarget());
	
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		(*it)->update();
	}
	//run the update function within the main script
	mainScript->update();
	//update the sound driver
	sound->update();
}

void SCENE::render()
{
	//draw all things in the manager and GUI.
	manager->drawAll();
	gui->drawAll();
	//run the render function in the main script
	mainScript->render();
}
//misc functions
LOGGER* SCENE::getLog()
{
	return log;
}

IrrlichtDevice* SCENE::getDevice()
{
	return device;
}

//
int SCENE::addSound(std::string filename, core::vector3df pos, bool loop)
{
	SOUND* temp = new SOUND(sound, log);
	temp->setID(lastID);
	temp->load(filename, loop);
	temp->setPosition(pos);
	temp->setVolume(1.0f);
	
	if(!temp->getSound()->getSoundEffectControl())
	{
		log->logData("Effects not supported!");
		
	}
	lastID++;
	objects.push_back(temp);
	return lastID-1;
}

void SCENE::stopSound(int id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((SOUND*)(*it))->getID() == id)
		{
			((SOUND*)(*it))->getSound()->stop();
		}
	}
}

SOUND* SCENE::editSound(int id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((SOUND*)(*it))->getID()==id)
		{
			return ((SOUND*)(*it));
		}
	}
	log->logData("Sound doesn't exist. ID", id);
	return NULL;
}
bool SCENE::isPlaying(int id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((SOUND*)(*it))->getID() == id)
		{
			if(!((SOUND*)(*it))->getSound()->isFinished())
			{
				return true;
			}
		}
	}
	return false;
}

int SCENE::addParticleSystem(core::vector3df pos, core::vector3df dir, core::vector3df scale, std::string filename)
{
	log->debugData("Creating new particle system");
	PARTICLE* system1;
	system1 = new PARTICLE(device, log);
	system1->setID(lastID);
	system1->setName("PARTICLE");
	system1->setColors(video::SColor(0, 200, 200, 200), video::SColor(0, 255, 255, 255));
	system1->setDirection(dir);
	system1->setPosition(pos);
	system1->setScale(scale);
	system1->setRate(500, 500);
	system1->setSize(core::dimension2df(1,1), core::dimension2df(2, 2));
	system1->setAge(200, 300);
	system1->loadTexture(filename);
	lastID++;
	system1->init();
	objects.push_back(system1);
	log->debugData("Particle system added", lastID-1);
	return lastID-1;
}
PARTICLE* SCENE::editParticleSystem(int id)
{
	//log->debugData("Getting particle system", id);
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((PARTICLE*)(*it))->getID()==id)
		{
			//log->debugData("Found particle system", id);
			return ((PARTICLE*)(*it));
		}
	}
	log->logData("Particle system doesn't exist. ID", id);
	return NULL;
}
int SCENE::addMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale)
{
	MESH* tmp = new MESH(manager, node, log);
	tmp->setID(lastID);
	tmp->load(filename);
	tmp->setName("MESH");
	tmp->getNode()->setMaterialFlag(video::EMF_LIGHTING, true);
	tmp->getNode()->setMaterialType(video::EMT_SOLID);
	tmp->setPosition(pos);
	tmp->setRotation(rot);
	tmp->setScale(scale);
	lastID++;
	objects.push_back(tmp);
	return lastID-1;
}
MESH* SCENE::editMesh(int id)
{
	//log->debugData("Getting mesh", id);
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((MESH*)(*it))->getID()==id)
		{
			//log->debugData("Got mesh", id);
			return ((MESH*)(*it));
		}
	}
	log->logData("Couldn't find mesh");
	return NULL;
}
int SCENE::addLight(core::vector3df pos, core::vector3df rot, core::vector3df scale, video::E_LIGHT_TYPE type)
{
	LIGHT* temp = new LIGHT(manager, node, log);
	temp->setColor(video::SColor(0, 1, 1, 1));
	temp->setPosition(pos);
	temp->setScale(scale);
	temp->setRotation(rot);
	temp->setType(type);
	temp->setDropoff(1000);
	temp->setID(lastID);
	temp->setName("LIGHT");
	lastID++;
	temp->init();
	objects.push_back(temp);
	return lastID-1;
}
LIGHT* SCENE::editLight(int id)
{
	//log->debugData("Checking for light", id);
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((LIGHT*)(*it))->getID()==id)
		{
			//log->debugData("Found light", id);
			return ((LIGHT*)(*it));
		}
	}
	log->logData("Light not found. ID", id);
	return NULL;
}
