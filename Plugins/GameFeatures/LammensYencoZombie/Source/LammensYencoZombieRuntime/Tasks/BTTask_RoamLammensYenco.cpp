#include "BTTask_RoamLammensYenco.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_RoamLammensYenco::UBTTask_RoamLammensYenco()
{
	NodeName = TEXT("Roam");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RoamLammensYenco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn) return EBTNodeResult::Failed;

	FNavLocation NavLoc;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys || !NavSys->GetRandomReachablePointInRadius(
			Pawn->GetActorLocation(), SearchRadius, NavLoc))
		return EBTNodeResult::Failed;

	auto Result = AIC->MoveToLocation(NavLoc.Location, 50.f);
	if (Result == EPathFollowingRequestResult::Failed) return EBTNodeResult::Failed;
	if (Result == EPathFollowingRequestResult::AlreadyAtGoal) return EBTNodeResult::Succeeded;
	return EBTNodeResult::InProgress;
}

void UBTTask_RoamLammensYenco::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	if (AIC->GetMoveStatus() == EPathFollowingStatus::Idle)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_RoamLammensYenco::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		AIC->StopMovement();
	return EBTNodeResult::Aborted;
}