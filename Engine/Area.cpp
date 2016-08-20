/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Area.h"
#include <cfloat>
#include <SDL_opengl.h>

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
			if (a == i) continue;
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

#ifdef _DEBUG
void Area::renderDebug(float deltaTime) {
	glColor3f(0, 0, 0);
	glLineWidth(5.f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < _points.size(); ++i)
	{
		float x = _points[i].x;
		float y = _points[i].y;
		float z = _points[i].z;
		glVertex3f(_points[i].x, _points[i].y, _points[i].z);
	}
	glVertex3f(_points[0].x, _points[0].y, _points[0].z);
	glEnd();
	glFlush();
	//glPointSize(10.f);
	//glBegin(GL_POINTS);
	//glVertex3f(55, 45, 0);
	//glEnd();
	//glFlush();
}

#endif // _DEBUG
