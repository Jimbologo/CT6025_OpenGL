#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>

#include "Utilities.h"

#include "Lighting_FBX_Normal.h"

#include "Application_Log.h"

#include <regex>

#define SHADOW_BUFFER_SIZE 1024


Model::Model(const char* a_filePath, std::vector<std::string> a_normalPaths, FBXFile::UNIT_SCALE a_scale, unsigned int a_windowWidth, unsigned int a_windowHeight, bool a_loadShadows)
{
	m_normalPaths = a_normalPaths;
	m_loadShadows = a_loadShadows;

	//load in our FBX model();
	m_fbxModel = new FBXFile();
	m_fbxModel->load(a_filePath, a_scale);

	std::string path = std::string(a_filePath);

	const size_t last_slash_idx = path.rfind('/');
	if (std::string::npos != last_slash_idx)
	{
		fileDirectory = path.substr(0, last_slash_idx);
		fileDirectory += "/";
	}

	

	if (a_loadShadows)
	{
		//load in and link shader program
		m_vertexShader = Utility::loadShader("shaders/vertex_s.glsl", GL_VERTEX_SHADER);
		m_fragmentShader = Utility::loadShader("shaders/fragment_s.glsl", GL_FRAGMENT_SHADER);
		//define inputs and outputs for out shaders
		const char* szInputs[] = { "Position", "Colour", "Normal","Tex1" };
		const char* szOutputs[] = { "FragColor" };
		//bind the shaders to create our shader program
		m_shaderProgramID = Utility::createProgram(
			m_vertexShader,
			0,
			0,
			0,
			m_fragmentShader,
			4, szInputs, 1, szOutputs);

		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragmentShader);

		//Load the shaders for rendering the shadows
		m_vertexShader = Utility::loadShader("shaders/vertex_shadow.glsl", GL_VERTEX_SHADER);
		m_fragmentShader = Utility::loadShader("shaders/fragment_shadow.glsl", GL_FRAGMENT_SHADER);
		//Define the input and output varialbes in the shaders
		//Note: these names are taken from the glsl files -- added in inputs for UV coordinates
		const char* szInputsShadow[] = { "Position", "Colour", "Normal","Tex1" };
		const char* szOutputsShadow[] = { "FragDepth" };
		//bind the shaders to create our shader program
		m_shaderProgramID_Shadow = Utility::createProgram(
			m_vertexShader,
			0,
			0,
			0,
			m_fragmentShader,
			4, szInputsShadow, 1, szOutputsShadow);

		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragmentShader);

	}
	else 
	{
		Lighting_FBX_Normal lighting_fbx_normal = Lighting_FBX_Normal();

		for (int i = 0; i < m_normalPaths.size(); ++i)
		{
			std::string conconateString = fileDirectory + m_normalPaths[i];
			lighting_fbx_normal.LoadImageFromFile(conconateString, m_normalTextureIDs[i]);
		}

		m_vertexShader = Utility::loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
		m_fragmentShader = Utility::loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
		//define inputs and outputs for out shaders
		const char* szInputs[] = { "Position", "Colour", "Normal", "Tex1", "Tangent", "BiTangent" };
		const char* szOutputs[] = { "fragColour" };
		//link shaders together to create shader program
		m_shaderProgramID = Utility::createProgram(m_vertexShader, 0, 0, 0, m_fragmentShader, 6, szInputs, 1, szOutputs);


	}

	m_modelMatrix = glm::mat4();

	InitialiseModel(a_windowWidth, a_windowHeight);
}

Model::~Model()
{
}

