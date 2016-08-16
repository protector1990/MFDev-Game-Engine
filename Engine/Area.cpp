/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Area.h"
#include <cfloat>

using namespace std;
using namespace glm;

vector<vec3>* Area::getPoints() const 
{
	return new vector<vec3>(_points);
}

const vector<vec3>* Area::getPointsConst() const {
	return new vector<vec3>(_points);
}

void Area::addPoint(vec3 point) {
	_points.push_back(point);
}

bool Area::contains(vec3 point) const {
	size_t p1Count = _points.size();
	for (int i = 0; i < p1Count; i++) {
		int j = i + 1;
		if (j == p1Count) {
			j = 0;
		}
		vec3 edgeTested = _points[j] - _points[i];
		vec3 axis(-edgeTested.y, edgeTested.x, 0.f);
		float poly1Min = FLT_MAX;
		float poly1Max = -FLT_MAX;
		for (int a = 0; a < p1Count; a++) {
			vec3 verticeToVertice = _points[i] - _points[a];
			float projection = axis.x * verticeToVertice.x + axis.y * verticeToVertice.y;
			if (projection > poly1Max) {
				poly1Max = projection;
			}
			if (projection < poly1Min) {
				poly1Min = projection;
			}
		}

		vec3 dotVector = point - _points[i];
		float projection = axis.x * dotVector.x + axis.y * dotVector.y;

		if (projection < poly1Min || projection > poly1Max) {
			return false;
		}
	}
	return true;
}

void Area::init() {
}

void Area::update(float deltaTime) {
}
