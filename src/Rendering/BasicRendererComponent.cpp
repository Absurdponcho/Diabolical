#include "BasicRendererComponent.h"
#include "../Utility/Utility.h"
BasicRendererComponent::BasicRendererComponent() {}

void BasicRendererComponent::Render(CameraComponent& Camera)
{
	Utility::DrawDebugLine(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
}