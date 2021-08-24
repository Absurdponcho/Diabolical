#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING


#include "Utility.h"
#include <box2d/b2_math.h>
#include <sstream>
#include <SDL2/SDL.h>
#include "../WindowManager.h"
#include "../Rendering/Camera.h"
#include <glm/glm.hpp>
#include <locale>
#include <codecvt>
#include <string>



std::string Utility::ToString(const b2Vec2& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " )";
	return StringStream.str();
}

std::string Utility::ToString(const glm::vec4& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " , " << Vector.z << " , " << Vector.w << " )";
	return StringStream.str();
}

std::string Utility::ToString(const glm::vec3& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " , " << Vector.z << " )";
	return StringStream.str();
}

void Utility::DrawDebugLine(const glm::vec3& From, const glm::vec3& To)
{
	SDL_SetRenderDrawColor(WindowManager::GetSDLRenderer(), 255, 0, 0, 255);

	CameraComponent* Camera = CameraComponent::GetActiveCamera();
	if (!Camera) return;

	glm::vec4 From4 = glm::vec4(From, 1);
	glm::vec4 To4 = glm::vec4(To, 1);

	glm::mat4x4 ViewMatrix = Camera->GetViewMatrix();
	glm::mat4x4 ProjectionMatrix = Camera->GetProjectionMatrix();
	glm::mat4x4 VPMatrix = ProjectionMatrix * ViewMatrix;

	From4 = VPMatrix * From4;
	To4 = VPMatrix * To4;

	From4 = glm::vec4(WindowManager::Get().ScreenSpaceToPixelCoord(From4), 0);
	To4 = glm::vec4(WindowManager::Get().ScreenSpaceToPixelCoord(To4), 0);

	SDL_RenderDrawLineF(WindowManager::GetSDLRenderer(), From4.x, From4.y, To4.x, To4.y);
}

std::wstring Utility::StringToWString(const std::string& String)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return (converter.from_bytes(String));
}
std::string Utility::WStringToString(const std::wstring& String)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return (converter.to_bytes(String));
}