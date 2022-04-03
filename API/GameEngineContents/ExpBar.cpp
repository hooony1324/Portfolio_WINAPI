#include "ExpBar.h"
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
//#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngine/GameEngineRenderer.h>

ExpBar::ExpBar() 
{
}

ExpBar::~ExpBar() 
{
}

void ExpBar::Start()
{
	SetPosition({GameEngineWindow::GetScale().Half().x, 15});
	SetScale({ 1280, 30 });

	GameEngineRenderer* BlackBar = CreateRenderer("ExpBar.bmp");
	//BlackBar->SetPivot(BlackBar->GetScale().Half());
	BlackBar->CameraEffectOff();

	GameEngineRenderer* BlueBar = CreateRenderer("ExpBarBlue.bmp");
	//BlueBar->SetPivot(BlueBar->GetScale().Half());
	BlueBar->CameraEffectOff();
}

void ExpBar::Update()
{
}

void ExpBar::Render()
{
	
}
