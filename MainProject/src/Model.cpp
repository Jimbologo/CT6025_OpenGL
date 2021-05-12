#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>

#include "Utilities.h"

#include "Lighting_FBX_Normal.h"

#include "Application_Log.h"

#include <regex>

#define SHADOW_BUFFER_SIZE 2048


Model::Model(const char* a_filePath, FBXFile::UNIT_SCALE a_scale, unsigned int a_windowWidth, unsigned int a_windowHeight)
{

	std::string path = std::string(a_filePath);

	const size_t last_slash_idx = path.rfind('/');
	if (std::string::npos != last_slash_idx)
	{
		fileDirectory = path.substr(0, last_slash_idx);
		fileDirectory += "/";
	}

	//Load the shaders for this program
	unsigned int vertexShader = Utility::loadShader("./resources/shaders/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = Utility::loadShader("./resources/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	//Define the input and output varialbes in the shaders
	//Note: these names are taken from the glsl files -- added in inputs for UV coordinates
	const char* szInputs[] = { "Position", "Colour", "Normal","Tex1" };
	const char* szOutputs[] = { "FragColor" };
	//bind the shaders to create our shader program
	m_shaderProgramID = Utility::createProgram(
		vertexShader,
		0,
		0,
		0,
		fragmentShader,
		4, szInputs, 1, szOutputs);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Load the shaders for rendering the shadows
	vertexShader = Utility::loadShader("./resources/shaders/vertex_shadow.glsl", GL_VERTEX_SHADER);
	fragmentShader = Utility::loadShader("./resources/shaders/fragment_shadow.glsl", GL_FRAGMENT_SHADER);
	//Define the input and output varialbes in the shaders
	//Note: these names are taken from the glsl files -- added in inputs for UV coordinates
	const char* szInputs2[] = { "Position", "Colour", "Normal","Tex1" };
	const char* szOutputs2[] = { "FragDepth" };
	//bind the shaders to create our shader program
	m_shaderProgramID_Shadow = Utility::createProgram(
		vertexShader,
		0,
		0,
		0,
		fragmentShader,
		4, szInputs2, 1, szOutputs2);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	m_fbxModel = new FBXFile();
	m_fbxModel->load("./resources/models/ruinedtank/tank.fbx", FBXFile::UNITS_DECIMETER);

	glEnable(GL_CULL_FACE);

	//Generate our OpenGL Vertex and Index Buffers for rendering our FBX Model Data
	// OPENGL: genorate the VBO, IBO and VAO
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenVertexArrays(1, &m_vao);

	// OPENGL: Bind  VAO, and then bind the VBO and IBO to the VAO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//There is no need to populate the vbo & ibo buffers with any data at this stage
	//this can be done when rendering each mesh component of the FBX model

	// enable the attribute locations that will be used on our shaders
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

	m_frameBuffers.push_back(new FrameBufferObject());
	m_frameBuffers.push_back(new FrameBufferObject());
	m_frameBuffers.push_back(new FrameBufferObject());

	for (int i = 0; i < m_frameBuffers.size(); ++i)
	{
		m_frameBuffers[i]->m_FBO_texture;
		SetupFrameBuffer(m_frameBuffers[i]->m_FBO_ID, SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE, m_frameBuffers[i]->m_FBO_texture, m_frameBuffers[i]->m_FBO_depth_texture);

		//create a texture to hold the linear depth buffer samples
		//texture for linear depth buffer visualisation
		glGenTextures(1, &m_frameBuffers[i]->m_FBO_Linear_Depth);

		// bind the texture for editing
		glBindTexture(GL_TEXTURE_2D, m_frameBuffers[i]->m_FBO_Linear_Depth);

		// create the texture. Note, the width and height are the dimensions of the screen and the final
		// data pointer is 0 as we aren't including any initial data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_windowWidth, m_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		// set the filtering if we intend to sample within a shader
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//\===========================================================================================================
	
	m_modelMatrix = glm::mat4();
}

Model::~Model()
{
}

void Model::Update(unsigned int a_windowWidth, unsigned int a_windowHeight)
{
	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;

	for (int i = 0; i < m_fbxModel->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbxModel->getMeshByIndex(i);
		mesh->updateGlobalTransform();
	}
}

void Model::Draw(glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, Light* a_directionalLight, std::vector<Light*> a_pointLights, std::vector<Light*> a_spotLights)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffers[0]->m_FBO_ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE);
	glCullFace(GL_FRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	DrawShadowScene(m_shaderProgramID_Shadow, a_projectionMatrix, a_cameraMatrix, a_directionalLight);

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffers[1]->m_FBO_ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE);
	glCullFace(GL_FRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	DrawShadowScene(m_shaderProgramID_Shadow, a_projectionMatrix, a_cameraMatrix, a_pointLights[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffers[2]->m_FBO_ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE);
	glCullFace(GL_FRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	DrawShadowScene(m_shaderProgramID_Shadow, a_projectionMatrix, a_cameraMatrix, a_spotLights[0]);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glCullFace(GL_BACK);
	DrawScene(m_shaderProgramID, a_projectionMatrix, a_cameraMatrix, a_directionalLight, a_pointLights, a_spotLights, LIGHT_TYPE::END);

	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - m_windowWidth * 0.3, 0));
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth * 0.3, m_windowHeight * 0.4));
	ImGui::Begin("Framebuffers");
	ImGui::BeginTabBar("FrameBuffer Textures");

	if (ImGui::BeginTabItem("Dir Depth Buffer")) {
		ImTextureID texID = (void*)(intptr_t)m_frameBuffers[0]->m_FBO_depth_texture;
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.25, m_windowHeight * 0.25), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::BeginTabBar("FrameBuffer Textures");
	if (ImGui::BeginTabItem("Point Depth Buffer")) {
		ImTextureID texID = (void*)(intptr_t)m_frameBuffers[1]->m_FBO_depth_texture;
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.25, m_windowHeight * 0.25), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::BeginTabBar("FrameBuffer Textures");
	if (ImGui::BeginTabItem("Spot Depth Buffer")) {
		ImTextureID texID = (void*)(intptr_t)m_frameBuffers[2]->m_FBO_depth_texture;
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.25, m_windowHeight * 0.25), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();
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


