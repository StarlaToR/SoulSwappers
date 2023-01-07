#pragma once
#include "CoreMinimal.h"
#include "Laser.h"
#include "Weapon.h"
#include "Rocket.h"
#include "Gatling.h"
#include "Shotgun.h"
#include "GameFramework/Actor.h"
#include "GearsManager.generated.h"

enum WeaponType
{
	GATLING,
	ROCKET,
	LASER,
	SHOTGUN,
};

UCLASS()
class MAGIC_API AGearsManager : public AActor
{
	GENERATED_BODY()

	bool beginGame;

	Rocket* rocket;
	Gatling* gatling;
	Shotgun* shotgun;

public:	
	AGearsManager();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetWeaponSound();

	Laser* laser;
	WeaponType weaponType;

	void SetWeapon(WeaponType type);
	void SetRandomWeapon(bool deactiveLaser = false);

	Weapon* GetWeapon() { return weapon; }

	virtual void Tick(float deltaTime) override;

protected:
	Weapon* weapon;

	virtual void BeginPlay() override;
};