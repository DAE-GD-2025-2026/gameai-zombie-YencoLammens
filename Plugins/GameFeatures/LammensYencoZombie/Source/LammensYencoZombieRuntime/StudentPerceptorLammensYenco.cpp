// Fill out your copyright notice in the Description page of Project Settings.

#include "StudentPerceptorLammensYenco.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "LammensYencoZombieRuntime/SurvivorAPIHelpersLammensYenco.h"

namespace BBKeys
{
    const FName NearestZombie = TEXT("NearestZombie");
    const FName NearestWeapon = TEXT("NearestWeapon");
    const FName NearestItem = TEXT("NearestItem");
    const FName NearestHouse = TEXT("NearestHouse");
    const FName NearestPurgeZone = TEXT("NearestPurgeZone");
    const FName bIsInPurgeZone = TEXT("bIsInPurgeZone");
    const FName bHasWeapon = TEXT("bHasWeapon");
    const FName HealthValue = TEXT("HealthValue");
    const FName EnergyValue = TEXT("EnergyValue");
}

UStudentPerceptorLammensYenco::UStudentPerceptorLammensYenco()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f;
}

void UStudentPerceptorLammensYenco::BeginPlay()
{
    Super::BeginPlay();
    TryBindPerception();
}

void UStudentPerceptorLammensYenco::TryBindPerception()
{
    if (bPerceptionBound) return;

    UAIPerceptionComponent* PC = GetOwner()->GetComponentByClass<UAIPerceptionComponent>();

    if (!PC)
    {
        APawn* Pawn = Cast<APawn>(GetOwner());
        if (Pawn)
        {
            if (AAIController* AIC = Cast<AAIController>(Pawn->GetController()))
                PC = AIC->GetComponentByClass<UAIPerceptionComponent>();
        }
    }

    if (!PC) return;

    UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>(PC);
    SightConfig->SightRadius = 1500.f;
    SightConfig->LoseSightRadius = 1800.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    PC->ConfigureSense(*SightConfig);

    UAISenseConfig_Damage* DamageConfig = NewObject<UAISenseConfig_Damage>(PC);
    DamageConfig->SetMaxAge(5.f);
    PC->ConfigureSense(*DamageConfig);

    PC->RequestStimuliListenerUpdate();
    PC->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorLammensYenco::OnPerceptionUpdated);
    bPerceptionBound = true;
    PerceptionComp = PC;
}

void UStudentPerceptorLammensYenco::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TryBindPerception();

    VisibleZombies.RemoveAll([](const TObjectPtr<AActor>& A) { return !IsValid(A); });
    KnownItems.RemoveAll([](const TObjectPtr<AActor>& A) { return !IsValid(A); });
    KnownWeapons.RemoveAll([](const TObjectPtr<AActor>& A) { return !IsValid(A); });
    KnownHouses.RemoveAll([](const TObjectPtr<AActor>& A) { return !IsValid(A); });
    ActivePurgeZones.RemoveAll([](const TObjectPtr<AActor>& A) { return !IsValid(A); });
    
    if (PerceptionComp)
        {
            TArray<AActor*> Perceived;
            PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Perceived);
            for (AActor* A : Perceived)
            {
                if (IsValid(A) && IsZombie(A))
                {
                    VisibleZombies.AddUnique(A);
                    LastZombieSeenTime = GetWorld()->GetTimeSeconds();
                }
            }
        }
    
        if (!VisibleZombies.IsEmpty() && GetWorld()->GetTimeSeconds() - LastZombieSeenTime > 3.f)
            VisibleZombies.Empty();
    
        const FVector OwnerPos = GetOwner()->GetActorLocation();
        KnownHouses.RemoveAll([&](const TObjectPtr<AActor>& A) {
            return !IsValid(A) || FVector::DistSquared(OwnerPos, A->GetActorLocation()) < FMath::Square(150.f);
        });

    UpdateBlackboardKeys();
}

