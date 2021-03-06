#include "JellyBear.h"
#include "StateManager.h"

JellyBear::JellyBear(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYBEAR_TEXTURE);

	JellyBear::GenerateStats();
	ResetLife();

	m_isDashing = false;
	m_dashTimer = 0.0f;
	m_dashMaxDuration = 0.2f;
}

void JellyBear::OnDeath()
{
	m_sharedContext->m_audioManager->PlaySound("Death_Jelly");
	m_sharedContext->m_gameInfo->m_toothPaste += 3;
}

void JellyBear::GenerateStats()
{
	m_velocity					= __JELLYBEAR_SPEED;
	m_maxVelocity				= m_velocity;

	m_specialAbilityCooldown	= __JELLYBEAR_SPECIAL_ABILITY_COOLDOWN;

	m_maxLife = CalculateStat(__JELLYBEAR_LIFE, __JELLYBEAR_LIFE_INCREMENTATION_COEFFICIENT, __JELLYBEAR_MAX_LIFE);
	m_damages = CalculateStat(__JELLYBEAR_DAMAGES, __JELLYBEAR_DAMAGES_INCREMENTATION_COEFFICIENT, __JELLYBEAR_MAX_DAMAGES);
}

void JellyBear::Update(const sf::Time& l_time)
{
	if (m_isDashing)
	{
		m_specialAbilityTimer = 0.0f;
		m_velocity = __JELLYBEAR_DASH_SPEED;
		m_dashTimer += l_time.asSeconds();
		if (m_dashTimer >= m_dashMaxDuration)
			m_isDashing = false;
	}
	else
	{
		m_velocity = __JELLYBEAR_SPEED;
		SetTarget(m_sharedContext->m_actorManager->GetPlayer());
	}

	Enemy::Update(l_time);
}

void JellyBear::SpecialAbility(const sf::Time& l_time)
{
	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (it->IsFriendly() &&
			(m_position.AngleTo(it->GetPosition()) < 5 || m_position.AngleTo(it->GetPosition()) > -5) &&
			!it->MustDie() &&
			m_position.DistanceTo(it->GetPosition()) <= __JELLYBEAR_DASH_MIN_DISTANCE)
		{
			SetTarget(it);
			m_isDashing = true;
			m_dashTimer = 0.0f;
			StartSpecialAbilityCooldown();
			return;
		}
	}
}
