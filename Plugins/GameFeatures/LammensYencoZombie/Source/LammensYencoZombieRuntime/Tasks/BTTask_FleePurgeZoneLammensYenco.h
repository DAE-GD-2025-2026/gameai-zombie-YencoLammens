#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_FleePurgeZoneLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_FleePurgeZoneLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FleePurgeZoneLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask (UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Flee")
	FBlackboardKeySelector NearestPurgeZoneKey;

	UPROPERTY(EditAnywhere, Category="Flee")
	float FleeDistance = 2000.f;
};