void UStudentPerceptorLammensYenco::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!IsValid(Actor)) return;

    if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())
    {
        if (IsZombie(Actor) && Stimulus.WasSuccessfullySensed())
        {
            VisibleZombies.AddUnique(Actor);
            LastZombieSeenTime = GetWorld()->GetTimeSeconds();
        }
        UpdateBlackboardKeys();
        return;
    }

    const bool bSensed = Stimulus.WasSuccessfullySensed();

    if (IsZombie(Actor))
    {
        if (bSensed)
        {
            VisibleZombies.AddUnique(Actor);
            LastZombieSeenTime = GetWorld()->GetTimeSeconds();
        }
    }
    else if (IsWeapon(Actor))
    {
        if (bSensed) KnownWeapons.AddUnique(Actor);
        else KnownWeapons.Remove(Actor);
    }
    else if (IsItem(Actor))
    {
        if (bSensed) KnownItems.AddUnique(Actor);
        else KnownItems.Remove(Actor);
    }
    else if (IsHouse(Actor))
    {
        if (bSensed) KnownHouses.AddUnique(Actor);
    }
    else if (IsPurgeZone(Actor))
    {
        if (bSensed) ActivePurgeZones.AddUnique(Actor);
        else ActivePurgeZones.Remove(Actor);
    }

    UpdateBlackboardKeys();
}

UBlackboardComponent* UStudentPerceptorLammensYenco::GetBlackboard()
{
    if (BlackboardComp) return BlackboardComp;

    if (APawn* Pawn = Cast<APawn>(GetOwner()))
        if (AAIController* AIC = Cast<AAIController>(Pawn->GetController()))
            BlackboardComp = AIC->GetBlackboardComponent();

    return BlackboardComp;
}

void UStudentPerceptorLammensYenco::UpdateBlackboardKeys()
{
    UBlackboardComponent* BB = GetBlackboard();
    if (!BB) return;

    BB->SetValueAsObject(BBKeys::NearestZombie, FindNearest(VisibleZombies));
    BB->SetValueAsObject(BBKeys::NearestWeapon, FindNearest(KnownWeapons));
    BB->SetValueAsObject(BBKeys::NearestItem, FindNearest(KnownItems));
    BB->SetValueAsObject(BBKeys::NearestHouse, FindNearest(KnownHouses));
    BB->SetValueAsObject(BBKeys::NearestPurgeZone, FindNearest(ActivePurgeZones));
    BB->SetValueAsBool(BBKeys::bIsInPurgeZone, !ActivePurgeZones.IsEmpty());

    if (APawn* Pawn = Cast<APawn>(GetOwner()))
    {
        BB->SetValueAsFloat(BBKeys::HealthValue, SurvivorAPI::GetHealthRatio(Pawn));
        BB->SetValueAsFloat(BBKeys::EnergyValue, SurvivorAPI::GetStaminaRatio(Pawn));
        
        if (UActorComponent* Inv = SurvivorAPI::FindComp(Pawn, TEXT("Inventory")))
            BB->SetValueAsBool(BBKeys::bHasWeapon, SurvivorAPI::FindWeaponSlot(Inv) != -1);
    }
}

AActor* UStudentPerceptorLammensYenco::FindNearest(const TArray<TObjectPtr<AActor>>& Actors) const
{
    AActor* Nearest = nullptr;
    float BestDist = FLT_MAX;
    const FVector OwnerPos = GetOwner()->GetActorLocation();

    for (const TObjectPtr<AActor>& Actor : Actors)
    {
        if (!IsValid(Actor)) continue;
        const float Dist = FVector::DistSquared(OwnerPos, Actor->GetActorLocation());
        if (Dist < BestDist) { BestDist = Dist; Nearest = Actor; }
    }
    return Nearest;
}

bool UStudentPerceptorLammensYenco::IsZombie(AActor* A) const { return A->GetClass()->GetName().Contains(TEXT("Zombie")); }
bool UStudentPerceptorLammensYenco::IsWeapon(AActor* A) const { const FString N = A->GetClass()->GetName(); return N.Contains(TEXT("Pistol")) || N.Contains(TEXT("Shotgun")); }
bool UStudentPerceptorLammensYenco::IsItem(AActor* A) const { return A->GetClass()->GetName().Contains(TEXT("Item")); }
bool UStudentPerceptorLammensYenco::IsHouse(AActor* A) const { return A->GetClass()->GetName().Contains(TEXT("House")); }
bool UStudentPerceptorLammensYenco::IsPurgeZone(AActor* A) const { return A->GetClass()->GetName().Contains(TEXT("Purge")); }