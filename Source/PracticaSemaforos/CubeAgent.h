#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TargetPoint.h"
#include "CubeAgent.generated.h"

UCLASS()
class PRACTICASEMAFOROS_API ACubeAgent : public AActor
{
    GENERATED_BODY()

public:
    ACubeAgent();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementSpeed(float NewSpeed) { MovementSpeed = NewSpeed; }

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MovementSpeed = 300.f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float CloseEnoughDistance = 100.f;

    UPROPERTY(EditAnywhere, Category = "Target Points")
    TArray<class ATargetPoint*> TargetPoints;

private:
    int32 CurrentTargetIndex = 0;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    void MoveToTarget(float DeltaTime);
};