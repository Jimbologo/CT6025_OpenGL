#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp> 

#include "Model.h"

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
	
	glm::vec4 m_lightPos;

	Model* m_tankModel;

	std::vector<Model*> sceneModels;

	

};

#endif // __Application_H_