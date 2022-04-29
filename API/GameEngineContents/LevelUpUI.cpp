#include "LevelUpUI.h"
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineSound.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <string>

#include "GameEnum.h"
#include "GameInfo.h"

#include "StatUI.h"

int LevelUpUI::CreateCount_ = 0;
bool LevelUpUI::IsActivated_ = false;
LevelUpUI::STATE LevelUpUI::State_ = STATE::NONE;

LevelUpUI::LevelUpUI() 
{
	RandomSkills_.reserve(4);

}

LevelUpUI::~LevelUpUI() 
{
}

void LevelUpUI::Start()
{
	// 배경 
	GameEngineRenderer* Renderer = CreateRenderer("LevelUpUI.bmp", static_cast<int>(RENDER_ORDER::UI));
	SetScale(Renderer->GetScale());
	Renderer->CameraEffectOff();
	Renderer->SetPivot(GameEngineWindow::GetScale().Half() + float4{0, 10});

	// 스탯 UI 같이 킴
	if (nullptr == StatUI_)
	{
		StatUI_ = GetLevel()->CreateActor<StatUI>(static_cast<int>(ACTOR_ORDER::UI), "UI");
	}
	else
	{
		StatUI_->On();
	}
	

	// 사운드
	GameEngineSound::SoundPlayOneShot("LevelUp.MP3", 0);
	IsActivated_ = true;


	if (true == GameInfo::SkillLevelFull())
	{
		ChangeState(STATE::HPMONEY);
	}
	else
	{
		// 조합 여부 확인
		SkillType EvolveSkill = GameInfo::SkillEvolveCheck();
		if (SkillType::NONE != EvolveSkill)
		{
			ChangeState(STATE::EVOLVE);
		}

		// 조합 여부 없으면 일반 
		ChangeState(STATE::BOXES);
	}

}

void LevelUpUI::Update()
{
	//if (0 >= CreateCount_)
	//{
	//	return;
	//}

	UpdateState();


	/*if (0 < SelectNum_)
	{
		SelectSkillBox();
		return;
	}*/



}

void LevelUpUI::Render()
{
}


void LevelUpUI::ShowRandomSkills()
{
	if (0 >= SelectNum_)
	{
		int a = 0;
		return;
	}

	if (1 <= SelectNum_)
	{
		int SelectSkill = RandomSkills_[0];
		Renderer1_->SetImage("LevelUp" + SkillOrder[SelectSkill] + ".bmp");
	}


	if (2 <= SelectNum_)
	{
		int SelectSkill = RandomSkills_[1];
		Renderer2_->SetImage("LevelUp" + SkillOrder[SelectSkill] + ".bmp");
	}


	if (3 <= SelectNum_)
	{
		int SelectSkill = RandomSkills_[2];
		Renderer3_->SetImage("LevelUp" + SkillOrder[SelectSkill] + ".bmp");
	}


	if (4 <= SelectNum_)
	{
		int SelectSkill = RandomSkills_[3];
		Renderer4_->SetImage("LevelUp" + SkillOrder[SelectSkill] + ".bmp");
	}

	
}

void LevelUpUI::SelectSkillBox()
{
	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num1") && SelectNum_ >= 1)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[0]);

		RandomSkills_.clear();

		// 종료
		Death();
		CreateCount_--;
		IsActivated_ = false;
		StatUI_->Off();
		return;
	}

	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num2") && SelectNum_ >= 2)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[1]);

		RandomSkills_.clear();

		// 종료
		Death();
		CreateCount_--;
		IsActivated_ = false;
		StatUI_->Off();
		return;
	}

	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num3") && SelectNum_ >= 3)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[2]);

		RandomSkills_.clear();

		// 종료
		Death();
		CreateCount_--;
		IsActivated_ = false;
		StatUI_->Off();
		return;
	}

	// 아이템 선택
	if (true == GameEngineInput::GetInst()->IsDown("Num4") && SelectNum_ >= 4)
	{
		// 플레이어에게 무기 정보 갱신
		SelectSkill(RandomSkills_[3]);

		RandomSkills_.clear();

		// 종료
		Death();
		CreateCount_--;
		IsActivated_ = false;
		StatUI_->Off();
		return;
	}
}

void LevelUpUI::SelectSkill(int _SkillNumber)
{
	// SkillSlot에 있는지 확인
	// 없으면 추가
	// 있으면 스킬레벨업
	GameInfo::GetPlayerInfo()->ActiveSkillSlot_;
	GameInfo::GetPlayerInfo()->PassiveSkillSlot_;

	int SkillLevel = GameInfo::GetPlayerInfo()->SkillLevelInfo_[_SkillNumber];


	if (0 == SkillLevel)
	{
		SkillType Type = static_cast<SkillType>(_SkillNumber);

		if (_SkillNumber < ACTIVE_MAX)
		{
			GameInfo::GetPlayerInfo()->ActiveSkillSlot_.push_back(Type);
		}
		else
		{
			GameInfo::GetPlayerInfo()->PassiveSkillSlot_.push_back(Type);
		}

	}
	
	GameInfo::GetPlayerInfo()->SkillLevelInfo_[_SkillNumber] += 1;

	
}


