/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "NavGrid.h"
using namespace glm;

NavPoint::NavPoint() {
}

NavPoint::NavPoint(const NavPoint& other) {
	_position = other._position;
	_neighbours = other._neighbours;
}

NavPoint::NavPoint(vec3 &position, std::vector<NavPoint*> neighbours) {
	_position = _position;
	_neighbours = _neighbours;
}

NavPoint::NavPoint(const glm::vec3& position) {
	_position = position;
}

void NavGrid::getNavPath(vec3 start, vec3 end, NavPath& out) const {
	// Main pathfinding algorithm implementation
}

NavGrid::~NavGrid() {
}

void NavGrid::init() {
	// Lay out the nav points
	float step = 1.f / _sampleDensity;
	float area = (_borders.y - _borders.x) * (_borders.w - _borders.z);
	int numberOfNavPoints = int(area * _sampleDensity) + 1;
	_navPoints.reserve(numberOfNavPoints);
	int columnHeight = int((_borders.w - _borders.z)*_sampleDensity) + 1;
	NavPoint **previousColumn = new NavPoint*[columnHeight];
	for (size_t i = 0; i < columnHeight; ++i)
	{
		previousColumn[i] = nullptr;
	}

	// TODO: Make the grid relative to its local coordinates. Fuck, we need that transform implemented really soon!
	for (float x = _borders.x; x < _borders.y; x += step)
	{
		// Not the best of solutions. Use matrix underneath instead of vector someday
		NavPoint *previous = nullptr;
		size_t previousColumnIndex = 0;
		for (float y = _borders.z; x < _borders.w; y += step) {
			NavPoint *a = new NavPoint(vec3(x, y, 0.f));
			_navPoints.push_back(a);
			if (previous) {
				a->_neighbours.push_back(previous);
				previous->_neighbours.push_back(a);
			}
			if (previousColumn[previousColumnIndex])
			{
				a->_neighbours.push_back(previousColumn[previousColumnIndex]);
				previousColumn[previousColumnIndex]->_neighbours.push_back(a);
			}
			previous = a;
			previousColumn[previousColumnIndex] = a;
			++previousColumnIndex;
		}
	}
	delete[] previousColumn;

	// Now calculate to see which nav points are available and which are not
	_scene->getObjectsWithTags(TAG_NAV_OBSTACLE);
}

void NavGrid::update(float deltaTime) {
	// This can be useful someday if we wanted dynamically changing nav grid (unlike those unity nav mesh that can be prebaked
	// and not changed
	// That could be implemented with having game objects with colliders registering their move, and then nav grid would 
	// check if boundaries of the object have moved more than half of the _sampleDensity and then recalculate the grid.
	//P.S. That won't suffice for rotation...
}

void NavGrid::render(Renderer* renderer) {
	// This can be used for debug purposes
}

void NavGrid::setSampleDensity(int density) {
}

void NavGrid::setBorders(glm::vec4 borders) {
}
