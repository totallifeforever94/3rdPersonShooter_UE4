// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();

	void Reload();

	FString GetAmmoLeft() const;

	bool HasAmmo() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase *MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem *ShotImpact;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase *ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo = 120;

	UPROPERTY(EditDefaultsOnly)
	int32 InitialAmmoPerMagazine = 30;

	int32 CurrentAmmoInMagazine;

	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);

	AController *GetOwnerController() const;
};
