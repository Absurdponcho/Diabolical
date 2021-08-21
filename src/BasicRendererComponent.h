#pragma once
#include "Rendering/GameRendererComponent.h"
#include "WindowManager.h"
class BasicRendererComponent : public GameRendererComponent
{
public:
	BasicRendererComponent();
	virtual void Render() override;
};

