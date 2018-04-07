#include "ModuleCollision.h"
#include "App.h"
#include "ModuleRender.h"
#include <stdlib.h>

ModuleCollision::ModuleCollision() {
	name = "collision";
}

bool ModuleCollision::Awake(xmlNode & config) {
	memset(&interactionTable, false, MAX_TAGS*MAX_TAGS);
	xmlNode tags = config.child("colliderTags");
	size_t tagPos = 0;
	bool ret = true;

	const char* interactionsRaw[MAX_TAGS];
	for (size_t i = 0; i < MAX_TAGS; i++) {
		interactionsRaw[i] = '\0';
	}

	for (pugi::xml_node_iterator iter = tags.begin(); iter != tags.end(); iter++, tagPos++) {
		const char* tag = iter->attribute("value").as_string();
		tagList.push_back(tag);
		interactionsRaw[tagPos] = iter->attribute("interactions").as_string();
	}

	for (size_t j = 0; j < tagPos; j++) {
		size_t i = 0, indexInTmpString = 0;
		char tmpString[64];
		memset(&tmpString, '\0', 64);
		while (interactionsRaw[j] != '\0') {
			char c = interactionsRaw[j][i];
			if (c == ';' || c == '\0') {
				for (size_t pos = 0; pos < tagList.size(); pos++) {
					if (strcmp(tagList[pos].c_str(), tmpString) == 0) {
						interactionTable[pos][j] = true;
						interactionTable[j][pos] = true;
						break;
					}
				}
				if (c == '\0') break;
				memset(&tmpString, '\0', 64);
				indexInTmpString = 0;
			}
			else {
				tmpString[indexInTmpString++] = c;
			}
			i++;
		}
	}

	return ret;
}

bool ModuleCollision::Start() {
	return true;
}

bool ModuleCollision::PreUpdate() {
	for (size_t i = 0; i < colliders.size(); i++) {
		for (size_t j = i + 1; j < colliders.size(); j++) {
			if (!interactionTable[colliders[i]->tag][colliders[j]->tag])
				continue;

			SDL_Rect result;
			if (SDL_IntersectRect(&colliders[i]->collider.toSDL_Rect(), &colliders[j]->collider.toSDL_Rect(), &result) == SDL_TRUE) {
				Collision* col = nullptr;
				for (LIST_ITERATOR(Collision*) c = colliders[i]->collisions.begin(); c != colliders[i]->collisions.end(); c++) {
					if ((*c)->c1 == colliders[j] || (*c)->c2 == colliders[j]) {
						col = *c;
						switch (col->state) {
						case Collision::ON_ENTER:
							col->state = Collision::ON_STAY;
						case Collision::ON_STAY:
							col->CallOnStay();
							break;
						}
						break;
					}
				}
				if (col == nullptr) {
					col = new Collision(colliders[i], colliders[j]);
					col->CallOnEnter();
					colliders[i]->collisions.push_back(col);
					//colliders[j]->collisions.push_back(col);
				}
				col->collisionArea = result;
			}
		}
		VECTOR(LIST_ITERATOR(Collision*)) cols;
		for (LIST_ITERATOR(Collision*) col = colliders[i]->collisions.begin(); col != colliders[i]->collisions.end(); col++) {
			SDL_Rect result;
			if (SDL_IntersectRect(&(*col)->c2->collider.toSDL_Rect(), &colliders[i]->collider.toSDL_Rect(), &result) == SDL_FALSE) {
				(*col)->CallOnExit();
				cols.push_back(col);
			}
		}

		for (size_t c = 0; c < cols.size(); c++) {
			Utils::Release(*cols[c]);
			colliders[i]->collisions.erase(cols[c]);
			//(*col)->c2->collisions.erase(cols[c]);
		}
	}
	return true;
}

bool ModuleCollision::Update(float dt) {
	if (App->debug) {
		for (std::vector<Collider*>::const_iterator c = colliders.begin(); c != colliders.end(); c++) {
			App->render->DrawQuad((*c)->collider.toSDL_Rect(), 255 * (1 / ((*c)->tag + 1)), 255 * (1 / ((*c)->tag + 1)), 255, 128);
		}
	}
	return true;
}

bool ModuleCollision::PostUpdate() {
	return true;
}

bool ModuleCollision::CleanUp(xmlNode & config) {
	return true;
}

void ModuleCollision::AddCollider(Collider * c, Entity * e)
{
	c->entity = e;
	colliders.push_back(c);
}
 
ARRAY(String) ModuleCollision::GetTags()
{
	return tagList;
}

String ModuleCollision::GetTag(uint tag)
{
	return (tag >= tagList.size()) ? "" : tagList[tag];
}

String ModuleCollision::GetTag(Collider * c)
{
	return GetTag(c->tag);
}

String ModuleCollision::GetTag(const Collider & c)
{
	return GetTag(c.tag);
}
