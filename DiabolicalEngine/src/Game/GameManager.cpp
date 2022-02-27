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
#include "Graphics/Rendering/Camera.h"

class DMeshRendererComponent
{
public:
	DMeshRendererComponent(){};

	DMeshRendererComponent(DSharedPtr<DMesh> NewMesh, DSharedPtr<DMaterial> NewMaterial)
		: Mesh(NewMesh), MaterialInstance(std::make_unique<DMaterialInstance>(NewMaterial)) {}

	DSharedPtr<DMesh> Mesh;
	DUniquePtr<DMaterialInstance> MaterialInstance;
};

DSharedPtr<DMaterial> TestMaterial;

void DGameManager::RenderingTest()
{
	DString VertexShader;
	DString FragmentShader;

	VertexShader = DAssetManager::Get().SynchronousLoadAsset("Assets/simple.vert")->AsString();
	FragmentShader = DAssetManager::Get().SynchronousLoadAsset("Assets/simple.frag")->AsString();

	TestMaterial = std::make_shared<DMaterial>();
	TestMaterial->BuildShader(VertexShader, FragmentShader);

	auto& TestRenderEntity = ECSWorld.entity()
	.set<DMeshRendererComponent>({ MeshPrimitives::Cube, TestMaterial })
	.add<Transform3D>();

	TestRenderEntity.get<DMeshRendererComponent>()->MaterialInstance->SetUniform("Color", Vector3(0, 1, 0));

	ECSWorld.system<DMeshRendererComponent, Transform3D>("Render")
		.kind(flecs::OnStore)
		.each([&](const flecs::entity& ent, DMeshRendererComponent& Renderer, Transform3D& Transform)
	{
		DMesh* Mesh = Renderer.Mesh.Get();
		if (!Mesh) return;

		DMaterialInstance* MaterialInstance = Renderer.MaterialInstance.Get();
		if (!MaterialInstance) return;

		DCameraComponent* CameraComponent = DCameraComponent::GetActiveCamera();
		Check (CameraComponent);
		if (!CameraComponent) return;

		Matrix4x4 ViewMatrix = CameraComponent->GetViewMatrix();
		Matrix4x4 ProjectionMatrix = CameraComponent->GetPerspectiveProjectionMatrix();

		Matrix4x4 ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, Vector3(0, sin(GetGameTime()), 0));
		ModelMatrix = glm::scale(ModelMatrix, Vector3(1, 1, 1));
		ModelMatrix = glm::rotate(ModelMatrix, 0.f, Vector3(0, 1, 0));
		
		Matrix4x4 MVPMatrix =  ProjectionMatrix * ViewMatrix * ModelMatrix;

		MaterialInstance->SetUniform("MVP", MVPMatrix);

		MaterialInstance->Bind();
		Mesh->Draw();
		
	});

	ECSWorld.system<DCameraComponent>("Camera Initialize")
		.kind(flecs::OnSet)
		.kind(flecs::OnAdd)
		.each([](const flecs::entity& ent, DCameraComponent& Camera)
	{
		Camera.SetParentEntity(ent);
	});

	auto& TestCamera = ECSWorld.entity()
	.add<DCameraComponent>()
	.add<Transform3D>();

	TestCamera.get_mut<Transform3D>()->SetPosition(Vector3(1.5, 1.5, 20));

	DCameraComponent::SetActiveCamera(TestCamera);
}

void DGameManager::Exit()
{
	bMainLoopRunning = false;
}

DSharedPtr<DAudioSource> AudioSource = std::make_shared<DAudioSource>();

void DGameManager::MainGameLoop()
{
	DWAVFile::LoadAsync("Assets/Sussy Baka.wav", [&](DSharedPtr<DWAVFile> NewWav)
	{
		Check(NewWav.Get());
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
