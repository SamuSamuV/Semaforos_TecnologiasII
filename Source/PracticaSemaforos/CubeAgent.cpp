#include "CubeAgent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TargetPoint.h"
#include "UObject/ConstructorHelpers.h"

ACubeAgent::ACubeAgent()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded()) MeshComponent->SetStaticMesh(CubeMesh.Object);

    MeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
}

void ACubeAgent::BeginPlay()
{
    Super::BeginPlay();

    if (TargetPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CubeAgent no tiene Target Points asignados!"));
    }
}

void ACubeAgent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (MovementSpeed > 0.f && TargetPoints.Num() > 0)
    {
        MoveToTarget(DeltaTime);
    }
}

void ACubeAgent::MoveToTarget(float DeltaTime)
{
    const ATargetPoint* CurrentTarget = TargetPoints[CurrentTargetIndex];
    if (!CurrentTarget) return;

    const FVector Direccion = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    const FVector NuevaPosicion = GetActorLocation() + Direccion * MovementSpeed * DeltaTime;

    SetActorLocation(NuevaPosicion);

    if (FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation()) < CloseEnoughDistance)
    {
        CurrentTargetIndex = (CurrentTargetIndex + 1) % TargetPoints.Num();
    }
}