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

void SCENE::init(FEventReceiver receiver)
{
	this->receiver = receiver;
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
	lastGUI = 1;
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
	log->logData("Created button");
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
	tmp->getIrrNode()->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
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
	tmp->setPosition(pos);
	tmp->setRotation(rot);
	tmp->setScale(scale);
	tmp->init();
	tmp->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, true);
	tmp->getIrrNode()->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	tmp->getIrrNode()->setMaterialType(video::EMT_SOLID);
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
	log->logData("Couldn't find animated mesh", id);
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
	log->logData("Object doesn't exist. ID", id);
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
	skyFile = filename;
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

bool SCENE::MouseDown(u8 button)
{
	return receiver.MouseButton(button);
}

position2di SCENE::MousePos()
{
	return receiver.mousePos();
}
u32 SCENE::addGui(GUI* gui, u32 parentID)
{
	gui->guiCaller = lastGUI;
	gui->element->setID(lastGUI);
	if(parentID > 0)
	{
		bool found = false;
		for(std::vector<GUI*>::iterator it = callers.begin(); it < callers.end(); it++)
		{
			if((*it)->guiCaller == parentID)
			{
				gui->setParent((*it));
				found = true;
			}
		}
		if(!found)
		{
			for(std::vector<GUI*>::iterator it = guiObjects.begin(); it < guiObjects.end(); it++)
			{
				if((*it)->guiCaller == parentID)
				{
					gui->setParent((*it));
					found = true;
				}
			}
			if(!found)
			{
				log->logData("Parent ID not found", parentID);
				log->logData("Failed to set parent for", gui->guiCaller);
			}
		}
	}
	guiObjects.push_back(gui);
	lastGUI++;
	return lastGUI-1;
}

void SCENE::setMetaData(std::string key, f32 data)
{
	metadata[key] = data;
}
f32 SCENE::getMetaData(std::string key)
{
	return metadata[key];
}

