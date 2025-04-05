#pragma once
#include <Windows.h>
#include "defs.h"
#include "offsets.h"
#define SpoofCode 0x13041052

#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <array>
#include <future>
#include <winternl.h>


namespace Variables
{
	SDK2::UCanvas* Canvas = nullptr;
	SDK2::APlayerController* PlayerController = nullptr;
	SDK2::APlayerPawn_Athena_C* AcknowledgedPawn = nullptr;
	SDK2::FName MaterialParameter = SDK2::FName(0);
	SDK2::FName MaterialParameter2 = SDK2::FName(0);
	SDK2::FName MaterialParameter3 = SDK2::FName(0);
	SDK2::FName MaterialParameter4 = SDK2::FName(0);
	SDK2::FName MaterialParameter5 = SDK2::FName(0);
	SDK2::FName MaterialParameter6 = SDK2::FName(0);

	const wchar_t* BuildTime = nullptr;

	//Variables
	static bool DrawMenu = true;
	static FVector2D ScreenCenter = FVector2D();
	static FVector2D ScreenSize = FVector2D();

	//Camera
	static FVector CameraLocation = FVector();
	static FRotator CameraRotation = FRotator();
	static float FieldOfView = 0.f;

	//Bullet tracer
	static int LastBulletIndexPushed = 0;
	static FVector Last5BulletImpacts[5];
	static FVector Last5BulletImpactsMuzzle[5];

	//Exploits
	static FVector BulletTeleportHead = FVector();

	namespace GetDamageStartLocation
	{
		static bool bSilentAimActive = false;
		static FVector SilentLocationTarget = FVector();
		static FRotator LastTargetRotation = FRotator();
	}

	namespace GetTargetingTransform
	{
		static bool bPickaxeRangeActive = false;
		static bool bShouldStartAtLocationTarget = false;
		static FVector LocationTarget = FVector();
	}

	namespace GetPlayerViewPoint
	{
		static bool bShouldStartAtLocationTarget = false;
		static FVector SilentLocationTarget = FVector();
		static FRotator LastTargetRotation = FRotator();
		static FVector OriginalLocation = FVector();
		static FRotator OriginalRotation = FRotator();
	}
	namespace GetCameraViewPoint
	{
		static bool bFreecamActive = false;

		static FRotator LastStickyRotation = FRotator();
		static FVector StickyLocation = FVector();
		static FVector CachedFreecamLocation = FVector();
	}
}

namespace Keys
{
	FKey LeftMouseButton;
	FKey RightMouseButton;
	FKey Insert;
	FKey F2;
	FKey F8;
	FKey W;
	FKey A;
	FKey S;
	FKey D;
	FKey SpaceBar;
	FKey LeftShift;
	FKey Capslock;
}

namespace Classes
{
	SDK2::UObject* PlayerClass = nullptr;
	SDK2::UObject* PickupClass = nullptr;
	SDK2::UObject* ContainerClass = nullptr;
	SDK2::UObject* ProjectileClass = nullptr;
	SDK2::UObject* WeakspotClass = nullptr;
	SDK2::UObject* BuildingTrapClass = nullptr;
	SDK2::UObject* VehicleClass = nullptr;
	SDK2::UObject* MarkerClass = nullptr;
	SDK2::UObject* SupplyDropClass = nullptr;
	SDK2::UObject* RangedWeaponClass = nullptr;
	SDK2::UObject* FortWeaponClass = nullptr;
	SDK2::UObject* PickaxeClass = nullptr;
	SDK2::UObject* LocalPlayer = nullptr;
	SDK2::UObject* PlayerController = nullptr;
	SDK2::UObject* PlayerCameraManager = nullptr;
}

enum Bones : int {
	Root = 0,
	pelvis = 2,
	spine_01 = 3,
	spine_02 = 4,
	spine_03 = 5,
	spine_04 = 6,
	spine_05 = 7,
	clavicle_l = 8,
	upperarm_l = 9,
	lowerarm_l = 10,
	hand_l = 33,
	clavicle_r = 37,
	upperarm_r = 38,
	lowerarm_r = 65,
	hand_r = 62,
	neck_01 = 66,
	neck_02 = 67,
	head = 68,
	thigh_l = 71,
	calf_l = 72,
	calf_twist_01_l = 73,
	calf_twist_02_l = 74,
	foot_l = 76,
	thigh_r = 78,
	calf_r = 79,
	calf_twist_01_r = 80,
	calf_twist_02_r = 81,
	foot_r = 83,
	camera_root = 92,
	Camera = 93,
	attach_fp = 94,
};

