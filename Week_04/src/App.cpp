#include "App.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include <imgui.h>

#include "Application_Log.h"
#include "Lighting_FBX_Normal.h"

App::App()
{

}

App::~App()
{

}

bool App::onCreate()
{
	// initialise the Gizmos helper class
	Gizmos::create();

	//load in our FBX model();
	m_fbxModel = new FBXFile();
	m_fbxModel->load("models/ruinedtank/tank.fbx", FBXFile::UNITS_CENTIMETER);

	Lighting_FBX_Normal lighting_fbx_normal = Lighting_FBX_Normal();
	lighting_fbx_normal.LoadImageFromFile("models/ruinedtank/left_engine_norm.png", m_textureID[0]);
	lighting_fbx_normal.LoadImageFromFile("models/ruinedtank/turret_norm.png", m_textureID[1]);

	//load in and link shader program
	m_vertexShader = Utility::loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	m_fragmentShader = Utility::loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	//define inputs and outputs for out shaders
	const char* szInputs[] = { "Position", "Colour", "Normal", "Tangent", "BiTangent","Tex1" };
	const char* szOutputs[] = { "fragColour" };
	//link shaders together to create shader program
	m_programID = Utility::createProgram(m_vertexShader, 0, 0, 0, m_fragmentShader, 6, szInputs, 1, szOutputs);


	//generate our vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//generate and bind vertex buffer
	glGenBuffers(2, m_modelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_modelBuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelBuffer[1]);

	//set attributes for out buffer
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //color
	glEnableVertexAttribArray(2); //normal
	glEnableVertexAttribArray(3); //tangent
	glEnableVertexAttribArray(4); //bitangent
	glEnableVertexAttribArray(5); //tex1

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::PositionOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::ColourOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TangentOffset);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::BiNormalOffset);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);

	m_modelMatrix = glm::mat4();
	m_lightPos = glm::vec4(20.f, 15.f, 0.f, 1.f);


	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void App::Update(float a_deltaTime)
{
	// update our camera matrix using the keyboard/mouse
	Utility::freeMovement(m_cameraMatrix, a_deltaTime, 10);

	// clear all gizmos from last frame
	Gizmos::clear();

	// add an identity matrix gizmo
	Gizmos::addTransform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

	float speed = 0.2f;
	float s = sinf(a_deltaTime * speed);
	float c = cosf(a_deltaTime * speed);
	glm::vec4 temp = m_lightPos;
	m_lightPos.x = temp.x * c - temp.z * s;
	m_lightPos.z = temp.x * s + temp.z * c;
	Gizmos::addBox(m_lightPos.xyz, glm::vec3(0.2f, 0.2f, 0.2f), true, glm::vec4(1.f, 0.85f, 0.05f, 1.f));

	for (int i = 0; i < m_fbxModel->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbxModel->getMeshByIndex(i);
		mesh->updateGlobalTransform();
	}

	static bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);
	Application_Log* log = Application_Log::Get();
	if (log != nullptr && show_demo_window)
	{
		log->showLog(&show_demo_window);
	}
	//show application log window
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS) {
		show_demo_window = !show_demo_window;
	}
	// quit our application when escape is pressed
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		quit();
}

