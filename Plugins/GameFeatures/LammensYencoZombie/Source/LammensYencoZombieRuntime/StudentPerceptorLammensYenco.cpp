// Fill out your copyright notice in the Description page of Project Settings.

#include "StudentPerceptorLammensYenco.h"

UStudentPerceptorLammensYenco::UStudentPerceptorLammensYenco()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptorLammensYenco::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorLammensYenco::OnPerceptionUpdated);
	}
}

void UStudentPerceptorLammensYenco::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
		FString::Printf(TEXT("Saw Something!")));
}
