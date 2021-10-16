#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();
	basicShader = new Shader("basicVertex.glsl", "colourFragment.glsl");

	if (!basicShader->LoadSuccess()) {
		return;
	}
	init = true;
}
Renderer::~Renderer(void) {
	delete triangle;
	delete basicShader;
}
void Renderer::RenderScene() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //初始化屏幕颜色 大概是灰色
	glClear(GL_COLOR_BUFFER_BIT); 
	BindShader(basicShader);
	triangle->Draw();
}