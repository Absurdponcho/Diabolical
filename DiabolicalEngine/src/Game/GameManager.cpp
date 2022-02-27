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
#include "ECS/flecs.h"
#include "GUI/GUI.h"
#include "Input/Input.h"
#include "Maths/Maths.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/MeshPrimitives.h"
#include "Graphics/Rendering/Material.h"
#include "Graphics/Rendering/MaterialInstance.h"
#include "FileSystem/Filesystem.h"

class DMeshRenderer
{
public:
	DMeshRenderer(){};

	DMeshRenderer(std::shared_ptr<DMesh> NewMesh, std::shared_ptr<DMaterial> NewMaterial)
		: Mesh(NewMesh), MaterialInstance(std::make_unique<DMaterialInstance>(NewMaterial)) {}

	std::shared_ptr<DMesh> Mesh;
	std::unique_ptr<DMaterialInstance> MaterialInstance;
};

std::shared_ptr<DMaterial> TestMaterial;

void DGameManager::RenderingTest()
{
	DString VertexShader;
	DString FragmentShader;

	VertexShader = DAssetManager::Get().SynchronousLoadAsset("Assets/simple.vert")->AsString();
	FragmentShader = DAssetManager::Get().SynchronousLoadAsset("Assets/simple.frag")->AsString();

	TestMaterial = std::make_shared<DMaterial>();
	TestMaterial->BuildShader(VertexShader, FragmentShader);

	auto TestRenderEntity = ECSWorld.entity()
	.set<DMeshRenderer>({ MeshPrimitives::Cube, TestMaterial })
	.add<Transform3D>();

	TestRenderEntity.get<DMeshRenderer>()->MaterialInstance->SetUniform("Color", Vector3(0, 1, 0));

	ECSWorld.system<DMeshRenderer, Transform3D>("Render")
		.kind(flecs::OnStore)
		.each([](const flecs::entity& ent, DMeshRenderer& Renderer, Transform3D& Transform)
	{
		DMesh* Mesh = Renderer.Mesh.get();
		if (!Mesh) return;

		DMaterialInstance* MaterialInstance = Renderer.MaterialInstance.get();
		if (!MaterialInstance) return;

		MaterialInstance->Bind();
		Mesh->Draw();

	});
}

void DGameManager::Exit()
{
	bMainLoopRunning = false;
}

std::shared_ptr<DAudioSource> AudioSource = std::make_shared<DAudioSource>();

void DGameManager::MainGameLoop()
{
	DWAVFile::LoadAsync("Assets/Sussy Baka.wav", [&](std::shared_ptr<DWAVFile> NewWav)
	{
		Check(NewWav.get());
		AudioSource->SetAudioFile(NewWav);
		AudioSource->Play();
	}); // Hey dude, you're being quite sussy

	RenderingTest();

	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
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
		GameTime += ECSWorld.delta_time();
	}
}

void DGameManager::GameTick()
{
	ECSWorld.progress();
}

float DGameManager::GetDeltaTime()
{
	return ECSWorld.delta_time();
}

float DGameManager::GetGameFPS()
{
	return 1/ECSWorld.delta_time();
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
