#include "RenderPipeline.h"
#include "Framebuffer.h"
#include "../WindowManager.h"
#include "Camera.h"
#include <gl/glew.h>
#include "../Assets/GameAsset.h"
#include "../Assets/GameAssetSoftPointer.h"
#include "FreetypeFontAsset.h"

RenderPipeline* RenderPipeline::ActiveRenderPipeline = nullptr;

RenderPipeline::RenderPipeline()
{
	BackgroundContainer.Initialize();
}

void RenderPipeline::Bind()
{
	if (RenderPipeline* Pipeline = GetActive())
	{
		Pipeline->Unbind();
	}
	ActiveRenderPipeline = this;
	MainFramebuffer = new Framebuffer(WindowManager::Get().GetScreenSize().x, WindowManager::Get().GetScreenSize().y);
}


void RenderPipeline::Unbind( )
{
	if (RenderPipeline* Pipeline = GetActive())
	{
		if (Pipeline == this)
		{
			ActiveRenderPipeline = nullptr;
		}
	}
	if (MainFramebuffer)
	{
		delete MainFramebuffer;
	}
}

void RenderPipeline::Draw(CameraComponent* Camera, float DeltaTime)
{
	Check(MainFramebuffer);
	Framebuffer::Unbind();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MainFramebuffer->Bind();
	MainFramebuffer->Clear();

	BackgroundContainer.Draw(Camera);

	if (CameraComponent* Camera = CameraComponent::GetActiveCamera())
	{
		Camera->Draw(DeltaTime);
	}

	GameAssetSoftPointer<FreetypeFontAsset> Font("GameAssetFiles/Fonts/CreeperPixelRegular/CreeperPixelRegular-DMYx.ttf");
	Font.LoadSynchronous();

	Framebuffer::Unbind();

	MainFramebuffer->Draw();


	SDL_GL_SwapWindow(WindowManager::GetSDLWindow());
}

RenderPipeline* RenderPipeline::GetActive()
{
	return ActiveRenderPipeline;
}