void LevelUpUI::UpdateState()
{
	switch (State_)
	{
	case LevelUpUI::STATE::BOXES:
		BoxesUpdate();
		break;
	case LevelUpUI::STATE::EVOLVE:
		EvolveUpdate();
		break;
	case LevelUpUI::STATE::HPMONEY:
		HpMoneyUpdate();
		break;
	default:
		break;
	}
}

void LevelUpUI::ChangeState(STATE _State)
{
	switch (_State)
	{
	case LevelUpUI::STATE::BOXES:
		BoxesStart();
		break;
	case LevelUpUI::STATE::EVOLVE:
		EvolveStart();
		break;
	case LevelUpUI::STATE::HPMONEY:
		HpMoneyStart();
		break;
	default:
		break;
	}

	State_ = _State;
}

void LevelUpUI::BoxesStart()
{
	// 무기 선택 박스 1~4에 띄울 스킬(액티브/패시브) 선택
	SelectNum_ = 3;

	// 무기 선택 박스 1
	Renderer1_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
	Renderer1_->CameraEffectOff();
	Renderer1_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, -Renderer1_->GetImageScale().y });


	// 무기 선택 박스 2
	Renderer2_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
	Renderer2_->CameraEffectOff();
	Renderer2_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, 0 });

	// 무기 선택 박스 3
	Renderer3_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
	Renderer3_->CameraEffectOff();
	Renderer3_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, Renderer1_->GetImageScale().y });

	// 무기 선택 박스 4 띄울지 여부
	float PlayerLuck = GameInfo::GetPlayerInfo()->Luck_;
	//PlayerLuck = 30;
	//GameEngineRandom Random;
	int Success = Random.RandomInt(1, 101);

	// 성공
	if (Success <= PlayerLuck)
	{
		// 무기 선택 박스 4
		Renderer4_ = CreateRenderer("LevelupBlank.bmp", static_cast<int>(RENDER_ORDER::UI));
		Renderer4_->CameraEffectOff();
		Renderer4_->SetPivot(GameEngineWindow::GetScale().Half() + float4{ 2, Renderer1_->GetImageScale().y * 2 });

		SelectNum_ = 4;
	}

	// 연속으로 SelectNum개의 겹치지 않는 랜덤수 뽑기
	bool SelectedSkills[static_cast<int>(SkillType::MAX)] = { false, };
	int TrueCount = 0;

	// 스킬 여유공간 체크
	//int SlotSpare = 12- (static_cast<int>(GameInfo::GetPlayerInfo()->ActiveSkillSlot_.size()) + static_cast<int>(GameInfo::GetPlayerInfo()->PassiveSkillSlot_.size()));

	// SelectNum_ -> 남은 스킬중 8레벨이 아닌 스킬들 선택
	int MaxLevelCount = 0;
	for (int i = 0; i < 12; i++)
	{
		if (SKILL_LEVELMAX == GameInfo::GetPlayerInfo()->SkillLevelInfo_[i])
		{
			MaxLevelCount++;
		}
	}

	int SelectableCount = static_cast<int>(SkillType::MAX) - MaxLevelCount;
	if (SelectableCount < SelectNum_)
	{
		SelectNum_ = SelectableCount;
	}



	while (TrueCount < SelectNum_)
	{
		int Index = Random.RandomInt(0, static_cast<int>(SkillType::MAX) - 1);

		// 8레벨 미만 스킬들만 고름
		if (false == SelectedSkills[Index] && GameInfo::GetPlayerInfo()->SkillLevelInfo_[Index] < SKILL_LEVELMAX)
		{
			// 스킬슬릇(Active/Passive)에 6개 다 차면 뽑을 수 없음
			if (Index < ACTIVE_MAX && 6 <= static_cast<int>(GameInfo::GetPlayerInfo()->ActiveSkillSlot_.size()))
			{
				continue;
			}
			if (Index >= ACTIVE_MAX && 6 <= static_cast<int>(GameInfo::GetPlayerInfo()->PassiveSkillSlot_.size()))
			{
				continue;
			}

			TrueCount++;
			SelectedSkills[Index] = true;

			// 3~4개의 박스 UI에 올릴 무기 선택
			RandomSkills_.push_back(Index);
		}
	}

	ShowRandomSkills();
}

void LevelUpUI::EvolveStart()
{

}

void LevelUpUI::HpMoneyStart()
{

}

void LevelUpUI::BoxesUpdate()
{
	SelectSkillBox();
}

void LevelUpUI::EvolveUpdate()
{

}

void LevelUpUI::HpMoneyUpdate()
{

}
