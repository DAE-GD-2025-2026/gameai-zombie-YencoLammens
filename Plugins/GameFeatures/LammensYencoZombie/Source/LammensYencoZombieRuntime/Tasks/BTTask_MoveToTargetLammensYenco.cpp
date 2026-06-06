#include "BTTask_MoveToTargetLammensYenco.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToTargetLammensYenco::UBTTask_MoveToTargetLammensYenco()
{
	NodeName = TEXT("MoveToTarget");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToTargetLammensYenco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(Target)) return EBTNodeResult::Failed;

	auto Result = AIC->MoveToActor(Target, AcceptanceRadius);
	if (Result == EPathFollowingRequestResult::Failed) return EBTNodeResult::Failed;
	if (Result == EPathFollowingRequestResult::AlreadyAtGoal) return EBTNodeResult::Succeeded;
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToTargetLammensYenco::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	if (AIC->GetMoveStatus() == EPathFollowingStatus::Idle)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_MoveToTargetLammensYenco::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		AIC->StopMovement();
	return EBTNodeResult::Aborted;
}