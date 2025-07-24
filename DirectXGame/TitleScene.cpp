#include "TitleScene.h"
#include <KamataEngine.h>
void TitleScene::Initialize() 
{

}
void TitleScene::Update()
{
	if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}
void TitleScene::Draw() 
{

}