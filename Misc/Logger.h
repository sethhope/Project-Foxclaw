#ifndef _FLOGGER_H_
#define _FLOGGER_H_

#include "stdafx.h"
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <sstream>
namespace FCE
{
	class LOGGER
	{
		public:
			LOGGER();
			~LOGGER();
			int init(std::string filename);
			void logData(std::string data);
			void logData(std::string data, float num);
			void logData(std::string data1, std::string data2);
			void logData(std::string data1, char* data2);
			void debugData(std::string data);
			void debugData(std::string data, float num);
			void debugData(std::string data1, std::string data2);
			void debugData(std::string data1, char* data2);
			void setDebug(bool debug);
			std::string backlog[10];
		private:
			bool debug;
			std::ofstream file;
			
			
	};
}
#endif //_LOGGER_H_
