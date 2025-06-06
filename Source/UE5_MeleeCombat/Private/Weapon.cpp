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

void AWeapon::DeactivateEmbers()
{
    if(IsValid(ItemEffect))
    {
        ItemEffect->Deactivate();
    }
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner,APawn* NewInstigator)
{
    ItemState = EItemState::EIS_Equipped;
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);
    AttachMeshToSocket(InParent, InSocketName);
    DeactivateEmbers();
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::ExecuteGetHit(FHitResult BoxHit)
{
    IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
    if (HitInterface)
    {
        HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
    }
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
    return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ActorIsSameType(OtherActor))return;
    FHitResult BoxHit;
    BoxTrace(BoxHit);
    
    if (BoxHit.GetActor())
    {
        if (ActorIsSameType(BoxHit.GetActor()))return;
        UGameplayStatics::ApplyDamage(BoxHit.GetActor(),Damage,GetInstigator()->GetController(),this,UDamageType::StaticClass());
        ExecuteGetHit(BoxHit);
        CreateFields(BoxHit.ImpactPoint);
    }

}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetOwner());

    // Ignore the owner and instigator
    if (GetOwner())
    {
        ActorsToIgnore.Add(GetOwner());
    }
    if (GetInstigator())
    {
        ActorsToIgnore.Add(GetInstigator());
    }
    for (AActor* Actor:IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }
    UKismetSystemLibrary::BoxTraceSingle(
        this,
        Start,
        End,
        BoxTraceExtent,
        BoxTraceStart->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore,
       bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        BoxHit,
        true);

    IgnoreActors.AddUnique(BoxHit.GetActor());
}




