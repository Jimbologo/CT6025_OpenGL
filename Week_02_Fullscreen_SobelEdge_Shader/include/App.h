#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp> 
#include <string>

// Derived application class that wraps up all globals neatly
class App : public Application
{
public:

	App();
	virtual ~App();

protected:

	bool LoadImageFromFile(std::string a_filepath, unsigned int& a_textureID, int& a_width, int& a_height);
	
	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	struct Vertex
	{
		glm::vec4 position; //vertex pos
		glm::vec4 colour; //vertex color
		glm::vec2 uvs;  //vertex uv coords
	};

	Vertex* m_vertices;

	unsigned int m_textureID; //handle top our texture
	int			 m_imageWidth;
	int			 m_imageHeight;
	unsigned int m_programID; //Handle to our compiled and linked shader program
	unsigned int m_vertexShader; //Handle to our compiled vertex shader
	unsigned int m_fragmentShader; //Handle to our compiled fragment shader

	unsigned int m_maxVerts;
	unsigned int m_vao; //vertex array object
	unsigned int m_vbo; //vertex buffer object
	unsigned int m_ibo; //index buffer object
};

#endif // __Application_H_