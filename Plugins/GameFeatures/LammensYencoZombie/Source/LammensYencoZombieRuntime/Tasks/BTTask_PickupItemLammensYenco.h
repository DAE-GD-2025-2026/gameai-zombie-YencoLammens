#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_PickupItemLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_PickupItemLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PickupItemLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category="Item")
	FBlackboardKeySelector NearestItemKey;

	UPROPERTY(EditAnywhere, Category="Item")
	FBlackboardKeySelector bHasWeaponKey;
};
