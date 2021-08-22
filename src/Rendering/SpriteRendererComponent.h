#pragma once
#include "GameRendererComponent.h"
#include <string>

class SpriteRendererComponent : public GameRendererComponent
{
public:
	virtual void Render(CameraComponent& Camera) override;
};