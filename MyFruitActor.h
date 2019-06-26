#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyFruitActor.generated.h"

class AMySnakeActor;

UCLASS()
class MYSNAKE_API AMyFruitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyFruitActor();

private:

	float LifeCycleTimer;
	void FoodCollector();
	bool CanGrow(AMySnakeActor* const Snake);
	void LifeCycle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* OurRootComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FoodChank;

	UPROPERTY(EditAnywhere)
	int32 AmountOfFood;

	UPROPERTY(EditAnywhere)
	const float LifeCycleDeley = 10.f;

};
