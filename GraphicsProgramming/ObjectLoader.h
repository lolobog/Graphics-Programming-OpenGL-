#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "glm.hpp"
#include "Vertex.h"
#include <map>

using namespace std;

class OBJLoader
{
public:
	static vector<Vertex>LoadOBJ(const string Filename ,const string& FolderLoc, string& AmbiantLoc,
		string& DiffLoc, string& specLoc, string& NormalLoc, vector<glm::uint>& indicies);

	static void LoadMaterial(const string& MatLibLoc, string& AmbiantLoc, string& DiffLoc,
		string& specLoc, string& NormalLoc);
};