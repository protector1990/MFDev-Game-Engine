/**  Copyright 2016 MarFil Studios. All rights reserved.  **/
#ifndef _CONFIGURATION_MANAGER_H
#define _CONFIGURATION_MANAGER_H

#include "TextAsset.h"
#include <rapidxml-1.13/rapidxml.hpp>

//TODO: Find a better string class
//#include <string>
//
//using namespace std;

class ConfigurationManager {
protected:
	//TODO: Add support for multiple config files
	TextAsset* _configFileRaw;
	rapidxml::xml_document<> _configFile;
public:
	void init();
	//General config
	const char* getString(const char* id);
	int getInt(const char* id);
	float getFloat(const char* id);
	//Scenes
	const char* getScenePath(const char* name) const;
	const char* getScenePath(int orderNum) const;
};

#endif