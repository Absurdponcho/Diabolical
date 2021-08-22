#pragma once
#include "GameRendererComponent.h"
#include "../WindowManager.h"
class BasicRendererComponent : public GameRendererComponent
{
public:
	BasicRendererComponent();
	virtual void Render(CameraComponent& Camera) override;
};

