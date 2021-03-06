#pragma once

#include "Actor.h"

enum class SpawnerType
{
	JELLY_SPAWNER = 0,
	LOLLIPOP_SPAWNER = 1,
	JELLYBEAR_SPAWNER = 2,
	CROCODILE_SPAWNER = 3,
	CAKEMONSTER_SPAWNER = 4,
	NOT_SET = 5
};

class SpawnPoint : public Actor
{
	const std::string	__SPAWNPOINT_TEXTURE = "SpawnPoint";
	const float			__DISTANCE_MIN_TO_OTHER_SPAWNPOINT = 150.0f;
	const float			__DISTANCE_MIN_TO_DOOR = 150.0f;

	//Jelly
	const int			__SPAWNPOINT_JELLY_MAXSPAWN = 5;
	const float			__SPAWNPOINT_JELLY_SPAWN_FREQUENCY = 2.0f;
	//JellyBear
	const int			__SPAWNPOINT_JELLYBEAR_MAXSPAWN = 3;
	const float			__SPAWNPOINT_JELLYBEAR_SPAWN_FREQUENCY = 7.0f;
	//Crocodile
	const int			__SPAWNPOINT_CROCODILE_MAXSPAWN = 2;
	const float			__SPAWNPOINT_CROCODILE_SPAWN_FREQUENCY = 10.0f;
	//CakeMonster
	const int			__SPAWNPOINT_CAKEMONSTER_MAXSPAWN = 3;
	const float			__SPAWNPOINT_CAKEMONSTER_SPAWN_FREQUENCY = 5.0f;
	//Lollipop
	const int			__SPAWNPOINT_LOLLIPOP_MAXSPAWN = 7;
	const float			__SPAWNPOINT_LOLLIPOP_SPAWN_FREQUENCY = 1.0f;

public:
	explicit SpawnPoint(SharedContext* p_sharedContext, const SpawnerType p_spawnerType = SpawnerType::NOT_SET);
	~SpawnPoint();
	
	bool IsDone() const;
	bool IsActive() const;

	void Desactivate();
	void Activate();
	void ToggleActive();

	Vector2D<float> GetFuturePosition() const;

	void SetActivationDelay(const float p_delay);

	void SpawnEnemy();

	void Update(const sf::Time& l_time) override;
	void Draw() const override;

private:
	Actor* m_target;

	uint8_t m_maxSpawn;
	uint8_t m_spawnedCounter;

	bool m_active;
	bool m_hovered;

	Vector2D<float> m_futurePosition;

	float m_spawnFrequency;
	float m_timer;

	float m_secondsBeforeActivation;

	float m_activationTimer;

	SpawnerType m_type;

};