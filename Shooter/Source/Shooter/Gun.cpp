// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoInMagazine = InitialAmmoPerMagazine;
}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController *AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}

	AController *OwnerController = OwnerPawn->GetController();

	return OwnerController;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	if (CurrentAmmoInMagazine > 0)
	{
		CurrentAmmoInMagazine--;
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

		FHitResult Hit;
		FVector ShotDirection;
		if (GunTrace(Hit, ShotDirection))
		{

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotImpact, Hit.Location, ShotDirection.Rotation());
			AActor *HitActor = Hit.GetActor();

			if (HitActor != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController *OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
		}
	}
}

void AGun::Reload()
{
	if (MaxAmmo - InitialAmmoPerMagazine < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shit, out of ammo!!!"));
		return;
	}

	MaxAmmo -= InitialAmmoPerMagazine;
	CurrentAmmoInMagazine = InitialAmmoPerMagazine;
}

FString AGun::GetAmmoLeft() const
{
	return FString::Printf(TEXT("%i / %i"), CurrentAmmoInMagazine, MaxAmmo);
}

bool AGun::HasAmmo() const
{
	return CurrentAmmoInMagazine > 0 || MaxAmmo > 0;
}
