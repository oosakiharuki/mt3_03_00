#include <Novice.h>
#include "MyMath.h"
#include <cstdint>
#include <cassert>
#include<imgui.h>

const char kWindowTitle[] = "LE2C_07_オオサキ_ハルキ_タイトル";


Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vector3 Bezier(const Vector3& p0, const Vector3& p1, float t) {

	Vector3 a = { t * p0.x ,t * p0.y ,t * p0.z };
	Vector3 b = { (1.0f - t) * p1.x,(1.0f - t) * p1.y,(1.0f - t) * p1.z };
	Vector3	c = { a.x + b.x,a.y + b.y,a.z + b.z };

	return c;
}

Vector3 World(Vector3 a)
{
	a.y += -500;
	a.y *= -1;
	return a;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHandleWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHandleWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHandleWidth + (xIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ x,0.0f,-kGridHandleWidth };
		Vector3 end{ x,0.0f,kGridHandleWidth };

		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		if (x == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHandleWidth + (zIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ -kGridHandleWidth,0.0f,z };
		Vector3 end{ kGridHandleWidth,0.0f,z };

		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		if (z == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);

	}
}


//void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
//	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
//
//	
//
//
//
//}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 p0 = { 100.0f,100.0f,10.0f };
	Vector3 p1 = { 400.0f,400.0f,10.0f };
	Vector3 p2 = { 700.0f,100.0f,10.0f };
	int link = 32;
	int index = 0;

	Vector3 a = { 100.0f,100.0f,10.0f };





	Vector3 cameraPosition = { 0.0f ,0.0f,-20.0f };
	Vector3 cameraTranslate = { 0.0f,-1.0f,-6.49f };
	Vector3 cameraRotate = { -0.26f,0.26f,0.0f };

	MyMath* myMath_ = new MyMath();



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		Matrix4x4 worldMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 cameraMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = myMath_->Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = myMath_->MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);
		Matrix4x4 WorldViewProjectionMatrix = myMath_->Multiply(worldMatrix, myMath_->Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = myMath_->MakeViewportMatrix(0, 0, float(1280.0f), float(720.0f), 0.0f, 1.0f);

		Vector3 pw0 = World(p0);
		Vector3 pw1 = World(p1);
		Vector3 pw2 = World(p2);

		Novice::DrawEllipse((int)pw0.x, (int)pw0.y, 10, 10, 0.0f, WHITE, kFillModeSolid);
		Novice::DrawEllipse((int)pw1.x, (int)pw1.y, 10, 10, 0.0f, WHITE, kFillModeSolid);
		Novice::DrawEllipse((int)pw2.x, (int)pw2.y, 10, 10, 0.0f, WHITE, kFillModeSolid);



		Vector3 pw0Screen = Transform(Transform(pw0, WorldViewProjectionMatrix), viewportMatrix);
		Vector3 pw1Screen = Transform(Transform(pw1, WorldViewProjectionMatrix), viewportMatrix);
		Vector3 pw2Screen = Transform(Transform(pw2, WorldViewProjectionMatrix), viewportMatrix);

		for (index = 0; index < link; index++) {
			float t0 = index / float(link);
			float t1 = index / float(link);

			Vector3 bezier0 = Bezier(pw0, pw1, t0);
			Vector3 bezier1 = Bezier(pw1, pw2, t1);
			Vector3 p = Bezier(bezier0, bezier1, t0);

			if (index == 0) {
				Novice::DrawLine((int)pw0.x, (int)pw0.y, (int)a.x, (int)a.y, BLUE);
			}
			else {
				Novice::DrawLine((int)a.x, (int)a.y, (int)p.x, (int)p.y, BLUE);
			}
			a.x = p.x;
			a.y = p.y;


		}


		DrawGrid(WorldViewProjectionMatrix, viewportMatrix);

		ImGui::Begin("window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("pw0", &pw0.x, 0.1f);
		ImGui::DragFloat3("pw0S", &pw0Screen.x, 0.1f);


		ImGui::End();



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
