#ifndef ENTITY_SHAPE_H_
#define ENTITY_SHAPE_H_

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <memory>
#include <iostream>
#include <map>

///----------------------------------------------------------------------------------------

class EntityShape : public sf::Drawable {
public:
	virtual void setRotationBasedVelocity(sf::Vector2f velocity) = 0;
	virtual sf::Vector2f getPosition() const = 0;
	virtual void setPosition(sf::Vector2f position) = 0;
	virtual void setPosition(float x, float y) = 0;
	virtual void move(sf::Vector2f vec) = 0;
	virtual void setRotation(float angle) = 0;
	virtual void setOrigin(sf::Vector2f vec) = 0;
	virtual void setTexture(const std::string& textureLocation) = 0;
	virtual sf::Vector2f getSize() const = 0;
	virtual std::unique_ptr<EntityShape> copy() const = 0; 
};

///----------------------------------------------------------------------------------------

struct FishTextures {
	std::string m_name; 
	sf::Texture m_texture; 
};

class ClassicFishShape : public EntityShape {

private:
	sf::RectangleShape m_fish;
	static std::map<std::string, FishTextures> m_loadedTextures;
	std::string m_textureLocation;

public:
	ClassicFishShape(sf::Vector2f size, const std::string& textureLocation) {

		auto iterator = m_loadedTextures.find(textureLocation);
		if (iterator == m_loadedTextures.end()) {
			sf::Texture tempTexture;
			if (!tempTexture.loadFromFile(textureLocation)) {
				std::cout << "Error\n";
			}
			m_loadedTextures[textureLocation] = FishTextures{ textureLocation, tempTexture };
		}

		m_fish.setTexture(&(m_loadedTextures[textureLocation]).m_texture);
		m_textureLocation = textureLocation;
		m_fish.setSize(size);
		m_fish.setOrigin(size / 2.f);

	}

	ClassicFishShape(const ClassicFishShape& obj)
		: ClassicFishShape(obj.m_fish.getSize(), obj.m_textureLocation) {
		m_fish = obj.m_fish;

	}

	virtual void setRotationBasedVelocity(sf::Vector2f velocity) override {
		m_fish.setRotation(atan2(velocity.y, velocity.x)*(180.f / 3.14159265359f) + 90.f);
	}

	virtual sf::Vector2f getPosition() const override {
		return m_fish.getPosition();
	}

	virtual void setPosition(sf::Vector2f position) override {
		m_fish.setPosition(position);
	}

	virtual void setPosition(float x, float y) override {
		m_fish.setPosition(x, y);
	}

	virtual void move(sf::Vector2f vec) override {
		m_fish.move(vec);
	}

	virtual void setRotation(float angle) override {
		m_fish.setRotation(angle);
	}

	virtual void setOrigin(sf::Vector2f vec) override {
		m_fish.setOrigin(vec);
	}

	virtual void setTexture(const std::string& textureLocation) override {
		auto iterator = m_loadedTextures.find(textureLocation);
		if (iterator == m_loadedTextures.end()) {
			sf::Texture tempTexture;
			tempTexture.loadFromFile(textureLocation);
			m_loadedTextures[textureLocation] = FishTextures{ textureLocation, tempTexture };
		}
		m_fish.setTexture(&(m_loadedTextures[textureLocation]).m_texture);
		m_textureLocation = textureLocation;
	}

	virtual sf::Vector2f getSize() const override {
		return m_fish.getSize(); 
	}

public:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(m_fish);
	}

	virtual std::unique_ptr<EntityShape> copy() const {
		return std::make_unique<ClassicFishShape>(*this);
	}
};

#endif 