#include "Light.h"
#include <Gizmos.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <glad/glad.h>



# define PI 3.14159265358979323846  // pi 

Light::Light()
{
	m_lightType = LIGHT_TYPE::DIRECTIONAL;
	m_position = glm::vec4();
	m_direction = glm::vec4(0,-1,0,1);
	m_ambient = glm::vec3();
	m_diffuse = glm::vec3();
	m_specular = glm::vec3();
	m_brightness = 1.0f;
	m_rotationalSpeed = 0.2f;
	m_enabled = true;
	m_range = 10;
	m_cutoff = 0.005f;
	m_maxDistance = 20;

	switch (m_lightType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
		m_name = "Directional Light";
		break;
	case LIGHT_TYPE::SPOT:
		m_name = "Spot Light";
		break;
	case LIGHT_TYPE::POINT:
		m_name = "Point Light";
		break;
	default:
		break;
	}
}

Light::Light(LIGHT_TYPE a_lightType, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, float a_brightness, glm::vec4 a_position)
{
	m_lightType = a_lightType;
	m_position = a_position;
	m_direction = glm::vec4(0, -1, 0, 1);
	m_ambient = a_ambient;
	m_diffuse = a_diffuse;
	m_specular = a_specular;
	m_brightness = a_brightness;
	m_rotationalSpeed = 0.2f;
	m_enabled = true;
	m_range = 10;
	m_maxDistance = 20;

	switch (m_lightType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
		m_name = "Directional Light";
		break;
	case LIGHT_TYPE::SPOT:
		m_name = "Spot Light";
		m_cutoff = 25.0f;
		m_outerCutoff = 24.9f;
		break;
	case LIGHT_TYPE::POINT:
		m_name = "Point Light";
		m_cutoff = 0.005f;
		break;
	default:
		break;
	}
}

Light::~Light()
{
}

void Light::Update(float a_deltaTime, unsigned int a_windowWidth, unsigned int a_windowHeight)
{
	if (!m_enabled)
	{
		return;
	}

	

	if (m_lightType == LIGHT_TYPE::DIRECTIONAL)
	{
		// rotate point
		float s = sinf(a_deltaTime * m_rotationalSpeed);
		float c = cosf(a_deltaTime * m_rotationalSpeed);
		glm::vec4 temp = m_position;
		m_position.x = temp.x * c - temp.z * s;
		m_position.z = temp.x * s + temp.z * c;
		m_position.w = 0;
		m_position = glm::normalize(m_position) * 25.f;

		Gizmos::addSphere(m_position.xyz, 6, 6, 0.2f, glm::vec4(m_ambient, 1.f), true);
		glm::vec3 lightDir = glm::normalize(-m_position).xyz;
		Gizmos::addLine(m_position.xyz, m_position.xyz + (lightDir * 1.0f), glm::vec4(m_ambient, 1.f), glm::vec4(m_ambient, 1.f));
		Gizmos::addLine(m_position.xyz + glm::vec3(0,0.2f,0), m_position.xyz + (lightDir * 1.0f), glm::vec4(m_ambient, 1.f), glm::vec4(m_ambient, 1.f));
		Gizmos::addLine(m_position.xyz + (glm::vec3(0, 0.2f, 0) * -1.0f), m_position.xyz + (lightDir * 1.0f), glm::vec4(m_ambient, 1.f), glm::vec4(m_ambient, 1.f));

		//Update the shadow projection view matrix 
		glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(m_position.xyz), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -100, 100);
		m_shadowProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;
	}
	else if (m_lightType == LIGHT_TYPE::POINT)
	{
		Gizmos::addSphere(m_position.xyz, 6, 6, 0.2f, glm::vec4(m_ambient, 1.f), true);
		Gizmos::addSphere(m_position.xyz, 6, 6, m_range, glm::vec4(m_ambient, 1.f), false);

		//Update the shadow projection view matrix 
		//glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(m_position.xyz), glm::vec3(m_position.xyz) + glm::vec3(glm::normalize(m_direction) * m_maxDistance), glm::vec3(0, 1, 0));
		//glm::mat4 depthProjectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, a_windowWidth / (float)a_windowHeight, 0.1f, 1000.0f);
		//m_shadowProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;

		//Update the shadow projection view matrix 
		glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(m_position.xyz), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -100, 100);
		m_shadowProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;
	}
	else if (m_lightType == LIGHT_TYPE::SPOT)
	{
		float width = 1;
		if (glm::radians(m_cutoff) < PI / 4.0f)
		{
			width = m_maxDistance * sinf(glm::radians(m_cutoff));
		}
		else
		{
			width = m_maxDistance / (2.0f * cos(glm::radians(m_cutoff)));
		}
		Gizmos::addCone(m_position.xyz, 0.0f, width, m_maxDistance, 25, false, glm::vec4(m_ambient, 1.f));

		//Update the shadow projection view matrix 
		//glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(m_position.xyz), glm::vec3(m_position.xyz) + glm::vec3(glm::normalize(m_direction) * m_maxDistance), glm::vec3(0, 1, 0));
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, a_windowWidth / (float)a_windowHeight, 0.1f, 1000.0f);
		//m_shadowProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;

		//Update the shadow projection view matrix 
		glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(m_position.xyz), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -100, 100);
		m_shadowProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;
	}
}

void Light::ImguiUpdate(float a_windowWidth, float a_windowHeight)
{
	ImGui::BeginTabBar(m_name.c_str());

	if (ImGui::BeginTabItem(m_name.c_str())) {

		if (ImGui::Checkbox("Enabled", &m_enabled))
		{
			m_ambient = glm::vec3(0.125, 0.125, 0.125);
		}

		if (m_enabled)
		{
			float* color = glm::value_ptr(m_ambient);
			ImGui::ColorEdit3("Color", color);
			m_ambient = glm::make_vec3(color);
		}
		else
		{
			m_ambient = glm::vec3(0, 0, 0);
		}

		ImGui::SliderFloat("Brightness", &m_brightness, 0, 1, "%.1f");

		

		float* pos = glm::value_ptr(m_position);
		ImGui::InputFloat3("Position", pos);
		m_position = glm::make_vec4(pos);

		if (m_lightType == LIGHT_TYPE::POINT)
		{
			ImGui::SliderFloat("Range", &m_range, 0, 50, "%.2f");
		}
		else if (m_lightType == LIGHT_TYPE::SPOT)
		{
			ImGui::SliderFloat("Max Distance", &m_maxDistance, 0, 50, "%.2f");
			ImGui::SliderFloat("Inner Radius", &m_cutoff, 0, 25, "%.2f");

			m_outerCutoff = m_outerCutoff > m_cutoff - 0.1f ? m_cutoff - 0.1f : m_outerCutoff;
			ImGui::SliderFloat("Outer Radius", &m_outerCutoff, 0, m_cutoff - 0.1f, "%.2f");
		}

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
}