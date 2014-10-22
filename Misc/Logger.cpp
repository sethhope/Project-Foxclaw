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
	for(int i = 1; i < 10; i++)
	{
		backlog[i] = backlog[i-1];
	}
	backlog[0] = data;
}

void LOGGER::logData(std::string data, int num)
{
	file << data << " : " << num << std::endl << std::flush;
	std::cout  << data << " : " << num << std::endl << std::flush;
	for(int i = 1; i < 10; i++)
	{
		backlog[i] = backlog[i-1];
	}
	std::stringstream tmp;
	tmp << data << " : " << num;
	backlog[0] = tmp.str();
}

void LOGGER::logData(std::string data1, std::string data2)
{
	file  << data1 << " : " << data2 << std::endl << std::flush;
	std::cout  << data1 << " : " << data2 << std::endl << std::flush;
	for(int i = 1; i < 10; i++)
	{
		backlog[i] = backlog[i-1];
	}
	std::stringstream tmp;
	tmp << data1 << " : " << data2;
	backlog[0] = tmp.str();
}
void LOGGER::logData(std::string data1, char* data2)
{
	file  << data1 << " : " << data2 << std::endl << std::flush;
	std::cout  << data1 << " : " << data2 << std::endl << std::flush;
	for(int i = 1; i < 10; i++)
	{
		backlog[i] = backlog[i-1];
	}
	std::stringstream tmp;
	tmp << data1 << " : " << data2;
	backlog[0] = tmp.str();
}
void LOGGER::debugData(std::string data)
{
	if(debug)
	{
		file <<"DEBUG: " << data << std::endl << std::flush;
		std::cout <<"DEBUG: " << data << std::endl << std::flush;
		for(int i = 1; i < 10; i++)
		{
			backlog[i] = backlog[i-1];
		}
		backlog[0] = data;
	}
}

void LOGGER::debugData(std::string data, int num)
{
	if(debug)
	{
		file <<"DEBUG: "<< data << " : " << num << std::endl << std::flush;
		std::cout <<"DEBUG: " << data << " : " << num << std::endl << std::flush;
		for(int i = 1; i < 10; i++)
		{
			backlog[i] = backlog[i-1];
		}
		std::stringstream tmp;
		tmp << data << " : " << num;
		backlog[0] = tmp.str();
	}
}

void LOGGER::debugData(std::string data1, std::string data2)
{
	if(debug)
	{
		file << "DEBUG: " << data1 << " : " << data2 << std::endl << std::flush;
		std::cout << "DEBUG: " << data1 << " : " << data2 << std::endl << std::flush;
		for(int i = 1; i < 10; i++)
		{
			backlog[i] = backlog[i-1];
		}
		std::stringstream tmp;
		tmp << data1 << " : " << data2;
		backlog[0] = tmp.str();
	}
}
void LOGGER::debugData(std::string data1, char* data2)
{
	if(debug)
	{
		file << "DEBUG: " << data1 << " : " << data2 << std::endl << std::flush;
		std::cout << "DEBUG: " << data1 << " : " << data2 << std::endl << std::flush;
		for(int i = 1; i < 10; i++)
		{
			backlog[i] = backlog[i-1];
		}
		std::stringstream tmp;
		tmp << data1 << " : " << data2;
		backlog[0] = tmp.str();
	}
}
void LOGGER::setDebug(bool debug)
{
	this->debug = debug;
}
