#include "SpawnVolume.h"
#include "Components/BoxComponent.h"



ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);

    ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
    if (FItemSpawnRow* SelectedRow = GetRandomItem())
    {
        if (UClass* ActualClass = SelectedRow->ItemClass.Get())
        {
            AActor* SpawnedItem = SpawnItem(ActualClass);
            if (SpawnedItem)
            {
                SpawnedActors.Add(SpawnedItem); 
            }
            return SpawnedItem;
        }
    }

    return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
    if (!ItemDataTable) return nullptr;

    TArray<FItemSpawnRow*> AllRows; //모든 행 가져오기
    static const FString ContextString(TEXT("ItemSpawnContext")); //디버깅 정보 제공
    ItemDataTable->GetAllRows(ContextString, AllRows); 

    if (AllRows.IsEmpty()) return nullptr; // 비어있으면 null

    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    
    const float RandValue = FMath::FRandRange(0.0f, TotalChance); // 총합과 0사이 숫자뽑기
    float AccumulateChance = 0.0f;

    // 누적 랜덤값 뽑기(A 0.5, B 0.3, C 0.2일때 랜덤값 0.7이면 B)

    for (FItemSpawnRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    // 박스 컴포넌트 크기 가져오기
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    // 중심 좌표(위치하는 곳)
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT(" ASpawnVolume::SpawnItem()")));

    if (!ItemClass) return nullptr;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
        ItemClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );

    return SpawnedActor;
}

void ASpawnVolume::ClearSpawnedItems()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT(" ASpawnVolume::ClearSpawnedItems()")));
    for (AActor* Item : SpawnedActors)
    {
        if (Item && IsValid(Item))
        {
            Item->Destroy();
        }
    }
    SpawnedActors.Empty();
}

void ASpawnVolume::RemoveSpawnedItem(AActor* Item)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT(" ASpawnVolume::RemoveSpawnedItem()")));
    if (SpawnedActors.Contains(Item))
    {
        SpawnedActors.Remove(Item);
    }
}