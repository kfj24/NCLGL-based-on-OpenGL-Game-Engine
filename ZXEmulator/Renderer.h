#pragma once
#include "../NCLGL/OGLRenderer.h"

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	 ~Renderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;

	 void ClearPixels();
	 void ClearAttributes(int index);

	 void SetPaperColour(unsigned int x, unsigned int y, unsigned int entry);
	 void SetInkColour(unsigned int x, unsigned int y, unsigned int entry);
	 void SetBright(unsigned int x, unsigned int y, bool state);

	 void SetPixel(unsigned int x, unsigned int y, bool state);

protected:
	Shader* zxShader;

	GLuint colourUBO;
	GLuint pixelSSBO;
	GLuint attributeSSBO;
	GLuint emptyVAO;

	int* pixelData;
	int* attributeData;

	float time;
};
