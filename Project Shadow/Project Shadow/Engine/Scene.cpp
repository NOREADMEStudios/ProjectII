#include "Scene.h"



Scene::Scene(){}


Scene::~Scene(){}

bool Scene::Awake(pugi::xml_node&) {
	loaded = true;
}

bool Scene::CleanUp(pugi::xml_node&) {
	loaded = false;
}