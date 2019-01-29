#pragma once
#include "ogldev_math_3d.h"

class PolyMesh
{
	struct TriFace
	{
		unsigned int v[3];	//!< vertex indices
	};


protected:
	Vector3f * v;		//!< vertices
	TriFace *f;		//!< faces
	Vector3f *vt;	//!< texture vertices


};