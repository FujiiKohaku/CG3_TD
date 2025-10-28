#include "Stage2Logic.h"
#include "Coin.h"
#include "Player.h"

void Stage2Logic::Initialize() {
	// �R�C�����c�~���ɐ���
	for (int y = 0; y < 10; y++) { // �c�����i�i�j
		for (int x = 0; x < 10; x++) { // �������i��j
			Coin* coin = new Coin();

			Vector3 pos = { -6.0f + x * 3.0f, 3.0f + y * 2.5f, 0.0f };

			coin->Initialize(pos, 1.0f, 100, object3dManager_, "Coin.obj");
			coins_.push_back(coin);
		}
	}
}

void Stage2Logic::Update() {
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

void Stage2Logic::Draw() {
	for (auto& coin : coins_) {
		coin->Draw();
	}
}

void Stage2Logic::Finalize() {
	for (auto& coin : coins_) {
		delete coin;
	}
	coins_.clear();
}

