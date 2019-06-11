#ifndef FLOCKING_OBSTACLE_H_
#define FLOCKING_OBSTACLE_H_

#include <SFML\Graphics.hpp>
#include <cmath>
#include "Flocking.h" 

//----------------------------------SeparationBall-----------------------------------------

class ObstacleBall : public sf::Drawable, public FlockingAlgorithm::FlockingObstacle {
public:
	const float m_ObstacleSeparateWeight;

private:
	bool m_update;
	sf::CircleShape m_obstacleShape;
	const long double m_ObstacleSquaredZone;
	sf::Vector2f m_velocity;

	float getLeft() const;
	float getRight() const;
	float getTop() const;
	float getBottom() const;

public:
	ObstacleBall(float diameter, int vertices, sf::Color color, float obstacleSquareRootZone, sf::Vector2f position, float speed, float obstacleSeparateWeight, bool update);
	FlockingAlgorithm::FlockingObstacle convertToFlockingObstacle();

public:
	void update(sf::FloatRect displayAreaSize, float deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

//----------------------------------MagnetBall---------------------------------------------

class MagnetBall : public sf::Drawable {
public:
	const float m_MagnetAttractWeight;

private:
	bool m_update;
	sf::CircleShape m_magnetShape;
	const long double m_MagnetSquaredZone;
	sf::Vector2f m_velocity; 

	float getLeft() const;
	float getRight() const;
	float getTop() const;
	float getBottom() const;

public:
	MagnetBall(float diameter, int vertices, sf::Color color, float magnetSquareRootZone, sf::Vector2f position, float speed, float magnetAttractWeight, bool update);
	FlockingAlgorithm::FlockingMagnet convertToFlockingMagnet();

public:
	void update(sf::FloatRect displayAreaSize, float deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif