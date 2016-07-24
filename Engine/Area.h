/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _AREA_H
#define _AREA_H

#include <glm/glm.hpp>
#include <vector>
#include "Component.h"

class Area : public Component {
public:
	std::vector<glm::vec3>* getPoints() const;
	const std::vector<glm::vec3>* getPointsConst() const;
	void addPoint(glm::vec3 point);
	virtual bool contains(glm::vec3 point) const;
private:
	std::vector<glm::vec3> _points;
};

#endif 