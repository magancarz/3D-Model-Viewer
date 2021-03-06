#pragma once

#include "../Headers.h"
#include "../shaders/StaticShader.h"
#include "../shaders/TerrainShader.h"
#include "../skybox/SkyboxShader.h"
#include "../skybox/SkyboxRenderer.h"
#include "../normalMappingRenderer/NormalMappingRenderer.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"

void enableCulling();
void disableCulling();
const float RED = 0.4f, GREEN = 0.4f, BLUE = 0.4f;

class MasterRenderer {
public:
	MasterRenderer(Loader* loader);
	~MasterRenderer();

	void render(std::vector<Light*>& lights, Camera& camera, glm::vec4 clipPlane);
	void prepare();


	void processEntity(Entity& entity);
	void processNormalMapEntity(Entity& entity);
	void processTerrain(Terrain* terrain);

	inline glm::mat4 getProjectionMatrix() { return m_projectionMatrix; };

	void cleanUp();
private:
	StaticShader* m_shader;
	NormalMappingShader* m_normalMappingShader;
	EntityRenderer* m_renderer;

	TerrainShader* m_terrainShader;
	TerrainRenderer* m_terrainRenderer;

	SkyboxRenderer* m_skyboxRenderer;

	NormalMappingRenderer* m_normalMappingRenderer;

	glm::mat4 m_projectionMatrix;
	const float FOV = 70.0f;
	const float NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;

	std::map<TexturedModel*, std::vector<Entity*>*>* m_entities;
	std::map<TexturedModel*, std::vector<Entity*>*>* m_normalMappingEntities;
	std::vector<Terrain*>* m_terrains;

	void createProjectionMatrix();
};