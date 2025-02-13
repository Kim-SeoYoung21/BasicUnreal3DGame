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

    TArray<FItemSpawnRow*> AllRows; //��� �� ��������
    static const FString ContextString(TEXT("ItemSpawnContext")); //����� ���� ����
    ItemDataTable->GetAllRows(ContextString, AllRows); 

    if (AllRows.IsEmpty()) return nullptr; // ��������� null

    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    
    const float RandValue = FMath::FRandRange(0.0f, TotalChance); // ���հ� 0���� ���ڻ̱�
    float AccumulateChance = 0.0f;

    // ���� ������ �̱�(A 0.5, B 0.3, C 0.2�϶� ������ 0.7�̸� B)

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
    // �ڽ� ������Ʈ ũ�� ��������
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    // �߽� ��ǥ(��ġ�ϴ� ��)
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