#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInPurgeZoneLammensYenco.generated.h"

UCLASS()
class LAMMENSYENCOZOMBIERUNTIME_API UBTDecorator_IsInPurgeZoneLammensYenco : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInPurgeZoneLammensYenco();

	UPROPERTY(EditAnywhere, Category="Condition")
	FBlackboardKeySelector bIsInPurgeZoneKey;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};