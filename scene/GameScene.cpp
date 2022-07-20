#include "GameScene.h"
#include "TextureManager.h"
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include"SetMatrix.h"
#include <cassert>
using namespace MathUtility;


//インストラクタ
GameScene::GameScene() 
{

}

//デストラクタ
GameScene::~GameScene() 
{
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("ri-fia.png");
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロダクションの初期化
	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロダクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロダクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	Matrix4 matScale;

	//各軸用回転行列を宣言
	worldTransform_.TransferMatrix();

	worldTransform_.scale_ = { 10,10,10 };
    //スケーリング倍率を行列に設定する
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	//worldTransform_,matWorld_に単位行列を代入する
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//worldTransform_,matWorld_にmatScaleを掛け算して代入する
	worldTransform_.matWorld_ *= matScale;
	//行列の転送
	worldTransform_.TransferMatrix();


    //X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = {0.0f,MathUtility::PI,0.0f};
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転行列を宣言
	Matrix4 matRotX,matRotY,matRotZ;

	//Z軸回転行列の各要素を確定する
	matRotZ = MathUtility::Matrix4Identity();
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= matRotZ;
	//行列の転送
	worldTransform_.TransferMatrix();

	//X軸回転行列の各要素を確定する
	matRotX = MathUtility::Matrix4Identity();
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= matRotX;
	//行列の転送
	worldTransform_.TransferMatrix();

	//Y軸回転行列の各要素を確定する
	matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.y);
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= matRotY;
	//行列の転送
	worldTransform_.TransferMatrix();

	//各軸の回転行列を合成
	matRot = matRotZ * matRotX * matRotY;


	//x,y,z軸周りの平行移動を設定
	worldTransform_.translation_ = { 0,10,0 };

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//移動量を行列に設定する
	//matTrans=MathUtility::Matrix4Identi();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	worldTransform_.matWorld_= MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= matTrans;

	//行列の合成
	worldTransform_.matWorld_= MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ = matScale * matRot * matTrans;

	//行列の転送
	worldTransform_.TransferMatrix();
}

void GameScene::Update() 
{
	//デバッグカメラの更新
	debugCamera_->Update();
	worldTransform_.translation_.x += 0.1f;
	SetMatrix(worldTransform_);
}

void GameScene::Draw() 
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	//ライン描画が参照するビュープロダクションを指定する(アドレス渡し)
	for  (float i = 0; i < 10; i++)
	{
      PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,i,0 }, { 10,i,0 }, { 1,0,1,1 });
	}
	
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
