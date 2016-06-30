/**  Copyright 2016 MarFil Studios. All rights reserved.  **/
#ifndef _ConfigurationManager_H
#define _ConfigurationManager_H

//TODO: Find a better string class
//#include <string>
//
//using namespace std;

class ConfigurationManager {
public:
	//General config
	const char* getString(const char* id);
	int getInt(const char* id);
	float getFloat(const char* id);
	//Scenes
	const char* getScenePath(const char* id);
	const char* getScenePath(int id);
};

#endif