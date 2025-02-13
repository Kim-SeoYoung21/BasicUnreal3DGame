#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType) //����ü��
struct FItemSpawnRow : public FTableRowBase
{
    GENERATED_BODY()

public: //�� �ϳ��� 3���� ������ ������ ����ü�̴�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance;
};

// TSubclassOf - �ϵ� ���۷���(Ŭ������ �׻� �޸𸮿� �ε�)
// TSoftclassPtr - ����Ʈ���۷���(Ŭ���� ��θ� ����)