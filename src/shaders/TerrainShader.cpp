#include "TerrainShader.h"

TerrainShader::TerrainShader()
	: ShaderProgram("res/shaders/terrainVert.shader", "res/shaders/terrainFrag.shader"), location_transformationMatrix(0) {
	bindAttributes();
	getAllUniformLocations();
}

void TerrainShader::loadTransformationMatrix(glm::mat4 matrix) {
	loadMatrix(location_transformationMatrix, matrix);
}

void TerrainShader::loadProjectionMatrix(glm::mat4 matrix) {
	loadMatrix(location_projectionMatrix, matrix);
}

void TerrainShader::loadViewMatrix(Camera& camera) {
	glm::mat4 view = camera.getView();
	loadMatrix(location_viewMatrix, view);
}

void TerrainShader::loadLights(std::vector<Light*>& lights) {
	for(size_t i = 0; i < MAX_LIGHTS; i++) {
		if(i < lights.size()) {
			loadVector3f(location_lightPosition[i], lights[i]->getPosition());
			loadVector3f(location_lightColor[i], lights[i]->getColor());
			loadVector3f(location_attenuation[i], lights[i]->getAttenuation());
		} else {
			loadVector3f(location_lightPosition[i], glm::vec3(0));
			loadVector3f(location_lightColor[i], glm::vec3(0));
			loadVector3f(location_attenuation[i], glm::vec3(1, 0, 0));
		}
	}
}

void TerrainShader::loadShineVariables(float shineDamper, float reflectivity) {
	loadFloat(location_shineDamper, shineDamper);
	loadFloat(location_reflectivity, reflectivity);
}

void TerrainShader::loadSkyColor(float r, float g, float b) {
	loadVector3f(location_skyColor, glm::vec3(r, g, b));
}

void TerrainShader::loadClipPlane(glm::vec4 plane) {
	loadVector4f(location_plane, plane);
}

void TerrainShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void TerrainShader::getAllUniformLocations() {
	//MVP calculation uniforms
	location_transformationMatrix = getUniformLocation("model");
	location_projectionMatrix = getUniformLocation("proj");
	location_viewMatrix = getUniformLocation("view");

	//Light calculation uniforms
	for(int i = 0; i < MAX_LIGHTS; i++) {
		location_lightPosition[i] = getUniformLocation("lightPosition[" + std::to_string(i) + "]");
		location_lightColor[i] = getUniformLocation("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = getUniformLocation("attenuation[" + std::to_string(i) + "]");
	}
	location_shineDamper = getUniformLocation("shineDamper");
	location_reflectivity = getUniformLocation("reflectivity");

	//Fog calculation uniforms
	location_skyColor = getUniformLocation("skyColor");

	//Texture samplers
	location_backgroundTexture = getUniformLocation("backgroundTexture");
	location_rTexture = getUniformLocation("rTexture");
	location_gTexture = getUniformLocation("gTexture");
	location_bTexture = getUniformLocation("bTexture");
	location_blendMap = getUniformLocation("blendMap");

	//Clip plane
	location_plane = getUniformLocation("plane");
}

void TerrainShader::connectTextureUnits() {
	loadInt(location_backgroundTexture, 0);
	loadInt(location_rTexture, 1);
	loadInt(location_gTexture, 2);
	loadInt(location_bTexture, 3);
	loadInt(location_blendMap, 4);
}