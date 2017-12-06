#pragma once

#include <vector>

#include "Actor.h"
#include "Player.h"
#include "Npc.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Door.h"
#include "SpawnPoint.h"

class ActorManager
{
public:
	ActorManager();
	~ActorManager();

	void AddEnemy(Enemy* p_newActor);
	void AddProjectile(Projectile* p_newActor);
	void AddSpawnPoint(SpawnPoint* p_newSpawnPoint);

	static void AddActor(std::multimap<uint16_t&, Actor*>& p_actors, Actor* p_actor);

	void SetPlayer(Player* p_player);
	void SetNpc(Npc* p_npc);
	void SetDoor(const uint8_t p_id, Door* p_door);

	void CleanActors();
	void Update(const sf::Time& l_time);
	void CheckDeads();
	void Draw();

	Player* GetPlayer() const;
	Npc* GetNpc() const;
	Door* GetDoor(const uint8_t p_id);
	std::vector<Projectile*>& GetProjectile();
	std::vector<Enemy*>& GetEnemies(); 
	std::vector<SpawnPoint*>& GetSpawnPoints();

private:
	Player* m_player;
	Npc* m_npc;
	Door* m_doors[2];
	std::vector<Enemy*> m_enemies;
	std::vector<Projectile*> m_projectiles;
	std::vector<SpawnPoint*> m_spawnPoints;
};
