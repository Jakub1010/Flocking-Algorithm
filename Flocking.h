#ifndef FLOCKING_H_
#define FLOCKING_H_

#include <SFML\Graphics.hpp>
#include <list>
#include <cmath>
#include <list>
#include <memory>
#include "EntityShape.h" 
#include "Chronometer.h"

namespace FlockingAlgorithm {

	class FlockingObstacle;
	class FlockingMagnet;
	class Entity;
	class Flock;

	//--------------------------AdditionalMath-----------------------------------------

	namespace AdditionalMatch {
		sf::Vector2f getNormalizedVector(const sf::Vector2f& vec2);
		long double getVectorLength(const sf::Vector2f& vec2);
		int getRandomIntigerValue(int min, int max);
		float getRandomFloatValue(float min, float max);
	}

	//----------------------------------AdditionalInfo---------------------------------

	typedef void(*EventStaffObstacleSignature)(FlockingObstacle& signal, Entity& slot);
	typedef void(*EventStaffMagnetSignature)(FlockingMagnet& signal, Entity& slot);
	static enum SpecialFlockingObjectsType { MOUSE_CURSOR };

	//--------------------------------FlockingObstacle---------------------------------

	class FlockingObstacle {

	private:
		long double m_ObstacleSquaredZone; 
		sf::Vector2f m_position; 
		EventStaffObstacleSignature m_eventStaff;
		float m_ObstacleSeparateWeight;

	public:
		FlockingObstacle(float obstacleSquareRootZone=0,
			float obstacleSeparateWeight=0, sf::Vector2f position=sf::Vector2f(0,0));
		FlockingObstacle(const FlockingObstacle& flockingObstacle); 
		FlockingObstacle(const FlockingObstacle& flockingObstacle, sf::Vector2f position);
		void operator=(const FlockingObstacle& flockingObstacle);

	public:
		sf::Vector2f getPosition() const;
		long double getSeparationSquaredZone() const;
		float getSeparationSquareRootZone() const;
		long double getDistanceSquared(Entity& flockingEntity) const; 
		long double getSimpleDistanceSquared(Entity& flockingEntity) const;

		float getObstacleSeparateWeight() const; 

	public:
		void setEventStaff(EventStaffObstacleSignature eventStaff);
		bool isEventStaffSet() const; 
	};

	//--------------------------------FlockingMagnesium--------------------------------

	class FlockingMagnet {

	private:
		long double m_MagnetSquaredZone; 
		sf::Vector2f m_position;
		EventStaffMagnetSignature m_eventStaff;
		float m_MagnetAttractWeight;

	public:
		FlockingMagnet(float magnetSquareRootZone=0,
			float magnetAttractWeight=0, sf::Vector2f position=sf::Vector2f(0,0));
		FlockingMagnet(const FlockingMagnet& flockingMagnet);
		FlockingMagnet(const FlockingMagnet& flockingMagnet, sf::Vector2f position);
		void operator=(const FlockingMagnet& flockingMagnet);

	public:
		sf::Vector2f getPosition() const;
		long double getAttractSquaredZone() const;
		float getAttractSquareRootZone() const;
		long double getDistanceSquared(Entity& flockingEntity) const; 
		long double getSimpleDistanceSquared(Entity& flockingEntity) const;
		float getMagnetAttractWeight() const;

	public:
		void setEventStaff(EventStaffMagnetSignature eventStaff);
		bool isEventStaffSet() const;
	};

	//----------------------------------Entity-----------------------------------------

	class Entity : public sf::Drawable {

	private:
		friend class Flock;

	private:
		bool m_toRemove;

	public:
		void remove();
		bool isToRemoved() const; 

	private:
		std::unique_ptr<EntityShape> m_entityShape;
		sf::Vector2f m_velocity;
	
		FlockingObstacle m_obstacleBehavioursInfo;
		FlockingMagnet m_magnetBehavioursInfo;

		long double m_SeparationCubedZone;
		int m_ID;
		EventStaffObstacleSignature m_eventStaff;

	public:
		Entity(std::unique_ptr<EntityShape> entityShape, float angle, float separationSquareRootZone, int ID, sf::Vector2f position);
		Entity(const Entity& entity); 
		Entity(Entity&& entity);
		const Entity& operator=(const Entity& entity);

		long double getSeparationSquaredZone() const;
		float getSeparationSquareRootZone() const;
		bool operator!=(const Entity& otherEntity) const;
		long double getDistanceSquared(sf::Vector2f position) const; 
		sf::Vector2f getVelocity() const;
		sf::Vector2f getPosition() const;

		std::unique_ptr<EntityShape>& getDrawableObjectPointer();
		auto& getDrawableObjectRef();
		const auto& getDrawableObjectConstRef() const;

		void operator+=(sf::Vector2f velocity);
		void operator=(sf::Vector2f velocity);
		void update(float speed, float elapedTime, sf::FloatRect displayAreaSize);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void setObstacleBehavioursInfo(float obstacleSquareRootZone = 0, float obstacleSeparateWeight = 0);
		void setMagnetBehavioursInfo(float magnetSquareRootZone = 0, float magnetAttractWeight = 0);
		const FlockingObstacle& getObstacleBehavioursInfo() const; 
		const FlockingMagnet& getMagnetBehavioursInfo() const;
		FlockingObstacle convertToFlockingObstacle() const;
		FlockingMagnet convertToFlockingMagnet() const;
	};

