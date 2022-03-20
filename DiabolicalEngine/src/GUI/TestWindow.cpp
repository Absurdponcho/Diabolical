#include "TestWindow.h"
#include "gl/glew.h"
#include "Check.h"


TestWindow::TestWindow()
{
	//Tex = SOIL_load_OGL_texture("UITestTexture.png", 4, 0, 0);
	Check(Tex != 0);
	glBindTexture(GL_TEXTURE_2D, TestWindow::Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void TestWindow::Draw()
{
	ImGui::Begin("TestWindow");
	ImGui::Text("Hello There Sussy");
	ImGui::Image((ImTextureID)(uint64_t)Tex, {100, 100});
	ImGui::ImageButton((ImTextureID)(uint64_t)Tex, { 128,64 });
	ImGui::End();
}
