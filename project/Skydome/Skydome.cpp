#include "Skydome.h"

void Skydome::Initialize(Object3dManager* object3dManager)
{
	// オブジェクト生成
	object_ = new Object3d();
	object_->Initialize(object3dManager); 
	object_->SetModel("skydome.obj");
	object_->SetScale({ 1000.0f, 1000.0f, 1000.0f });
}

void Skydome::Update()
{
	object_->Update();
}

void Skydome::Draw()
{
	object_->Draw();
}

Skydome::~Skydome() {
	delete object_;
}
