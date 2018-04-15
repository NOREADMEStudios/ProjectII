#include "ModuleEntityManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Hero.h"
#include "ModuleAudio.h"
#include "ModuleSceneManager.h"


ModuleEntityManager::ModuleEntityManager()
{
	name = "entities";
}


ModuleEntityManager::~ModuleEntityManager(){}

bool ModuleEntityManager::Awake(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Awake(n);
	}

	// Load SFX from XML
	xmlDocument audio_xml_file;
	xmlNode audio_xml;
	xmlNode iter;
	xmlNode iter2;

	pugi::xml_parse_result result = audio_xml_file.load_file("Assets/Audio/audio_xml.xml");
	if (result != NULL)
	{
		audio_xml = audio_xml_file.child("audio");
	}
	
	std::list<std::string> sounds_list;

	// SFX names finding loop
	for (iter = audio_xml.first_child(); iter; iter = iter.next_sibling())
	{
		for (iter2 = iter.first_child(); iter2; iter2 = iter2.next_sibling())
		{
			if (iter2.text().as_string() != "")
			{
				if (!IsSFXRepeated(sounds_list, iter2.text().as_string()))
				{
					sounds_list.push_back(iter2.text().as_string());
					App->audio->LoadFx(iter2.text().as_string());
				}
			}
		}
	}
	sounds_list.clear();
	

	return true;
}

bool ModuleEntityManager::Start() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Start();
	}

	StartItems();
	return true;
}

bool ModuleEntityManager::PreUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PreUpdate();
	}
	return true;
}

bool ModuleEntityManager::Update(float dt) {
	uint i = 0;
	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		if ((*item)->active)
		{
			(*item)->Update(dt);
			winner = (*item)->hero_num;
			i++;
		}
	}
	if (i == 1)
	{
		finish = true;
	}
	return true;
}

bool ModuleEntityManager::PostUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PostUpdate();

	}
	return true;
}

bool ModuleEntityManager::CleanUp(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->CleanUp(n);
	}
	StartItems();
	return true;
}

bool ModuleEntityManager::Load(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Load(n);
	}
	return true;
}

bool ModuleEntityManager::Save(pugi::xml_node& n)const {

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Save(n);
	}
	return true;
}

Entity* ModuleEntityManager::CreateCharacter(CharacterInfo charInfo) {

	Entity* ret = nullptr;

	if (charInfo.chType == CharacterTypes::ENEMY)
	{
		ret = new Enemy();
		numofplayers++;
	}
	else if (charInfo.chType == CharacterTypes::HERO)
	{
		ret = new Hero();
		numofplayers++;
		ret->hero_num = numofplayers;
	}
	else
	{
		//ret = new Entity();
		return nullptr;//
	}

	ret->type = CHARACTER;
	ret->SetPos(charInfo.pos.x, charInfo.pos.y);

	
	entities.push_back(ret);
	ret->Start();

	return ret;
}


void ModuleEntityManager::DestroyEntity(Entity* entity) {
	pugi::xml_node n;
	if (entity->hero_num != 0) {
		numofplayers--;
	
	}
	entity->CleanUp(n);
	entities.remove(entity);
	entity = nullptr;
}

void ModuleEntityManager::CheckMidPos(float &min_x, float &max_x)
{
	uint current_players = 0;
	if (entities.size() > 0) {
		min_x = entities.front()->GetPosX();
		max_x = entities.front()->GetPosX();

		for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
			if ((*item)->GetType() == CHARACTER)
			{
				current_players++;
				if ((*item)->GetPosX() < min_x)
				{
					min_x = (*item)->GetPosX();
				}
				else if ((*item)->GetPosX() + (*item)->GetCollider().w > max_x)
				{
					max_x = (*item)->GetPosX() + (*item)->GetCollider().w;
				}
			}

			if (current_players == numofplayers)
			{
				break;
			}
		}
	}
}

void ModuleEntityManager::CheckMidPosY(float &min_y, float &max_y)
{
	uint current_players = 0;
	if (entities.size() > 0) {
		min_y = entities.front()->GetPosY();
		max_y = entities.front()->GetPosY();

		for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
			if ((*item)->GetType() == CHARACTER)
			{
				current_players++;
				if ((*item)->GetPosX() < min_y)
				{
					min_y = (*item)->GetPosY();
				}
				else if ((*item)->GetPosY() + (*item)->GetCollider().h > max_y)
				{
					max_y = (*item)->GetPosY() + (*item)->GetCollider().h;
				}
			}

			/*if (current_players == numofplayers)
			{
				break;
			}*/
		}
	}
}

void ModuleEntityManager::PauseEntities(bool pause) {
	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->paused = pause;
	}
}

bool ModuleEntityManager::IsSFXRepeated(std::list<std::string> list, std::string string) const
{
	for (std::list<std::string> newList = list; !newList.empty();)
	{
		if (newList.front() == string)
			return true;
		newList.pop_front();
	}
	return false;
}

Entity* ModuleEntityManager::GetEntity(uint num)
{
	Entity* ret = nullptr;

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		if ((*item)->hero_num == num)
		{
			ret == (*item);
		}
	}
	return ret;
}

void ModuleEntityManager::StartItems()
{
	for (int i = 0; i < 3; i++)
	{
		items[i].atk = 0;
		items[i].def = 0;
		items[i].spd = 0;
		items[i].life = 0;
		items[i].mgk = 0;
	}
}