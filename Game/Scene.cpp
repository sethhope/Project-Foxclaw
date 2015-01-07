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
 	debug = false;
	gui = device->getGUIEnvironment();
	manager = device->getSceneManager();
	
}

SCENE::~SCENE()
{
	objects.clear();
	//drop the sound driver
	sound->drop();
	delete world;
	objects.clear();
	delete mainScript;
	
}

void SCENE::init()
{
	deltaTime = 0;
	timeStamp = device->getTimer()->getTime();
	//begin scene initialization
	log->logData("Initiating Scene");
	log->logData("Initializing Physics World");
	world = createIrrBulletWorld(device, true, true);
	world->setDebugMode(EPDM_DrawWireframe | EPDM_FastWireframe);
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
	lastID = 0;
	soundID = 0;
	camera = new CAMERA(manager, log);
	timeScale = 0.004;

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
	mainScript->run((std::string)(device->getFileSystem()->getAbsolutePath("Scripts/startup.lua").c_str()));
	log->logData("Loading scene objects");
	//run the init function within the startup script
	mainScript->runInit();
	manager->setAmbientLight(video::SColorf(0.2,0.2, 0.2, 1));
	u32 l = addLight(core::vector3df(1, 1, 0), core::vector3df(45, 0, 0), core::vector3df(1, 1, 1), 10000, video::ELT_DIRECTIONAL);
	log->logData("Loaded scene objects");
	log->logData("Finished initializing");
}

void SCENE::update(FEventReceiver receiver)
{
	log->debugData(EXTRA, "Starting scene update");
	this->receiver = receiver;
	log->debugData(EXTRA, "Calculating deltaTime");
	deltaTime = device->getTimer()->getTime() - timeStamp;
	timeStamp = device->getTimer()->getTime();
	log->debugData(EXTRA, "Updating Physics");
	world->stepSimulation(deltaTime*timeScale, 4);
	log->debugData(EXTRA, "Adding camera constants");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getAbsolutePosition().X);
	lua_setglobal(mainScript->L, "CAM_X");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getAbsolutePosition().Y);
	lua_setglobal(mainScript->L, "CAM_Y");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getAbsolutePosition().Z);
	lua_setglobal(mainScript->L, "CAM_Z");
	lua_pushnumber(mainScript->L, deltaTime);
	lua_setglobal(mainScript->L, "deltaTime");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getRotation().X);
	lua_setglobal(mainScript->L, "CAM_ROT_X");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getRotation().Y);
	lua_setglobal(mainScript->L, "CAM_ROT_Y");
	lua_pushnumber(mainScript->L, manager->getActiveCamera()->getRotation().Z);
	lua_setglobal(mainScript->L, "CAM_ROT_Z");
	lua_pushnumber(mainScript->L, (lastID-1));
	lua_setglobal(mainScript->L, "OBJECTS");
	log->debugData(EXTRA, "Updating script");
	//run the update function within the main script
	mainScript->update();
	log->debugData(EXTRA, "Updating objects");
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		log->debugData(EXTRA, (*it)->getName());
		(*it)->update();
	}
	log->debugData(EXTRA, "updating sound");
	//update the sound driver
	sound->update();
	log->debugData(EXTRA, "updating camera");
	camera->update();
	log->debugData(EXTRA, "Setting listener position");
	//set listener position for the sound manager
	sound->setListenerPosition(manager->getActiveCamera()->getAbsolutePosition(), manager->getActiveCamera()->getTarget());
}

void SCENE::render()
{
	//draw all things in the manager and GUI.
	manager->drawAll();
	gui->drawAll();
	//run the render function in the main script
	mainScript->render();
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		log->debugData(EXTRA, (*it)->getName());
		(*it)->render();
	}
	if(debug)
	{
		world->debugDrawWorld(true);
		world->debugDrawProperties(true);
	}
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
u32 SCENE::addSound(std::string filename, core::vector3df pos, bool loop)
{
	SOUND* temp = new SOUND(sound, log);
	temp->setID(lastID);
	temp->load((std::string)(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str()), loop);
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

SOUND* SCENE::editSound(u32 id)
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

u32 SCENE::addParticleSystem(core::vector3df pos, core::vector3df dir, core::vector3df scale, std::string filename)
{
	log->debugData(MAJOR, "Creating new particle system");
	PARTICLE* tmp;
	tmp = new PARTICLE(device, log);
	tmp->init();
	tmp->setID(lastID);
	tmp->setName("PARTICLE");
	tmp->setColors(video::SColor(0, 200, 200, 200), video::SColor(0, 255, 255, 255));
	tmp->setDirection(dir);
	tmp->setPosition(pos);
	tmp->setScale(scale);
	tmp->setRate(500, 500);
	tmp->setSize(core::dimension2df(1,1), core::dimension2df(2, 2));
	tmp->setAge(200, 300);
	tmp->loadTexture((std::string)(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str()));
	lastID++;
	
	tmp->update();
	log->debugData(MAJOR, "created at", tmp->getPosition().X);
	objects.push_back(tmp);
	log->debugData(MAJOR, "Particle system added", lastID-1);
	
	return lastID-1;
}
PARTICLE* SCENE::editParticleSystem(u32 id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((PARTICLE*)(*it))->getID()==id)
		{
			return ((PARTICLE*)(*it));
		}
	}
	log->logData("Couldn't find particle system");
	return NULL;
}
u32 SCENE::addMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale)
{
	MESH* tmp = new MESH(manager, log);
	tmp->setID(lastID);
	tmp->load((std::string)(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str()));
	tmp->setName("MESH");
	tmp->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, true);
	tmp->getIrrNode()->setMaterialType(video::EMT_SOLID);
	tmp->setPosition(pos);
	tmp->setRotation(rot);
	tmp->setScale(scale);
	tmp->init();
	lastID++;
	objects.push_back(tmp);
	tmp->update();
	return lastID-1;
}
MESH* SCENE::editMesh(u32 id)
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