void Model::DrawShadowScene(unsigned int a_programID, glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, Light* a_Light)
{
	//bing our shader program
	glUseProgram(a_programID);
	//bind our vertex array object
	glBindVertexArray(m_vao);

	unsigned int shadowProjectionViewUniform = glGetUniformLocation(a_programID, "ShadowProjectionView");
	glUniformMatrix4fv(shadowProjectionViewUniform, 1, false, glm::value_ptr(a_Light->GetShadowProjectionViewMatrix()));

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

		// Send the vertex data to the VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, pMesh->m_vertices.size() * sizeof(FBXVertex), pMesh->m_vertices.data(), GL_STATIC_DRAW);

		// send the index data to the IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_indices.size() * sizeof(unsigned int), pMesh->m_indices.data(), GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, pMesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

void Model::DrawScene(unsigned int a_programID, glm::mat4 a_projectionMatrix, glm::mat4 a_cameraMatrix, Light* a_directionalLight, std::vector<Light*> a_pointLights, std::vector<Light*> a_spotLights, LIGHT_TYPE a_lightType)
{

	//bing our shader program
	glUseProgram(a_programID);
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse(a_cameraMatrix);
	//bind our vertex array object
	glBindVertexArray(m_vao);


	unsigned int shadowProjectionViewUniform = glGetUniformLocation(a_programID, "DirShadowProjectionView");
	glUniformMatrix4fv(shadowProjectionViewUniform, 1, false, glm::value_ptr(a_directionalLight->GetShadowProjectionViewMatrix()));

	//get our shaders uniform location for our projectionViewMatrix and then use glUniformMatrix4fv to fill it with the correct data
	unsigned int projectionViewUniform = glGetUniformLocation(a_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(a_projectionMatrix * viewMatrix));

	//pass throught the view matrix
	unsigned int viewMatrixUniform = glGetUniformLocation(a_programID, "ViewMatrix");
	glUniformMatrix4fv(viewMatrixUniform, 1, false, glm::value_ptr(viewMatrix));

	//pass our camera position to our fragment shader uniform
	unsigned int cameraPosUniform = glGetUniformLocation(a_programID, "CameraPosition");
	glUniform4fv(cameraPosUniform, 1, glm::value_ptr(a_cameraMatrix[3]));

	//pass the directional light direction to our fragment shader
	glm::vec4 lightDir = -a_directionalLight->GetPositon();
	lightDir.w = 0.f;
	lightDir = glm::normalize(lightDir);
	unsigned int lightDirUniform = glGetUniformLocation(a_programID, "LightDirection");
	glUniform4fv(lightDirUniform, 1, glm::value_ptr(lightDir));

	glm::vec3 lightAmbient = glm::normalize(a_directionalLight->GetAmbient()) * a_directionalLight->GetBrightness();
	unsigned int lightAmbientUniform = glGetUniformLocation(a_programID, "dirLight.ambient");
	glUniform3fv(lightAmbientUniform, 1, glm::value_ptr(lightAmbient));

	glm::vec3 lightDiffuse = glm::normalize(a_directionalLight->GetDiffuse());
	unsigned int lighDiffuseUniform = glGetUniformLocation(a_programID, "dirLight.diffuse");
	glUniform3fv(lighDiffuseUniform, 1, glm::value_ptr(lightDiffuse));

	glm::vec3 lightSpecular = glm::normalize(a_directionalLight->GetSpecular());
	unsigned int lighSpecularUniform = glGetUniformLocation(a_programID, "dirLight.specular");
	glUniform3fv(lighSpecularUniform, 1, glm::value_ptr(lightSpecular));

	//Set the shadow texture
	unsigned int shadowTexUniformID = glGetUniformLocation(a_programID, "dirShadowTexture");
	glUniform1i(shadowTexUniformID, 1);
	//Set our active texture, and bind our loaded texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_frameBuffers[0]->m_FBO_depth_texture);

	for (GLuint i = 0; i < a_pointLights.size(); i++)
	{
		std::string number = std::to_string(i);

		shadowProjectionViewUniform = glGetUniformLocation(a_programID, ("PointShadowProjectionView[" + number + "]").c_str());
		glUniformMatrix4fv(shadowProjectionViewUniform, 1, false, glm::value_ptr(a_pointLights[i]->GetShadowProjectionViewMatrix()));

		//Set the shadow texture
		unsigned int pointshadowTexUniformID = glGetUniformLocation(a_programID, ("pointShadowTextures[" + number + "]").c_str());
		glUniform1i(pointshadowTexUniformID, 2);
		//Set our active texture, and bind our loaded texture
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_frameBuffers[1]->m_FBO_depth_texture);

		glm::vec3 position = a_pointLights[i]->GetPositon().xyz;
		unsigned int pointLightPos = glGetUniformLocation(a_programID, ("pointLights[" + number + "].position").c_str());
		glUniform3fv(pointLightPos, 1, glm::value_ptr(position));
		
		glm::vec3 pointLightAmbient = glm::normalize(a_pointLights[i]->GetAmbient()) * a_pointLights[i]->GetBrightness();
		unsigned int pointLightAmbientUniform = glGetUniformLocation(a_programID, ("pointLights[" + number + "].ambient").c_str());
		glUniform3fv(pointLightAmbientUniform, 1, glm::value_ptr(pointLightAmbient));

		glm::vec3 pointLightDiffuse = glm::normalize(a_pointLights[i]->GetDiffuse());
		unsigned int pointLightDiffuseUniform = glGetUniformLocation(a_programID, ("pointLights[" + number + "].diffuse").c_str());
		glUniform3fv(pointLightDiffuseUniform, 1, glm::value_ptr(pointLightDiffuse));

		glm::vec3 pointLightSpecular = glm::normalize(a_pointLights[i]->GetSpecular());
		unsigned int pointLightSpecularUniform = glGetUniformLocation(a_programID, ("pointLights[" + number + "].specular").c_str());
		glUniform3fv(pointLightSpecularUniform, 1, glm::value_ptr(pointLightSpecular));

		glUniform1f(glGetUniformLocation(a_programID, ("pointLights[" + number + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(a_programID, ("pointLights[" + number + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(a_programID, ("pointLights[" + number + "].quadratic").c_str()), 0.032f);

		glUniform1f(glGetUniformLocation(a_programID, ("pointLights[" + number + "].range").c_str()), a_pointLights[i]->GetRange());
		glUniform1f(glGetUniformLocation(a_programID, ("pointLights[" + number + "].cutoff").c_str()), a_pointLights[i]->GetCutOff());
	}

	for (GLuint i = 0; i < a_spotLights.size(); i++)
	{
		std::string number = std::to_string(i);

		shadowProjectionViewUniform = glGetUniformLocation(a_programID, ("SpotShadowProjectionView[" + number + "]").c_str());
		glUniformMatrix4fv(shadowProjectionViewUniform, 1, false, glm::value_ptr(a_spotLights[i]->GetShadowProjectionViewMatrix()));

		//Set the shadow texture
		unsigned int spotshadowTexUniformID = glGetUniformLocation(a_programID, ("spotShadowTextures[" + number + "]").c_str());
		glUniform1i(spotshadowTexUniformID, 3);
		//Set our active texture, and bind our loaded texture
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_frameBuffers[2]->m_FBO_depth_texture);

		glm::vec3 position = a_spotLights[i]->GetPositon().xyz;
		unsigned int pointLightPos = glGetUniformLocation(a_programID, ("spotLights[" + number + "].position").c_str());
		glUniform3fv(pointLightPos, 1, glm::value_ptr(position));

		glm::vec3 direction = a_spotLights[i]->GetDirection().xyz;
		unsigned int pointLightDir = glGetUniformLocation(a_programID, ("spotLights[" + number + "].direction").c_str());
		glUniform3fv(pointLightDir, 1, glm::value_ptr(direction));

		glm::vec3 pointLightAmbient = glm::normalize(a_spotLights[i]->GetAmbient()) * a_spotLights[i]->GetBrightness();
		unsigned int pointLightAmbientUniform = glGetUniformLocation(a_programID, ("spotLights[" + number + "].ambient").c_str());
		glUniform3fv(pointLightAmbientUniform, 1, glm::value_ptr(pointLightAmbient));

		glm::vec3 pointLightDiffuse = glm::normalize(a_spotLights[i]->GetDiffuse());
		unsigned int pointLightDiffuseUniform = glGetUniformLocation(a_programID, ("spotLights[" + number + "].diffuse").c_str());
		glUniform3fv(pointLightDiffuseUniform, 1, glm::value_ptr(pointLightDiffuse));

		glm::vec3 pointLightSpecular = glm::normalize(a_spotLights[i]->GetSpecular());
		unsigned int pointLightSpecularUniform = glGetUniformLocation(a_programID, ("spotLights[" + number + "].specular").c_str());
		glUniform3fv(pointLightSpecularUniform, 1, glm::value_ptr(pointLightSpecular));

		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].quadratic").c_str()), 0.032f);

		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].range").c_str()), a_spotLights[i]->GetRange());
		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].cutoff").c_str()), glm::cos(glm::radians(a_spotLights[i]->GetCutOff())));
		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].outerCutOff").c_str()), glm::cos(glm::radians(a_spotLights[i]->GetOuterCutOff())));
		glUniform1f(glGetUniformLocation(a_programID, ("spotLights[" + number + "].maxDistance").c_str()), a_spotLights[i]->GetMaxDistance());

		
	}

	

	
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
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, pMesh->m_vertices.size() * sizeof(FBXVertex), pMesh->m_vertices.data(), GL_STATIC_DRAW);

		// send the index data to the IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_indices.size() * sizeof(unsigned int), pMesh->m_indices.data(), GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, pMesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);

}