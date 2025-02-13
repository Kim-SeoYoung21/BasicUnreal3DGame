#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// 수정할 필요 없음. 언리얼에서 인식하도록 도움을 줌.
UINTERFACE(MinimalAPI) //인터페이스 등록(현재 모듈에서만 작동하도록)
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
		UPrimitiveComponent* OverlappedComp, //오버랩이 발생한 자기자신(Sphere)
		AActor* OtherActor, //부딪힌 객체(플레이어)
		UPrimitiveComponent* OtherComp,// 액터에 붙어있던 원인 컴포넌트(플레이어의 캡슐)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0; // 순수가상함수
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;
	// FName: 타입형. 속도,메모리 절약에 좋음. String은 생각보다 무거움
	// 타입 알아내고 "비교할 때" 좋음

};
