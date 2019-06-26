#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyDeathActor.generated.h"

UCLASS()
class MYSNAKE_API AMyDeathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDeathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* OurRootComponent;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* WallColor;

	void CrashingWithWall();

};
