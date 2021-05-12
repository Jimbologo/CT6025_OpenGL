#include "App.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include <imgui.h>

#include "Application_Log.h"


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

	//Load Tank Model and push to models vector
	std::vector<std::string> normalTextures = { "left_engine_norm.png", "turret_norm.png" };
	sceneModels.push_back(new Model("models/ruinedtank/tank.fbx", normalTextures, FBXFile::UNITS_CENTIMETER, m_windowWidth, m_windowHeight, true));

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);
	//Initalise light position
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

	//Update Directional light position in scene
	float speed = 0.2f;
	float s = sinf(a_deltaTime * speed);
	float c = cosf(a_deltaTime * speed);
	glm::vec4 temp = m_lightPos;
	m_lightPos.x = temp.x * c - temp.z * s;
	m_lightPos.z = temp.x * s + temp.z * c;
	Gizmos::addBox(m_lightPos.xyz, glm::vec3(0.2f, 0.2f, 0.2f), true, glm::vec4(1.f, 0.85f, 0.05f, 1.f));

	//Loop through all models and call update
	for (int i = 0; i < sceneModels.size(); ++i)
	{
		sceneModels[i]->Update(m_lightPos, m_windowWidth, m_windowHeight);
	}


	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - m_windowWidth * 0.3, m_windowHeight - m_windowHeight * 0.4));
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth * 0.3, m_windowHeight * 0.4));
	ImGui::Begin("Framebuffer");
	ImGui::BeginTabBar("FrameBuffer Textures");

	if (ImGui::BeginTabItem("Depth Buffer")) {
		ImTextureID texID = (void*)(intptr_t)sceneModels[0]->m_FBO_depth_texture;
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.25, m_windowHeight * 0.25), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();


	ImGui::End();

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

	// draw the gizmos from this frame
	Gizmos::draw(glm::inverse(m_cameraMatrix), m_projectionMatrix);

	//Loop through all models and call Draw
	for (int i = 0; i < sceneModels.size(); ++i)
	{
		sceneModels[i]->Draw(m_projectionMatrix, m_cameraMatrix, m_lightPos);
	}
}

void App::Destroy()
{
	//Loop through all models and delete
	for (int i = 0; i < sceneModels.size(); ++i)
	{
		delete sceneModels[i];
	}

	//Clear scenemodels array for extra safty
	sceneModels.clear();

	Gizmos::destroy();
}