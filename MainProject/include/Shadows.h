#ifndef __Shadows_H_
#define __Shadows_H_

#include "Application.h"
#include <glm/glm.hpp>
#include "Model.h"


class FBXFile;


// Derived application class that wraps up all globals neatly
class Shadows : public Application
{
public:

	Shadows();
	virtual ~Shadows();

protected:

	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();
	void ImguiUpdates();

	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;

	std::vector<Model*> m_sceneModels;

	Light* m_sceneDirLight;
	std::vector<Light*> m_scenePointLights;
	std::vector<Light*> m_sceneSpotLights;
};

#endif // __Application_H_