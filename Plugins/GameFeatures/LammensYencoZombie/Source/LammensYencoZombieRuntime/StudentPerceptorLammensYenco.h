// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptorLammensYenco.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LAMMENSYENCOZOMBIERUNTIME_API UStudentPerceptorLammensYenco : public UActorComponent
{
	GENERATED_BODY()

public:
	UStudentPerceptorLammensYenco();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryBindPerception();
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> VisibleZombies;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> KnownItems;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> KnownWeapons;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> KnownHouses;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActivePurgeZones;

	float LastZombieSeenTime = -1.f;
	bool bPerceptionBound = false;

	UBlackboardComponent* GetBlackboard();
	void UpdateBlackboardKeys();
	AActor* FindNearest(const TArray<TObjectPtr<AActor>>& Actors) const;

	bool IsZombie(AActor* Actor) const;
	bool IsWeapon(AActor* Actor) const;
	bool IsItem(AActor* Actor) const;
	bool IsHouse(AActor* Actor) const;
	bool IsPurgeZone(AActor* Actor) const;
};