void App::Draw()
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);

	// draw the gizmos from this frame
	Gizmos::draw(viewMatrix, m_projectionMatrix);

	//use our shader
	glUseProgram(m_programID);
	//bind out vertex array object to get the buffers bound that we previously had bound
	glBindVertexArray(m_vao);

	//set shader uniform data from CPU to GPU
	int projectViewUniformLocation = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(projectViewUniformLocation, 1, false, glm::value_ptr(m_projectionMatrix * viewMatrix));

	int viewMatrixUniform = glGetUniformLocation(m_programID, "ViewMatrix");
	glUniformMatrix4fv(viewMatrixUniform, 1, false, glm::value_ptr(viewMatrix));

	int cameraPositionUniformLocation = glGetUniformLocation(m_programID, "CameraPosition");
	glUniform4fv(cameraPositionUniformLocation, 1, glm::value_ptr(m_cameraMatrix[3]));

	glm::vec4 lightDir = glm::normalize(glm::vec4(0.f, 0.f, 0.f, 1.f) - m_lightPos);
	int lightDirUniformLoc = glGetUniformLocation(m_programID, "LightDirection");
	glUniform4fv(lightDirUniformLoc, 1, glm::value_ptr(lightDir));

	int lightPosUniform = glGetUniformLocation(m_programID, "LightPos");
	glUniform4fv(lightPosUniform, 1, glm::value_ptr(m_lightPos));

	int lightColourUniformLoc = glGetUniformLocation(m_programID, "LightColour");
	glUniform4fv(lightColourUniformLoc, 1, glm::value_ptr(glm::vec4(1.f, 0.85f, 0.05f, 1.0f)));

	int lightSpecularUniformLoc = glGetUniformLocation(m_programID, "LightSpecular");
	glUniform4fv(lightSpecularUniformLoc, 1, glm::value_ptr(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f)));

	for (int i = 0; i < m_fbxModel->getMeshCount(); ++i)
	{
		FBXMeshNode* pMesh = m_fbxModel->getMeshByIndex(i);

		//set each model matrix for mesh
		int modelMatrixUniformLoc = glGetUniformLocation(m_programID, "ModelMatrix");
		glUniformMatrix4fv(modelMatrixUniformLoc, 1, false, glm::value_ptr(pMesh->m_globalTransform));
		//pass through normal matrix
		int normalMatrixUniformLoc = glGetUniformLocation(m_programID, "NormalMatrix");
		glUniformMatrix4fv(normalMatrixUniformLoc, 1, false, glm::value_ptr(glm::transpose(glm::inverse(pMesh->m_globalTransform))));

		glm::vec3 rafValues = glm::vec3(0.2f, 0.7f, 0.1f);
		if (strcmp("ground1", pMesh->m_material->name) == 0)
		{
			rafValues.x = 1.0f;
			rafValues.y = 0.0f;
			rafValues.z = 0.0f;
		}

		int rafUniform = glGetUniformLocation(m_programID, "RAF");
		glUniform3fv(rafUniform, 1, glm::value_ptr(rafValues));

		//send the normal matrix
		//this is only beneficial if the model has a non-uniform scale or non-orth model matrix
		glm::mat4 m4Model = pMesh->m_globalTransform;
		glm::mat4 m4Normal = glm::transpose(m4Model);
		int normalMatrixUniform = glGetUniformLocation(m_programID, "NormalMatrix");
		glUniformMatrix4fv(normalMatrixUniform, 1, false, glm::value_ptr(glm::transpose(glm::inverse(viewMatrix * m4Normal))));

		//texture on the model send to shader
		int texUniformLoc = glGetUniformLocation(m_programID, "DiffuseTexture");
		glUniform1i(texUniformLoc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pMesh->m_material->textureIDs[FBXMaterial::DiffuseTexture]);

		//Lets send through our loaded normal textures
		texUniformLoc = glGetUniformLocation(m_programID, "NormalTexture");
		glUniform1i(texUniformLoc, 1);
		//set our active texture and bind our loaded texture
		int useNormalTextureUniform = glGetUniformLocation(m_programID, "useNormalMap");
		if (strcmp(pMesh->m_material->textureFilenames[FBXMaterial::DiffuseTexture], "left_engine_diff.png") == 0)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_textureID[0]);

			glUniform1i(useNormalTextureUniform, 1);
		}
		else if (strcmp(pMesh->m_material->textureFilenames[FBXMaterial::DiffuseTexture], "turret_diff.png") == 0)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_textureID[1]);

			glUniform1i(useNormalTextureUniform, 1);
		}
		else
		{
			glUniform1i(useNormalTextureUniform, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_modelBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, pMesh->m_vertices.size() * sizeof(FBXVertex), pMesh->m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelBuffer[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_indices.size() * sizeof(unsigned int), pMesh->m_indices.data(), GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, pMesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}



	glBindVertexArray(0);
	glUseProgram(0);
}

void App::Destroy()
{
	//glDeleteTextures();
	delete m_fbxModel;
	Gizmos::destroy();
}