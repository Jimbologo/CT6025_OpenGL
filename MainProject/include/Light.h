#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
#include <string>

enum LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,

	END
};

class Light
{
public:
	Light();
	Light(LIGHT_TYPE a_lightType, glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, float a_brightness, glm::vec4 a_position);
	~Light();

	void Update(float a_deltaTime, unsigned int a_windowWidth, unsigned int a_windowHeight);

	void ImguiUpdate(float a_windowWidth, float a_windowHeight);

	glm::vec4 GetPositon() { return m_position; }
	glm::vec4 GetDirection() { return m_direction; }
	glm::vec3 GetAmbient() { return m_ambient; }
	glm::vec3 GetDiffuse() { return m_diffuse; }
	glm::vec3 GetSpecular() { return m_specular; }
	float GetBrightness() { return m_brightness; }
	float GetRange() { return m_range; }
	float GetMaxDistance() { return m_maxDistance; }
	float GetCutOff() { return m_cutoff; }
	float GetOuterCutOff() { return m_outerCutoff; }
	glm::mat4 GetShadowProjectionViewMatrix() { return m_shadowProjectionViewMatrix; }

	void SetPositon(glm::vec4 a_val) { m_position = a_val; }
	void SetDirection(glm::vec4 a_val) { m_direction = a_val; }
	void SetAmbient(glm::vec3 a_val) { m_ambient = a_val; }
	void SetDiffuse(glm::vec3 a_val) { m_diffuse = a_val; }
	void SetSpecular(glm::vec3 a_val) { m_specular = a_val; }
	void SetBrightness(float a_val) { m_brightness = a_val; }
	void SetRotSpeed(float a_val) { m_rotationalSpeed = a_val; }
	void SetRange(float a_val) { m_range = a_val; }
	void SetMaxDistance(float a_val) { m_maxDistance = a_val; }

private:

	bool m_enabled;

	LIGHT_TYPE m_lightType;
	std::string m_name;

	glm::mat4 m_shadowProjectionViewMatrix;

	glm::vec4 m_position;
	glm::vec4 m_direction;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_brightness;

	//DirectionalLightVaribles
	float m_rotationalSpeed;

	//Spot&PointLightVariables
	float m_range;
	float m_maxDistance;
	float m_cutoff;
	float m_outerCutoff;
};




#endif // !__LIGHT_H__
