#pragma once
#include "GameRendererComponent.h"
#include <string>

class SquareRendererComponent : public GameRendererComponent
{
public:
	virtual void Render(CameraComponent& Camera) override;
};