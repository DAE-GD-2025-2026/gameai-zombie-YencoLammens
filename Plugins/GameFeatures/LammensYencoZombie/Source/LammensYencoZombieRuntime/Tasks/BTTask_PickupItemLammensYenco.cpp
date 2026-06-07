#include "BTTask_PickupItemLammensYenco.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "LammensYencoZombieRuntime/SurvivorAPIHelpersLammensYenco.h"

UBTTask_PickupItemLammensYenco::UBTTask_PickupItemLammensYenco()
{
	NodeName    = TEXT("PickupItem");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_PickupItemLammensYenco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return EBTNodeResult::Failed;

	APawn*  Pawn = AIC->GetPawn();
	AActor* Item = Cast<AActor>(BB->GetValueAsObject(NearestItemKey.SelectedKeyName));
	if (!Pawn || !IsValid(Item)) return EBTNodeResult::Failed;

	UActorComponent* Inv = SurvivorAPI::FindComp(Pawn, TEXT("Inventory"));
	if (!Inv) return EBTNodeResult::Failed;

	const int32 Slot = SurvivorAPI::FindFreeSlot(Inv);
	if (Slot == -1) return EBTNodeResult::Failed;

	if (!SurvivorAPI::CallGrabItem(Inv, Slot, Item))
		return EBTNodeResult::Failed;

	const FString ClassName = Item->GetClass()->GetName();
	if (ClassName.Contains(TEXT("Pistol")) || ClassName.Contains(TEXT("Shotgun")))
	{
		BB->SetValueAsBool(bHasWeaponKey.SelectedKeyName, true);
	}
	else
	{
		SurvivorAPI::CallUseItem(Inv, Slot);
		SurvivorAPI::CallRemoveItem(Inv, Slot);
	}

	BB->SetValueAsObject(NearestItemKey.SelectedKeyName, nullptr);
	return EBTNodeResult::Succeeded;
}