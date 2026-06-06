#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_FleeFromZombieLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_FleeFromZombieLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FleeFromZombieLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask (UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Flee")
	FBlackboardKeySelector NearestZombieKey;

	UPROPERTY(EditAnywhere, Category="Flee")
	float FleeDistance = 1500.f;
};