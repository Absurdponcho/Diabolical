#include "GameManager.h"
#include <chrono>
#include <SDL2/SDL.h>
#include "Graphics/WindowManager.h"
#include <gl/glew.h>
#include <gl/GL.h>
#include "Audio/AudioSource.h"
#include "Audio/WAVFile.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl.h"
#include "GUI/IWindow.h"
#include "GUI/TestWindow.h"
#include "Thread/GameThread.h"
#include "Thread/Thread.h"
#include "AssetManager/AssetManager.h"
#include "ECS/ECS.h"
#include "GUI/GUI.h"
#include "Input/Input.h"
#include "Maths/Maths.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/MeshPrimitives.h"
#include "Graphics/Rendering/Material.h"
#include "Graphics/Rendering/MaterialInstance.h"
#include "FileSystem/Filesystem.h"
#include "Graphics/Rendering/Camera.h"
#include "Utility/Random.h"
#include "Graphics/Rendering/MeshRendererComponent.h"
#include "ECS/ECS.h"
#include "Graphics/Rendering/Scene.h"

void DGameManager::RenderingTest()
{
	DEntity Cube = DEntity::CreatePrefab("RendererPrefab");
	Cube.Set(DMeshRendererComponent({ MeshPrimitives::Cube, nullptr }));
	Cube.Set(Transform3D());

	DEntity TestCamera = DEntity::CreateEntity("Camera");
	TestCamera.SetParent(DScene::SceneRoot);
	TestCamera.Set(DCameraComponent());
	TestCamera.Set(Transform3D({ Vector3(5,5,5), Vector3(1,1,1), Vector3(45,-45,0) }));

	DCameraComponent::SetActiveCamera(TestCamera);


	DEntity TestParent = Cube.Clone();
	TestParent.GetComponentMutable<Transform3D>()->SetPosition({ -2.0f, 0, 0 });


	DEntity TestChild = Cube.Clone();
	TestChild.GetComponentMutable<Transform3D>()->SetPosition({ 3.0f, 0, 0 });
	TestChild.GetComponentMutable<Transform3D>()->SetEulerRotation({0, 45, 0});


}

void DGameManager::Exit()
{
	bMainLoopRunning = false;
}

DSharedPtr<DAudioSource> AudioSource = std::make_shared<DAudioSource>();

void Test(DSharedPtr<DWAVFile> NewWav)
{
	Check(NewWav.Get());
	AudioSource->SetAudioFile(NewWav);
	AudioSource->Play();
}

void DGameManager::MainGameLoop()
{
	DWAVFile::LoadAsync("Assets/Sussy Baka.wav", &Test); // Hey dude, you're being quite sussy

	RenderingTest();

	glClearColor(0.f, 0.f, 0.f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	EventTick();
	SDL_ShowWindow(DWindowManager::GetSDLWindow());

	while (bMainLoopRunning)
	{
		EventTick();
		GameTick();
		ImGuiTick();
		SDL_GL_SwapWindow(DWindowManager::GetSDLWindow());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DThread::CheckManagedThreads();
		DGameThread::RunInvokedFunctions();

		Frame++;
		GameTime += DUtilityECS::GetECSWorld().delta_time();
	}
}

void DGameManager::GameTick()
{
	DUtilityECS::GetECSWorld().progress();
}

float DGameManager::GetDeltaTime()
{
	return DUtilityECS::GetECSWorld().delta_time();
}

float DGameManager::GetGameFPS()
{
	return 1/DUtilityECS::GetECSWorld().delta_time();
}

float DGameManager::GetGameTime()
{
	return GameTime;
}

int DGameManager::GetFrame()
{
	return Frame;
}

DGameManager& DGameManager::Get()
{
	Check (DEngine::GameManager);
	return *DEngine::GameManager;
}
