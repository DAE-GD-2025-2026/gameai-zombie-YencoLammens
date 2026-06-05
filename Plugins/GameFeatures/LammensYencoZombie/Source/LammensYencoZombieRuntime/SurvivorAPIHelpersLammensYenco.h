#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/UnrealType.h"

namespace SurvivorAPI
{
    inline UActorComponent* FindComp(APawn* Pawn, const TCHAR* Fragment)
    {
        for (UActorComponent* C : Pawn->GetComponents())
            if (C->GetClass()->GetName().Contains(Fragment))
                return C;
        return nullptr;
    }

    inline int32 FindFreeSlot(UActorComponent* Inv)
    {
        FArrayProperty* Prop = FindFProperty<FArrayProperty>(Inv->GetClass(), TEXT("Items"));
        if (!Prop) return -1;
        FScriptArrayHelper Helper(Prop, Prop->ContainerPtrToValuePtr<void>(Inv));
        FObjectProperty* Elem = CastField<FObjectProperty>(Prop->Inner);
        if (!Elem) return -1;
        for (int32 i = 0; i < Helper.Num(); ++i)
            if (!IsValid(Elem->GetObjectPropertyValue(Helper.GetRawPtr(i))))
                return i;
        return -1;
    }

    inline int32 FindWeaponSlot(UActorComponent* Inv)
    {
        FArrayProperty* Prop = FindFProperty<FArrayProperty>(Inv->GetClass(), TEXT("Items"));
        if (!Prop) return -1;
        FScriptArrayHelper Helper(Prop, Prop->ContainerPtrToValuePtr<void>(Inv));
        FObjectProperty* Elem = CastField<FObjectProperty>(Prop->Inner);
        if (!Elem) return -1;
        for (int32 i = 0; i < Helper.Num(); ++i)
        {
            UObject* Obj = Elem->GetObjectPropertyValue(Helper.GetRawPtr(i));
            if (!IsValid(Obj)) continue;
            const FString N = Obj->GetClass()->GetName();
            if (N.Contains(TEXT("Pistol")) || N.Contains(TEXT("Shotgun")))
                return i;
        }
        return -1;
    }

    inline int32 GetSlotValue(UActorComponent* Inv, int32 Slot)
    {
        FArrayProperty* Prop = FindFProperty<FArrayProperty>(Inv->GetClass(), TEXT("Items"));
        if (!Prop) return 0;
        FScriptArrayHelper Helper(Prop, Prop->ContainerPtrToValuePtr<void>(Inv));
        FObjectProperty* Elem = CastField<FObjectProperty>(Prop->Inner);
        if (!Elem || Slot >= Helper.Num()) return 0;
        UObject* Item = Elem->GetObjectPropertyValue(Helper.GetRawPtr(Slot));
        if (!IsValid(Item)) return 0;
        if (FIntProperty* IP = FindFProperty<FIntProperty>(Item->GetClass(), TEXT("Value")))
            return IP->GetPropertyValue_InContainer(Item);
        if (FFloatProperty* FP = FindFProperty<FFloatProperty>(Item->GetClass(), TEXT("Value")))
            return static_cast<int32>(FP->GetPropertyValue_InContainer(Item));
        return 0;
    }

    inline bool CallGrabItem(UActorComponent* Inv, int32 Slot, AActor* Item)
    {
        UFunction* F = Inv->FindFunction(TEXT("GrabItem"));
        if (!F) return false;
        struct FP { int32 SlotIdx; UObject* ItemPtr; bool ReturnValue; };
        FP P{ Slot, Item, false };
        Inv->ProcessEvent(F, &P);
        return P.ReturnValue;
    }

    inline bool CallUseItem(UActorComponent* Inv, int32 Slot)
    {
        UFunction* F = Inv->FindFunction(TEXT("UseItem"));
        if (!F) return false;
        struct FP { int32 SlotIdx; bool ReturnValue; };
        FP P{ Slot, false };
        Inv->ProcessEvent(F, &P);
        return P.ReturnValue;
    }

    inline bool CallRemoveItem(UActorComponent* Inv, int32 Slot)
    {
        UFunction* F = Inv->FindFunction(TEXT("RemoveItem"));
        if (!F) return false;
        struct FP { int32 SlotIdx; bool ReturnValue; };
        FP P{ Slot, false };
        Inv->ProcessEvent(F, &P);
        return P.ReturnValue;
    }

    inline float GetHealthRatio(APawn* Pawn)
    {
        UActorComponent* HC = FindComp(Pawn, TEXT("Health"));
        if (!HC) return 1.f;
        struct FInt { int32 R; };
        FInt MaxP{ 0 }, CurP{ 0 };
        if (UFunction* F = HC->FindFunction(TEXT("GetMaxHealth"))) HC->ProcessEvent(F, &MaxP);
        if (MaxP.R == 0) return 1.f;
        if (UFunction* F = HC->FindFunction(TEXT("GetHealth")))    HC->ProcessEvent(F, &CurP);
        return static_cast<float>(CurP.R) / static_cast<float>(MaxP.R);
    }

    inline float GetStaminaRatio(APawn* Pawn)
    {
        UActorComponent* SC = FindComp(Pawn, TEXT("Stamina"));
        if (!SC) return 1.f;
        struct FFloat { float R; };
        FFloat MaxP{ 0.f }, CurP{ 0.f };
        if (UFunction* F = SC->FindFunction(TEXT("GetMaxStamina")))     SC->ProcessEvent(F, &MaxP);
        if (MaxP.R == 0.f) return 1.f;
        if (UFunction* F = SC->FindFunction(TEXT("GetCurrentStamina"))) SC->ProcessEvent(F, &CurP);
        return CurP.R / MaxP.R;
    }
}
