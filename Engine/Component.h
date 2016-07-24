/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _COMPONENT_H
#define _COMPONENT_H

class Component {
public:
	virtual void update(float deltaTime) = 0;
	virtual ~Component();
};

#endif