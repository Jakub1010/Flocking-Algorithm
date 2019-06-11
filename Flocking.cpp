#include <SFML\Graphics.hpp>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <random>
#include <list>
#include <iostream>
#include <memory>
#include <functional>
#include "EntityShape.h" 
#include "Flocking.h"
#include "Chronometer.h"

namespace FlockingAlgorithm {

	//--------------------------AdditionalMath-----------------------------------------

	namespace AdditionalMatch {

		constexpr double DEGREES_TO_RADIANS{ 3.14159265359f / 180.f };

		sf::Vector2f getNormalizedVector(const sf::Vector2f& vec2) {
			float diagonal = std::sqrt(std::pow(vec2.x, 2) + std::pow(vec2.y, 2));
			return vec2 / diagonal;
		}

		long double getVectorLength(const sf::Vector2f& vec2) {
			float diagonal = std::sqrt(std::pow(vec2.x, 2) + std::pow(vec2.y, 2));
			return diagonal;
		}

		int getRandomIntigerValue(int min, int max) {
			static std::random_device r;
			static std::default_random_engine gen(r());
			std::uniform_int_distribution<int> dist(min, max);
			return dist(gen);
		}

		float getRandomFloatValue(float min, float max) {
			static std::random_device r;
			static std::mt19937 gen(r());
			std::uniform_real_distribution<> dist(min, max);
			return dist(gen);
		}
	}

	//--------------------------------FlockingObstacle---------------------------------

	FlockingObstacle::FlockingObstacle(float obstacleSquareRootZone, float obstacleSeparateWeight, sf::Vector2f position) 
		: m_ObstacleSquaredZone(std::pow(obstacleSquareRootZone, 2)), m_ObstacleSeparateWeight(obstacleSeparateWeight), m_position(position), m_eventStaff(nullptr)
	{}

	///-------------------------------------------------------------------------

	FlockingObstacle::FlockingObstacle(const FlockingObstacle& flockingObstacle) 
		: m_ObstacleSquaredZone(flockingObstacle.m_ObstacleSquaredZone),
			m_ObstacleSeparateWeight(flockingObstacle.m_ObstacleSeparateWeight),
			m_position(flockingObstacle.getPosition()), m_eventStaff(nullptr)
	{}

	///-------------------------------------------------------------------------

	FlockingObstacle::FlockingObstacle(const FlockingObstacle& flockingObstacle, sf::Vector2f position)
		: m_ObstacleSquaredZone(flockingObstacle.m_ObstacleSquaredZone),
		m_ObstacleSeparateWeight(flockingObstacle.m_ObstacleSeparateWeight),
		m_position(position), m_eventStaff(nullptr)
	{}

	///-------------------------------------------------------------------------

