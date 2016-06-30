/** Copyright 2016 MarFil Studios. All rights reserved. **/

#include "Physfs2AssimpInterface.h"

P2AIOStream::P2AIOStream(const char *filePath) {
	file = PHYSFS_openRead(filePath);
}

P2AIOStream::~P2AIOStream() {
	PHYSFS_close(file);
}

size_t P2AIOStream::Read(void* pvBuffer, size_t pSize, size_t pCount) {
	return PHYSFS_read(file, pvBuffer, pSize, pCount);
}

size_t P2AIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
	//Writing forbidden. Dobro nam došo Stole na brod... :D
	return 0;
}

aiReturn P2AIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
	// There'll be some real work here
	int result = -1;
	switch (pOrigin) {
	case aiOrigin_SET: {
		result = PHYSFS_seek(file, pOffset);
	}; break;
	case aiOrigin_CUR: {
		result = PHYSFS_seek(file, PHYSFS_tell(file) + pOffset);
	}; break;
	case aiOrigin_END: {
		result = PHYSFS_seek(file, PHYSFS_fileLength(file) - pOffset);
	}; break;
	}
	if (result) {
		// TODO: Implement support for aiReturn_OUTOFMEMORY
		return aiReturn_FAILURE;
	}
	else {
		return aiReturn_SUCCESS;
	}
}

size_t P2AIOStream::Tell() const {
	return PHYSFS_tell(file);
}

size_t P2AIOStream::FileSize() const {
	return PHYSFS_fileLength(file);
}
void P2AIOStream::Flush() {
	PHYSFS_flush(file);
}




bool P2AIOSystem::Exists(const char* pFile) const {
	return PHYSFS_exists(pFile);
}

char P2AIOSystem::getOsSeparator() const {
	return '/';
}

IOStream* P2AIOSystem::Open(const char* pFile, const char* pMode) {
	return new P2AIOStream(pFile);
}

void P2AIOSystem::Close(IOStream* pFile) {
	delete pFile;
}