	//----------------------------------FlockInfo--------------------------------------

	class FlockingBehaviourParameters {
	public:
		enum class MouseEventsSettings { LEFT_MAGNET_RIGHT_OBSTACLE=1, LEFT_OBSTACLE_RIGHT_MAGNET=2, ALL_OBSTACLE=3, ALL_MAGNET=4, NONE=5};

	private:
		float m_CohereWeight;
		float m_SeparateWeight;
		float m_AlignmentWeight;
		float m_MouseCursorSeparateWeight;
		float m_MouseCursorCohereWeight;
		MouseEventsSettings m_mouseEventsSettings; 

	public:

		FlockingBehaviourParameters(
			float cohereWeight=1.f,
			float separateWeight=6.f,
			float alignmentWeight=1.f,
			float mouseCursorSeparateWeight=20.f,
			float mouseCursorCohereWeight=3.f,
			MouseEventsSettings mouseEventsSettings = MouseEventsSettings::LEFT_OBSTACLE_RIGHT_MAGNET
		);

	public:

		void setCohereWeight(float data);
		void setSeparateWeight(float data);
		void setAlignmentWeight(float data);
		void setMouseCursorSeparateWeight(float data);
		void setMouseCursorCohereWeight(float data);
		void setMouseEventsSettings(MouseEventsSettings mouseEventsSettings);
		
		float getCohereWeight() const;
		float getSeparateWeight() const;
		float getAlignmentWeight() const;
		float getMouseCursorSeparateWeight() const;
		float getMouseCursorCohereWeight() const; 
		MouseEventsSettings getMouseEventsSettings() const; 
	};

	//----------------------------------Flock------------------------------------------

	class Flock : public sf::Drawable {

	private:
		bool m_running; 

	public:
		bool isRunning() const;
		void start();
		void stop(); 
		bool ckeckID();

	private:
		sf::FloatRect m_displayAreaSize;

		const float m_speed;
		const long double m_NeighborhoodSquaredZone;

		FlockingBehaviourParameters m_flockingInfo; 

		std::vector<Entity> m_entities;

		sf::Vector2f cohere(Entity& currentEntity);
		sf::Vector2f separate(Entity& currentEntity);
		sf::Vector2f alignment(Entity& currentEntity);

		sf::Vector2f mouseCursorSeparate(Entity& currentEntity, sf::Vector2f mouseCursor);
		sf::Vector2f mouseCursorCohere(Entity& currentEntity, sf::Vector2f mouseCursor);

		sf::Vector2f obstaclesSeparate(Entity& currentEntity, const std::vector<FlockingObstacle>& obstacles);
		sf::Vector2f magnetsAttract(Entity& currentEntity, const std::vector<FlockingMagnet>& magnets);

		void mouseInteraction(sf::Vector2f& result, Entity& currentEntity, sf::Vector2f& mouseCursor);
		sf::Vector2f flocking(Entity& currentEntity, sf::Vector2f mouseCursor, const std::vector<FlockingObstacle>& obstacles, const std::vector<FlockingMagnet>& magnets);
		sf::Vector2f flocking(Entity& currentEntity, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups);

		void removeProcess();

	public:
		std::vector<FlockingObstacle> convertAllEntitiesToFlockingObstacle() const;
		std::vector<FlockingMagnet> convertAllEntitiesToFlockingMagnet() const;

		void setObstacleBehavioursInfo(float obstacleSquareRootZone = 0, float obstacleSeparateWeight = 0);
		void setMagnetBehavioursInfo(float magnetSquareRootZone = 0, float magnetAttractWeight = 0);

		sf::Vector2f getFlockPositionGeneralization() const; 
		const FlockingBehaviourParameters& getFlockingInfo() const;
		void setFlockingInfo(const FlockingBehaviourParameters& flockingInfo);

		std::vector<Entity>& getListOfEntities();
		const std::vector<Entity>& getListOfEntities() const;

		void addNewEntity(const Entity& entity); 
		void removeEntity(int i);
		const Entity& getEntityRef(int i);
		const Entity& getEntityConstRef(int i) const;

		int getNumberOfEntities() const;

	public:
		Flock(std::vector<Entity>&& entities, float speed = 0, float neighborhoodSquareRootZone = 0, sf::FloatRect displayAreaSize = {}, const FlockingBehaviourParameters& flockingInfo = {});
		Flock(const Flock& flock) = delete; 

		void update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingObstacle>& obstacles, std::vector<FlockingMagnet>& magnets);
		void update(float elapsedTime, sf::Vector2f mouseCursor);
		void update(float elapsedTime);
		void update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingObstacle>& obstacles);
		void update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingMagnet>& magnets);
		void update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups);
		void update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups);
		void update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups);
		void update(float elapsedTime, sf::Vector2f mouseCursor, const std::vector<std::vector<FlockingObstacle>>& obstaclesGroups, std::vector<FlockingMagnet>& magnets);
		void update(float elapsedTime, sf::Vector2f mouseCursor, std::vector<FlockingObstacle>& obstacles, const std::vector<std::vector<FlockingMagnet>>& magnetsGroups);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}

#endif