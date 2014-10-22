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
	meshID = 0;
	soundID = 0;
	partID = 0;
	lightID = 0;
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
	manager->setAmbientLight(video::SColor(0.5f, 0.5f, 0.5f, 0.5f));
	//run the init function within the startup script
	mainScript->runInit();
	log->logData("Loaded scene objects");
	log->logData("Finished initializing");
}

void SCENE::update()
{
	//set listener position for the sound manager
	sound->setListenerPosition( manager->getActiveCamera()->getAbsolutePosition(), manager->getActiveCamera()->getTarget());
	
	//CRAZY LOOPS ARE TO BE FIXED SOON. DISREGARD THESE FOR LOOPS
	for(std::vector<SFX>::iterator it = sfx.begin(); it < sfx.end(); it++)
	{
		
		if(it->s->isFinished())
		{
			it->s->drop();
			
			it=sfx.erase(it);
		}else
		{
			it->s->setPosition(it->pos);
		}
	}
	for(std::vector<MESH *>::iterator it = meshs.begin(); it < meshs.end(); it++)
	{
		(*it)->update();
	}
	for(std::vector<LIGHT*>::iterator it = lights.begin(); it < lights.end(); it++)
	{
		(*it)->update();
	}
	for(std::vector<PARTICLE*>::iterator it = particles.begin(); it < particles.end();  it++)
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
int SCENE::addSound(std::string filename, vec3df pos, bool loop)
{
	ISound* temp = sound->play3D(filename.c_str(), pos, loop, false, true);
	if(temp)
	{
		SFX tmp;
		tmp.s = temp;
		tmp.pos = pos;
		tmp.minDistance = 1.0f;
		tmp.id = soundID;
		soundID++;
		sfx.push_back(tmp);
		return soundID-1;
	}
	return -1;
}

void SCENE::stopSound(int id)
{
	for(std::vector<SFX>::iterator it = sfx.begin(); it < sfx.end(); it++)
	{
		if(it->id == id)
		{
			it->s->stop();
		}
	}
}

bool SCENE::isPlaying(int id)
{
	for(std::vector<SFX>::iterator it = sfx.begin(); it < sfx.end(); it++)
	{
		if(it->id == id)
		{
			if(!it->s->isFinished())
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
	system1->setID(partID);
	system1->setColors(video::SColor(0, 200, 200, 200), video::SColor(0, 255, 255, 255));
	system1->setDirection(dir);
	system1->setPosition(pos);
	system1->setScale(scale);
	system1->setRate(500, 500);
	system1->setSize(core::dimension2df(1,1), core::dimension2df(2, 2));
	system1->setAge(200, 300);
	system1->loadTexture(filename);
	partID++;
	system1->init();
	particles.push_back(system1);
	log->debugData("Particle system added", partID-1);
	return partID-1;
}
PARTICLE* SCENE::editParticleSystem(int id)
{
	//log->debugData("Getting particle system", id);
	for(std::vector<PARTICLE*>::iterator it = particles.begin(); it < particles.end(); it++)
	{
		if((*it)->getID()==id)
		{
			//log->debugData("Found particle system", id);
			return (*it);
		}
	}
	log->logData("Particle system doesn't exist. ID", id);
	return NULL;
}
int SCENE::addMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale)
{
	MESH* tmp = new MESH(manager, node, log);
	tmp->load(filename);
	tmp->setID(meshID);
	tmp->getNode()->setMaterialFlag(video::EMF_LIGHTING, true);
	tmp->getNode()->setMaterialType(video::EMT_SOLID);
	tmp->setPosition(pos);
	tmp->setRotation(rot);
	tmp->setScale(scale);
	meshID++;
	meshs.push_back(tmp);
	return meshID-1;
}
MESH* SCENE::editMesh(int id)
{
	//log->debugData("Getting mesh", id);
	for(std::vector<MESH*>::iterator it = meshs.begin(); it < meshs.end(); it++)
	{
		if((*it)->getID()==id)
		{
			//log->debugData("Got mesh", id);
			return (*it);
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
	temp->setID(lightID);
	lightID++;
	temp->init();
	lights.push_back(temp);
	return lightID-1;
}
LIGHT* SCENE::editLight(int id)
{
	//log->debugData("Checking for light", id);
	for(std::vector<LIGHT*>::iterator it = lights.begin(); it < lights.end(); it++)
	{
		if((*it)->getID()==id)
		{
			//log->debugData("Found light", id);
			return (*it);
		}
	}
	log->logData("Light not found. ID", id);
	return NULL;
}
