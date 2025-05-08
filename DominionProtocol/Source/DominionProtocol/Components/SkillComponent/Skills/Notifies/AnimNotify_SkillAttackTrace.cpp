#include "Components/SkillComponent/Skills/Notifies/AnimNotify_SkillAttackTrace.h"
#include "Components/SkillComponent/SkillComponentBase.h"

void UAnimNotify_SkillAttackTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			USkillComponentBase* SkillComponentBase = Owner->FindComponentByClass<USkillComponentBase>();

			if (IsValid(SkillComponentBase))
			{
				// 현재 실행중인 스킬의 AttackTrace 함수 실행
			}
		}
	}
}
