#include <SFML\Graphics.hpp>
#include <cmath>
#include "FlockingObjects.h"

//----------------------------------SeparationBall-----------------------------------------

float ObstacleBall::getLeft() const {
	return m_obstacleShape.getPosition().x - m_obstacleShape.getRadius();
}

float ObstacleBall::getRight() const {
	return m_obstacleShape.getPosition().x + m_obstacleShape.getRadius();
}

float ObstacleBall::getTop() const {
	return m_obstacleShape.getPosition().y - m_obstacleShape.getRadius();
}

float ObstacleBall::getBottom() const {
	return m_obstacleShape.getPosition().y + m_obstacleShape.getRadius();
}

ObstacleBall::ObstacleBall(float diameter, int vertices, sf::Color color, float obstacleSquareRootZone, sf::Vector2f position, float speed, float obstacleSeparateWeight, bool update)
	: m_obstacleShape(diameter/2.f, vertices), m_ObstacleSquaredZone(std::pow(obstacleSquareRootZone, 2)), m_ObstacleSeparateWeight(obstacleSeparateWeight),
	FlockingAlgorithm::FlockingObstacle (
		obstacleSquareRootZone,
		m_ObstacleSeparateWeight,
		position) {
	m_obstacleShape.setFillColor(color);
	m_obstacleShape.setOrigin(diameter/2.f, diameter/2.f);
	m_obstacleShape.setPosition(position); 
	m_velocity = sf::Vector2f(speed, speed);
	m_update = update;
}

///--------------------------------------------------------------------------

FlockingAlgorithm::FlockingObstacle ObstacleBall::convertToFlockingObstacle() {
	return FlockingAlgorithm::FlockingObstacle(
		std::sqrt(m_ObstacleSquaredZone),
		m_ObstacleSeparateWeight,
		m_obstacleShape.getPosition()
	);
}

///--------------------------------------------------------------------------

void ObstacleBall::update(sf::FloatRect displayAreaSize, float deltaTime) {

	if (m_update) {

		if (getLeft() <= displayAreaSize.left) {
			if (m_velocity.x < 0) m_velocity.x = -m_velocity.x;
		}

		else if (getRight() >= displayAreaSize.left + displayAreaSize.width) {
			if (m_velocity.x > 0) m_velocity.x = -m_velocity.x;
		}

		if (getTop() <= displayAreaSize.top) {
			if (m_velocity.y < 0) m_velocity.y = -m_velocity.y;
		}

		else if (getBottom() >= displayAreaSize.top + displayAreaSize.height) {
			if (m_velocity.y > 0) m_velocity.y = -m_velocity.y;
		}

		m_obstacleShape.move(m_velocity * deltaTime);
	}
}

///--------------------------------------------------------------------------

void ObstacleBall::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_obstacleShape);
}

//----------------------------------MagnetBall---------------------------------------------

float MagnetBall::getLeft() const {
	return m_magnetShape.getPosition().x - m_magnetShape.getRadius();
}

float MagnetBall::getRight() const {
	return m_magnetShape.getPosition().x + m_magnetShape.getRadius();
}

float MagnetBall::getTop() const {
	return m_magnetShape.getPosition().y - m_magnetShape.getRadius();
}

float MagnetBall::getBottom() const {
	return m_magnetShape.getPosition().y + m_magnetShape.getRadius();
}

///--------------------------------------------------------------------------

MagnetBall::MagnetBall(float diameter, int vertices, sf::Color color, float magnetSquareRootZone, sf::Vector2f position, float speed, float magnetAttractWeight, bool update)
	: m_magnetShape(diameter / 2.f, vertices), m_MagnetSquaredZone(std::pow(magnetSquareRootZone, 2)), m_MagnetAttractWeight(magnetAttractWeight) {
	m_magnetShape.setFillColor(sf::Color::Black);
	m_magnetShape.setOrigin(diameter / 2.f,  diameter / 2.f);
	m_magnetShape.setPosition(position);
	m_velocity = sf::Vector2f(speed, speed);
	m_update = update;
}

///--------------------------------------------------------------------------

FlockingAlgorithm::FlockingMagnet MagnetBall::convertToFlockingMagnet() {
	return FlockingAlgorithm::FlockingMagnet(
		std::sqrt(m_MagnetSquaredZone),
		m_MagnetAttractWeight,
		m_magnetShape.getPosition()
	);
}

///--------------------------------------------------------------------------

void MagnetBall::update(sf::FloatRect displayAreaSize, float deltaTime) {

	if (m_update) {

		if (getLeft() <= displayAreaSize.left) {
			if (m_velocity.x < 0) m_velocity.x = -m_velocity.x;
		}

		else if (getRight() >= displayAreaSize.left + displayAreaSize.width) {
			if (m_velocity.x > 0) m_velocity.x = -m_velocity.x;
		}

		if (getTop() <= displayAreaSize.top) {
			if (m_velocity.y < 0) m_velocity.y = -m_velocity.y;
		}

		else if (getBottom() >= displayAreaSize.top + displayAreaSize.height) {
			if (m_velocity.y > 0) m_velocity.y = -m_velocity.y;
		}

		m_magnetShape.move(m_velocity * deltaTime);
	}
}

///--------------------------------------------------------------------------

void MagnetBall::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_magnetShape);
}