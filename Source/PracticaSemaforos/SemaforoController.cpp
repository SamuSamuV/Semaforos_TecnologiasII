#include "SemaforoController.h"
#include "Engine/Engine.h"
#include "CubeAgent.h"

ASemaforoController::ASemaforoController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Configurar Trigger
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(300.f, 300.f, 200.f));
    TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    // Configurar Luz
    SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
    SpotLight->SetupAttachment(RootComponent);
    SpotLight->SetLightColor(FLinearColor::Green);
    SpotLight->SetIntensity(10000.f);

    bIsLightGreen = true;
}

void ASemaforoController::BeginPlay()
{
    Super::BeginPlay();
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASemaforoController::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASemaforoController::OnOverlapEnd);
}

void ASemaforoController::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (ACubeAgent* Cube = Cast<ACubeAgent>(OtherActor))
    {
        // Primer cubo en entrar activa el semáforo en rojo pero sigue moviéndose
        if (CubosEnTransito.IsEmpty())
        {
            bIsLightGreen = false;
            SpotLight->SetLightColor(FLinearColor::Red);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SEMÁFORO ROJO - Cubo prioritario pasando"));
        }

        CubosEnTransito.Add(Cube);
    }
}

void ASemaforoController::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (ACubeAgent* Cube = Cast<ACubeAgent>(OtherActor))
    {
        CubosEnTransito.Remove(Cube);
        ActualizarEstadoSemaforo();
    }
}

void ASemaforoController::ActualizarEstadoSemaforo()
{
    if (CubosEnTransito.IsEmpty())
    {
        bIsLightGreen = true;
        SpotLight->SetLightColor(FLinearColor::Green);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SEMÁFORO VERDE - Vía libre"));
    }
}

void ASemaforoController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Aplicar lógica de movimiento a los cubos
    for (ACubeAgent* Cube : CubosEnTransito)
    {
        // Solo el primer cubo puede moverse libremente
        const bool bEsPrimerCube = (Cube == CubosEnTransito[0]);
        Cube->SetMovementSpeed(bEsPrimerCube ? 1000.f : 0.f);
    }
}