void SCENE::load(std::string filename)
{
	log->debugData(MAJOR, "Loading level", filename);
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if((*it)->getOType() != "CAMERA")
		{
			manager->addToDeletionQueue((*it)->getIrrNode());
			if((*it)->hasCollider)
			{
				world->addToDeletionQueue((*it)->getCollider()->body);
			}
		}
	}
	camera->setTarget(core::vector3df(0, 0, 0));
	objects.clear();
	lastID = 0;
	io::IXMLReader* xml = device->getFileSystem()->createXMLReader(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str());
	bool inObject = false;
	u32 currentID;
	core::stringw currentType;
	std::map<int, int> parentIDs;
	xml->read();
	while(xml)
	{
		switch(xml->getNodeType())
		{
			case io::EXN_ELEMENT:
				if(core::stringw(L"SCENECONFIG").equals_ignore_case(xml->getNodeName()))
				{
					log->debugData(MAJOR, "Within SCENECONFIG");
					bool inConfig = true;
					int last;
					core::stringw skyfile;
					while(xml && xml->read() && inConfig)
					{
						switch(xml->getNodeType())
						{
							case io::EXN_ELEMENT:
								if(core::stringw(L"lastID").equals_ignore_case(xml->getNodeName()))
								{
									last = xml->getAttributeValueAsFloat(0);
								}
								if(core::stringw(L"skybox").equals_ignore_case(xml->getNodeName()))
								{
									skyfile = xml->getAttributeValue(0);
								}
							break;
							case io::EXN_ELEMENT_END:
								if(core::stringw(L"SCENECONFIG").equals_ignore_case(xml->getNodeName()))
								{
									log->debugData(MAJOR, "leaving SCENECONFIG");
									inConfig = false;
								}
							break;
						}
					}
					if(!skyfile.empty())
					{
						core::stringc sfile = core::stringc(skyfile);
						setSkydome(sfile.c_str());
					}
					lastID = last;
				}
				else if(core::stringw(L"MESH").equals_ignore_case(xml->getNodeName()))
				{
					log->logData("FOUND MESH");
					bool inMesh = true;
					core::stringw name;
					core::stringw filename;
					core::vector3df pos;
					core::vector3df rot;
					core::vector3df scale;
					int id;
					int parentID;
					bool hasCollider=false;
					int colliderType;
					f32 mass;
					while(xml && xml->read() && inMesh)
					{
						switch(xml->getNodeType())
						{
							case io::EXN_ELEMENT:
								if(core::stringw(L"file").equals_ignore_case(xml->getNodeName()))
								{
									filename = xml->getAttributeValue(0);
								}
								if(core::stringw(L"id").equals_ignore_case(xml->getNodeName()))
								{
									id = xml->getAttributeValueAsFloat(0);
									parentID = xml->getAttributeValueAsFloat(1);
									parentIDs[id] = parentID;
								}
								if(core::stringw(L"name").equals_ignore_case(xml->getNodeName()))
								{
									name = xml->getAttributeValue(0);
								}
								if(core::stringw(L"position").equals_ignore_case(xml->getNodeName()))
								{
									pos = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"rotation").equals_ignore_case(xml->getNodeName()))
								{
									rot = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"scale").equals_ignore_case(xml->getNodeName()))
								{
									scale = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"Collider").equals_ignore_case(xml->getNodeName()))
								{
									if(core::stringw(L"true").equals_ignore_case(xml->getAttributeValue(0)))
									{
										hasCollider=true;
										colliderType = xml->getAttributeValueAsFloat(1);
										mass = xml->getAttributeValueAsFloat(2);
									}
								}
							break;
							case io::EXN_ELEMENT_END:
								if(core::stringw(L"MESH").equals_ignore_case(xml->getNodeName()))
								{
									log->debugData(MAJOR, "leaving MESH");
									inMesh = false;
								}
							break;
						}
					}
					core::stringc cname = core::stringc(name);
					core::stringc cfilename = core::stringc(filename);
					
					MESH* tmpMesh = new MESH(manager, log);
					tmpMesh->setID(id);
					tmpMesh->load((std::string)(device->getFileSystem()->getAbsolutePath(cfilename.c_str()).c_str()));
					tmpMesh->setName(cname.c_str());
					tmpMesh->setPosition(pos);
					tmpMesh->setRotation(rot);
					tmpMesh->setScale(scale);
					tmpMesh->init();
					tmpMesh->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, true);
					tmpMesh->getIrrNode()->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
					tmpMesh->getIrrNode()->setMaterialType(video::EMT_SOLID);
					tmpMesh->update();
					if(hasCollider)
					{
						tmpMesh->addCollider(log, colliderType, manager, world, mass, tmpMesh->getMesh());
					}
					objects.push_back(tmpMesh);

				}
				else if(core::stringw(L"ANIMATEDMESH").equals_ignore_case(xml->getNodeName()))
				{
					log->logData("FOUND ANIMATED MESH");
					bool inMesh = true;
					core::stringw name;
					core::stringw filename;
					core::vector3df pos;
					core::vector3df rot;
					core::vector3df scale;
					int id;
					int parentID;
					bool hasCollider=false;
					int colliderType;
					f32 mass;
					while(xml && xml->read() && inMesh)
					{
						switch(xml->getNodeType())
						{
							case io::EXN_ELEMENT:
								if(core::stringw(L"file").equals_ignore_case(xml->getNodeName()))
								{
									filename = xml->getAttributeValue(0);
								}
								if(core::stringw(L"id").equals_ignore_case(xml->getNodeName()))
								{
									id = xml->getAttributeValueAsFloat(0);
									parentID = xml->getAttributeValueAsFloat(1);
									parentIDs[id] = parentID;
								}
								if(core::stringw(L"name").equals_ignore_case(xml->getNodeName()))
								{
									name = xml->getAttributeValue(0);
								}
								if(core::stringw(L"position").equals_ignore_case(xml->getNodeName()))
								{
									pos = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"rotation").equals_ignore_case(xml->getNodeName()))
								{
									rot = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"scale").equals_ignore_case(xml->getNodeName()))
								{
									scale = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"Collider").equals_ignore_case(xml->getNodeName()))
								{
									if(core::stringw(L"true").equals_ignore_case(xml->getAttributeValue(0)))
									{
										hasCollider=true;
										colliderType = xml->getAttributeValueAsFloat(1);
										mass = xml->getAttributeValueAsFloat(2);
									}
								}
							break;
							case io::EXN_ELEMENT_END:
								if(core::stringw(L"ANIMATEDMESH").equals_ignore_case(xml->getNodeName()))
								{
									inMesh = false;
								}
							break;
						}
					}
					core::stringc cname = core::stringc(name);
					core::stringc cfilename = core::stringc(filename);

					ANIMATEDMESH* tmpMesh = new ANIMATEDMESH(manager, log);
					tmpMesh->setID(id);
					tmpMesh->load((std::string)(device->getFileSystem()->getAbsolutePath(cfilename.c_str()).c_str()));
					tmpMesh->setName(cname.c_str());
					tmpMesh->setPosition(pos);
					tmpMesh->setRotation(rot);
					tmpMesh->setScale(scale);
					tmpMesh->init();
					tmpMesh->getIrrNode()->setMaterialFlag(video::EMF_LIGHTING, true);
					tmpMesh->getIrrNode()->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
					tmpMesh->getIrrNode()->setMaterialType(video::EMT_SOLID);
					tmpMesh->update();
					if(hasCollider)
					{
						tmpMesh->addCollider(log, colliderType, manager, world, mass, tmpMesh->getMesh());
					}
					objects.push_back(tmpMesh);
				}
				else if(core::stringw(L"LIGHT").equals_ignore_case(xml->getNodeName()))
				{
					log->logData("FOUND LIGHT");
					bool inLight = true;
					core::stringw name;
					core::vector3df pos;
					core::vector3df rot;
					core::vector3df scale;
					int id;
					int parentID;
					int type;
					float dropoff;
					video::SColorf color;
					/*bool hasCollider=false;
					int colliderType;
					f32 mass;*/
					while(xml && xml->read() && inLight)
					{
						switch(xml->getNodeType())
						{
							case io::EXN_ELEMENT:
								if(core::stringw(L"id").equals_ignore_case(xml->getNodeName()))
								{
									id = xml->getAttributeValueAsFloat(0);
									parentID = xml->getAttributeValueAsFloat(1);
									parentIDs[id] = parentID;
								}
								if(core::stringw(L"name").equals_ignore_case(xml->getNodeName()))
								{
									name = xml->getAttributeValue(0);
								}
								if(core::stringw(L"data").equals_ignore_case(xml->getNodeName()))
								{
									type = xml->getAttributeValueAsInt(0);
									dropoff = xml->getAttributeValueAsFloat(1);
								}
								if(core::stringw(L"color").equals_ignore_case(xml->getNodeName()))
								{
									color = video::SColorf(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"position").equals_ignore_case(xml->getNodeName()))
								{
									pos = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"rotation").equals_ignore_case(xml->getNodeName()))
								{
									rot = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								if(core::stringw(L"scale").equals_ignore_case(xml->getNodeName()))
								{
									scale = core::vector3df(xml->getAttributeValueAsFloat(0), xml->getAttributeValueAsFloat(1), xml->getAttributeValueAsFloat(2));
								}
								/*if(core::stringw(L"Collider").equals_ignore_case(xml->getNodeName()))
								{
									if(core::stringw(L"true").equals_ignore_case(xml->getAttributeValue(0)))
									{
										hasCollider=true;
										colliderType = xml->getAttributeValueAsFloat(1);
										mass = xml->getAttributeValueAsFloat(2);
									}
								}*/
							break;
							case io::EXN_ELEMENT_END:
								if(core::stringw(L"LIGHT").equals_ignore_case(xml->getNodeName()))
								{
									inLight = false;
								}
							break;
						}
					}
					core::stringc cname = core::stringc(name);
					video::E_LIGHT_TYPE light;
					if(type == 0)
					{
						light = video::ELT_POINT;
					}
					if(type == 1)
					{
						light = video::ELT_SPOT;
					}
					if(type == 2)
					{
						light = video::ELT_DIRECTIONAL;
					}
					LIGHT* tempLight = new LIGHT(manager, log);
					tempLight->setColor(color);
					tempLight->setPosition(pos);
					tempLight->setScale(scale);
					tempLight->setRotation(rot);
					tempLight->setType(light);
					tempLight->setDropoff(dropoff);
					tempLight->setID(id);
					tempLight->setName(cname.c_str());
					tempLight->init();
					tempLight->update();
					objects.push_back(tempLight);
				}
				else if(core::stringw(L"PARTICLE").equals_ignore_case(xml->getNodeName()))
				{
					log->logData("FOUND PARTICLE");
				}
				else
				{
					if(!xml->read())
					{
						return;
					}
				}
			break;
			case io::EXN_ELEMENT_END:
				if(!xml->read())
				{
					return;
				}
				break;
			default:
				if(!xml->read())
				{
					return;
				}
			break;
		}
	}
}

