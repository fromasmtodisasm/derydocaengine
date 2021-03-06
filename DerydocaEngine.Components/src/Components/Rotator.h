#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Components
{

	class Rotator : public GameComponent, SelfRegister<Rotator>
	{
	public:
		GENINSTANCE(Rotator);
		Rotator();
		Rotator(float rotationSpeed);
		~Rotator();

		void update(const float deltaTime);

		void deserialize(const YAML::Node& node);

		inline void setSpeed(float const& rotSpeed) { m_rotSpeed = rotSpeed; }
		inline float& getSpeed() { return m_rotSpeed; }
		inline float getSpeed() const { return m_rotSpeed; }
	private:
		float m_rotVal;
		float m_rotSpeed;
	};

}
