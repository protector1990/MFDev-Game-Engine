/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _NAV_GRID_H
#define _NAV_GRID_H

#include "Common.h"
#include "GameObject.h"
#include "IComparer.h"
#include <glm/glm.hpp>

struct NavPoint {
	glm::vec3 _position;
	std::vector <NavPoint*> _neighbours;
	bool _reachable = true;
	float _cost = 1.f;
	NavPoint();
	NavPoint(const NavPoint& other);
	NavPoint(glm::vec3 &position, std::vector<NavPoint*> neighbours);
	NavPoint(const glm::vec3 &position);
};

struct NavPointSearchAdapter {
	NavPointSearchAdapter(NavPoint* navPoint, float score);
	NavPointSearchAdapter(NavPoint* navPoint);
	NavPoint *_navPoint;
	NavPointSearchAdapter* _previous;
	float _scoreToArrive;
	float _funScoreToArrive() const;
	bool equals(NavPointSearchAdapter* other);
	float _distanceToDestination;
};

class NavPointSearchAdapterComparer : public IComparer<NavPointSearchAdapter*> {
protected:
	NavPoint* _destination;
public:
	NavPointSearchAdapterComparer(NavPoint* destination);

	bool compare(NavPointSearchAdapter* const& left, NavPointSearchAdapter* const& right) override;
};

class NavPath {
	// Implement each NavPath as a fixed size reusable collection of NavPoints. Grow only as needed, remove visited nodes
	// and keep a pointer to next target node. That is a general plan
	friend class NavGrid;
public:
	std::vector<NavPoint*> _path;
};

class NavGrid : public GameObject {
public:
	explicit NavGrid()
		: _sampleDensity(10),
		  _initialized(false) {}

	NavPath* getNavPath(glm::vec3 start, glm::vec3 end, NavPath &out) const;
	~NavGrid() override;
	void init() override;
	void update(float deltaTime) override;
	void render(SpriteRenderer* renderer) override;
	void setSampleDensity(int density);
	void setBorders(glm::vec4 borders);
protected:
	std::vector <NavPoint*> _navPoints;
	int _sampleDensity; // How dense should the grid place waypoints
	bool _initialized;
	glm::vec4 _borders;
};

#endif