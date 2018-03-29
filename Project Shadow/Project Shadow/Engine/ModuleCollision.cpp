#include "ModuleCollision.h"
#include <stdlib.h>

ModuleCollision::ModuleCollision() {
	name = "collision";
}

bool ModuleCollision::Awake(xmlNode & config) {
	memset(&interactionTable, false, MAX_TAGS*MAX_TAGS);
	xmlNode tags = config.child("colliderTags");
	uint tagPos = 0;
	bool ret = true;

	const char* interactionsRaw[MAX_TAGS];
	for (uint i = 0; i < MAX_TAGS; i++) {
		interactionsRaw[i] = '\0';
	}

	for (pugi::xml_node_iterator iter = tags.begin(); iter != tags.end(); iter++, tagPos++) {
		const char* tag = iter->attribute("value").as_string();
		tagList.push_back(tag);
		interactionsRaw[tagPos] = iter->child_value();
	}

	for (uint j = 0; j < tagPos; j++) {
		uint i = 0, indexInTmpString = 0;
		char tmpString[64];
		memset(&tmpString, '\0', 64);
		while (interactionsRaw[j] != '\0') {
			char c = interactionsRaw[j][i];
			if (c == ';' || c == '\0') {
				for (uint pos = 0; pos < tagList.size(); pos++) {
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
	return false;
}

bool ModuleCollision::PreUpdate() {
	return false;
}

bool ModuleCollision::Update(float dt) {
	return false;
}

bool ModuleCollision::PostUpdate() {
	return false;
}

bool ModuleCollision::CleanUp(xmlNode & config) {
	return false;
}
