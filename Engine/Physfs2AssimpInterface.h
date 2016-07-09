/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

//Contains necessary components for interfacing assimp with physfs, assimp physfs to read files through physfs

#ifndef _PHYSFS_2_ASSIMP_INTERFACE_H
#define _PHYSFS_2_ASSIMP_INTERFACE_H

#include <assimp-3.2/include/assimp/IOStream.hpp>
#include <assimp-3.2/include/assimp/IOSystem.hpp>
#include <physfs.h>

class P2AIOStream : public Assimp::IOStream {
	friend class MyIOSystem;
protected:
	PHYSFS_File* file;
	P2AIOStream(void) = default;
public:
	P2AIOStream(const char* filePath);
	~P2AIOStream(void);
	size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;
	size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;
	aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;
	size_t Tell() const override;
	size_t FileSize() const override;
	void Flush() override;
};

class P2AIOSystem : public Assimp::IOSystem {
public:
	P2AIOSystem() {}
	~P2AIOSystem() {}

	bool Exists(const char* path) const override;

	char getOsSeparator() const override;

	Assimp::IOStream* Open(const char* pFile, const char* pMode) override;
	void Close(Assimp::IOStream* pFile) override;
};

#endif