#include "ModuleCollision.h"
#include "App.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
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
			if (colliders[i]->collider.Intersect(colliders[j]->collider)) {
				Collision* col = nullptr;
				for (LIST_ITERATOR(Collision*) c = colliders[i]->collisions.begin(); c != colliders[i]->collisions.end(); c++) {
					if ((*c)->c1 == colliders[j] || (*c)->c2 == colliders[j]) {
						col = *c;
						if (col->updated)
							continue;

						col->updated = true;
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
					colliders[j]->collisions.push_back(col);
				}
				col->collisionArea = result;
			}
		}
		VECTOR(LIST_ITERATOR(Collision*)) cols;
		for (LIST_ITERATOR(Collision*) col = colliders[i]->collisions.begin(); col != colliders[i]->collisions.end(); col++) {
			if ((*col)->updated)
				continue;

			(*col)->updated = true;
			SDL_Rect result;
			if ((*col)->c2->collider.Intersect(colliders[i]->collider)) {
					(*col)->CallOnExit();
					cols.push_back(col);
			}
		}

		for (size_t c = 0; c < cols.size(); c++) {
			Collision* col = (*cols[c]);
			col->CleanUp();
			Utils::Release(col);
		}
	}
	return true;
}

bool ModuleCollision::Update(float dt) {
	if (App->debug) {
		for (std::vector<Collider*>::const_iterator c = colliders.begin(); c != colliders.end(); c++) {
			SDL_Color color;
			switch ((*c)->type) {
			case Collider::Type::FEET:
				color = { 255,255,0,128 };//Yellow
				break;
			case Collider::Type::ATK:
				color = { 255,0,0,128 };//Red
				break;
			case Collider::Type::SPELL:
				color = { 143,0,255,128 };//violet
				break;
			case Collider::Type::HITBOX:
				color = { 0,0,255,128 };//Blue
				break;
			case Collider::Type::TRIGGER:
				color = { 0,255,0,128 };//Green
				break;
			case Collider::Type::DEF:
				color = { 255,105,180,128 };//Pink
				break;
			case Collider::Type::PARRY:
				color = { 255,165,0,128 };//Orange
				break;
			}
			float scale = App->win->GetScale();
			App->render->DrawQuad(((*c)->collider.GetRectXY()).toSDL_Rect(), color.r, color.g, color.b, color.a, 1.0f);
		}
	}
	return true;
}

bool ModuleCollision::PostUpdate() {
	for (size_t i = 0; i < colliders.size(); i++) {
		for (LIST_ITERATOR(Collision*) col = colliders[i]->collisions.begin(); col != colliders[i]->collisions.end(); col++) {
			(*col)->updated = false;
		}
	}
	return true;
}

bool ModuleCollision::CleanUp(xmlNode & config) {
	for (VECTOR_ITERATOR(Collider*) it = colliders.begin(); it != colliders.end(); it++) {
		(*it)->CleanUp();
		Utils::Release(*it);
	}
	colliders.clear();
	return true;
}

Collider * ModuleCollision::CreateCollider(iCube dims, String tag, Collider::Type type)
{
	Collider* c = new Collider();
	c->collider = dims;
	c->sTag = tag;
	c->type = type;
	c->tag = Utils::FindInVector(tag, tagList);
	if (c->tag == (uint)-1) {
		c->tag = 0;
		LOG("Collider tag not defined");
	}
	return c;
}

void ModuleCollision::AddCollider(Collider * c, Entity * e)
{
	c->entity = e;
	colliders.push_back(c);
}

bool ModuleCollision::RemoveCollider(Collider * c)
{
	bool ret = Utils::RemoveFromVector<Collider*>(c, colliders);
	c->CleanUp();
	Utils::Release(c);
	return ret;
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

void Collider::CleanUp() {
	VECTOR(Collision*) toDestroy;
	for (LIST_ITERATOR(Collision*) it = collisions.begin(); it != collisions.end(); it++) {
		toDestroy.push_back(*it);
	}
	for (size_t i = 0; i < toDestroy.size(); i++) {
		Collision* c = toDestroy[i];
		c->CallOnExit();
		c->CleanUp();
		Utils::Release(c);
	}
	collisions.clear();
}