#pragma once
#include "Actor.h"

class Projectile : public Actor
{
private:
	const std::string __PROJECTILE_TEXTURE = "Chocolate";
	const std::string __ENEMY_PROJECTILE_TEXTURE = "Sugar";
	const std::string __ENEMY_PROJECTILE_LASER_TEXTURE = "Cream";

	const float __PROJECTILE_SPEED = 800;
	const float __PROJECTILE_DAMAGES = 50;
	const float __PROJECTILE_HITRATE = 100;

public:
	explicit Projectile(SharedContext* p_sharedContext, const Vector2D<float> p_direction, Actor* p_source, const float p_x = 0, const float p_y = 0, const bool p_friendly = true, const bool p_isLaser = false);
	~Projectile();

	void Update(const sf::Time& l_time) override;

	void MultiplyDamages(const float p_value);
	void MultiplySpeed(const float p_value);
	void MultiplyHitrate(const float p_value);

	void Kill();

	Actor* GetSource() const;

	bool IsFriendly() const;
	bool DealsConstantDamages() const;

	void SetDamages(const float p_value);
	void SetSpeed(const float p_speed);

private:
	bool m_friendly;
	bool m_constantDamages;
	bool m_constantlyRotate;

	Actor* m_source;

	float m_damages;
	float m_hitrate;
};
