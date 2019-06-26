#include "MySnakeActor.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "MyPawnCamera.h"


// Sets default values
AMySnakeActor::AMySnakeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OurRootComponent = CreateDefaultSubobject<UBoxComponent>("OurRootComponent");
	RootComponent = OurRootComponent;

	Score = 0;
	VisibleBodyChunk = 3;
	SnakeStepTimer = 0;
	bSnakeStop = true;


	CreateSnakeBody();
}


// Called when the game starts or when spawned
void AMySnakeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySnakeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetSnakeVisibility();

	SnakeStepTimer += DeltaTime;

	MoveSnake();
	CrashingWithBody();
}

void AMySnakeActor::SetSnakeStop(bool canStop)
{
	bSnakeStop = canStop;
}

void AMySnakeActor::HaveDamage()
{
	WhoPawn->SnakeDestroy();
}

void AMySnakeActor::CrashingWithBody()
{
	for (int i = 0; i < VisibleBodyChunk; i++)
	{
		if (SnakeBody[0]->K2_GetComponentLocation() == SnakeBody[i]->K2_GetComponentLocation())
			HaveDamage();
	}
}

void AMySnakeActor::GrowVisibleBodyChunkOn(int32 countOfFood)
{
	VisibleBodyChunk += countOfFood;
	Score += countOfFood;
}

void AMySnakeActor::CreateSnakeBody()
{
	// find mesh
	UStaticMesh* SnakeBodyMesh;
	SnakeBodyMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/SnakeContent/TexturesLowPoly/Rocks/Rock_01.Rock_01")).Object;

	// find body color
	UMaterialInstance* BodyColor;
	BodyColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("/Game/Platformer/Materials/M_Plastic_blue_Inst.M_Plastic_blue_Inst")).Get();

	// find head color
	UMaterialInstance* HeadColor;
	HeadColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("/Game/Platformer/Materials/M_Plastic_Orange_Inst.M_Plastic_Orange_Inst")).Get();

	FVector NextPoint = GetActorLocation();
	// Create snake head
	UStaticMeshComponent* SnakeHead = CreateDefaultSubobject<UStaticMeshComponent>("Chank0");
	SetBodyChunk(SnakeHead, HeadColor, SnakeBodyMesh, NextPoint);
	

	// Create snake body
	for (int32 i = 1; i < SnakeSize; i++)
	{
		FName PartName = NamesGenerator("Chank", i);

		UStaticMeshComponent* BodyChank = CreateDefaultSubobject<UStaticMeshComponent>(PartName);
		NextPoint.X -= SnakeStep;
		
		SetBodyChunk(BodyChank, BodyColor, SnakeBodyMesh, NextPoint);
		BodyChank->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SetSnakeVisibility();
}

FName AMySnakeActor::NamesGenerator(FString String, int32 number)
{
	String += FString::FromInt(number);
	return FName(*String);
}

void AMySnakeActor::SetBodyChunk
(
	UStaticMeshComponent* BodyPart,
	UMaterialInterface* _Color,
	UStaticMesh* SnakeBodyMesh, 
	FVector NextPoint
)
{
	BodyPart->SetStaticMesh(SnakeBodyMesh);
	BodyPart->SetRelativeLocation(NextPoint);
	BodyPart->SetupAttachment(OurRootComponent);
	BodyPart->SetMaterial(0, _Color);
	BodyPart->SetSimulatePhysics(true);
	SnakeBody.Add(BodyPart);
}

void AMySnakeActor::SetSnakeVisibility()
{
	// set visible part of snake
	for (int32 i = 0; i < VisibleBodyChunk; i++)
	{
		SnakeBody[i]->SetVisibility(true, true);
	}

	// set unvisible part of snake
	for (int32 i = VisibleBodyChunk; i < SnakeSize; i++)
	{
		SnakeBody[i]->SetVisibility(false, true);
	}
}

void AMySnakeActor::MoveSnake()
{
	if(!bSnakeStop)
	{
		if (SnakeStepTimer > SnakeStepDelay)
		{
			TailMovementInReverseOrder();
			HeadMovement();
			SnakeStepTimer = 0;
		}
	}
}

void AMySnakeActor::TailMovementInReverseOrder()
{
	for (int Chunk = SnakeBody.Num() - 1; Chunk > 0; Chunk--)
	{
		FRotator NewRotation = SnakeBody[Chunk - 1]->RelativeRotation;
		FVector NewLocation = SnakeBody[Chunk - 1]->RelativeLocation;

		SnakeBody[Chunk]->SetRelativeRotation(NewRotation);
		SnakeBody[Chunk]->SetRelativeLocation(NewLocation);
	}
}

void AMySnakeActor::HeadMovement()
{
	FVector NewLocation = SnakeBody[0]->RelativeLocation;
	FRotator NewRotation(0,0,0);
	
	NewHeadDirection(NewLocation, NewRotation);

	SnakeBody[0]->SetRelativeLocation(NewLocation);
	SnakeBody[0]->SetRelativeRotation(NewRotation);
}

void AMySnakeActor::NewHeadDirection(FVector & RelativeLocation, FRotator & RelativeRotation)
{
	switch (DirectionMoveSnake)
	{
	case Turn::Right:
		RelativeLocation.Y += SnakeStep;
		SetNewSnakeYawn(RelativeRotation, 90.f);
		break;

	case Turn::Left:
		RelativeLocation.Y -= SnakeStep;
		SetNewSnakeYawn(RelativeRotation, 90.f);
		break;

	case Turn::Up:
		RelativeLocation.X += SnakeStep;
		SetNewSnakeYawn(RelativeRotation, 0.f);
		break;

	case Turn::Down:
		RelativeLocation.X -= SnakeStep;
		SetNewSnakeYawn(RelativeRotation, 0.f);
		break;
	default:
		break;
	}
}

void AMySnakeActor::SetNewSnakeYawn(FRotator & RelativeRotation, float Rotation)
{
	RelativeRotation.Yaw = Rotation;
}