void Model::InitialiseModel(unsigned int a_windowWidth, unsigned int a_windowHeight)
{
	//===========================================================================================================
	//Model
	//generate our vertex array object
	glGenBuffers(2, m_modelBuffer);
	glGenBuffers(1, &m_modelBuffer[0]);
	glGenBuffers(1, &m_modelBuffer[1]);
	glGenVertexArrays(1, &m_vao);

	

	//generate and bind vertex buffer
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_modelBuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelBuffer[1]);

	//set attributes for out buffer
	glEnableVertexAttribArray(0); //Pos
	glEnableVertexAttribArray(1); //Col
	glEnableVertexAttribArray(2); //Norm
	glEnableVertexAttribArray(3); //Tex1


								  // tell our shaders where the information within our buffers lie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::PositionOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::ColourOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);

	// finally, where done describing our mesh to the shader
	// we can describe the next mesh
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//\===========================================================================================================
	//\Set up the frame buffer for rendering our shadows into
	m_FBO_texture = -1;
	SetupFrameBuffer(m_FBO, SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE, m_FBO_texture, m_FBO_depth_texture);
	//\===========================================================================================================
	//create a texture to hold the linear depth buffer samples
	//texture for linear depth buffer visualisation
	glGenTextures(1, &m_FBO_Linear_Depth);

	// bind the texture for editing
	glBindTexture(GL_TEXTURE_2D, m_FBO_Linear_Depth);

	// create the texture. Note, the width and height are the dimensions of the screen and the final
	// data pointer is 0 as we aren't including any initial data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_windowWidth, m_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	// set the filtering if we intend to sample within a shader
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	//\===========================================================================================================
}

void Model::Update(glm::vec4 a_directionalLightPos, unsigned int a_windowWidth, unsigned int a_windowHeight)
{
	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;

	//Update the shadow projection view matrix 
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(a_directionalLightPos.xyz), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -100, 100);
	m_shadowProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;

	for (int i = 0; i < m_fbxModel->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbxModel->getMeshByIndex(i);
		mesh->updateGlobalTransform();
	}
}

void Model::Draw(glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, glm::vec4 a_directionalLightPos)
{
	if (m_loadShadows)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE);
		glCullFace(GL_FRONT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		DrawScene(m_shaderProgramID_Shadow, a_projectionMatrix, a_cameraMatrix, a_directionalLightPos);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glCullFace(GL_BACK);
	DrawScene(m_shaderProgramID, a_projectionMatrix, a_cameraMatrix, a_directionalLightPos);
}

