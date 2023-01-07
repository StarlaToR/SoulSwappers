#include "Checkpoint.h"
#include "ToolBox.h"
#include "MyPlayer.h"


void ACheckpoint::ActorEnteredVolume(AActor* other)
{
	Super::ActorEnteredVolume(other);
	if (other->ActorHasTag(PlayerTag))
	{
		AMyPlayer* player = Cast<AMyPlayer>(other);
		player->SetCheckpoint(GetActorLocation()); 
	}
}