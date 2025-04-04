// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Math/UnrealMathUtility.h"
#include "SlashCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HitInterface.h"
#include "NiagaraComponent.h"


AWeapon::AWeapon()
{
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    WeaponBox->SetupAttachment(GetRootComponent());
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    if (IsValid(WeaponBox))
    {
        WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
    }
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
  
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner,APawn* NewInstigator)
{
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);
    AttachMeshToSocket(InParent, InSocketName);
    ItemState = EItemState::EIS_Equipped;
    
    if(IsValid(EmbersEffect))
    {
        EmbersEffect->Deactivate();
    }
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    for (AActor* Actor:IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }
    FHitResult BoxHit;
    UKismetSystemLibrary::BoxTraceSingle(
        this,
        Start,
        End,
        FVector(5.f, 5.f, 5.f),
        BoxTraceStart->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        BoxHit,
        true);
    //for drawing sphere debug macro implemented in enemy
    if (BoxHit.GetActor())
    {
        CreateFields(BoxHit.ImpactPoint);

        UGameplayStatics::ApplyDamage(
        BoxHit.GetActor(),
        Damage,
        GetInstigator()->GetController(),
        this,
        UDamageType::StaticClass()
            );
        
        IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
        if (HitInterface)
        {
            HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
        }
        IgnoreActors.AddUnique(BoxHit.GetActor());
    }

}