void Model::SetupFrameBuffer(unsigned int& a_fbo,
	unsigned int a_targetWidth,
	unsigned int a_targetHeight,
	unsigned int& a_renderTexture,
	unsigned int& a_depthTexture)
{
	//\======================================================================================
	// Create our frame buffer object
	//\=====================================================================================
	// this would be within your onCreate() function
	glGenFramebuffers(1, &a_fbo);

	// bind the framebuffer for editing
	glBindFramebuffer(GL_FRAMEBUFFER, a_fbo);

	if (a_renderTexture != -1)
	{
		// create a texture to be attached to the framebuffer, stored in the derived app class as a member
		glGenTextures(1, &a_renderTexture);

		// bind the texture for editing
		glBindTexture(GL_TEXTURE_2D, a_renderTexture);

		// create the texture. Note, the width and height are the dimensions of the screen and the final
		// data pointer is 0 as we aren't including any initial data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_targetWidth, a_targetHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// set the filtering if we intend to sample within a shader
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, a_renderTexture, 0);
	}
	// m_FBO_depth would be a member for the derived application class
	glGenTextures(1, &a_depthTexture);
	glBindTexture(GL_TEXTURE_2D, a_depthTexture);

	// note the use of GL_DEPTH_COMPONENT32F and GL_DEPTH_COMPONENT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, a_targetWidth,
		a_targetHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach it as a GL_DEPTH_ATTACHMENT
	// attach the texture to the 0th color attachment of the framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, a_depthTexture, 0);

	// Here we tell the framebuffer which color attachments we will be drawing to and how many
	if (a_renderTexture != -1)
	{
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}
	else
	{
		glDrawBuffer(GL_NONE);
	}
	// if Status doesn't equal GL_FRAMEBUFFER_COMPLETE there has been an error when creating it
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	// binding 0 to the framebuffer slot unbinds the framebuffer and means future render calls will be sent to 
	// the back buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void Model::DrawScene(unsigned int a_programID, glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, glm::vec4 a_directionalLightPos)
{

	//bing our shader program
	glUseProgram(a_programID);
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse(a_cameraMatrix);
	//bind our vertex array object
	glBindVertexArray(m_vao);

	unsigned int shadowProjectionViewUniform = glGetUniformLocation(a_programID, "ShadowProjectionView");
	glUniformMatrix4fv(shadowProjectionViewUniform, 1, false, glm::value_ptr(m_shadowProjectionViewMatrix));

	//get our shaders uniform location for our projectionViewMatrix and then use glUniformMatrix4fv to fill it with the correct data
	unsigned int projectionViewUniform = glGetUniformLocation(a_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(a_projectionMatrix * viewMatrix));

	//pass throught the view matrix
	unsigned int viewMatrixUniform = glGetUniformLocation(a_programID, "ViewMatrix");
	glUniformMatrix4fv(viewMatrixUniform, 1, false, glm::value_ptr(viewMatrix));

	//pass our camera position to our fragment shader uniform
	unsigned int cameraPosUniform = glGetUniformLocation(a_programID, "cameraPosition");
	glUniform4fv(cameraPosUniform, 1, glm::value_ptr(a_cameraMatrix[3]));

	//pass the directional light direction to our fragment shader
	glm::vec4 lightDir = -a_directionalLightPos;
	lightDir.w = 0.f;
	lightDir = glm::normalize(lightDir);
	unsigned int lightDirUniform = glGetUniformLocation(a_programID, "lightDirection");
	glUniform4fv(lightDirUniform, 1, glm::value_ptr(lightDir));


	//Set the shadow texture
	unsigned int shadowTexUniformID = glGetUniformLocation(a_programID, "ShadowTexture");
	glUniform1i(shadowTexUniformID, 1);
	//Set our active texture, and bind our loaded texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_FBO_depth_texture);

	//Draw our FBX Model
	// for each mesh in the model...
	for (unsigned int i = 0; i < m_fbxModel->getMeshCount(); ++i)
	{
		// get the current mesh
		FBXMeshNode* pMesh = m_fbxModel->getMeshByIndex(i);

		// send the Model
		glm::mat4 m4Model = pMesh->m_globalTransform;// *m_modelMatrix;
		unsigned int modelUniform = glGetUniformLocation(a_programID, "Model");
		glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(m4Model));
		//send the normal matrix
		//this is only beneficial if a model has a non-uniform scale or non-orthoganal model matrix
		glm::mat4 m4Normal = glm::transpose(glm::inverse(pMesh->m_globalTransform));// *m_modelMatrix;
		unsigned int normalMatrixUniform = glGetUniformLocation(a_programID, "NormalMatrix");
		glUniformMatrix4fv(normalMatrixUniform, 1, false, glm::value_ptr(m4Normal));

		// Bind the texture to one of the ActiveTextures
		// if your shader supported multiple textures, you would bind each texture to a new Active Texture ID here
		//bind our textureLocation variable from the shaders and set it's value to 0 as the active texture is texture 0
		unsigned int texUniformID = glGetUniformLocation(a_programID, "DiffuseTexture");
		glUniform1i(texUniformID, 0);
		//set our active texture, and bind our loaded texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pMesh->m_material->textureIDs[FBXMaterial::DiffuseTexture]);

		// Send the vertex data to the VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_modelBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, pMesh->m_vertices.size() * sizeof(FBXVertex), pMesh->m_vertices.data(), GL_STATIC_DRAW);

		// send the index data to the IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelBuffer[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_indices.size() * sizeof(unsigned int), pMesh->m_indices.data(), GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, pMesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);

}