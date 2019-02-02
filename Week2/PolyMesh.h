#pragma once
#include "ogldev_math_3d.h"

#include <string>
#include <stdio.h>
#include <iostream>

class PolyMesh
{
	struct TriFace
	{
		unsigned int v[3];	//!< vertex indices
	};


public:
	bool LoadFromFileObj(const char *filename, bool loadMtl = true, std::ostream *outStream = &std::cout);

protected:
	Vector3f * v;		//!< vertices
	TriFace *f;		//!< faces
	Vector3f *vt;	//!< texture vertices


};

inline bool PolyMesh::LoadFromFileObj(const char *filename, bool loadMtl, std::ostream *outStream)
{
	FILE *FilePath = fopen(filename, "r");
	if (!FilePath)
	{
		std::cout<<
	}
}