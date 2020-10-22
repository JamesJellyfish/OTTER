#pragma once

#include <vector>
#include <algorithm>
#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <GLM/glm.hpp>

#include "MeshFactory.h"
#include "MeshBuilder.h"

class ObjLoader
{
public:
	static VertexArrayObject::sptr OBJLoader(const std::string& file);
	
protected:
	ObjLoader() = default;
	~ObjLoader() = default;
};

