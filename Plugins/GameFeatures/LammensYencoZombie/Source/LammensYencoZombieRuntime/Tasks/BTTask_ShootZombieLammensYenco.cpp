#include "BTTask_ShootZombieLammensYenco.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "LammensYencoZombieRuntime/SurvivorAPIHelpersLammensYenco.h"

UBTTask_ShootZombieLammensYenco::UBTTask_ShootZombieLammensYenco()
{
	NodeName = TEXT("ShootZombie");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_ShootZombieLammensYenco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return EBTNodeResult::Failed;

	APawn* Pawn= AIC->GetPawn();
	AActor* Zombie = Cast<AActor>(BB->GetValueAsObject(NearestZombieKey.SelectedKeyName));
	if (!Pawn || !IsValid(Zombie)) return EBTNodeResult::Failed;

	if (FVector::DistSquared(Pawn->GetActorLocation(), Zombie->GetActorLocation()) > FMath::Square(ShootRange))
		return EBTNodeResult::Failed;

	UActorComponent* Inv = SurvivorAPI::FindComp(Pawn, TEXT("Inventory"));
	if (!Inv) return EBTNodeResult::Failed;

	const int32 WeaponSlot = SurvivorAPI::FindWeaponSlot(Inv);
	if (WeaponSlot == -1)
	{
		BB->SetValueAsBool(bHasWeaponKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}
	
	AIC->SetFocus(Zombie);
	const bool bFired = SurvivorAPI::CallUseItem(Inv, WeaponSlot);
	AIC->ClearFocus(EAIFocusPriority::Gameplay);
	
	if (SurvivorAPI::GetSlotValue(Inv, WeaponSlot) <= 0)
	{
		SurvivorAPI::CallRemoveItem(Inv, WeaponSlot);
		BB->SetValueAsBool(bHasWeaponKey.SelectedKeyName, false);
	}

	return bFired ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}