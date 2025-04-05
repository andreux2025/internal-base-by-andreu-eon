#pragma once
#include <Windows.h>
#include "defs.h"
#include "offsets.h"
#define SpoofCode 0x13041052

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

		EObjectFlags GetObjectFlags()
		{
			return *(EObjectFlags*)(this + 0x0008); // UObject::ObjectFlags
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

}