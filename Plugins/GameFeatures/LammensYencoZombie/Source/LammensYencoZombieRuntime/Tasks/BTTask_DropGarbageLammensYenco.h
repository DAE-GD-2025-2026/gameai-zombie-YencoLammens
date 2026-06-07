#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_DropGarbageLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_DropGarbageLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DropGarbageLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category="Item")
	FBlackboardKeySelector NearestItemKey;
};