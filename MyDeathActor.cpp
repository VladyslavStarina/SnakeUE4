#include "MyDeathActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Components/BoxComponent.h"
#include "MySnakeActor.h"

// Sets default values
AMyDeathActor::AMyDeathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OurRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootModel"));
	
	UStaticMesh* WallMesh;
	WallMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object;
	WallColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("/Game/SnakeContent/Materials/Danger_Inst.Danger_Inst")).Get();

	UStaticMeshComponent* WallChunk;
	WallChunk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	OurRootComponent->SetupAttachment(WallChunk);
	WallChunk->SetStaticMesh(WallMesh);
	WallChunk->SetRelativeLocation(FVector(0,0,0));
	WallChunk->SetMaterial(0, WallColor);
	WallChunk->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AMyDeathActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyDeathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CrashingWithWall();
}

void AMyDeathActor::CrashingWithWall()
{
	TArray<AActor*> CollectedActors;
	GetOverlappingActors(CollectedActors);

	for (int32 i = 0; i < CollectedActors.Num(); ++i)
	{
		AMySnakeActor* const Snake = Cast<AMySnakeActor>(CollectedActors[i]);
		AMyDeathActor*const Death = Cast<AMyDeathActor>(CollectedActors[i]);
		if (Snake)
		{
			Snake->HaveDamage();
		}
		else if(!Death)
		{
			CollectedActors[i]->Destroy(true, true);
		}
	}
}

