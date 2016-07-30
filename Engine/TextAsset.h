/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _TEXT_ASSET_H
#define _TEXT_ASSET_H

//TODO: remove const chars and make a copy constructor
class TextAsset {
public:
	TextAsset(){}
	TextAsset(const char *name, const char *contents, int size) :
		_name(name),
		_contents(contents),
		_size(size) {}
	virtual ~TextAsset() {
		delete[] _name;
		delete[] _contents;
	}
	TextAsset(const TextAsset& other);
	const char * _name;
	const char * _contents;
	int _size;
};

#endif // #ifndef _TEXT_ASSET_H