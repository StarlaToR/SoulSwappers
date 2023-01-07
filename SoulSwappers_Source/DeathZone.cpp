#include "DeathZone.h"
#include "ToolBox.h"
#include "MyPlayer.h"


void ADeathZone::ActorEnteredVolume(AActor* other)
{
	if (other->ActorHasTag(PlayerTag))
	{
		AMyPlayer* player = Cast<AMyPlayer>(other);
		player->Death();
	}
}