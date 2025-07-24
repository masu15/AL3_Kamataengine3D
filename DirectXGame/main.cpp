#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"
#include"TitleScene.h"
using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
enum class Scene {

	KUnknown = 0,

	kTitle,
	kGame,

};
Scene scene = Scene::KUnknown;
void ChansScene();
void UpdateScene();
void DrawScene();
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    KamataEngine::Initialize(L"GC2D_06_マスヤマ_リョウタ_AL3");

	

	gameScene = new GameScene;
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

    ChansScene();
	UpdateScene();

	DrawScene();

	gameScene->Initialize();

	while (true) {
		if (KamataEngine::Update())
		{
			break;
		}
		gameScene->Update();

		dxCommon->PreDraw();

		gameScene->Draw();
		
		dxCommon->PostDraw();
	}
	delete gameScene;

	delete titleScene;

	gameScene = nullptr;

	KamataEngine::Finalize();
	return 0;
}

void ChansScene() 
{
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->ISFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->ISFinished()) {
			scene = Scene::kTitle;
			delete gameScene;
			gameScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	default:
		break;
	}
	

}

void UpdateScene() 
{
	switch (scene) {
	
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	
	}
}

void DrawScene() 
{
	switch (scene) {

	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}