const wchar_t* GetBuildTime()
{
	const char* BuildTimeA = __TIME__;
	const int BuildTimeALength = NoCRT::__strlen(BuildTimeA);
	wchar_t* WideArray = new wchar_t[BuildTimeALength + 1];

	for (int i = 0; i < BuildTimeALength; i++)
	{
		WideArray[i] = static_cast<wchar_t>(BuildTimeA[i]);
	}

	WideArray[BuildTimeALength] = L'\0';

	return WideArray;
}


namespace SDK
{
	// Forward declarations of classes
	class UWorld;
	class UScruct;
	class UObject;

	class UObject
	{
	public:
		uint64_t GetVTable()
		{
			return *(uint64_t*)(this + 0x0); // UObject::vtable
		}

		SDK2::EObjectFlags GetObjectFlags()
		{
			return *(SDK2::EObjectFlags*)(this + 0x0008); // UObject::ObjectFlags
		}

		int GetInternalIndex()
		{
			return *(int*)(this + 0x000C); // UObject::InternalIndex
		}

		FName GetNamePrivate()
		{
			return *(FName*)(this + 0x0018); // UObject::NamePrivate
		}

		UObject* GetOuterPrivate()
		{
			return *(UObject**)(this + 0x0020); // UObject::OuterPrivate
		}

		void ProcessEvent(UObject* UFunction, void* Params)
		{
			if (auto VTable = *(void***)this)
			{
				reinterpret_cast<void(__cdecl*)(UObject*, UObject*, void*)>(VTable[0x48])(this, UFunction, Params);// offset is 0x02E13BF0
			}
		}

		static UObject* StaticFindObject(UObject* Class, UObject* Outer, const wchar_t* Name, bool ExactClass)
		{
			using StaticFindObject_t = UObject * (__cdecl*)(UObject*, UObject*, const wchar_t*, bool, uintptr_t, void*);

			StaticFindObject_t fn = reinterpret_cast<StaticFindObject_t>(GameBase + 0x2e1c4b0);

			return fn(Class, Outer, Name, ExactClass, SpoofCode, nullptr);
		}

		static UObject* FindObject(const wchar_t* Name, UObject* Outer = nullptr)
		{
			return StaticFindObject(nullptr, Outer, Name, false);
		}

	};

	class UActorComponent : public UObject
	{
	public:

	};

	class USceneComponent : public UActorComponent
	{
	public:
		FRotator RelativeRotation()
		{
			return *(FRotator*)(this + 0x128);
		}
	};

	class AActor : public UObject
	{
	public:
		USceneComponent* RootComponent()
		{
			return *(USceneComponent**)(this + 0x0130);
		}

		FVector GetVelocity()
		{
			static UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.GetVelocity");

			struct {
				FVector ReturnValue;
			} Params = { };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		bool K2_TeleportTo(FVector DestLocation, FRotator DestRotation)
		{
			UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.K2_TeleportTo");

			struct {
				FVector DestLocation;
				FRotator DestRotation;
				bool ReturnValue;
			} Params = { DestLocation, DestRotation };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		float GetActorTimeDilation()
		{
			static UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.GetActorTimeDilation");

			struct {
				float ReturnValue;
			} Params = { };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		bool K2_SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation)
		{
			static UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.K2_SetActorLocationAndRotation");

			struct {
				FVector NewLocation;
				FRotator NewRotation;
				bool bSweep;
				FHitResult SweepHitResult;
				bool bTeleport;
				bool ReturnValue;
			} Params = { NewLocation, NewRotation, false, FHitResult(), true };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		FVector K2_GetActorLocation()
		{
			static UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.K2_GetActorLocation");

			struct {
				FVector ReturnValue;
			} Params = { };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		FRotator K2_GetActorRotation()
		{
			static UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.K2_GetActorRotation");

			struct {
				FRotator ReturnValue;
			} Params = { };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		float GetGameTimeSinceCreation()
		{
			static UObject* Function = 0;
			if (!Function) Function = FindObject(L"Actor.GetGameTimeSinceCreation");

			struct {
				float ReturnValue;
			} Params = { };

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}
	};

}