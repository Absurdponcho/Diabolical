#include "Input.h"

void HandleSDLDown(SDL_Event& Event)
{
	if (ImGui::GetIO().WantTextInput)
	{
		ImGui::GetIO().KeysDown[Event.key.keysym.scancode] = true;

		if (Event.key.keysym.scancode == SDL_SCANCODE_LSHIFT ||
			Event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)
		{
			ImGui::GetIO().KeyShift = true;
		}

		if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL ||
			Event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
		{
			ImGui::GetIO().KeyCtrl = true;
		}

		if (Event.key.keysym.scancode == SDL_SCANCODE_LALT ||
			Event.key.keysym.scancode == SDL_SCANCODE_RALT)
		{
			ImGui::GetIO().KeyAlt = true;
		}
	}
	else
	{
		if (Event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
		{
			if (DEngine::DevConsole.get())
			{
				DEngine::DevConsole->Toggle();
			}
		}
	}
}

void HandleSDLUp(SDL_Event& Event)
{
	ImGui::GetIO().KeysDown[Event.key.keysym.scancode] = false;

	if (Event.key.keysym.scancode == SDL_SCANCODE_LSHIFT ||
		Event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)
	{
		ImGui::GetIO().KeyShift = false;
	}

	if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL ||
		Event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
	{
		ImGui::GetIO().KeyCtrl = false;
	}

	if (Event.key.keysym.scancode == SDL_SCANCODE_LALT ||
		Event.key.keysym.scancode == SDL_SCANCODE_RALT)
	{
		ImGui::GetIO().KeyAlt = false;
	}
}

void EventTick()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {

		case SDL_KEYDOWN:
		HandleSDLDown(Event);
		break;
		case SDL_KEYUP:
		HandleSDLUp(Event);
		break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		break;
		case SDL_MOUSEWHEEL:
		break;
		case SDL_QUIT:
		DEngine::GameManager->Exit();
		break;
		case SDL_WINDOWEVENT: // screen resize thing
		if (Event.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
		DWindowManager::Get().RefreshViewport();
		break;
		case SDL_TEXTINPUT:
		if (ImGui::GetIO().WantTextInput)
		{
			ImGui::GetIO().AddInputCharactersUTF8(Event.text.text);
		}
		default:
		break;
		}
	}
}