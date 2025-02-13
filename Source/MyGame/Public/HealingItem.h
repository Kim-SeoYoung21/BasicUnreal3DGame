#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class MYGAME_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AHealingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Itme")
	int32 HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
