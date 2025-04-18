#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "EquippedOneHandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon")

};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName="Unoccupied"),
	EAS_Attacking UMETA(DisplayName="Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "EquippingWeapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName="Alive"),
	EDP_Death1 UMETA(DisplayName="Death1"),
	EDP_Death2 UMETA(DisplayName="Death2"),
	EDP_Death3 UMETA(DisplayName="Death3")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};