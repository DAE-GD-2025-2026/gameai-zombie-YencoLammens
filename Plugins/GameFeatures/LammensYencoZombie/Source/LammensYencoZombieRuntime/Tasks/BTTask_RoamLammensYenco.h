#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RoamLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_RoamLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RoamLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask (UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Roam")
	float SearchRadius = 1500.f;
};