// ReadLineTest.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <vector>
#include "ogldev_math_3d.h"
#include <string>
#include <stdio.h>
#include <iostream>

class PolyMesh
{
	// TODO: Look
	struct TriFace
	{
		unsigned int v[3];	//!< vertex indices

		void Print()
		{
			printf("%d / %d / %d \n", v[0], v[1], v[2]);
		}
	};

protected:
	Vector3f * v;		//!< vertices
	TriFace *f;		//!< faces

	unsigned int nv;	//!< number of vertices
	unsigned int nf;	//!< number of faces

public:
	PolyMesh() { v = nullptr; }

	//!@name Component Access Methods
	const Vector3f& V(int i) const { return v[i]; }		//!< returns the i^th vertex
	Vector3f&       V(int i) { return v[i]; }		//!< returns the i^th vertex			 { return v[i]; }
	Vector3f* V() { return v; }
													// TODO: Take a look
	const TriFace& F(int i) const { return f[i]; }		//!< returns the i^th face
	TriFace&       F(int i) { return f[i]; }		//!< returns the i^th face

	bool LoadFromFileObj(const char *filename, std::ostream *outStream = &std::cout);

	unsigned int NV() const { return nv; }		//!< returns the number of vertices
	unsigned int NF() const { return nf; }		//!< returns the number of faces

												// TODO : Take a look
	void SetNumVertex(unsigned int n) { Allocate(n, v, nv); }
	void SetNumFaces(unsigned int n) { Allocate(n, f, nf); }
private:
	// TODO : Take a look
	template <class T> void Allocate(unsigned int n, T* &t) { if (t) delete[] t; if (n>0) t = new T[n]; else t = nullptr; }
	template <class T> bool Allocate(unsigned int n, T* &t, unsigned int &nt) { if (n == nt) return false; nt = n; Allocate(n, t); return true; }

};

inline bool PolyMesh::LoadFromFileObj(const char *filename, std::ostream *outStream)
{
	FILE* fp;

	// Open file
	int err = fopen_s(&fp, filename, "r");

	// Check open status
	if (err == 0)
	{
		printf("The file %s was opened\n", filename);
	}
	else
	{
		printf("ERROR: Fail to open File\n", filename);
	}


	// Deletes all components of the mesh
	// Clear()

	// Create class Buffer
	class Buffer
	{
		char data[1024];
		int readline;
	public:
		char& operator[](int i) { return data[i]; }
		int Realine(FILE *fp)
		{
			char c = fgetc(fp);
			//printf_s(c);

			// TODO : Take a look
			while (!feof(fp))
			{
				while (isspace(c) && (!feof(fp) || c != '\0'))
				{
					c = fgetc(fp);	// skip empty space
				}
				if (c == '#') while (!feof(fp) && c != '\n' && c != '\r' && c != '\0')
				{
					c = fgetc(fp);	// skip comment line
				}
				else
				{
					break;
				}
			}


			int i = 0;
			bool inspace = false;
			while (i < 1024 - 1)
			{
				if (feof(fp) || c == '\n' || c == '\r' || c == '\0')
				{
					break;
				}
				if (isspace(c))
				{
					inspace = true; // only use a single space as the space character
				}
				else
				{
					if (inspace)
					{
						data[i++] = ' ';
					}
					inspace = false;
					data[i++] = c;
				}
				c = fgetc(fp);
			}
			data[i] = '\0';
			readline = i;
			return i;
		}
		void printout()
		{
			printf("Data = %s", &data);
		}
		bool IsCommand(const char* cmd)
		{
			int i = 0;
			while (cmd[i] != '\0')
			{
				if (cmd[i] != data[i]) { return false; }
				i++;
			}
			return (data[i] == '\0' || data[i] == ' ');
		}
		const void ReadVector(Vector3f &v) { v.Normalize(); sscanf_s(data + 2, "%f %f %f", &v.x, &v.y, &v.z); }
	};

	Buffer buffer;

	std::vector<Vector3f> _v;
	std::vector<TriFace>  _f;

	// Read Vertex
	while (int rb = buffer.Realine(fp))
	{
		if (buffer.IsCommand("v"))
		{
			Vector3f vertex;
			buffer.ReadVector(vertex);
			_v.push_back(vertex);
		}
		else if (buffer.IsCommand("f")) {
			int facevert = -1;
			bool inspace = true;
			bool negative = false;
			int type = 0;
			unsigned int index;
			TriFace face;
			unsigned int nFacesBefore = (unsigned int)_f.size();
			for (int i = 2; i<rb; i++) {
				if (buffer[i] == ' ') inspace = true;
				else {
					if (inspace) {
						inspace = false;
						negative = false;
						type = 0;
						index = 0;
						switch (facevert) {
						case -1:
							// initialize face
							face.v[0] = face.v[1] = face.v[2] = 0;
						case 0:
						case 1:
							facevert++;
							break;
						case 2:
							// copy the first two vertices from the previous face
							_f.push_back(face);
							face.v[1] = face.v[2];
							break;
						}
					}
					if (buffer[i] == '/') { type++; index = 0; }
					if (buffer[i] == '-') negative = true;
					if (buffer[i] >= '0' && buffer[i] <= '9') {
						index = index * 10 + (buffer[i] - '0');
						switch (type) {
						case 0: face.v[facevert] = negative ? (unsigned int)_v.size() - index : index - 1; break;
						}
					}
				}
			}
			_f.push_back(face);
		}

		if (feof(fp)) { break; }
	}


	fclose(fp);
	printf("\nFile Close");
	// Skip empty space and comment line

	SetNumVertex((unsigned int)_v.size());
	SetNumFaces((unsigned int)_f.size());


	// Copy data
	memcpy(v, _v.data(), sizeof(Vector3f)*_v.size());
	memcpy(f, _f.data(), sizeof(TriFace)*_f.size());

	return true;
}