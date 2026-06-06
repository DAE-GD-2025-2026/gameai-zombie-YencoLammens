#include "BTTask_FleeFromZombieLammensYenco.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_FleeFromZombieLammensYenco::UBTTask_FleeFromZombieLammensYenco()
{
	NodeName = TEXT("FleeFromZombie");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FleeFromZombieLammensYenco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return EBTNodeResult::Failed;

	APawn* Pawn = AIC->GetPawn();
	AActor* Zombie = Cast<AActor>(BB->GetValueAsObject(NearestZombieKey.SelectedKeyName));
	if (!Pawn || !IsValid(Zombie)) return EBTNodeResult::Failed;

	const FVector AwayDir = (Pawn->GetActorLocation() - Zombie->GetActorLocation()).GetSafeNormal();
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

void UBTTask_FleeFromZombieLammensYenco::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	if (AIC->GetMoveStatus() == EPathFollowingStatus::Idle)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_FleeFromZombieLammensYenco::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		AIC->StopMovement();
	return EBTNodeResult::Aborted;
}