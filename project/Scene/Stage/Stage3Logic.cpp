#include "Stage3Logic.h"
#include "Player.h"
#include "Coin.h"

void Stage3Logic::Initialize() {
    for (int i = 0; i < 8; i++) { // �c�����i�i�j
            Coin* coin = new Coin();

            // �����Ɉړ�
            Vector3 pos = { -17.5f + i * 5.0f, -17.5f + i * 5.0f, 0.0f };
            coin->Initialize(pos, 1.0f, 10, object3dManager_, "Coin.obj");
			coin->SetVelocity({ 0.1f,0.0f,0.0f });
            coins_.push_back(coin);
        
    }
}

void Stage3Logic::Update() {

	// �v���C���[�̋��̏����擾
	Sphere playerSphere = { pendulumPlayer_->GetPosition(), pendulumPlayer_->GetRadius() };
	// pendulum ���u�؂�Ă��Ȃ��v���܂��U��q�ŗh��Ă���Ԃ̓R�C�����擾�ł��Ȃ�
	if (pendulumPlayer_->GetPendulum()->GetIsCut()) {
		// �����ړ����̂݃R�C��������s��
		for (auto& coin : coins_) {
			coin->Update();
			if (coin->IsCollision(playerSphere)) {
				pendulumPlayer_->AddScore(coin->GetScore());
			}
		}
	} else {
		// �܂����[�v�łȂ����Ă���ԁi�X�C���O���j�̓R�C�������X�V���Ă���
		for (auto& coin : coins_) {
			coin->Update();
		}
	}
}

void Stage3Logic::Draw() {
	for (auto& coin : coins_) {
		coin->Draw();
	}
}

void Stage3Logic::Finalize() {
	for (auto& coin : coins_) {
		delete coin;
	}
	coins_.clear();
}
