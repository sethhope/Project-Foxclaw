#include "Misc/Logger.h"
using namespace FCE;

LOGGER::LOGGER()
{
	
}

LOGGER::~LOGGER()
{
	
}

int LOGGER::init(std::string filename)
{
	file.open(filename.c_str());
	debug = true;
	return 0;
}

void LOGGER::logData(std::string data)
{
	file  << data << std::endl << std::flush;
	std::cout  << data << std::endl << std::flush;
	for(int i = 0; i < 9; i++)
	{
		backlog[i]=backlog[i+1];
	}
	std::stringstream tmp;
	tmp << data;
	backlog[9] = tmp.str();
	
}

void LOGGER::logData(std::string data, float num)
{
	file << data << " : " << num << std::endl << std::flush;
	std::cout  << data << " : " << num << std::endl << std::flush;
	for(int i = 0; i < 9; i++)
	{
		backlog[i]=backlog[i+1];
	}
	std::stringstream tmp;
	tmp << data << " : " << num;
	backlog[9] = tmp.str();
}

void LOGGER::logData(std::string data1, std::string data2)
{
	file  << data1 << " : " << data2 << std::endl << std::flush;
	std::cout  << data1 << " : " << data2 << std::endl << std::flush;
	for(int i = 0; i < 9; i++)
	{
		backlog[i]=backlog[i+1];
	}
	std::stringstream tmp;
	tmp << data1 << " : " << data2;
	backlog[9] = tmp.str();
}
void LOGGER::logData(std::string data1, char* data2)
{
	file  << data1 << " : " << data2 << std::endl << std::flush;
	std::cout  << data1 << " : " << data2 << std::endl << std::flush;
	for(int i = 0; i < 9; i++)
	{
		backlog[i]=backlog[i+1];
	}
	std::stringstream tmp;
	tmp << data1 << " : " << data2;
	backlog[9] = tmp.str();
}
void LOGGER::debugData(int level, std::string data)
{
	if(debug >= level)
	{
		file <<"DEBUG LV."<< level << " : " << data << std::endl << std::flush;
		std::cout <<"DEBUG LV."<< level << " : " << data << std::endl << std::flush;
		for(int i = 0; i < 9; i++)
		{
			backlog[i]=backlog[i+1];
		}
		std::stringstream tmp;
		tmp << "DEBUG LV."<<level<<" : "<<data;
		backlog[9] = tmp.str();
	}
}

void LOGGER::debugData(int level, std::string data, float num)
{
	if(debug >= level)
	{
		file <<"DEBUG LV."<< level << " : " << data << " : " << num << std::endl << std::flush;
		std::cout <<"DEBUG LV."<< level << " : "  << data << " : " << num << std::endl << std::flush;
  for(int i = 0; i < 9; i++)
		{
			backlog[i]=backlog[i+1];
		}
  std::stringstream tmp;
		tmp <<"DEBUG LV."<< level << " : " << data << " : " << num;
		backlog[9] = tmp.str();
	}
}

void LOGGER::debugData(int level, std::string data1, std::string data2)
{
	if(debug >= level)
	{
		file << "DEBUG LV."<< level << " : "  << data1 << " : " << data2 << std::endl << std::flush;
		std::cout << "DEBUG LV."<< level << " : "  << data1 << " : " << data2 << std::endl << std::flush;
  for(int i = 0; i < 9; i++)
		{
			backlog[i]=backlog[i+1];
		}
		std::stringstream tmp;
		tmp <<"DEBUG LV."<< level << " : " << data1 << " : " << data2;
		backlog[9] = tmp.str();
	}
}
void LOGGER::debugData(int level, std::string data1, char* data2)
{
	if(debug >= level)
	{
		file << "DEBUG LV."<< level << " : "  << data1 << " : " << data2 << std::endl << std::flush;
		std::cout << "DEBUG LV."<< level << " : "  << data1 << " : " << data2 << std::endl << std::flush;
  for(int i = 0; i < 9; i++)
		{
			backlog[i]=backlog[i+1];
		}
		std::stringstream tmp;
		tmp <<"DEBUG LV."<< level << " : " << data1 << " : " << data2;
		backlog[9] = tmp.str();
	}
}
void LOGGER::setDebug(int debug)
{
	this->debug = debug;
}
