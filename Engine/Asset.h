/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

// Simple asset class that provides management over what is loaded into memory and what is not
class Asset {
private:
	size_t _lock;
	bool _dummyLock = false;
public:
	void grab();
	void release();
protected:
	virtual void load();
	virtual void unload();
};