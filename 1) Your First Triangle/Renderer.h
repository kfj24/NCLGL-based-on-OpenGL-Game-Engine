#pragma once
#include "../nclgl/OGLRenderer.h"
class Renderer :public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);//���������黯��ֹ�ڴ�й©
	virtual void RenderScene();

protected:
	Mesh* triangle;
	Shader* basicShader;
};