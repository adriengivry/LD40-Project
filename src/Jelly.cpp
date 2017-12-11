#include "Jelly.h"
#include "StateManager.h"

Jelly::Jelly(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{

	SetTexture(__JELLY_TEXTURE);
	m_spriteScale.Set(1.2f, 1.2f);
	
	Jelly::GenerateStats();
	ResetLife();
}

void Jelly::OnDeath()
{
	m_sharedContext->m_audioManager->PlaySound("Death_Jelly");
}

void Jelly::GenerateStats()
{
	m_velocity = __JELLY_SPEED;
	m_maxVelocity = m_velocity;

	m_maxLife = CalculateStat(__JELLY_LIFE, __JELLY_LIFE_INCREMENTATION_COEFFICIENT, __JELLY_MAX_LIFE);
	m_damages = CalculateStat(__JELLY_DAMAGES, __JELLY_DAMAGES_INCREMENTATION_COEFFICIENT, __JELLY_MAX_DAMAGES);
}
