#include "MyFruitActor.h"
#include "MySnakeActor.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMyFruitActor::AMyFruitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OurRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootFood"));
	RootComponent = OurRootComponent;
	UStaticMesh* FoodMesh;
		
	float foodRate = FMath::FRand();
	if (foodRate > 0.3f)
	{
		FoodMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Platformer/Meshes/Pl_PowerUp_01.Pl_PowerUp_01")).Object;
		AmountOfFood = 1;
	}
	else
	{
		FoodMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Platformer/Meshes/Pl_PowerUp_06.Pl_PowerUp_06")).Object;
		AmountOfFood = -2;
	}
	
	LifeCycleTimer = 0.f;
	FVector Size = FVector(1.f, 1.f, 1.f);
	FoodChank->SetRelativeScale3D(Size);
	FoodChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Food"));
	FoodChank->SetStaticMesh(FoodMesh);
	FoodChank->SetRelativeLocation(FVector(0, 0, 0));
	FoodChank->SetupAttachment(OurRootComponent);
}

// Called when the game starts or when spawned
void AMyFruitActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyFruitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeCycleTimer += DeltaTime;
	LifeCycle();
	FoodCollector();
	FoodChank->AddLocalRotation(FRotator(5, 0, 0),false, nullptr,ETeleportType::None);
}

void AMyFruitActor::FoodCollector()
{
	TArray<AActor*> CollectedActors;
	GetOverlappingActors(CollectedActors);

	for (int32 i = 0; i < CollectedActors.Num(); ++i)
	{
		AMySnakeActor* const Snake = Cast<AMySnakeActor>(CollectedActors[i]);

		if (Snake && CanGrow(Snake))
		{
			if (Snake->VisibleBodyChunk + AmountOfFood < 1)
			{
				Snake->HaveDamage();
			}
			else
			{
				Snake->GrowVisibleBodyChunkOn(AmountOfFood);
			}	
			Destroy(true, true);
			break;
		}
	}
}

bool AMyFruitActor::CanGrow(AMySnakeActor* const Snake)
{
	if(Snake->VisibleBodyChunk + AmountOfFood <= Snake->SnakeSize)
		return true;
	else 
		return false;
}

void AMyFruitActor::LifeCycle()
{
	if (LifeCycleTimer > LifeCycleDeley)
	{
		Destroy(true, true);
	}
}


