#include "TestWindow.h"
#include "SOIL/SOIL.h"
#include "gl/glew.h"
unsigned int TestWindow::Tex = 0;
TestWindow::TestWindow()
{
	TestWindow::Tex = SOIL_load_OGL_texture("UITestTexture.png", 4, 0, 0);
}

void TestWindow::Draw()
{
	ImGui::Begin("TestWindow");
	ImGui::Text("Hello There Sussy");
	glBindTexture(GL_TEXTURE_2D, TestWindow::Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	ImGui::ImageButton((ImTextureID)TestWindow::Tex, { 128,64 });
	ImGui::End();
}
