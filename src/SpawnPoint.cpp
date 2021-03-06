#include "SpawnPoint.h"
#include "StateManager.h"
#include "Jelly.h"
#include "JellyBear.h"
#include "CakeMonster.h"
#include "Crocodile.h"
#include "Lollipop.h"


SpawnPoint::SpawnPoint(SharedContext * p_sharedContext, const SpawnerType p_spawnerType) :
	Actor(p_sharedContext)
{
	SetTexture(__SPAWNPOINT_TEXTURE);
	m_velocity = 0.0f;

	m_timer = 0;

	m_active = false;
	m_hovered = false;

	m_spawnedCounter = 0;

	m_activationTimer = 0;

	const bool isFirstSpawner = m_sharedContext->m_actorManager->GetSpawnPoints().size() == 1;

	if (isFirstSpawner)
		m_secondsBeforeActivation = 3;
	else
		m_secondsBeforeActivation = Utils::randomgen(3, 20);

	bool spawnIsCorrect;

	uint16_t tries = 0;
	Vector2D<float> newPos;
	ActorManager* actorManager = m_sharedContext->m_actorManager;

	do
	{
		spawnIsCorrect = true;
		newPos.Set(Utils::randomgen(250, 1600), Utils::randomgen(325, 950));

		for (auto it : actorManager->GetSpawnPoints())
			if (newPos.DistanceTo(it->GetFuturePosition()) <= __DISTANCE_MIN_TO_OTHER_SPAWNPOINT && this != it)
				spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetDoor(0)->GetPosition()) <= __DISTANCE_MIN_TO_DOOR)
			spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetDoor(1)->GetPosition()) <= __DISTANCE_MIN_TO_DOOR)
			spawnIsCorrect = false;

		++tries;
	}
	while (!spawnIsCorrect && tries < 9999);

	m_futurePosition = newPos;
	m_position = newPos;
	m_position.Y(-100);

	uint8_t enemyVariety = 1 + m_sharedContext->m_gameInfo->m_currentLevel / 5;

	if (enemyVariety > 4)
		enemyVariety = 4;

	if (p_spawnerType == SpawnerType::NOT_SET)
		m_type = static_cast<SpawnerType>(Utils::randomgen(0, enemyVariety));
	else
		m_type = p_spawnerType;

	switch (m_type)
	{
	case SpawnerType::JELLY_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_JELLY_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_JELLY_MAXSPAWN;
		break;
	case SpawnerType::LOLLIPOP_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_LOLLIPOP_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_LOLLIPOP_MAXSPAWN;
		break;
	case SpawnerType::JELLYBEAR_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_JELLYBEAR_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_JELLYBEAR_MAXSPAWN;
		break;
	case SpawnerType::CROCODILE_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_CROCODILE_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_CROCODILE_MAXSPAWN;
		break;
	case SpawnerType::CAKEMONSTER_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_CAKEMONSTER_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_CAKEMONSTER_MAXSPAWN;
		break;
	case SpawnerType::NOT_SET:
		break;
	}

	++m_sharedContext->m_gameInfo->m_spawnedSpawnPoints;
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::SpawnEnemy()
{
	m_timer = 0;

	switch (m_type)
	{
	case SpawnerType::JELLY_SPAWNER :
		m_sharedContext->m_actorManager->AddEnemy(new Jelly(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::JELLYBEAR_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new JellyBear(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::LOLLIPOP_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new Lollipop(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::CROCODILE_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new Crocodile(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::CAKEMONSTER_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new CakeMonster(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::NOT_SET:
		break;
	}
	++m_spawnedCounter;
}

void SpawnPoint::Update(const sf::Time & l_time)
{
	if (IsActive())
		m_timer += l_time.asSeconds();

	
	if (!IsActive())
	{
		m_activationTimer += l_time.asSeconds();
		if (m_activationTimer >= m_secondsBeforeActivation - 1 && m_position.Y() < m_futurePosition.Y())
		{
			m_position.Y() += 1500 * l_time.asSeconds();
		}
		
		if (m_position.Y() > m_futurePosition.Y())
			m_position = m_futurePosition;

		if (m_activationTimer >= m_secondsBeforeActivation)
			Activate();
	}

	m_hovered = false;

	for (auto it : m_sharedContext->m_actorManager->GetEnemies())
		if (m_position.DistanceTo(it->GetPosition()) <= 50)
		{
			m_timer = 0.f;
			m_hovered = true;
		}

	if (IsActive() && !IsDone() && m_timer >= m_spawnFrequency)
		SpawnEnemy();

	Actor::Update(l_time);
}

void SpawnPoint::ToggleActive()
{
	m_active = !m_active;
}

Vector2D<float> SpawnPoint::GetFuturePosition() const
{
	return m_futurePosition;
}

void SpawnPoint::SetActivationDelay(const float p_delay)
{
	m_secondsBeforeActivation = p_delay;
}

void SpawnPoint::Desactivate()
{
	if (m_active)
		m_active = false;
}

void SpawnPoint::Activate()
{
	if (!m_active)
	{
		m_active = true;
		m_timer = m_spawnFrequency;
	}
}

void SpawnPoint::Draw() const
{
	if (!IsDone())
	{
		Actor::Draw();
		if (m_sharedContext->m_gameInfo->m_debugMode && m_sharedContext->m_fontManager->RequireResource("Console"))
		{
			sf::Text spawnDelayLabel;
			spawnDelayLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Console"));

			if (m_hovered)
				spawnDelayLabel.setFillColor(sf::Color::Red);
			else
				spawnDelayLabel.setFillColor(sf::Color::Yellow);

			spawnDelayLabel.setString(std::to_string(m_spawnFrequency - m_timer));
			spawnDelayLabel.setPosition(m_position.ToSFVector());
			spawnDelayLabel.move(0, -55);
			spawnDelayLabel.setCharacterSize(25);
			Utils::centerOrigin(spawnDelayLabel);
			m_sharedContext->m_wind->GetRenderWindow()->draw(spawnDelayLabel);


			sf::Text toSpawnLabel;
			toSpawnLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Console"));
			toSpawnLabel.setFillColor(sf::Color::White);
			toSpawnLabel.setString(std::to_string(m_spawnedCounter) + "/" + std::to_string(m_maxSpawn));
			toSpawnLabel.setPosition(m_position.ToSFVector());
			toSpawnLabel.move(0, 55);
			toSpawnLabel.setCharacterSize(25);
			Utils::centerOrigin(toSpawnLabel);
			m_sharedContext->m_wind->GetRenderWindow()->draw(toSpawnLabel);
		}
	}
}

bool SpawnPoint::IsDone() const { return m_spawnedCounter == m_maxSpawn; }
bool SpawnPoint::IsActive() const { return m_active; }