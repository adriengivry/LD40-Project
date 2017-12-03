#include "Door.h"
#include "StateManager.h"

Door::Door(SharedContext* p_sharedContext, const float p_x, const float p_y, const bool p_answer)
	: Actor(p_sharedContext, p_x, p_y)
{
	Desactivate();

	SetTexture(__DOOR_TEXTURE);

	m_answer = p_answer;

	m_gotAShadow = false;

	m_velocity = 0.0f;
}

Door::~Door()
{
}

void Door::Activate()
{
	m_activated = true;
}

void Door::Desactivate()
{
	m_activated = false;
}

void Door::Use()
{
	if (m_answer == m_sharedContext->m_actorManager->GetNpc()->GetAnswer())
	{
		m_sharedContext->m_gameInfo->m_doorPassed = true;
		++m_sharedContext->m_gameInfo->m_currentLevel;
	}
	else
	{
		m_sharedContext->m_gameInfo->m_doorPassed = true;
		++m_sharedContext->m_gameInfo->m_currentLevel;
		SelectCurse();
	}
}

void Door::Draw() const
{
	Actor::Draw();

	sf::Text label;
	label.setCharacterSize(25);
	label.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
	label.setPosition(m_position.ToSFVector());
	if (m_answer)
		label.setString("TRUE");
	else
		label.setString("FALSE");
	Utils::centerOrigin(label);
	
	m_sharedContext->m_wind->GetRenderWindow()->draw(label);
}

bool Door::IsActivated() const { return m_activated; }

void Door::SelectCurse() const
{
	GameInfo* gameInfo = m_sharedContext->m_gameInfo;

	uint8_t curse;
		
	do
		curse = Utils::randomgen(0, 3);
	while (curse == REVERSE_MOVEMENT && gameInfo->m_reverseMovement);

	switch (curse)
	{
	case REVERSE_MOVEMENT:
		gameInfo->m_reverseMovement = true;
		break;

	case SLOWER_CARRIE:
		++gameInfo->m_slowerCarrie;
		break;

	case SLOWER_PROJECTILES:
		++gameInfo->m_slowerProjectiles;
		break;

	case WEAKER_PROJECTILES:
		++gameInfo->m_weakerProjectiles;
		break;

	default:
		break;
	}
}

void Door::Update(const sf::Time& l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_position.DistanceTo(player->GetPosition()) <= __DOOR_ACTIVATION_ZONE && IsActivated() && player->GetDirection().Y() < 0)
	{
		Use();
	}

	Actor::Update(l_time);
}