u32 SCENE::addAnimatedMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale)
{
	ANIMATEDMESH* tmp = new ANIMATEDMESH(manager, log);
	tmp->setID(lastID);
	tmp->load((std::string)(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str()));
	tmp->setName("ANIMATED_MESH");
	tmp->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, true);
	tmp->getIrrNode()->setMaterialType(video::EMT_SOLID);
	tmp->setPosition(pos);
	tmp->setRotation(rot);
	tmp->setScale(scale);
	tmp->init();
	lastID++;
	objects.push_back(tmp);
	tmp->update();
	return lastID-1;
}

ANIMATEDMESH* SCENE::editAnimatedMesh(u32 id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((ANIMATEDMESH*)(*it))->getID()==id)
		{
			return ((ANIMATEDMESH*)(*it));
		}
	}
	log->logData("Couldn't find animated mesh");
	return NULL;
}
u32 SCENE::addLight(core::vector3df pos, core::vector3df rot, core::vector3df scale, f32 dropoff, video::E_LIGHT_TYPE type)
{
	LIGHT* temp = new LIGHT(manager, log);
	temp->setColor(video::SColor(255, 255, 255, 255));
	temp->setPosition(pos);
	temp->setScale(scale);
	temp->setRotation(rot);
	temp->setType(type);
	temp->setDropoff(dropoff);
	temp->setID(lastID);
	temp->setName("LIGHT");
	lastID++;
	temp->init();
	temp->setPosition(pos);
	temp->setScale(scale);
	temp->setRotation(rot);
	objects.push_back(temp);
	temp->update();
	return lastID-1;
}
LIGHT* SCENE::editLight(u32 id)
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

u32 SCENE::addEmptyObject(core::vector3df pos, core::vector3df rot, core::vector3df scale)
{
	EMPTYOBJECT* e = new EMPTYOBJECT(manager);
	e->setPosition(pos);
	e->setRotation(rot);
	e->setScale(scale);
	e->setName("EMPTY");
	e->setID(lastID);
	lastID++;
	e->init();
	e->update();
	objects.push_back(e);
	return lastID-1;
}

EMPTYOBJECT* SCENE::editEmpty(u32 id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if(((EMPTYOBJECT*)(*it))->getID()==id)
		{
			return ((EMPTYOBJECT*)(*it));
		}
	}
	log->logData("EmptyObject not found. ID", id);
	return NULL;
}
OBJECT* SCENE::getObject(u32 id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if((*it)->getID()==id)
		{
			return *it;
		}
	}
	log->logData("Sound doesn't exist. ID", id);
	return NULL;
}
irrBulletWorld* SCENE::getWorld()
{
	return world;
}

bool SCENE::keyDown(EKEY_CODE keycode)
{
	return receiver.KeyDown(keycode);
}

CAMERA* SCENE::getCamera()
{
	return camera;
}

IKinematicCharacterController* SCENE::getCharacter()
{
	return character;
}
void SCENE::setCharacter(IKinematicCharacterController* character)
{
	this->character=character;
}
void SCENE::setDebug(bool debug)
{
	log->logData("Setting debug to", (int)debug);
	this->debug = debug;
}
void SCENE::setSkydome(std::string filename)
{
	skydome = manager->addSkyDomeSceneNode(manager->getVideoDriver()->getTexture(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str()), 32, 16, 0.95, 2.0);
}
void SCENE::removeObject(u32 id)
{
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if((*it)->getID()==id)
		{
			manager->addToDeletionQueue((*it)->getIrrNode());
			(*it)->~OBJECT();
			objects.erase(it);
			return;
		}
	}
}
f32 SCENE::getTimeScale()
{
	return timeScale;
}
void SCENE::setTimeScale(f32 timeScale)
{
	this->timeScale = timeScale;
}
