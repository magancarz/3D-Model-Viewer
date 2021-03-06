#pragma once

#include "../Headers.h"
#include "../shaders/ShaderProgram.h"
#include "../entities/Camera.h"

class SkyboxShader : public ShaderProgram {
public:
	SkyboxShader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadProjectionMatrix(glm::mat4& matrix);
	void loadViewMatrix(Camera& camera);
	void loadFogColor(GLfloat r, GLfloat g, GLfloat b);
	void loadBlendFactor(GLfloat blend);
	void connectTextureUnits();

	static constexpr GLfloat ROTATE_SPEED = 0.1f;
private:
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_fogColor;
	int location_cubeMap;
	int location_cubeMap2;
	int location_blendFactor;
	GLfloat rotation = 0;
};