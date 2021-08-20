#pragma once
#include "GameRendererComponent.h"
#include <string>

class TextRendererComponent : public GameRendererComponent
{
public:
	virtual void Render() override;

	std::string Text;
};