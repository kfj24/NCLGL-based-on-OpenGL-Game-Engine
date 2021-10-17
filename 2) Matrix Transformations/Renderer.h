#pragma once
#include "../nclgl/OGLRenderer.h"
class Renderer :public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	virtual void RenderScene();

	//����Ҫʵ�ֵĺ���������ͶӰ ͸��ͶӰ ���š���ת��ƽ��
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