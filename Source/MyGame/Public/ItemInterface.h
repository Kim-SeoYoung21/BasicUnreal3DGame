#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// ������ �ʿ� ����. �𸮾󿡼� �ν��ϵ��� ������ ��.
UINTERFACE(MinimalAPI) //�������̽� ���(���� ��⿡���� �۵��ϵ���)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class MYGAME_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, //�������� �߻��� �ڱ��ڽ�(Sphere)
		AActor* OtherActor, //�ε��� ��ü(�÷��̾�)
		UPrimitiveComponent* OtherComp,// ���Ϳ� �پ��ִ� ���� ������Ʈ(�÷��̾��� ĸ��)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0; // ���������Լ�
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;
	// FName: Ÿ����. �ӵ�,�޸� ���࿡ ����. String�� �������� ���ſ�
	// Ÿ�� �˾Ƴ��� "���� ��" ����

};
