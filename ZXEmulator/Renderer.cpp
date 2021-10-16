#include "Renderer.h"

const int NUM_COLOURS = 16;

const Vector4 palette[NUM_COLOURS] = {
	//Normal colours
	Vector4(0,0,0,1),
	Vector4(0,0,215.0f / 255.0f,1),
	Vector4(128.0f / 255.0f,0,0,1),
	Vector4(128.0f / 255.0f,0,128.0f / 255.0f,1),
	Vector4(0,128.0f / 255.0f,0,1),
	Vector4(0,128.0f / 255.0f,128.0f / 255.0f,1),
	Vector4(128.0f / 255.0f,128.0f / 255.0f,0,1),
	Vector4(128.0f / 255.0f,128.0f / 255.0f,128.0f / 255.0f,1),
	//Bright bit set colours...
	Vector4(0,0,0,1),
	Vector4(0,0,1,1),
	Vector4(1,0,0,1),
	Vector4(1,0,1,1),
	Vector4(0,1,0,1),
	Vector4(0,1,1,1),
	Vector4(1,1,0,1),
	Vector4(1,1,1,1)
};

const int ZX_WIDTH  = 256;
const int ZX_HEIGHT = 192;

const int ATTRIB_WIDTH  = 8;
const int ATTRIB_HEIGHT = 8;

const int PIXEL_COUNT  = (ZX_WIDTH * ZX_HEIGHT) / 8; //each pixel is a single bit!
const int ATTRIB_COUNT = (ZX_WIDTH / ATTRIB_WIDTH) * (ZX_HEIGHT / ATTRIB_HEIGHT);

const int ZX_ATTRIBS_WIDTH = ZX_WIDTH / ATTRIB_WIDTH;
const int ZX_ATTRIBS_HEIGHT = ZX_HEIGHT / ATTRIB_HEIGHT;

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	zxShader = new Shader("zxVertex.glsl","zxFragment.glsl");

	if(!zxShader->LoadSuccess()) {
		return;
	}

	glGenBuffers(1, &colourUBO);
	glBindBuffer(GL_ARRAY_BUFFER, colourUBO);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(palette), 0, GL_DYNAMIC_STORAGE_BIT );
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(palette), palette);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint ssboFlags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glGenBuffers(1, &pixelSSBO);
	glBindBuffer(GL_ARRAY_BUFFER, pixelSSBO);
	glBufferStorage(GL_ARRAY_BUFFER, PIXEL_COUNT * sizeof(int), 0, ssboFlags);
	pixelData = (int*)glMapBufferRange(GL_ARRAY_BUFFER, 0, PIXEL_COUNT, ssboFlags);

	glGenBuffers(1, &attributeSSBO);
	glBindBuffer(GL_ARRAY_BUFFER, attributeSSBO);
	glBufferStorage(GL_ARRAY_BUFFER, ATTRIB_COUNT * sizeof(int), 0, ssboFlags);
	attributeData = (int*)glMapBufferRange(GL_ARRAY_BUFFER, 0, ATTRIB_COUNT, ssboFlags);

	glGenVertexArrays(1, &emptyVAO); //modern OpenGL NEEDS a VAO, even if it's never used...

	init = true;
	time = 0;
}
Renderer::~Renderer(void)	{
	delete zxShader;
}

void Renderer::RenderScene() {
	glUseProgram(zxShader->GetProgram());

	const GLuint uboSlot	= 0;
	const GLuint pixelSlot	= 1;
	const GLuint attribSlot	= 2;

	unsigned int uboIndex = glGetUniformBlockIndex(zxShader->GetProgram(), "Palette");
	glUniformBlockBinding(zxShader->GetProgram(), uboIndex, uboSlot);
	glBindBufferRange(GL_UNIFORM_BUFFER, uboSlot, colourUBO, 0, sizeof(palette));

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pixelSlot,  pixelSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, attribSlot, attributeSSBO);

	glUniform2f(glGetUniformLocation(zxShader->GetProgram(), "screen"), width, height);

	glBindVertexArray(emptyVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Renderer::UpdateScene(float dt) {
	time += dt;
	int iTime = (int)time;
	for (int y = 0; y < ZX_HEIGHT; ++y) {
		for (int x = 0; x < ZX_WIDTH; ++x) {
			SetPixel(x, y, y % 2 ? x % 2 : x + 1 % 2);
		}
	}

	for (int y = 0; y < ZX_ATTRIBS_HEIGHT; ++y) {
		for (int x = 0; x < ZX_ATTRIBS_WIDTH; ++x) {
			SetPaperColour(x, y, (iTime + x) % 8);
			SetInkColour(x, y, (x + y) % 8);

			SetBright(x, y, x % 3);
		}
	}
}

void Renderer::ClearPixels() {
	for (int i = 0; i < PIXEL_COUNT; ++i) {
		pixelData[i] = 0;
	}
}

void Renderer::ClearAttributes(int index) {
	for (int i = 0; i < ATTRIB_COUNT; ++i) {
		attributeData[i] = index;
	}
}

void Renderer::SetPaperColour(unsigned int x, unsigned int y, unsigned int entry) {
	int realIndex = (y * ZX_ATTRIBS_WIDTH)+x;

	int realEntry = entry & 7;						//only the first 3 bits count!
	int oldEntry  = attributeData[realIndex] & 248; //Everything but the first 3 bits!
	attributeData[realIndex] = realEntry + oldEntry;

}

void Renderer::SetInkColour(unsigned int x, unsigned int y, unsigned int entry) {
	int realIndex = (y * ZX_ATTRIBS_WIDTH)+x;

	int realEntry = (entry<<3) & 56;				//only the next 3 bits count!
	int oldEntry  = attributeData[realIndex] & 199; //Everything but the next 3 bits!
	attributeData[realIndex] = realEntry + oldEntry;
}

void Renderer::SetBright(unsigned int x, unsigned int y, bool state) {
	int realIndex = (y * ZX_ATTRIBS_WIDTH)+x;

	attributeData[realIndex] = (attributeData[realIndex] & 127) + (state ? 128 : 0);
}

void Renderer::SetPixel(unsigned int x, unsigned int y, bool state) {
	int realIndex	= ((y * ZX_WIDTH) + x) / 8;
	int bit			= x % 8;

	unsigned int mask = (~(1 << bit)) & 255;

	pixelData[realIndex] = (pixelData[realIndex] & mask) +( state ? (1 << bit) : 0);
}