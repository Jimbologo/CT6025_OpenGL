#ifndef __MODEL_H_
#define __MODEL_H_
 

#include <FBXFile.h>
#include "Light.h"
#include "FrameBufferObject.h"

class Model
{
public:
	Model(const char* a_filePath, FBXFile::UNIT_SCALE a_scale, unsigned int a_windowWidth, unsigned int a_windowHeight);
	~Model();

	void Update( unsigned int a_windowWidth, unsigned int a_windowHeight);
	void Draw(glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, Light* a_directionalLight, std::vector<Light*> a_pointLights, std::vector<Light*> a_spotLights);

private:

	void DrawShadowScene(unsigned int a_programID, glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, Light* a_Light);
	void DrawScene(unsigned int a_programID, glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, Light* a_directionalLight, std::vector<Light*> a_pointLights, std::vector<Light*> a_spotLights, LIGHT_TYPE a_lightType);
	void SetupFrameBuffer(	unsigned int& a_fbo,
							unsigned int a_targetWidth,
							unsigned int a_targetHeight,
							unsigned int& a_renderTexture,
							unsigned int& a_depthTexture);

	FBXFile* m_fbxModel;
	std::vector<std::string> m_normalPaths;
	std::string fileDirectory;

	glm::mat4    m_modelMatrix;
	

	unsigned int m_shaderProgramID; //Handle to our compiled and linked shader program
	unsigned int m_vertexShader; //Handle to our compiled vertex shader
	unsigned int m_fragmentShader; //Handle to our compiled fragment shader

	unsigned int m_shaderProgramID_Shadow; //Handle to our compiled and linked shadow shader program
	unsigned int m_shaderProgramID_Shadow2;

	unsigned int m_vao; //vertex array object
	unsigned int m_vbo;
	unsigned int m_ibo;

	std::vector<FrameBufferObject*> m_frameBuffers;

	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	bool m_loadShadows;
};




#endif // !__MODEL_H_
