/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#ifndef _PHYSFS_2_ASSIMP_INTERFACE_H
#define _PHYSFS_2_ASSIMP_INTERFACE_H

#include <assimp-3.2\include\assimp\IOStream.hpp>
#include <assimp-3.2\include\assimp\IOSystem.hpp>
#include <physfs.h>

using namespace Assimp;

class P2AIOStream : public IOStream {
	friend class MyIOSystem;
protected:
	PHYSFS_File* file;
	P2AIOStream(void);
public:
	P2AIOStream(const char* filePath);
	~P2AIOStream(void);
	size_t Read(void* pvBuffer, size_t pSize, size_t pCount);
	size_t Write(const void* pvBuffer, size_t pSize, size_t pCount);
	aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
	size_t Tell() const;
	size_t FileSize() const;
	void Flush();
};

class P2AIOSystem : public IOSystem {
public:
	P2AIOSystem() {}
	~P2AIOSystem() {}

	bool Exists(const char* path) const override;

	char getOsSeparator() const override;

	IOStream* Open(const char* pFile, const char* pMode) override;
	void Close(IOStream* pFile);
};

#endif