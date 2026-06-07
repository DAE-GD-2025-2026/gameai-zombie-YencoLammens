#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_ShootZombieLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTTask_ShootZombieLammensYenco : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ShootZombieLammensYenco();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category="Combat")
	FBlackboardKeySelector NearestZombieKey;

	UPROPERTY(EditAnywhere, Category="Combat")
	FBlackboardKeySelector bHasWeaponKey;

	UPROPERTY(EditAnywhere, Category="Combat")
	float ShootRange = 400.f;
};