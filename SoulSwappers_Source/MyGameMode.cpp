#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"


void AMyGameMode::SpawnPlayers()
{
	if(UGameplayStatics::GetPlayerController(GetWorld(), 1) == nullptr)
		UGameplayStatics::CreatePlayer(GetWorld(), 1);

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AMyPlayer* player = GetWorld()->SpawnActor<AMyPlayer>(myPlayerBP, FVector(0,0,2000), FRotator(0, 0, 0), SpawnParams);
			player->PossessedBy(GetWorld()->GetFirstPlayerController());
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APlayerSpirit* spirit = GetWorld()->SpawnActor<APlayerSpirit>(mySpiritBP, FVector(0, 0, 2000), FRotator(0, 0, 0), SpawnParams);
			spirit->PossessedBy(UGameplayStatics::GetPlayerController(GetWorld(), 1));
		}
	}
}

void AMyGameMode::BeginPlay()
{
	SpawnPlayers();
}