#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"


ASpartaCharacter::ASpartaCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true; //�� ��Ʈ�ѷ��� ���� �������ϵ� ȸ��

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // ������ �� ���� ����
    CameraComp->bUsePawnControlRotation = false;

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(GetMesh());
    OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    MouseSensitivity = 3.0f;
    JumpCount = 0;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // ĳ���� �̵��ӵ� ��� �ٲ�

    MaxHealth = 100.0f;
    Health = MaxHealth;
}

void ASpartaCharacter::BeginPlay()
{
    Super::BeginPlay();
    UpdateOverheadHP();
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASpartaCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASpartaCharacter::StopSprint
                );
            }
        }
    }
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
    // ��Ʈ�ѷ� üũ
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}
void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
        JumpCount++;
    }
}
void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}
void ASpartaCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X * MouseSensitivity);
    AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}
void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}
void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
} 

int32 ASpartaCharacter::GetHealth() const
{
    return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    UpdateOverheadHP();
}

float ASpartaCharacter::TakeDamage(
    float DamageAmount, // ���� ������
    struct FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    // ����, ��� �� ����ϰ� ���� ������ ������
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    UpdateOverheadHP();

    if (Health <= 0.0f)
    {
        OnDeath();
    }

    return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
    ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
    if (SpartaGameState)
    {
        SpartaGameState->OnGameOver();
    }
}

void ASpartaCharacter::UpdateOverheadHP()
{
    if (!OverheadWidget) return;

    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance) return;

    if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
    {
        HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));

        FLinearColor NewColor = (Health >= 50.0f) ? FLinearColor(0.f, 1.f, 0.f, 1.f) : FLinearColor(1.f, 0.f, 0.f, 1.f);

        // ���� ����
        HPText->SetColorAndOpacity(NewColor);
    }
    else
    {
        return; // ���� ó��
    }
}