#include "SetMatrix.h"
#include <MathUtility.h>
#include <Math.h>
using namespace MathUtility;


// 行列の設定
void SetMatrix(WorldTransform& WT)
{
	// スケーリング行列を宣言
	Matrix4 matScale = Matrix4Identity();
	// スケーリング倍率を行列に設定する
	matScale.m[0][0] = WT.scale_.x;
	matScale.m[1][1] = WT.scale_.y;
	matScale.m[2][2] = WT.scale_.z;


	// Z軸回転行列を宣言
	Matrix4 matRotZ;
	matRotZ = Matrix4Identity();;
	// Z軸回転行列の各要素を設定する
	matRotZ.m[0][0] = cosf(WT.rotation_.z);
	matRotZ.m[0][1] = sinf(WT.rotation_.z);
	matRotZ.m[1][0] = -sinf(WT.rotation_.z);
	matRotZ.m[1][1] = cosf(WT.rotation_.z);

	// X軸回転行列を宣言
	Matrix4 matRotX;
	matRotX = Matrix4Identity();;
	// X軸回転行列の各要素を設定する
	matRotX.m[1][1] = cosf(WT.rotation_.x);
	matRotX.m[1][2] = sinf(WT.rotation_.x);
	matRotX.m[2][1] = -sinf(WT.rotation_.x);
	matRotX.m[2][2] = cosf(WT.rotation_.x);

	// Y軸回転行列を宣言
	Matrix4 matRotY;
	matRotY = Matrix4Identity();
	// Y軸回転行列の各要素を設定する
	matRotY.m[0][0] = cosf(WT.rotation_.y);
	matRotY.m[2][0] = sinf(WT.rotation_.y);
	matRotY.m[0][2] = -sinf(WT.rotation_.y);
	matRotY.m[2][2] = cosf(WT.rotation_.y);

	Matrix4 matRot;
	matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列を宣言
	Matrix4 matTrans = Matrix4Identity();

	// 移動量を行列に設定する
	matTrans.m[3][0] = WT.translation_.x;
	matTrans.m[3][1] = WT.translation_.y;
	matTrans.m[3][2] = WT.translation_.z;

	// 行列の合成
	WT.matWorld_ = matScale * matRot * matTrans;

	// 行列の転送
	WT.TransferMatrix();
}
