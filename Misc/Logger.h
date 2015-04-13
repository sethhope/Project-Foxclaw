#ifndef _FLOGGER_H_
#define _FLOGGER_H_

#include "stdafx.h"
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <sstream>
#define CLEAN 0
#define MINOR 1
#define MAJOR 2
#define EXTRA 3
#define ALL 3
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
    void debugData(int level, std::string data);
    void debugData(int level, std::string data, float num);
    void debugData(int level, std::string data1, std::string data2);
    void debugData(int level, std::string data1, char* data2);
    void setDebug(int debug);
    std::string backlog[10];
private:
    int debug;
    std::ofstream file;


};
}
#endif //_LOGGER_H_
