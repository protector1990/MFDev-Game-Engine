/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "NavGrid.h"
#include "Area.h"
#include <set>
#include "PriorityQueue.h"
#include <cfloat>
using namespace glm;
using namespace std;

NavPoint::NavPoint() {
}

NavPoint::NavPoint(const NavPoint& other) {
	_position = other._position;
	_neighbours = other._neighbours;
}

NavPoint::NavPoint(vec3 &position, vector<NavPoint*> neighbours) {
	_position = _position;
	_neighbours = _neighbours;
}

NavPoint::NavPoint(const vec3& position) {
	_position = position;
}

NavPointSearchAdapter::NavPointSearchAdapter(NavPoint* navPoint, float score) 
	: _navPoint(navPoint), _scoreToArrive(score)
{}

NavPointSearchAdapter::NavPointSearchAdapter(NavPoint* navPoint)
	: _navPoint(navPoint)
{
	_scoreToArrive = FLT_MAX;
}


float NavPointSearchAdapter::_funScoreToArrive() const {
	if (_previous)
	{
		return _previous->_funScoreToArrive() + _navPoint->_cost;
	}
	return _navPoint->_cost;
}

bool NavPointSearchAdapter::equals(NavPointSearchAdapter* other) {
	return _navPoint == other->_navPoint;
}

NavPointSearchAdapterComparer::NavPointSearchAdapterComparer(NavPoint* destination) : _destination(destination) {}

bool NavPointSearchAdapterComparer::compare(NavPointSearchAdapter* const& left, NavPointSearchAdapter* const& right) {
	if ((left)->_distanceToDestination < 0)
	{
		float xDist = (left)->_navPoint->_position.x - _destination->_position.x;
		float yDist = (left)->_navPoint->_position.y - _destination->_position.y;
		(left)->_distanceToDestination = sqrt(xDist * xDist + yDist * yDist);
	}
	if ((right)->_distanceToDestination < 0)
	{
		float xDist = (right)->_navPoint->_position.x - _destination->_position.x;
		float yDist = (right)->_navPoint->_position.y - _destination->_position.y;
		(right)->_distanceToDestination = sqrt(xDist * xDist + yDist * yDist);
	}
	return (left->_funScoreToArrive() + left->_distanceToDestination) < right->_funScoreToArrive() + right->_distanceToDestination;
}

NavPath* NavGrid::getNavPath(vec3 start, vec3 end, NavPath& out) const {
	// Main pathfinding algorithm implementation
	// First find closest navPoints to start and end
	NavPoint *startWaypoint, *endWaypoint;
	float waypointDensitySquared = _sampleDensity;
	waypointDensitySquared *= waypointDensitySquared;
	size_t navPointsSize = _navPoints.size();
	for (size_t i = 0; i < navPointsSize; ++i)
	{
		float distanceStartX = start.x - _navPoints[i]->_position.x;
		float distanceStartY = start.y - _navPoints[i]->_position.y;
		float distanceStartZ = start.z - _navPoints[i]->_position.z;
		float squareDistStart = distanceStartX * distanceStartX + distanceStartY * distanceStartY + distanceStartZ * distanceStartZ;
		if (squareDistStart <= waypointDensitySquared)
		{
			startWaypoint = _navPoints[i];
			break;
		}
	}
	for (size_t i = 0; i < navPointsSize; ++i)
	{
		float distanceEndX = start.x - _navPoints[i]->_position.x;
		float distanceEndY = start.y - _navPoints[i]->_position.y;
		float distanceEndZ = start.z - _navPoints[i]->_position.y;
		float squareDistStart = distanceEndX * distanceEndX + distanceEndY * distanceEndY + distanceEndZ * distanceEndZ;
		if (squareDistStart <= waypointDensitySquared)
		{
			endWaypoint = _navPoints[i];
			break;
		}
	}
	if (!startWaypoint || !endWaypoint) return nullptr;
	// Now find the shortest path

	PriorityQueue<NavPointSearchAdapter*> openSet;
	openSet.setComparer(new NavPointSearchAdapterComparer(endWaypoint));

	std::set<NavPointSearchAdapter*> closedSet;
	
	openSet.insert (new NavPointSearchAdapter(startWaypoint, 0));
	while (openSet.size() > 0)
	{
		NavPointSearchAdapter* a = openSet.top();
		if (a->_navPoint == endWaypoint)
		{
			NavPath* path = new NavPath();
			do
			{
				path->_path.push_back(a->_navPoint);
				a = a->_previous;
			} while (a);
			return path;
		}
		openSet.pop();
		for (size_t i = 0; i < a->_navPoint->_neighbours.size(); ++i)
		{
			float costToA = a->_funScoreToArrive();
			NavPointSearchAdapter* adapter = new NavPointSearchAdapter(a->_navPoint->_neighbours[i], costToA + a->_navPoint->_cost);
			if (!openSet._containsEqual(adapter)) {
				openSet.insert(adapter);
			}
			else
			{
				// Check if there is a shorter path
				if (a->_funScoreToArrive() + adapter->_navPoint->_cost < adapter->_funScoreToArrive())
				{
					adapter->_previous = a;
				}
			}
		}
	}
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
		//TODO: refactor this ASAP
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
	//TODO: Change the system so each obstacle has its cost of passing through (for those hard obstacles, like rocks, cost is infinity)
	vector<GameObject*> objects = _scene->getObjectsWithTags(TAG_NAV_OBSTACLE);
	size_t vectorSize = objects.size();
	size_t navPointsSize = _navPoints.size();
	for (size_t j = 0; j < navPointsSize; ++j) {
		for (size_t i = 0; i < vectorSize; ++i)
		{
			Area* currentArea = objects[i]->getChildPolymorphic<Area>();
			if (currentArea->contains(_navPoints[j]->_position))
			{
				_navPoints[j]->_reachable = false;
				break;
			}
		}
	}
}

void NavGrid::update(float deltaTime) {
	// This can be useful someday if we wanted dynamically changing nav grid (unlike those unity nav mesh that can be prebaked
	// and not changed
	// That could be implemented with having game objects with colliders registering their move, and then nav grid would 
	// check if boundaries of the object have moved more than half of the _sampleDensity and then recalculate the grid.
	//P.S. That won't suffice for rotation...
}

void NavGrid::render() {
	// This can be used for debug purposes
}

void NavGrid::setSampleDensity(int density) {
}

void NavGrid::setBorders(vec4 borders) {
}
