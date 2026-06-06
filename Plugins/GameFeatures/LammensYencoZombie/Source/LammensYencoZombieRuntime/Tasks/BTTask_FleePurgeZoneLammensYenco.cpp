#include "BTTask_FleePurgeZoneLammensYenco.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_FleePurgeZoneLammensYenco::UBTTask_FleePurgeZoneLammensYenco()
{
	NodeName = TEXT("FleePurgeZone");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FleePurgeZoneLammensYenco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return EBTNodeResult::Failed;

	APawn* Pawn = AIC->GetPawn();
	AActor* Zone = Cast<AActor>(BB->GetValueAsObject(NearestPurgeZoneKey.SelectedKeyName));
	if (!Pawn || !IsValid(Zone)) return EBTNodeResult::Failed;

	const FVector AwayDir = (Pawn->GetActorLocation() - Zone->GetActorLocation()).GetSafeNormal();
	const FVector FleeTarget = Pawn->GetActorLocation() + AwayDir * FleeDistance;

	FNavLocation NavLoc;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys || !NavSys->ProjectPointToNavigation(FleeTarget, NavLoc))
		return EBTNodeResult::Failed;

	auto Result = AIC->MoveToLocation(NavLoc.Location, 50.f);
	if (Result == EPathFollowingRequestResult::Failed) return EBTNodeResult::Failed;
	if (Result == EPathFollowingRequestResult::AlreadyAtGoal) return EBTNodeResult::Succeeded;
	return EBTNodeResult::InProgress;
}

void UBTTask_FleePurgeZoneLammensYenco::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	if (AIC->GetMoveStatus() == EPathFollowingStatus::Idle)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_FleePurgeZoneLammensYenco::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		AIC->StopMovement();
	return EBTNodeResult::Aborted;
}