#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "SemaforoController.generated.h"

UCLASS()
class PRACTICASEMAFOROS_API ASemaforoController : public AActor
{
    GENERATED_BODY()

public:
    ASemaforoController();

    UFUNCTION(BlueprintCallable, Category = "Traffic Light")
    bool IsLightGreen() const { return bIsLightGreen; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Control")
    USpotLightComponent* SpotLight;

    UPROPERTY(VisibleInstanceOnly, Category = "Traffic Light")
    bool bIsLightGreen;

    UPROPERTY(VisibleInstanceOnly, Category = "Traffic Light")
    TArray<class ACubeAgent*> CubosEnTransito;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ActualizarEstadoSemaforo();

public:
    virtual void Tick(float DeltaTime) override;
};