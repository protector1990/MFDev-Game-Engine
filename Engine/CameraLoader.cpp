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

}

GameObject* CameraLoader::load(xml_node<char>* configuration) {
	cameraLoadingMutex.lock();
	Camera* ret = new Camera();
}