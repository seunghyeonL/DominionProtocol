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
				// 스킬을 실행한 SkillComponent의 OnSkillEnd 델리게이트 실행
				if (SkillComponent->OnSkillEnd.IsBound())
				{
					if (auto CurrentSkill = SkillComponent->GetCurrentSkill())
					{
						SkillComponent->OnSkillEnd.Execute(CurrentSkill->GetControlEffectTag());
					}
				}
				else
				{
					Debug::PrintError(TEXT("OnSkillEnd Delegate is not bound."));
				}			
			}
		}
	}
}