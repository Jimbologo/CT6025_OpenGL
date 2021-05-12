#include "Shadows.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include <imgui.h>
#include "Application_Log.h"
#include "FBXFile.h"

#define SHADOW_BUFFER_SIZE 1024

Shadows::Shadows()
{

}

Shadows::~Shadows()
{

}

bool Shadows::onCreate()
{
	// initialise the Gizmos helper class
	Gizmos::create();

	//Inialise Models
	m_sceneModels.push_back(new Model("models/ruinedtank/tank.fbx", FBXFile::UNITS_CENTIMETER, m_windowWidth, m_windowHeight));

	//Inialise Lights
	m_sceneDirLight = new Light(LIGHT_TYPE::DIRECTIONAL, glm::vec3(0.125, 0.125, 0.125), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 0.4f, glm::vec4(20.f, 15.f, 0.f, 1.f));

	m_scenePointLights.push_back(new Light(LIGHT_TYPE::POINT, glm::vec3(0.9, 0.125, 0.125), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 1, glm::vec4(5.f, 0.f, 0.f, 1.f)));
	m_sceneSpotLights.push_back(new Light(LIGHT_TYPE::SPOT, glm::vec3(0.125, 0.925, 0.125), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 1, glm::vec4(1.f, 10.f, 0.f, 1.f)));

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);

	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void Shadows::Update(float a_deltaTime)
{
	// update our camera matrix using the keyboard/mouse
	Utility::freeMovement(m_cameraMatrix, a_deltaTime, 10);

	// clear all gizmos from last frame
	Gizmos::clear();

	// add an identity matrix gizmo
	Gizmos::addTransform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

	ImguiUpdates();

	//Loop through all lights and call update
	m_sceneDirLight->Update(a_deltaTime, m_windowWidth, m_windowHeight);
	for (int i = 0; i < m_scenePointLights.size(); ++i)
	{
		m_scenePointLights[i]->Update(a_deltaTime, m_windowWidth, m_windowHeight);
	}
	for (int i = 0; i < m_sceneSpotLights.size(); ++i)
	{
		m_sceneSpotLights[i]->Update(a_deltaTime, m_windowWidth, m_windowHeight);
	}
	//Loop through all models and call update
	for (int i = 0; i < m_sceneModels.size(); ++i)
	{
		m_sceneModels[i]->Update(m_windowWidth, m_windowHeight);
	}

	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);
	Gizmos::draw(viewMatrix, m_projectionMatrix);

	static bool show_demo_window = true;
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

void Shadows::ImguiUpdates()
{
	ImGui::SetNextWindowPos(ImVec2(0, m_windowHeight * 0.4));
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth * 0.25, m_windowHeight * 0.4));
	ImGui::Begin("Light Settings");

	//Loop through all lights and call update
	m_sceneDirLight->ImguiUpdate(m_windowWidth, m_windowHeight);
	for (int i = 0; i < m_scenePointLights.size(); ++i)
	{
		m_scenePointLights[i]->ImguiUpdate(m_windowWidth, m_windowHeight);
	}
	for (int i = 0; i < m_sceneSpotLights.size(); ++i)
	{
		m_sceneSpotLights[i]->ImguiUpdate(m_windowWidth, m_windowHeight);
	}

	ImGui::End();
}

void Shadows::Draw()
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Loop through all models and call Draw
	for (int i = 0; i < m_sceneModels.size(); ++i)
	{
		m_sceneModels[i]->Draw(m_projectionMatrix, m_cameraMatrix, m_sceneDirLight, m_scenePointLights, m_sceneSpotLights);
	}

	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);
	// draw the gizmos from this frame
	Gizmos::draw(viewMatrix, m_projectionMatrix);
}


void Shadows::Destroy()
{
	
	Gizmos::destroy();
}


