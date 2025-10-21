#include "Object3d.h"
#include "MatrixMath.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3dManager.h"
#include <cassert>
#include <fstream>
#include <sstream>
#pragma region 初期化処理
void Object3d::Initialize(Object3dManager* object3DManager)
{
    // Object3dManager と DebugCamera を受け取って保持
    object3dManager_ = object3DManager;
    camera_ = object3dManager_->GetDefaultCamera();
    // ================================
    // Transformバッファ初期化
    // ================================
    transformationMatrixResource = object3dManager_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
    transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
    transformationMatrixData->WVP = MatrixMath::MakeIdentity4x4();
    transformationMatrixData->World = MatrixMath::MakeIdentity4x4();

    // ================================
    // 平行光源データ初期化
    // ================================
    directionalLightResource = object3dManager_->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
    directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
    directionalLightData->color = { 1, 1, 1, 1 };
    directionalLightData->direction = MatrixMath::Normalize({ 0, -1, 0 });
    directionalLightData->intensity = 1.0f;

    // ================================
    // Transform初期値設定
    // ================================
    transform = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    cameraTransform = { { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.0f, 0.0f }, { 0.0f, 4.0f, -10.0f } };
}
#pragma endregion

#pragma region 更新処理

void Object3d::Update()
{
    // ================================
    // 各種行列を作成
    // ================================

    //  モデル自身のワールド行列（スケール・回転・移動）
    Matrix4x4 worldMatrix = MatrixMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

    Matrix4x4 worldViewProjectionMatrix;

    if (camera_) {
        const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
        worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, viewProjectionMatrix);
    } else {
        worldViewProjectionMatrix = worldMatrix;
    }

    // ================================
    // WVP行列を計算して転送
    // ================================
    transformationMatrixData->WVP = worldViewProjectionMatrix;

    // ワールド行列も送る（ライティングなどで使用）
    transformationMatrixData->World = worldMatrix;
}

#pragma endregion

#pragma region 描画処理
void Object3d::Draw()
{
    ID3D12GraphicsCommandList* commandList = object3dManager_->GetDxCommon()->GetCommandList();

    // Transform定数バッファをセット
    commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

    // ライト情報をセット
    commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

    // モデルが設定されていれば描画
    if (model_) {
        model_->Draw();
    }
}
#pragma endregion

#pragma region OBJ読み込み処理
// ===============================================
// OBJファイルの読み込み
// ===============================================
// ===============================================
// OBJファイルの読み込み（完全版）
// ===============================================
Object3d::ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string filename)
{
    ModelData modelData;
    std::vector<Vector4> positions; // 頂点座標
    std::vector<Vector3> normals; // 法線
    std::vector<Vector2> texcoords; // UV座標
    std::string line;

    // ファイルを開く
    std::ifstream file(directoryPath + "/" + filename);
    assert(file.is_open());

    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;

        if (identifier == "v") {
            // 頂点座標
            Vector4 pos;
            s >> pos.x >> pos.y >> pos.z;
            pos.x *= -1.0f; // 左手座標化
            pos.w = 1.0f;
            positions.push_back(pos);
        } else if (identifier == "vt") {
            // テクスチャ座標
            Vector2 uv;
            s >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y; // 上下反転
            texcoords.push_back(uv);
        } else if (identifier == "vn") {
            // 法線
            Vector3 n;
            s >> n.x >> n.y >> n.z;
            n.x *= -1.0f; // 左手座標化
            normals.push_back(n);
        } else if (identifier == "f") {
            // 面定義（3頂点以上にも対応）
            std::vector<VertexData> faceVertices;
            std::string vertexDef;

            while (s >> vertexDef) {
                std::istringstream v(vertexDef);
                uint32_t indices[3] = {};
                for (int i = 0; i < 3; i++) {
                    std::string index;
                    if (!std::getline(v, index, '/'))
                        break;
                    if (!index.empty())
                        indices[i] = std::stoi(index);
                }

                Vector4 pos = positions.at(indices[0] - 1);
                Vector2 uv = texcoords.at(indices[1] - 1);
                Vector3 nor = normals.at(indices[2] - 1);
                faceVertices.push_back({ pos, uv, nor });
            }

            // 🔹 多角形→三角形分割処理
            for (size_t i = 1; i + 1 < faceVertices.size(); i++) {
                modelData.vertices.push_back(faceVertices[0]);
                modelData.vertices.push_back(faceVertices[i]);
                modelData.vertices.push_back(faceVertices[i + 1]);
            }
        } else if (identifier == "mtllib") {
            // マテリアルファイル読み込み
            std::string mtlFile;
            s >> mtlFile;
            modelData.material = LoadMaterialTemplateFile(directoryPath, mtlFile);
        }
    }

    return modelData;
}

#pragma endregion

#pragma region MTL読み込み処理
// ===============================================
// マテリアル（.mtl）ファイルの読み込み
// ===============================================
Object3d::MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{

    // 1.中で必要となる変数の宣言
    MaterialData materialData; // 構築するMaterialData
    // 2.ファイルを開く
    std::string line; // ファイルから読んだ１行を格納するもの
    std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
    assert(file.is_open()); // とりあえず開けなかったら止める
    // 3.実際にファイルを読み、MaterialDataを構築していく
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;
        // identifierに応じた処理
        if (identifier == "map_Kd") {
            std::string textureFilename;
            s >> textureFilename;
            // 連結してファイルパスにする
            materialData.textureFilePath = directoryPath + "/" + textureFilename;
        }
    }
    // 4.materialDataを返す
    return materialData;
}

#pragma endregion

void Object3d::SetModel(const std::string& filePath)
{
    // モデルを検索してセットする
    model_ = ModelManager::GetInstance()->FindModel(filePath);
}