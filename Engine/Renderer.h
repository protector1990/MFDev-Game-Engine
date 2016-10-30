/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

class Engine;

class Renderer {
public:
	virtual void init() = 0;
	virtual void preRender() = 0;
	virtual void postRender() = 0;
protected:
	Renderer() {}
	friend class Video;
};