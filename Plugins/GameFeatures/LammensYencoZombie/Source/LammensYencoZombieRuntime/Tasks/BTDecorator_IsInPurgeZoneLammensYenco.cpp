#include "BTDecorator_IsInPurgeZoneLammensYenco.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInPurgeZoneLammensYenco::UBTDecorator_IsInPurgeZoneLammensYenco()
{
	NodeName = TEXT("Is In Purge Zone");
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UBTDecorator_IsInPurgeZoneLammensYenco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;
	return BB->GetValueAsBool(bIsInPurgeZoneKey.SelectedKeyName);
}