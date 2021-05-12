#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp> 

#include <FBXFile.h>

// Derived application class that wraps up all globals neatly
class App : public Application
{
public:

	App();
	virtual ~App();

protected:

	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	
	glm::mat4	m_cameraMatrix = glm::mat4();
	glm::mat4	m_projectionMatrix = glm::mat4();
	glm::mat4    m_modelMatrix;
	glm::vec4 m_lightPos;

	FBXFile* m_fbxModel;

	unsigned int m_programID; //Handle to our compiled and linked shader program
	unsigned int m_vertexShader; //Handle to our compiled vertex shader
	unsigned int m_fragmentShader; //Handle to our compiled fragment shader

	unsigned int m_vao; //vertex array object
	unsigned int m_modelBuffer[2];


};

#endif // __Application_H_