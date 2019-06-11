#include <SFML/Graphics.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <memory>
#include "FlockingObjects.h"
#include "Chronometer.h"
#include "EntityShape.h"
#include "Flocking.h"

int main() {

	//--------------------------------------------------------------------------------

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Flocking Algorithm", sf::Style::Close, settings);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60.f);

	//--------------------------------------------------------------------------------

	sf::Vector2f windowSize = window.mapPixelToCoords(sf::Vector2i(window.getSize().x, window.getSize().y));
	sf::FloatRect displayAreaSize(sf::Vector2f(0, 0), windowSize);

	int numberOfEntitiesInFlock = 85;
	sf::Vector2f entityGenerationPoint = sf::Vector2f(
		displayAreaSize.left + displayAreaSize.width,
		displayAreaSize.top + displayAreaSize.height) / 2.f;
	float entitySeparationDistance = 50.f;
	float entitiesSpeed = 250.f;
	float entitiesNeighborhoodZone = 450.f;
	float timeToStartFlocking = 1.f;

	FlockingAlgorithm::FlockingBehaviourParameters flockingInfo(4.f, 10.f, 4.f, 30.f, 30.f);
	float obstacleSquareRootZone = 200.f;
	float obstacleSeparateWeight = 45.f;

	std::vector<FlockingAlgorithm::Entity> tempEntitiesList;

	//--------------------------------------------------------------------------------


	for (int i = 0; i < numberOfEntitiesInFlock; ++i) {
		tempEntitiesList.emplace_back(std::make_unique<ClassicFishShape>(
			ClassicFishShape(sf::Vector2f(30.f, 45.f), std::string("fish.png"))), FlockingAlgorithm::AdditionalMatch::getRandomFloatValue(1.f, 360.f),
			entitySeparationDistance, i, entityGenerationPoint);
		tempEntitiesList.back().setObstacleBehavioursInfo(obstacleSquareRootZone, obstacleSeparateWeight);
		tempEntitiesList.back().setMagnetBehavioursInfo(100.f, 30.f);
	}

	auto flock = std::make_unique<FlockingAlgorithm::Flock>(std::move(tempEntitiesList), entitiesSpeed, entitiesNeighborhoodZone, displayAreaSize, flockingInfo);

	//--------------------------------------------------------------------------------

	std::vector<ObstacleBall> obstaclesBallList;
	std::vector<MagnetBall> magnetsBallList;

	//--------------------------------------------------------------------------------

	sftools::Chronometer clock;
	float deltaTime = 1 / 60.f;

	clock.resume();
	flock->stop();
	bool isPause = true;
	bool hideElements = false;

	//--------------------------------------------------------------------------------

	sf::Vector2f mouseCursorPosition = sf::Vector2f(0, 0);

	while (window.isOpen()) {

		mouseCursorPosition = window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));

		float frameStart = clock.getElapsedTime().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift) {
				obstaclesBallList.emplace_back(40.f, 15, sf::Color::Red, 100.f, mouseCursorPosition, 300.f, 20.f,
					sf::Mouse::isButtonPressed(sf::Mouse::Left) ? false : true);
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::RShift) {
				magnetsBallList.emplace_back(40.f, 15, sf::Color::Cyan, 100.f, mouseCursorPosition, 300.f, 20.f,
					sf::Mouse::isButtonPressed(sf::Mouse::Left) ? false : true);
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P) {
				if (isPause) {
					clock.resume();
					flock->start();
					isPause = false;
				}
				else {
					clock.pause();
					flock->stop();
					isPause = true;
				}
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::H) {
				hideElements = !hideElements;
			}
		}

		//--------------------------------------------------------------------------------

		std::vector<FlockingAlgorithm::FlockingObstacle> tempObstaclesList;

		std::transform(obstaclesBallList.begin(), obstaclesBallList.end(),
			std::back_insert_iterator<std::vector<FlockingAlgorithm::FlockingObstacle>>(tempObstaclesList),
			std::mem_fn(&ObstacleBall::convertToFlockingObstacle));

		std::vector<FlockingAlgorithm::FlockingMagnet> tempMagnetsList;

		std::transform(magnetsBallList.begin(), magnetsBallList.end(),
			std::back_insert_iterator<std::vector<FlockingAlgorithm::FlockingMagnet>>(tempMagnetsList),
			std::mem_fn(&MagnetBall::convertToFlockingMagnet));

		//--------------------------------------------------------------------------------

		flock->update(deltaTime, mouseCursorPosition, tempObstaclesList, tempMagnetsList);

		if (!isPause) {
			for (auto& it : magnetsBallList) {
				it.update(displayAreaSize, deltaTime);
			}for (auto& it : obstaclesBallList) {
				it.update(displayAreaSize, deltaTime);
			}
		}

		window.clear(sf::Color(255, 255, 255));
		window.draw(*flock);

		if (!hideElements) {
			for (const ObstacleBall& it : obstaclesBallList)
				window.draw(it);
			for (const MagnetBall& it : magnetsBallList)
				window.draw(it);
		}

		window.display();

		deltaTime = clock.getElapsedTime().asSeconds() - frameStart;
	}

}