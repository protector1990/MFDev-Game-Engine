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

	char* perspective = "perspective";
	char* near = "near";
	char* far = "far";
	char* horizontalAngle = "horizontalAngle";
	char* verticalAngle = "verticalAngle";

	char *viewport = "viewport";
	char *viewportX = "x";
	char *viewportY = "y";
	char *viewportWidth = "width";
	char *viewportHeight = "height";
	char *viewportFullScreen = "fullscreen";

	char *renderingOrder = "renderingOrder";
	char *clearFlags = "clearFlags";
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
	xml_node<char>* perspective = configuration->first_node(CameraLoaderTextConstants::perspective);
	if (perspective) {
		xml_node<char>* near = perspective->first_node(CameraLoaderTextConstants::near);
		xml_node<char>* far = perspective->first_node(CameraLoaderTextConstants::far);
		xml_node<char>* horizontalAngle = perspective->first_node(CameraLoaderTextConstants::horizontalAngle);
		xml_node<char>* verticalAngle = perspective->first_node(CameraLoaderTextConstants::verticalAngle);
		float nearFloat = atof(near->value());
		float farFloat = atof(far->value());
		float horizontalAngleFloat = atof(horizontalAngle->value());
		float verticalAngleFloat = atof(verticalAngle->value());
		ret->setPerspective(nearFloat, farFloat, horizontalAngleFloat, verticalAngleFloat);
	}
	xml_node<char>* viewport = configuration->first_node(CameraLoaderTextConstants::viewport);
	if (viewport) {
		xml_node<char>* fullScreen = viewport->first_node(CameraLoaderTextConstants::viewportFullScreen);
		if (fullScreen) {
			ret->setTakesWholeScreen(true);
		}
		else {
			xml_node<char>* x = viewport->first_node(CameraLoaderTextConstants::viewportX);
			xml_node<char>* y = viewport->first_node(CameraLoaderTextConstants::viewportY);
			xml_node<char>* width = viewport->first_node(CameraLoaderTextConstants::viewportWidth);
			xml_node<char>* height = viewport->first_node(CameraLoaderTextConstants::viewportHeight);
			ret->setViewport(glm::ivec4(atoi(x->value()), atoi(y->value()), atoi(width->value()), atoi(height->value())));
		}
	}
	else {
		// temporarily default to fullscreen
		ret->setTakesWholeScreen(true);
	}
	xml_node<char>* renderingOrder = configuration->first_node(CameraLoaderTextConstants::renderingOrder);
	if (renderingOrder){
		ret->setRenderingOrder(atoi(renderingOrder->value()));
	}
	xml_node<char>* clearFlags = configuration->first_node(CameraLoaderTextConstants::clearFlags);
	if (clearFlags){
		char *endchar = clearFlags->value() + clearFlags->value_size();
		unsigned short zz = strtoul(clearFlags->value(), &endchar, 10);
		ret->setClearFlags(zz);
	}
	cameraLoadingMutex.unlock();
	return ret;
}