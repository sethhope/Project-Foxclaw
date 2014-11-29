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
	deltaTime = 0;
	timeStamp = device->getTimer()->getTime();
	//begin scene initialization
	log->logData("Initiating Scene");
	log->logData("Initializing Physics World");
	world = createIrrBulletWorld(device, true, true);
	world->setDebugMode(EPDM_DrawAabb);
	if(!world)
	{
		log->logData("Failed to create world");
		return;
	}
	std::stringstream bulVer;
	bulVer << IRRBULLET_VER_MAJOR << "." << IRRBULLET_VER_MINOR << "." << IRRBULLET_VER_MICRO;
	std::string bulVer_ = bulVer.str();
	log->debugData(MINOR, "Initialized Physics version", bulVer_);
	world->setGravity(core::vector3df(0, -10, 0));
	world->debugDrawWorld(true);
	world->debugDrawProperties(true, video::SColor(255, 0, 255, 0));
	log->logData("Initializing Sound Engine");
	//create sound driver
	sound = createIrrKlangDevice();
	if(!sound)
	{
		//if driver fails to create, log it and return from initialization
		log->logData("Failed to create sound device");
		return;
	}
	log->debugData(MINOR, "Initialized Sound Engine Version", IRR_KLANG_VERSION);
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
	int m = addMesh("sibenik/sibenik.obj", core::vector3df(0, -50, 0), core::vector3df(0, 0, 0), core::vector3df(5,5,5));
	log->debugData(EXTRA, "Adding collider");
	int c = addCollider((OBJECT*)editMesh(m), COL_CUBE, 0.1, 0.1, 0.5, 0);
	int b = addMesh("sibenik/sibenik.obj", core::vector3df(0, 70, 0), core::vector3df(0, 0, 0), core::vector3df(1, 1, 1));
	int d = addCollider((OBJECT*)editMesh(b), COL_CUBE, 0.1, 0.1, 0.5, 1);
	editCollider(d)->setVelocity(core::vector3df(0, 10, 0));
	int l = addLight(core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(200, 200, 200), video::ELT_POINT);
	log->logData("Loaded scene objects");
	log->logData("Finished initializing");
}

void SCENE::update()
{
	log->debugData(EXTRA, "Starting scene update");
	log->debugData(EXTRA, "Calculating deltaTime");
	deltaTime = device->getTimer()->getTime() - timeStamp;
	timeStamp = device->getTimer()->getTime();
	log->debugData(EXTRA, "Setting listener position");
	//set listener position for the sound manager
	sound->setListenerPosition( manager->getActiveCamera()->getAbsolutePosition(), manager->getActiveCamera()->getTarget());
	log->debugData(EXTRA, "Updating Physics");
	world->stepSimulation(deltaTime*0.001f, 120);
	
	world->debugDrawWorld(true);
	world->debugDrawProperties(true);
	
	log->debugData(EXTRA, "Updating objects");
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		log->debugData(EXTRA, (*it)->getName());
		(*it)->update();
	}
	log->debugData(EXTRA, "Adding camera constants");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getAbsolutePosition().X);
	lua_setglobal(mainScript->L, "CAM_X");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getAbsolutePosition().Y);
	lua_setglobal(mainScript->L, "CAM_Y");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getAbsolutePosition().Z);
	lua_setglobal(mainScript->L, "CAM_Z");
	log->debugData(EXTRA, "Updating script");
	//run the update function within the main script
	mainScript->update();
	log->debugData(EXTRA, "updating sound");
	//update the sound driver
	sound->update();
}

void SCENE::render()
{
	//draw all things in the manager and GUI.
	world->debugDrawWorld(true);
	world->debugDrawProperties(true);
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
	log->debugData(MAJOR, "Creating new particle system");
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
	log->debugData(MAJOR, "Particle system added", lastID-1);
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
	MESH* tmp = new MESH(manager, log);
	tmp->setID(lastID);
	tmp->load(filename);
	tmp->setName("MESH");
	tmp->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, true);
	tmp->getIrrNode()->setMaterialType(video::EMT_SOLID);
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
	LIGHT* temp = new LIGHT(manager, log);
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

int SCENE::addCollider(OBJECT* parent, int type, float lDamping, float aDamping, float friction, float mass)
{
	COLLIDER* c = new COLLIDER((NODE*)parent, manager, world, log, mass, type);
	if(type == COL_MESH)
	{
		if(((MESH*)parent)->getMesh())
		{
			c->setMesh(((MESH*)parent)->getMesh());
		}
	}
	c->init();
	c->setDamping(lDamping, aDamping);
	c->setFriction(friction);
	
	
	c->setID(lastID);
	lastID++;
	objects.push_back(c);
	return lastID - 1 ;
}

COLLIDER* SCENE::editCollider(int id)
{
	//log->debugData("Checking for light", id);
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((COLLIDER*)(*it))->getID()==id)
		{
			//log->debugData("Found light", id);
			return ((COLLIDER*)(*it));
		}
	}
	log->logData("Collider not found. ID", id);
	return NULL;
}
