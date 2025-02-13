#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class MYGAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomItem();
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void ClearSpawnedItems(); // 스폰 아이템 삭제
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void RemoveSpawnedItem(AActor* Item);

    FVector GetRandomPointInVolume() const;
    FItemSpawnRow* GetRandomItem() const;
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

    TArray<AActor*> SpawnedActors; // 스폰된 아이템
};
