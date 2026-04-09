#pragma once
#include "../ecs/System.h"
#include <vector>

class ImmunitySystem : public ecs::System
{
public:
	ImmunitySystem();
	virtual ~ImmunitySystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;
private:
	bool _active;
	float _lastTime;

	const float IMMUNITY_TIME = 10.0f;
};

