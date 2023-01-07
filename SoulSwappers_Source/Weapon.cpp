#include "Weapon.h"
#include "PlayerSpirit.h"
#include "GearsManager.h"


Weapon::Weapon() {}

Weapon::~Weapon() {}

void Weapon::ResetCooldownShoot() { cooldownShoot = 0; }

void Weapon::EndShoot(APlayerController* spiritController) {}

void Weapon::StartShoot(APlayerController* spiritController) {}

void Weapon::SpawnBullet(APlayerController* spiritController) {}

void Weapon::Tick(float deltaTime, APlayerController* spiritController) {}

void Weapon::Shoot(APlayerController* spiritController, float DeltaTime) {}