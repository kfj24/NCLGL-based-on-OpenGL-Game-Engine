#pragma once
#include "../nclgl/OGLRenderer.h"
class Renderer :public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	virtual void RenderScene();

	//今天要实现的函数：正交投影 透视投影 缩放、旋转、平移
	void SwitchToPerspective();
	void SwitchToOrthographic();
	inline void SetScale(float s) { scale = s; }
	inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3 p) { position = p; }

protected:
	Mesh* triangle;
	Shader* matrixShader;
	float scale;
	float rotation;
	Vector3 position;
};