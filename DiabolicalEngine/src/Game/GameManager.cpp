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
	TestCamera.GetComponentMutable<Transform3D>()->Name = "Camera";

	DCameraComponent::SetActiveCamera(TestCamera);

	DEntity TestParent = Cube.Clone("TestParent");
	TestParent.GetComponentMutable<Transform3D>()->SetPosition({ -2.0f, 0, 0 });
	TestParent.GetComponentMutable<Transform3D>()->Name = "TestParent";
	TestParent.SetParent(DScene::SceneRoot);

	DEntity TestChild = Cube.Clone("TestChild");
	TestChild.GetComponentMutable<Transform3D>()->SetPosition({ 3.0f, 0, 0 });
	TestChild.GetComponentMutable<Transform3D>()->SetEulerRotation({0, 45, 0});
	TestChild.GetComponentMutable<Transform3D>()->Name = "TestChild";
	TestChild.SetParent(TestParent);

	DEntity Child1 = Cube.Clone();
	Transform3D* Child1Transform = Child1.GetComponentMutable<Transform3D>();
	Child1Transform->Name = "Child1";
	Child1.SetParent(TestChild);
	Check(Child1.GetParent() == TestChild);
	Check(TestChild.HasChild(Child1));

	DEntity Child2 = Cube.Clone();
	Transform3D* Child2Transform = Child2.GetComponentMutable<Transform3D>();
	Child2Transform->Name = "Child2";
	Child2.SetParent(Child1);
	Check(Child2.GetParent() == Child1);
	Check(Child1.HasChild(Child2));

	Child2.GetFlecsEntity().disable();

	Check (Child1.GetComponentMutable<Transform3D>() != Child2.GetComponentMutable<Transform3D>());

	DEntity Child3 = Cube.Clone();
	Transform3D* Child3Transform = Child3.GetComponentMutable<Transform3D>();
	Child3Transform->Name = "Child3";
	Child3.SetParent(Child2);
	Check(Child3.GetParent() == Child2);
	Check(Child2.HasChild(Child3));

}

void DGameManager::Exit()
{
	bMainLoopRunning = false;
}

DSharedPtr<DAudioSource> AudioSource = std::make_shared<DAudioSource>();

void DGameManager::MainGameLoop()
{
	TestReflectedClass TestClass = TestReflectedClass();
	


	DWAVFile::LoadAsync("Assets/Sussy Baka.wav", [](DSharedPtr<DWAVFile> NewWav){
		Check(NewWav.Get());
		AudioSource->SetAudioFile(NewWav);
		AudioSource->Play();
	}); // Hey dude, you're being quite sussy

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
