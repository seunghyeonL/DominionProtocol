#include "Components/SkillComponent/Skills/Notifies/AnimNotify_SkillAttackTrace.h"
#include "Components/SkillComponent/SkillComponent.h"

void UAnimNotify_SkillAttackTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>();

			if (IsValid(SkillComponent))
			{
				// 현재 실행중인 스킬의 AttackTrace 함수 실행
			}
		}
	}
}
