#ifndef __MODEL_H_
#define __MODEL_H_
 

#include <FBXFile.h>

class Model
{
public:
	Model(const char* a_filePath, std::vector<std::string> a_normalPaths, FBXFile::UNIT_SCALE a_scale, unsigned int a_windowWidth, unsigned int a_windowHeight, bool a_loadShadows);
	~Model();

	void Update(glm::vec4 a_directionalLightPos, unsigned int a_windowWidth, unsigned int a_windowHeight);
	void Draw(glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, glm::vec4 a_directionalLightPos);

	unsigned int m_FBO_depth_texture;
private:

	void DrawScene(unsigned int a_programID, glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, glm::vec4 a_directionalLightPos);
	void InitialiseModel(unsigned int a_windowWidth, unsigned int a_windowHeight);
	void SetupFrameBuffer(	unsigned int& a_fbo,
							unsigned int a_targetWidth,
							unsigned int a_targetHeight,
							unsigned int& a_renderTexture,
							unsigned int& a_depthTexture);

	FBXFile* m_fbxModel;
	std::vector<std::string> m_normalPaths;
	std::string fileDirectory;

	glm::mat4    m_modelMatrix;
	glm::mat4    m_shadowProjectionViewMatrix;

	unsigned int m_shaderProgramID; //Handle to our compiled and linked shader program
	unsigned int m_vertexShader; //Handle to our compiled vertex shader
	unsigned int m_fragmentShader; //Handle to our compiled fragment shader

	unsigned int m_shaderProgramID_Shadow; //Handle to our compiled and linked shadow shader program

	
	unsigned int m_vao; //vertex array object
	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_modelBuffer[2]; // model buffer
	unsigned int m_normalTextureIDs[2]; // texture IDs

	//frame buffer variables
	unsigned int m_FBO;
	unsigned int m_FBO_texture;
	//unsigned int m_FBO_depth_texture;
	unsigned int m_FBO_Linear_Depth;

	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	bool m_loadShadows;
};




#endif // !__MODEL_H_
