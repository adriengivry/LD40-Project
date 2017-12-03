#pragma once

#include "Actor.h"

class Enemy : public Actor
{
public:
	explicit Enemy(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Enemy();

	void SetTarget(Actor* p_target);

	void Kill();

	void RemoveLife(const float p_damages);

	void Update(const sf::Time& l_time) override;

	void StartCooldown();

	virtual void Attack();
	void FollowTarget();
	void CheckCollisions();
	void CheckCooldown(const sf::Time& l_time);

	void Draw() const override;

	void DrawLifebar() const;

	void DrawCooldownBar() const;

protected:
	Actor* m_target;
	bool m_followTarget;
	bool m_damagesOnContact;
	bool m_isReady;

	bool m_canCollide;

	float m_damages;

	float m_maxLife;
	float m_life;
	float m_cooldown;
	float m_timer;
};