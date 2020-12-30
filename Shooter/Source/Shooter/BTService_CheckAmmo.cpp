// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckAmmo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

UBTService_CheckAmmo::UBTService_CheckAmmo()
{
    NodeName = TEXT("Check If Ammo Left");
}

void UBTService_CheckAmmo::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AShooterCharacter *Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (Character == nullptr)
    {
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("I have ammo!!!"));
    bool bHasAmmo = Character->HasAmmo();
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bHasAmmo);
}
