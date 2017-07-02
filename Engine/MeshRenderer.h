/**  Copyright 2016 MarFil Studios. All rights reserved.  **/

#pragma once

#include "Renderer.h"
#include "Model3D.h"
#include "Material.h"

// TODO: remove this when we have Material class
class MeshRenderer : public Renderer {
public:
	void init() override;
	void addMesh(Model3D* model, Material* material);
	void preRender() override;
	void postRender() override;
};
