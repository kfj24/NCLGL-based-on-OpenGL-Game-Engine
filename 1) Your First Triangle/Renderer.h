#pragma once
#include "../nclgl/OGLRenderer.h"
class Renderer :public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);//Îö¹¹º¯ÊıĞé»¯·ÀÖ¹ÄÚ´æĞ¹Â©
	virtual void RenderScene();

protected:
	Mesh* triangle;
	Shader* basicShader;
};