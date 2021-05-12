#ifndef __LIGHTING_FBX_Normal_H__
#define __LIGHTING_FBX_Normal_H__


#include <iostream>

// Derived application class that wraps up all globals neatly
class Lighting_FBX_Normal
{
public:

	Lighting_FBX_Normal();
	~Lighting_FBX_Normal();

	bool LoadImageFromFile(std::string a_filePath, unsigned int& a_textureID);

};

#endif // __LIGHTING_FBX_Normal_H__