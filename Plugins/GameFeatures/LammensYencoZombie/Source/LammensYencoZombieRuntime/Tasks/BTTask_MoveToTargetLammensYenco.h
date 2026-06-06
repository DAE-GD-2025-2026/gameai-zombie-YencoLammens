#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_MoveToTargetLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_MoveToTargetLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToTargetLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask (UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Move")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="Move")
	float AcceptanceRadius = 100.f;
};