	void FlockingObstacle::operator=(const FlockingObstacle& flockingObstacle) {
		this->m_ObstacleSquaredZone = flockingObstacle.m_ObstacleSquaredZone;
		this->m_ObstacleSeparateWeight = flockingObstacle.m_ObstacleSeparateWeight;
		this->m_position = flockingObstacle.getPosition();
		this->m_eventStaff = flockingObstacle.m_eventStaff;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f FlockingObstacle::getPosition() const {
		return m_position;
	}

	///-------------------------------------------------------------------------

	long double FlockingObstacle::getSeparationSquaredZone() const {
		return m_ObstacleSquaredZone;
	}

	///-------------------------------------------------------------------------

	float FlockingObstacle::getSeparationSquareRootZone() const {
		return std::sqrt(m_ObstacleSquaredZone);
	}

	///-------------------------------------------------------------------------

	long double FlockingObstacle::getSimpleDistanceSquared(Entity& flockingEntity) const {
		return std::pow(this->m_position.x - flockingEntity.getPosition().x, 2) +
			std::pow(this->m_position.y - flockingEntity.getPosition().y, 2);
	}

	///-------------------------------------------------------------------------

	long double FlockingObstacle::getDistanceSquared(Entity& flockingEntity) const {

		long double distance = std::pow(this->m_position.x - flockingEntity.getPosition().x, 2) +
			std::pow(this->m_position.y - flockingEntity.getPosition().y, 2);

		if (m_eventStaff) {
			m_eventStaff(const_cast<FlockingObstacle&>(*this), flockingEntity);
		}

		return distance;
	}

	///-------------------------------------------------------------------------

	void FlockingObstacle::setEventStaff(EventStaffObstacleSignature eventStaff) {
		m_eventStaff = eventStaff;
	}

	///-------------------------------------------------------------------------

	bool FlockingObstacle::isEventStaffSet() const {
		return m_eventStaff != nullptr;
	}

	///-------------------------------------------------------------------------

	float FlockingObstacle::getObstacleSeparateWeight() const {
		return m_ObstacleSeparateWeight;
	}

	//--------------------------------FlockingMagnet-----------------------------------

	FlockingMagnet::FlockingMagnet(float magnetSquareRootZone, float magnetAttractWeight, sf::Vector2f position)
		: m_MagnetSquaredZone(std::pow(magnetSquareRootZone, 2)), m_MagnetAttractWeight(magnetAttractWeight), m_position(position), m_eventStaff(nullptr)
	{}

	///-------------------------------------------------------------------------

	FlockingMagnet::FlockingMagnet(const FlockingMagnet& flockingMagnet)
		: m_MagnetSquaredZone(flockingMagnet.m_MagnetSquaredZone),
		m_MagnetAttractWeight(flockingMagnet.m_MagnetAttractWeight),
		m_position(flockingMagnet.getPosition()), m_eventStaff(nullptr)
	{}

	///-------------------------------------------------------------------------

	FlockingMagnet::FlockingMagnet(const FlockingMagnet& flockingMagnet, sf::Vector2f position) 
		: m_MagnetSquaredZone(flockingMagnet.m_MagnetSquaredZone),
		m_MagnetAttractWeight(flockingMagnet.m_MagnetAttractWeight),
		m_position(position), m_eventStaff(nullptr)
	{}
	///-------------------------------------------------------------------------

	void FlockingMagnet::operator=(const FlockingMagnet& flockingMagnet) {
		this->m_MagnetSquaredZone = flockingMagnet.m_MagnetSquaredZone;
		this->m_MagnetAttractWeight = flockingMagnet.m_MagnetAttractWeight;
		this->m_position = flockingMagnet.getPosition();
		this->m_eventStaff = flockingMagnet.m_eventStaff;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f FlockingMagnet::getPosition() const {
		return m_position;
	}

	///-------------------------------------------------------------------------

	long double FlockingMagnet::getAttractSquaredZone() const {
		return m_MagnetSquaredZone;
	}

	///-------------------------------------------------------------------------

	float FlockingMagnet::getAttractSquareRootZone() const {
		return std::sqrt(m_MagnetSquaredZone);
	}

	///-------------------------------------------------------------------------

	long double FlockingMagnet::getDistanceSquared(Entity& flockingEntity) const {
		if (m_eventStaff) {
			m_eventStaff(const_cast<FlockingMagnet&>(*this), flockingEntity);
		}
		return std::pow(this->m_position.x - flockingEntity.getPosition().x, 2) +
			std::pow(this->m_position.y - flockingEntity.getPosition().y, 2);
	}

	long double FlockingMagnet::getSimpleDistanceSquared(Entity & flockingEntity) const {
		return std::pow(this->m_position.x - flockingEntity.getPosition().x, 2) +
			std::pow(this->m_position.y - flockingEntity.getPosition().y, 2);
	}

	///-------------------------------------------------------------------------

	void FlockingMagnet::setEventStaff(EventStaffMagnetSignature eventStaff) {
		m_eventStaff = eventStaff;
	}

	///-------------------------------------------------------------------------

	bool FlockingMagnet::isEventStaffSet() const {
		return m_eventStaff != nullptr;
	}

	///-------------------------------------------------------------------------

	float FlockingMagnet::getMagnetAttractWeight() const {
		return m_MagnetAttractWeight;
	}

	//----------------------------------Entity-----------------------------------------

	void Entity::remove() {
		m_toRemove = true;
	}

	bool Entity::isToRemoved() const {
		return m_toRemove;
	}

	Entity::Entity(std::unique_ptr<EntityShape> entityShape, float angle, float separationSquareRootZone, int ID, sf::Vector2f position)
		: m_SeparationCubedZone(std::pow(separationSquareRootZone, 2)), m_ID(ID), m_eventStaff(nullptr), m_toRemove(false) {

		angle *= AdditionalMatch::DEGREES_TO_RADIANS;
		m_velocity = sf::Vector2f(std::cos(angle), std::sin(angle));
		m_entityShape = std::move(entityShape);
		m_entityShape->setPosition(position);
	}

	///-------------------------------------------------------------------------

	Entity::Entity(const Entity& entity)
		: m_SeparationCubedZone(entity.m_SeparationCubedZone), m_ID(entity.m_ID), m_eventStaff(nullptr), m_velocity(entity.m_velocity),m_entityShape(std::move(entity.m_entityShape->copy())),
			m_obstacleBehavioursInfo(std::move(entity.m_obstacleBehavioursInfo)), m_magnetBehavioursInfo(std::move(entity.m_magnetBehavioursInfo)), m_toRemove(entity.m_toRemove) {
	}

	///-------------------------------------------------------------------------

	Entity::Entity(Entity&& entity)
		: m_SeparationCubedZone(entity.m_SeparationCubedZone), m_ID(entity.m_ID), m_eventStaff(nullptr), m_velocity(entity.m_velocity), 
		m_obstacleBehavioursInfo(entity.m_obstacleBehavioursInfo), m_magnetBehavioursInfo(entity.m_magnetBehavioursInfo), m_entityShape(std::move(entity.m_entityShape)), m_toRemove(entity.m_toRemove) {
	}

	///-------------------------------------------------------------------------
	
	const Entity& Entity::operator=(const Entity& entity) {
		m_SeparationCubedZone = entity.m_SeparationCubedZone;
		m_eventStaff = entity.m_eventStaff;
		m_velocity = entity.m_velocity;
		m_obstacleBehavioursInfo = entity.m_obstacleBehavioursInfo;
		m_magnetBehavioursInfo = entity.m_magnetBehavioursInfo;
		m_entityShape = std::move(entity.m_entityShape->copy());
		m_toRemove = entity.m_toRemove;
		return *this; 
	}
	 

	///-------------------------------------------------------------------------

	long double Entity::getSeparationSquaredZone() const {
		return m_SeparationCubedZone;
	}

	///-------------------------------------------------------------------------

	float Entity::getSeparationSquareRootZone() const {
		return std::sqrt(m_SeparationCubedZone);
	}

	///-------------------------------------------------------------------------

	bool Entity::operator!=(const Entity& otherEntity) const {
		return otherEntity.m_ID != this->m_ID;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Entity::getVelocity() const {
		return m_velocity;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Entity::getPosition() const {
		return m_entityShape->getPosition();
	}

	///-------------------------------------------------------------------------

	std::unique_ptr<EntityShape>& Entity::getDrawableObjectPointer() {
		return m_entityShape;
	}

    ///-------------------------------------------------------------------------

	auto& Entity::getDrawableObjectRef() {
		return *m_entityShape;
	}

	///-------------------------------------------------------------------------

	const auto& Entity::getDrawableObjectConstRef() const {
		return *m_entityShape;
	}

	///-------------------------------------------------------------------------

	long double Entity::getDistanceSquared(sf::Vector2f position) const {
		return std::pow(this->m_entityShape->getPosition().x - position.x, 2) +
			std::pow(this->m_entityShape->getPosition().y - position.y, 2);
	}

	///-------------------------------------------------------------------------

	void Entity::operator+=(sf::Vector2f velocity) {
		this->m_velocity += velocity;
	}

	///-------------------------------------------------------------------------

	void Entity::operator=(sf::Vector2f velocity) {
		this->m_velocity = velocity;
	}

	///-------------------------------------------------------------------------

	void Entity::update(float speed, float elapedTime, sf::FloatRect displayAreaSize) {
		float angle = std::atan2(m_velocity.y, m_velocity.x)*(180.f / 3.14159265359f) + 90.f;
		m_entityShape->setRotation(angle);

		m_velocity = AdditionalMatch::getNormalizedVector(m_velocity);
		m_velocity *= speed;
		m_entityShape->move(m_velocity*elapedTime);

		sf::Vector2f temp = m_entityShape->getPosition();
		if (temp.x > (displayAreaSize.left + displayAreaSize.width) + m_entityShape->getSize().x + 1) {
			m_entityShape->setPosition(displayAreaSize.left  -m_entityShape->getSize().x, temp.y);
		}
		else if (temp.y > (displayAreaSize.top + displayAreaSize.height) + m_entityShape->getSize().y + 1) {
			m_entityShape->setPosition(temp.x, displayAreaSize.top - m_entityShape->getSize().y);
		}
		else if (temp.x < displayAreaSize.left - m_entityShape->getSize().x) {
			m_entityShape->setPosition((displayAreaSize.left + displayAreaSize.width) + m_entityShape->getSize().x + 1, temp.y);
		}
		else if (temp.y < displayAreaSize.top - m_entityShape->getSize().y) {
			m_entityShape->setPosition(temp.x, (displayAreaSize.top + displayAreaSize.height) + m_entityShape->getSize().y + 1);
		}
	}

	///-------------------------------------------------------------------------

	void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*m_entityShape);
	}

	///-------------------------------------------------------------------------

	void Entity::setObstacleBehavioursInfo(float obstacleSquareRootZone, float obstacleSeparateWeight) {
		m_obstacleBehavioursInfo = FlockingObstacle(obstacleSquareRootZone, obstacleSeparateWeight, getPosition());
	}

	///-------------------------------------------------------------------------

	void Entity::setMagnetBehavioursInfo(float magnetSquareRootZone, float magnetAttractWeight) {
		m_magnetBehavioursInfo = FlockingMagnet(magnetSquareRootZone, magnetAttractWeight, getPosition());
	}

	///-------------------------------------------------------------------------

	FlockingObstacle Entity::convertToFlockingObstacle() const {
		return FlockingObstacle(m_obstacleBehavioursInfo, m_entityShape->getPosition());
	}

	///-------------------------------------------------------------------------

	FlockingMagnet Entity::convertToFlockingMagnet() const {
		return FlockingMagnet(m_magnetBehavioursInfo, m_entityShape->getPosition());
	}

	///-------------------------------------------------------------------------

	const FlockingObstacle& Entity::getObstacleBehavioursInfo() const {
		return m_obstacleBehavioursInfo;
	}

	///-------------------------------------------------------------------------

	const FlockingMagnet& Entity::getMagnetBehavioursInfo() const {
		return m_magnetBehavioursInfo;
	}

	///-------------------------------------------------------------------------

	std::vector<Entity>& Flock::getListOfEntities() {
		return m_entities;
	}

	///-------------------------------------------------------------------------

	const std::vector<Entity>& Flock::getListOfEntities() const {
		return m_entities; 
	}


	//----------------------------------FlockInfo--------------------------------------

	FlockingBehaviourParameters::FlockingBehaviourParameters(
		float cohereWeight,
		float separateWeight,
		float alignmentWeight,
		float mouseCursorSeparateWeight,
		float mouseCursorCohereWeight,
		MouseEventsSettings mouseEventsSettings
	) 
		: m_CohereWeight(cohereWeight), m_SeparateWeight(separateWeight), m_AlignmentWeight(alignmentWeight),
			m_MouseCursorSeparateWeight(mouseCursorSeparateWeight), m_MouseCursorCohereWeight(mouseCursorCohereWeight),
			m_mouseEventsSettings(mouseEventsSettings)
	{}

	///-------------------------------------------------------------------------

	void FlockingBehaviourParameters::setCohereWeight(float data) {
		m_CohereWeight = data; 
	}
	void FlockingBehaviourParameters::setSeparateWeight(float data) {
		m_SeparateWeight = data; 
	}
	void FlockingBehaviourParameters::setAlignmentWeight(float data) {
		m_AlignmentWeight = data; 
	}
	void FlockingBehaviourParameters::setMouseCursorSeparateWeight(float data) {
		m_MouseCursorSeparateWeight = data; 
	}
	void FlockingBehaviourParameters::setMouseCursorCohereWeight(float data) {
		m_MouseCursorCohereWeight = data; 
	}
	void FlockingBehaviourParameters::setMouseEventsSettings(MouseEventsSettings mouseEventsSettings) {
		m_mouseEventsSettings = mouseEventsSettings;
	}

	///-------------------------------------------------------------------------

	float FlockingBehaviourParameters::getCohereWeight() const {
		return m_CohereWeight;
	}
	float FlockingBehaviourParameters::getSeparateWeight() const {
		return m_SeparateWeight;
	}
	float FlockingBehaviourParameters::getAlignmentWeight() const {
		return m_AlignmentWeight;
	}
	float FlockingBehaviourParameters::getMouseCursorSeparateWeight() const {
		return m_MouseCursorSeparateWeight;
	}
	float FlockingBehaviourParameters::getMouseCursorCohereWeight() const {
		return m_MouseCursorCohereWeight;
	}
	FlockingBehaviourParameters::MouseEventsSettings FlockingBehaviourParameters::getMouseEventsSettings() const {
		return m_mouseEventsSettings;
	}


	//----------------------------------Flock------------------------------------------

	Flock::Flock(std::vector<Entity>&& entities, float speed, float neighborhoodSquareRootZone, sf::FloatRect displayAreaSize, const FlockingBehaviourParameters& flockingInfo)
		: m_speed(speed), m_entities(std::move(entities)), m_displayAreaSize(displayAreaSize),
		m_NeighborhoodSquaredZone(std::pow(neighborhoodSquareRootZone, 2)),
		m_flockingInfo(flockingInfo), m_running(true) {
	}

	///-------------------------------------------------------------------------

	bool Flock::isRunning() const {
		return m_running;
	}

	///-------------------------------------------------------------------------

	void Flock::start() {
		m_running = true;
	}

	///-------------------------------------------------------------------------

	void Flock::stop() {
		m_running = false;
	}

	///-------------------------------------------------------------------------

	bool Flock::ckeckID() {
		std::vector<int> temp;
		for (const auto& iy : m_entities) {
			if (std::find(temp.begin(), temp.end(), iy.m_ID) == temp.end()) {
				temp.push_back(iy.m_ID);
			}
			else return false;
		}
		return true;	
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::cohere(Entity& currentEntity) {

		sf::Vector2f forceVector(0, 0);
		sf::Vector2f centerOfMass(0, 0);
		int neighbours = 0;

		for (Entity& entity : m_entities) {

			float distance = entity.getDistanceSquared(currentEntity.getPosition());
			if (entity != currentEntity && 
				distance < m_NeighborhoodSquaredZone && distance>0) {

					centerOfMass += entity.getPosition();
					++neighbours;
			}
		}

		if (neighbours > 0) {
			centerOfMass /= static_cast<float>(neighbours);
			forceVector = centerOfMass - currentEntity.getPosition();
			forceVector = AdditionalMatch::getNormalizedVector(forceVector);
		}

		return forceVector;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::separate(Entity& currentEntity) {

		sf::Vector2f forceVector(0, 0);
		int neighbours = 0;

		for (Entity& entity : m_entities) {
			float distance = entity.getDistanceSquared(currentEntity.getPosition());
			if (entity != currentEntity &&
				distance < currentEntity.getSeparationSquaredZone() && distance>0) {

					sf::Vector2f headingVector = currentEntity.getPosition() - entity.getPosition();
					double scale = AdditionalMatch::getVectorLength(headingVector) /
						std::sqrt(currentEntity.getSeparationSquaredZone());
					headingVector = AdditionalMatch::getNormalizedVector(headingVector);
					headingVector = headingVector / static_cast<float>(scale);
					forceVector += headingVector;
					++neighbours;
				
			}
		}

		if (neighbours > 0) {
			forceVector = forceVector / static_cast<float>(neighbours);
		}
		return forceVector;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::alignment(Entity& currentEntity) {

		sf::Vector2f forceVector(0, 0);
		int neighbours = 0;

		for (Entity& entity : m_entities) {
			float distance = entity.getDistanceSquared(currentEntity.getPosition());
			if (entity != currentEntity &&
				distance < m_NeighborhoodSquaredZone && distance>0) {

					forceVector += entity.getVelocity();
					++neighbours;
			}
		}

		if (neighbours > 0) {
			forceVector /= static_cast<float>(neighbours);
			forceVector = AdditionalMatch::getNormalizedVector(forceVector);
		}
		return forceVector;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::mouseCursorSeparate(Entity& currentEntity, sf::Vector2f mouseCursor) {

		sf::Vector2f forceVector(0, 0);

		if (currentEntity.getDistanceSquared(mouseCursor) < currentEntity.getSeparationSquaredZone()) {
			sf::Vector2f headingVector = currentEntity.getPosition() - mouseCursor;
			double scale = AdditionalMatch::getVectorLength(headingVector) /
				std::sqrt(currentEntity.getSeparationSquaredZone());
			headingVector = AdditionalMatch::getNormalizedVector(headingVector);
			headingVector = headingVector / static_cast<float>(scale);
			forceVector += headingVector;
		}

		return forceVector;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::mouseCursorCohere(Entity& currentEntity, sf::Vector2f mouseCursor) {

		sf::Vector2f forceVector(0, 0);
		sf::Vector2f centerOfMass(0, 0);

		if (currentEntity.getDistanceSquared(mouseCursor) < m_NeighborhoodSquaredZone) {
			centerOfMass = mouseCursor;
			forceVector = centerOfMass - currentEntity.getPosition();
			forceVector = AdditionalMatch::getNormalizedVector(forceVector);

		}

		return forceVector;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::obstaclesSeparate(Entity& currentEntity, const std::vector<FlockingObstacle>& obstacles) {

		sf::Vector2f forceVector(0, 0);
		int neighbours = 0;

		for (const FlockingObstacle& obstacle : obstacles) {
			float distance = obstacle.getDistanceSquared(currentEntity);
			if (distance < obstacle.getSeparationSquaredZone() && distance>0) {
				sf::Vector2f headingVector = currentEntity.getPosition() - obstacle.getPosition();
				double scale = AdditionalMatch::getVectorLength(headingVector) /
					std::sqrt(currentEntity.getSeparationSquaredZone());
				headingVector = AdditionalMatch::getNormalizedVector(headingVector);
				headingVector = headingVector / static_cast<float>(scale);
				forceVector += headingVector;
			}
		}

		if (neighbours > 0) {
			forceVector /= static_cast<float>(neighbours);
			forceVector = AdditionalMatch::getNormalizedVector(forceVector);
		}
		return forceVector;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::magnetsAttract(Entity& currentEntity, const std::vector<FlockingMagnet>& magnets) {

		sf::Vector2f forceVector(0, 0);
		sf::Vector2f centerOfMass(0, 0);
		int neighbours = 0;

		for (const FlockingMagnet& magnet : magnets) {
			float distance = magnet.getDistanceSquared(currentEntity);
			if (
				distance < m_NeighborhoodSquaredZone && distance>0) {
				centerOfMass += magnet.getPosition();
				++neighbours;
			}
		}

		if (neighbours > 0) {
			centerOfMass /= static_cast<float>(neighbours);
			forceVector = centerOfMass - currentEntity.getPosition();
			forceVector = AdditionalMatch::getNormalizedVector(forceVector);
		}

		return forceVector;
	}

	///-------------------------------------------------------------------------

	void Flock::mouseInteraction(sf::Vector2f& result, Entity& currentEntity, sf::Vector2f& mouseCursor) {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::LEFT_MAGNET_RIGHT_OBSTACLE)
				result += mouseCursorCohere(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorCohereWeight();
			else if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::LEFT_OBSTACLE_RIGHT_MAGNET)
				result += mouseCursorSeparate(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorSeparateWeight();
			else if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::ALL_MAGNET)
				result += mouseCursorCohere(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorCohereWeight();
			else if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::ALL_OBSTACLE)
				result += mouseCursorSeparate(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorSeparateWeight();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

			if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::LEFT_MAGNET_RIGHT_OBSTACLE)
				result += mouseCursorSeparate(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorSeparateWeight();
			else if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::LEFT_OBSTACLE_RIGHT_MAGNET)
				result += mouseCursorCohere(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorCohereWeight();
			else if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::ALL_MAGNET)
				result += mouseCursorCohere(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorCohereWeight();
			else if (m_flockingInfo.getMouseEventsSettings() == FlockingBehaviourParameters::MouseEventsSettings::ALL_OBSTACLE)
				result += mouseCursorSeparate(currentEntity, mouseCursor) * m_flockingInfo.getMouseCursorSeparateWeight();
		}
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::flocking(Entity& currentEntity, sf::Vector2f mouseCursor, const std::vector<FlockingObstacle>& obstacles, const std::vector<FlockingMagnet>& magnets) {

		sf::Vector2f result(0, 0);

		result += cohere(currentEntity) * m_flockingInfo.getCohereWeight();
		result += separate(currentEntity) * m_flockingInfo.getSeparateWeight();
		result += alignment(currentEntity) * m_flockingInfo.getAlignmentWeight();

		mouseInteraction(result, currentEntity, mouseCursor);

		if (!obstacles.empty()) {
			result += obstaclesSeparate(currentEntity, obstacles) * obstacles.begin()->getObstacleSeparateWeight();
		}

		if (!magnets.empty()) {
			result += magnetsAttract(currentEntity, magnets) * magnets.begin()->getMagnetAttractWeight();
		}

		return result;
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::flocking(Entity& currentEntity, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups) {

		sf::Vector2f result(0, 0);

		result += cohere(currentEntity) * m_flockingInfo.getCohereWeight();
		result += separate(currentEntity) * m_flockingInfo.getSeparateWeight();
		result += alignment(currentEntity) * m_flockingInfo.getAlignmentWeight();

		mouseInteraction(result, currentEntity, mouseCursor);

		if (!obstaclesGroups.empty()) {
			for (const std::vector<FlockingObstacle>& it : obstaclesGroups) {
				if(!it.empty()) result += obstaclesSeparate(currentEntity, it) * it.begin()->getObstacleSeparateWeight(); 
			}
		}

		if (!magnetsGroups.empty()) {
			for (const std::vector<FlockingMagnet>& it : magnetsGroups) {
				if (!it.empty()) result += magnetsAttract(currentEntity, it) * it.begin()->getMagnetAttractWeight();
			}
		}

		return result;
	}

	///-------------------------------------------------------------------------

	void Flock::removeProcess() {
		auto iterator = std::remove_if(m_entities.begin(), m_entities.end(),
			[](Entity& entity) {return entity.isToRemoved(); });
		if (iterator != m_entities.end()) {
			m_entities.erase(iterator, m_entities.end());
		}
	}

	///-------------------------------------------------------------------------

	std::vector<FlockingObstacle> Flock::convertAllEntitiesToFlockingObstacle() const {

		std::vector<FlockingObstacle> tempObstaclesList;
		std::transform(m_entities.begin(), m_entities.end(),
			std::back_insert_iterator<std::vector<FlockingObstacle>>(tempObstaclesList),
			std::mem_fn(&Entity::convertToFlockingObstacle));
		return tempObstaclesList;
	}

	///-------------------------------------------------------------------------

	std::vector<FlockingMagnet> Flock::convertAllEntitiesToFlockingMagnet() const {

		std::vector<FlockingMagnet> tempMagnetsList;
		std::transform(m_entities.begin(), m_entities.end(),
			std::back_insert_iterator<std::vector<FlockingMagnet>>(tempMagnetsList),
			std::mem_fn(&Entity::convertToFlockingMagnet));
		return tempMagnetsList;
	}

	///-------------------------------------------------------------------------

	void Flock::setObstacleBehavioursInfo(float obstacleSquareRootZone, float obstacleSeparateWeight) {
		for (Entity& it : m_entities)
			it.setObstacleBehavioursInfo(obstacleSquareRootZone, obstacleSeparateWeight);
	}

	///-------------------------------------------------------------------------

	void Flock::setMagnetBehavioursInfo(float magnetSquareRootZone, float magnetAttractWeight) {
		for (Entity& it : m_entities)
			it.setMagnetBehavioursInfo(magnetSquareRootZone, magnetAttractWeight);
	}

	///-------------------------------------------------------------------------

	sf::Vector2f Flock::getFlockPositionGeneralization() const {
		sf::Vector2f result;
		for (const Entity& it : m_entities) {
			result += it.getPosition(); 
		}
		return result/ static_cast<float>(m_entities.size()); 
	}

	///-------------------------------------------------------------------------

	const FlockingBehaviourParameters& Flock::getFlockingInfo() const {
		return m_flockingInfo;
	}

	///-------------------------------------------------------------------------

	void Flock::setFlockingInfo(const FlockingBehaviourParameters& flockingInfo) {
		m_flockingInfo = flockingInfo; 
	}

	///-------------------------------------------------------------------------

	void Flock::addNewEntity(const Entity& entity) {
		m_entities.push_back(entity);
	}

	///-------------------------------------------------------------------------

	void Flock::removeEntity(int i) {
		m_entities[i].remove(); 
	}

	///-------------------------------------------------------------------------

	const Entity& Flock::getEntityRef(int i) {
		return m_entities[i];
	}

	///-------------------------------------------------------------------------

	const Entity& Flock::getEntityConstRef(int i) const {
		return m_entities[i];
	}

	///-------------------------------------------------------------------------

	int Flock::getNumberOfEntities() const {
		return m_entities.size(); 
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingObstacle>& obstacles, std::vector<FlockingMagnet>& magnets) {

		if (m_running) {

			for (Entity& it : m_entities) {
				it.update(m_speed, elapsedTime, m_displayAreaSize);
				it += flocking(it, mouseCursor, obstacles, magnets);
			}
			removeProcess();
		}
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor) {
		Flock::update(elapsedTime, mouseCursor, std::vector<FlockingObstacle>{}, std::vector<FlockingMagnet>{});
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime) {
		Flock::update(elapsedTime, sf::Vector2f(0,0), std::vector<FlockingObstacle>{}, std::vector<FlockingMagnet>{});
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingObstacle>& obstacles) {
		Flock::update(elapsedTime, mouseCursor, obstacles, std::vector<FlockingMagnet>{});
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingMagnet>& magnets) {
		Flock::update(elapsedTime, mouseCursor, std::vector<FlockingObstacle>{}, magnets);
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups) {

		if (m_running) {

			for (Entity& it : m_entities) {
				it += flocking(it, mouseCursor, obstaclesGroups, magnetsGroups);
				it.update(m_speed, elapsedTime, m_displayAreaSize);
			}
			removeProcess();
		}
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups) {
		Flock::update(elapsedTime, mouseCursor, obstaclesGroups, std::vector<std::vector<FlockingMagnet>>{});
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups) {
		Flock::update(elapsedTime, mouseCursor, std::vector<std::vector<FlockingObstacle>>{}, magnetsGroups);
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups, std::vector<FlockingMagnet>& magnets) {
		Flock::update(elapsedTime, mouseCursor, obstaclesGroups, std::vector<std::vector<FlockingMagnet>>{magnets});
	}

	///-------------------------------------------------------------------------

	void Flock::update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingObstacle>& obstacles, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups) {
		Flock::update(elapsedTime, mouseCursor, std::vector<std::vector<FlockingObstacle>>{obstacles}, magnetsGroups);
	}

	///-------------------------------------------------------------------------

	void Flock::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (const Entity& entity : m_entities) 
			target.draw(entity);
	}

}