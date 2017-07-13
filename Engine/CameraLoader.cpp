/** Copyright 2017 MarFil studios. All rights reserved. **/

#include "CameraLoader.h"
#include "Camera.h"
#include <mutex>

using namespace rapidxml;
using namespace std;

mutex cameraLoadingMutex;

namespace CameraLoaderTextConstants {
	char* ortho = "ortho";
	char* xMin = "xMin";
	char* xMax = "xMax";
	char* yMin = "yMin";
	char* yMax = "yMax";
	char* zMin = "zMin";
	char* zMax = "zMax";

	char *viewport = "viewport";
	char *viewportX = "x";
	char *viewportY = "y";
	char *viewportWidth = "width";
	char *viewportHeight = "height";
	char *viewportFullScreen = "fullscreen";
}

GameObject* CameraLoader::load(xml_node<char>* configuration) {
	cameraLoadingMutex.lock();
	Camera* ret = new Camera();
	_currentlyLoadingObject = ret;
	commonLoad(configuration);

	xml_node<char>* ortho = configuration->first_node(CameraLoaderTextConstants::ortho);
	if (ortho) {
		xml_node<char>* xMin = ortho->first_node(CameraLoaderTextConstants::xMin);
		xml_node<char>* xMax = ortho->first_node(CameraLoaderTextConstants::xMax);
		xml_node<char>* yMin = ortho->first_node(CameraLoaderTextConstants::yMin);
		xml_node<char>* yMax = ortho->first_node(CameraLoaderTextConstants::yMax);
		xml_node<char>* zMin = ortho->first_node(CameraLoaderTextConstants::zMin);
		xml_node<char>* zMax = ortho->first_node(CameraLoaderTextConstants::zMax);
		float xMinFloat = atof(xMin->value());
		float xMaxFloat = atof(xMax->value());
		float yMinFloat = atof(yMin->value());
		float yMaxFloat = atof(yMax->value());
		float zMinFloat = atof(zMin->value());
		float zMaxFloat = atof(zMax->value());
		ret->setOrtho(xMinFloat, xMaxFloat, yMinFloat, yMaxFloat, zMinFloat, zMaxFloat);
	}
	xml_node<char>* viewport = configuration->first_node(CameraLoaderTextConstants::viewport);
	if (viewport) {
		xml_node<char>* fullScreen = viewport->first_node(CameraLoaderTextConstants::viewportFullScreen);
		if (fullScreen) {
			ret->_takesWholeScreen = true;
		}
	}
	cameraLoadingMutex.unlock();
	return ret;
}