void SCENE::save(std::string filename)
{
	io::IXMLWriter* xml = device->getFileSystem()->createXMLWriter(device->getFileSystem()->getAbsolutePath(filename.c_str()).c_str());
	xml->writeXMLHeader();
	xml->writeComment(L"Project Foxclaw Generated Level File");
	xml->writeLineBreak();
	xml->writeElement(L"SCENECONFIG", false);
	xml->writeLineBreak();
	xml->writeElement(L"lastID", true, L"lastID", core::stringw(lastID).c_str());
	xml->writeLineBreak();
	if(!skyFile.empty())
	{
		xml->writeElement(L"skybox", true, L"filename", core::stringw(skyFile.c_str()).c_str());
		xml->writeLineBreak();
	}
	xml->writeClosingTag(L"SCENECONFIG");
	xml->writeLineBreak();
	for(std::vector<OBJECT*>::iterator it = objects.begin(); it < objects.end(); it++)
	{
		if((*it)->getOType() == "MESH")
		{
			xml->writeElement(L"MESH", false);
			xml->writeLineBreak();
			core::stringw file = ((MESH*)(*it))->getFilename().c_str();
			file = device->getFileSystem()->getRelativeFilename(file, device->getFileSystem()->getWorkingDirectory());
			xml->writeElement(L"file", true, L"filename", file.c_str());
			xml->writeLineBreak();
			int parentID = -1;
			if((*it)->getParent() != NULL)
			{
				parentID = ((OBJECT*)((*it)->getParent()))->getID();
			}
			xml->writeElement(L"id", true, L"id", core::stringw((*it)->getID()).c_str(), L"parent-id", core::stringw(parentID).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"name", true, L"name", core::stringw((*it)->getName().c_str()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"position", true, L"x", core::stringw((*it)->getPosition().X).c_str(), L"y", core::stringw((*it)->getPosition().Y).c_str(), L"z", core::stringw((*it)->getPosition().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"rotation", true, L"x", core::stringw((*it)->getRotation().X).c_str(), L"y", core::stringw((*it)->getRotation().Y).c_str(), L"z", core::stringw((*it)->getRotation().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"scale", true, L"x", core::stringw((*it)->getScale().X).c_str(), L"y", core::stringw((*it)->getScale().Y).c_str(), L"z", core::stringw((*it)->getScale().Z).c_str());
			xml->writeLineBreak();
			if((*it)->hasCollider)
			{
				xml->writeElement(L"Collider", true, L"hasCollider", L"true", L"type", core::stringw((*it)->getCollider()->getType()).c_str(), L"mass", core::stringw((*it)->getCollider()->getMass()).c_str());
			}else
			{
				xml->writeElement(L"Collider", true, L"hasCollider", L"false");
			}
			xml->writeLineBreak();
			xml->writeClosingTag(L"MESH");
			xml->writeLineBreak();
			xml->writeLineBreak();
		}
		if((*it)->getOType() == "ANIMATEDMESH")
		{
			xml->writeElement(L"ANIMATEDMESH", false);
			xml->writeLineBreak();
			core::stringw file = ((ANIMATEDMESH*)(*it))->getFilename().c_str();
			file = device->getFileSystem()->getRelativeFilename(file, device->getFileSystem()->getWorkingDirectory());
			xml->writeElement(L"file", true, L"filename", file.c_str());
			xml->writeLineBreak();
			int parentID = -1;
			if((*it)->getParent() != NULL)
			{
				parentID = ((OBJECT*)((*it)->getParent()))->getID();
			}
			xml->writeElement(L"id", true, L"id", core::stringw((*it)->getID()).c_str(), L"parent-id", core::stringw(parentID).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"name", true, L"name", core::stringw((*it)->getName().c_str()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"position", true, L"x", core::stringw((*it)->getPosition().X).c_str(), L"y", core::stringw((*it)->getPosition().Y).c_str(), L"z", core::stringw((*it)->getPosition().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"rotation", true, L"x", core::stringw((*it)->getRotation().X).c_str(), L"y", core::stringw((*it)->getRotation().Y).c_str(), L"z", core::stringw((*it)->getRotation().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"scale", true, L"x", core::stringw((*it)->getScale().X).c_str(), L"y", core::stringw((*it)->getScale().Y).c_str(), L"z", core::stringw((*it)->getScale().Z).c_str());
			xml->writeLineBreak();
			if((*it)->hasCollider)
			{
				xml->writeElement(L"Collider", true, L"hasCollider", L"true", L"type", core::stringw((*it)->getCollider()->getType()).c_str(), L"mass", core::stringw((*it)->getCollider()->getMass()).c_str());
			}else
			{
				xml->writeElement(L"Collider", true, L"hasCollider", L"false");
			}
			xml->writeLineBreak();
			xml->writeClosingTag(L"ANIMATEDMESH");
			xml->writeLineBreak();
			xml->writeLineBreak();
		}
		if((*it)->getOType() == "LIGHT")
		{
			xml->writeElement(L"LIGHT", false);
			xml->writeLineBreak();
			int parentID = -1;
			if((*it)->getParent() != NULL)
			{
				parentID = ((OBJECT*)((*it)->getParent()))->getID();
			}
			u8 type = 0;
			if(((LIGHT*)(*it))->getType() == video::ELT_POINT)
			{
				type = 0;
			}
			if(((LIGHT*)(*it))->getType() == video::ELT_SPOT)
			{
				type = 1;
			}
			if(((LIGHT*)(*it))->getType() == video::ELT_DIRECTIONAL)
			{
				type = 2;
			}
			xml->writeElement(L"id", true, L"id", core::stringw((*it)->getID()).c_str(), L"parent-id", core::stringw(parentID).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"name", true, L"name", core::stringw((*it)->getName().c_str()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"data", true, L"type", core::stringw(type).c_str(), L"dropoff", core::stringw(((LIGHT*)(*it))->getDropoff()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"color", true, L"r", core::stringw(((LIGHT*)(*it))->getColor().getRed()).c_str(), L"g", core::stringw(((LIGHT*)(*it))->getColor().getGreen()).c_str(), L"b", core::stringw(((LIGHT*)(*it))->getColor().getBlue()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"position", true, L"x", core::stringw((*it)->getPosition().X).c_str(), L"y", core::stringw((*it)->getPosition().Y).c_str(), L"z", core::stringw((*it)->getPosition().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"rotation", true, L"x", core::stringw((*it)->getRotation().X).c_str(), L"y", core::stringw((*it)->getRotation().Y).c_str(), L"z", core::stringw((*it)->getRotation().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"scale", true, L"x", core::stringw((*it)->getScale().X).c_str(), L"y", core::stringw((*it)->getScale().Y).c_str(), L"z", core::stringw((*it)->getScale().Z).c_str());
			xml->writeLineBreak();
			xml->writeClosingTag(L"LIGHT");
			xml->writeLineBreak();
			xml->writeLineBreak();
		}
		if((*it)->getOType() == "EMPTYOBJECT")
		{
			xml->writeElement(L"EMPTY", false);
			xml->writeLineBreak();
			int parentID = -1;
			if((*it)->getParent() != NULL)
			{
				parentID = ((OBJECT*)((*it)->getParent()))->getID();
			}
			xml->writeElement(L"id", true, L"id", core::stringw((*it)->getID()).c_str(), L"parent-id", core::stringw(parentID).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"name", true, L"name", core::stringw((*it)->getName().c_str()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"position", true, L"x", core::stringw((*it)->getPosition().X).c_str(), L"y", core::stringw((*it)->getPosition().Y).c_str(), L"z", core::stringw((*it)->getPosition().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"rotation", true, L"x", core::stringw((*it)->getRotation().X).c_str(), L"y", core::stringw((*it)->getRotation().Y).c_str(), L"z", core::stringw((*it)->getRotation().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"scale", true, L"x", core::stringw((*it)->getScale().X).c_str(), L"y", core::stringw((*it)->getScale().Y).c_str(), L"z", core::stringw((*it)->getScale().Z).c_str());
			xml->writeLineBreak();
			xml->writeClosingTag(L"EMPTY");
			xml->writeLineBreak();
			xml->writeLineBreak();
		}
		if((*it)->getOType() == "PARTICLE")
		{
			xml->writeElement(L"PARTICLE", false);
			xml->writeLineBreak();
			int parentID = -1;
			if((*it)->getParent() != NULL)
			{
				parentID = ((OBJECT*)((*it)->getParent()))->getID();
			}
			xml->writeElement(L"id", true, L"id", core::stringw((*it)->getID()).c_str(), L"parent-id", core::stringw(parentID).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"name", true, L"name", core::stringw((*it)->getName().c_str()).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"position", true, L"x", core::stringw((*it)->getPosition().X).c_str(), L"y", core::stringw((*it)->getPosition().Y).c_str(), L"z", core::stringw((*it)->getPosition().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"rotation", true, L"x", core::stringw((*it)->getRotation().X).c_str(), L"y", core::stringw((*it)->getRotation().Y).c_str(), L"z", core::stringw((*it)->getRotation().Z).c_str());
			xml->writeLineBreak();
			xml->writeElement(L"scale", true, L"x", core::stringw((*it)->getScale().X).c_str(), L"y", core::stringw((*it)->getScale().Y).c_str(), L"z", core::stringw((*it)->getScale().Z).c_str());
			xml->writeLineBreak();
			xml->writeClosingTag(L"PARTICLE");
			xml->writeLineBreak();
			xml->writeLineBreak();
		}
	}
	xml->drop();
}
