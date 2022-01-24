#include "TestWindow.h"

void TestWindow::Draw()
{
	static float col[3];
	ImGui::Begin("TestWindow");
	ImGui::Text("Hello There Sussy");
	ImGui::End();
}
