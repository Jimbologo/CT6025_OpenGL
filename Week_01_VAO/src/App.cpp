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
	
	m_maxVerts = 36;
	//load shaders
	m_vertexShader = Utility::loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	m_fragmentShader = Utility::loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	//define inputs and outputs for out shaders
	const char* szInputs[] = { "position", "colour" };
	const char* szOutputs[] = { "fragColour" };
	//link shaders together to create shader program
	m_programID = Utility::createProgram(m_vertexShader, 0, 0, 0, m_fragmentShader, 2, szInputs, 1, szOutputs);


	//create vertices for our cube
	m_vertices = new Vertex[8];
	//make a 2 unit cube
	glm::vec4 vX(2.f, 0.f, 0.f, 0.f);
	glm::vec4 vY(0.f, 2.f, 0.f, 0.f);
	glm::vec4 vZ(0.f, 0.f, 2.f, 0.f);
	//center of cube at following vector position
	glm::vec4 center(0.f, 0.f, 0.f, 1.f);
	//back part of cube
	m_vertices[0].position = center - vX - vY - vZ;
	m_vertices[1].position = center - vX + vY - vZ;
	m_vertices[2].position = center + vX + vY - vZ;
	m_vertices[3].position = center + vX - vY - vZ;
	//front part of cube
	m_vertices[4].position = center - vX - vY + vZ;
	m_vertices[5].position = center - vX + vY + vZ;
	m_vertices[6].position = center + vX + vY + vZ;
	m_vertices[7].position = center + vX - vY + vZ;

	for (int i = 0; i < 8; ++i)
	{
		m_vertices[i].colour = glm::vec4(((float)(i % 3) / 3.f), ((float)(i % 4) / 4.f), ((float)(i % 7) / 7), 1.0);
	}

	unsigned int indices[] = {
		2,1,0,3,2,0,
		5,6,4,6,7,4,
		4,3,0,7,3,4,
		1,2,5,2,6,5,
		0,1,4,1,5,4,
		2,3,7,6,2,7
	};


	//generate our vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//generate and bind vertex buffer
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), m_vertices, GL_DYNAMIC_DRAW);
	//set attributes for out buffer
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //color
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxVerts * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//create a World-space matrix for a camera
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,1,0)));

	//create a perspective projection matrix with a 90 degree feild-of-view and widescreen aspect ration
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, m_windowWidth/(float)m_windowHeight, 0.1f, 1000.0f);

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse( glm::lookAt(glm::vec3(10,10,10),glm::vec3(0,0,0), glm::vec3(0,1,0)) );
	
	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, m_windowWidth/(float)m_windowHeight, 0.1f, 1000.0f);

	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f,0.25f,0.25f,1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void App::Update(float a_deltaTime)
{
	// update our camera matrix using the keyboard/mouse
	Utility::freeMovement( m_cameraMatrix, a_deltaTime, 10 );

	// clear all gizmos from last frame
	Gizmos::clear();
	
	// add an identity matrix gizmo
	Gizmos::addTransform( glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1) );
	// add a 20x20 grid on the XZ-plane
	for ( int i = 0 ; i < 21 ; ++i )
	{
		Gizmos::addLine( glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
		
		Gizmos::addLine( glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
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
	if (glfwGetKey(m_window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		quit();
}

void App::Draw()
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse( m_cameraMatrix );
	
	// draw the gizmos from this frame
	Gizmos::draw(viewMatrix, m_projectionMatrix);
	

	//use our shader
	glUseProgram(m_programID);
	//bind out vertex array object to get the buffers bound that we previously had bound
	glBindVertexArray(m_vao);
	//set shader uniform data from CPU to GPU
	int projectViewUniformLocation = glGetUniformLocation(m_programID, "ProjectionView");
	if (projectViewUniformLocation < 0)
	{
		Application_Log* log = Application_Log::Create();
		if (log != nullptr)
		{
			log->addLog(LOG_LEVEL::LOG_WARNING, "Warning: Uniform location: ProjectionView not found in shader!\n");
		}
	}

	glUniformMatrix4fv(projectViewUniformLocation, 1, false, glm::value_ptr(m_projectionMatrix * viewMatrix));
	glCullFace(GL_FRONT);
	glDrawElements(GL_TRIANGLES,m_maxVerts,GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void App::Destroy()
{

	Gizmos::destroy();

	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);

	glDeleteProgram(m_programID);
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
}

