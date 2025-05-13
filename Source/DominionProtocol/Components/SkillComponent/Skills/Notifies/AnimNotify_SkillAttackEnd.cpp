#include "Components/SkillComponent/Skills/Notifies/AnimNotify_SkillAttackEnd.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "GameFramework/Character.h"
#include "Util/DebugHelper.h"

void UAnimNotify_SkillAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>();

			if (IsValid(SkillComponent))
			{
				// 현재 실행중인 스킬의 OnSkillEnd 델리게이트 실행
				UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

				if (BaseSkill->OnSkillEnd.IsBound())
				{
					BaseSkill->OnSkillEnd.Execute();
				}
				else
				{
					Debug::PrintError(TEXT("OnSkillEnd Delegate is not bound."));
				}			
			}
		}
	}
}