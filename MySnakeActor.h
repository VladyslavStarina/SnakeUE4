#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySnakeActor.generated.h"

UENUM()
enum class Turn : uint8 
{ Left, Right, Up, Down };

UCLASS()
class MYSNAKE_API AMySnakeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySnakeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	bool bSnakeStop;
	
	float SnakeStepTimer;
	
	void CreateSnakeBody();
	
	FName NamesGenerator(FString String, int32 number);
	
	void SetBodyChunk
	(
		UStaticMeshComponent* BodyPart,
		UMaterialInterface* _Color, 
		UStaticMesh* SnakeBodyMesh,
		FVector NextPoint
	);

	void SetSnakeVisibility();

	void MoveSnake();
	
	void TailMovementInReverseOrder();
	
	void HeadMovement();
	
	void NewHeadDirection(FVector & RelativeLocation, FRotator & RelativeRotation);

	void SetNewSnakeYawn(FRotator & RelativeRotation, float Rotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AMyPawnCamera* WhoPawn;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* OurRootComponent;

	UPROPERTY(EditAnywhere)
	TArray <UStaticMeshComponent*> SnakeBody;

	UPROPERTY(EditAnywhere)
	const int32 SnakeSize = 200;

	UPROPERTY(EditAnywhere)
	int32 VisibleBodyChunk;

	UPROPERTY(EditAnywhere)
	Turn DirectionMoveSnake;

	UPROPERTY(EditAnywhere)
	const float SnakeStep = 40.f;

	int32 Score;

	const float SnakeStepDelay = 0.06f;

	void SetSnakeStop(bool stop);

	void HaveDamage();

	void CrashingWithBody();

	void GrowVisibleBodyChunkOn(int32 countOfFood);

};
