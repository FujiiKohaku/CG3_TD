#pragma once
#include "Object3d.h"

class Skydome {
public:
	void Initialize(Object3dManager* object3dManager);
	void Update();
	void Draw();
	~Skydome();
private:
	// オブジェクト
	Object3d* object_ = nullptr;
};

