#pragma once
#include "Object3d.h"

class Skydome {
public:
	void Initialize(Object3dManager* object3dManager);
	void Update();
	void Draw();
	~Skydome();
private:
	// �I�u�W�F�N�g
	Object3d* object_ = nullptr;
};

