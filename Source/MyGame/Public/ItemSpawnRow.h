#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType) //구조체로
struct FItemSpawnRow : public FTableRowBase
{
    GENERATED_BODY()

public: //행 하나가 3가지 정보를 가지는 구조체이다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance;
};

// TSubclassOf - 하드 레퍼런스(클래스가 항상 메모리에 로드)
// TSoftclassPtr - 소프트레퍼런스(클래스 경로만 유지)