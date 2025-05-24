// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>

/**
 * will save assets in a map and provide nessecarry methods
 * THIS CLASS WILL ONLY RETURN POINTERS AND IS DESIGNED TO HOLD THESE
 * FOR EXAMPLE FOR UCLASS* or UMATERIAL*
 * E is recommended to be an enum!
 * T is recommended to be a UClass type!
 * 
 * T must always be a UClass type
 */
template <typename E, typename T>
class ASSETPLUGIN_API assetManagerGeneric
{
	static_assert(std::is_enum<E>::value, "must be an enum");
	static_assert(std::is_base_of<UObject, T>::value, "must be an UObject");

public:
	assetManagerGeneric(){

	}
	~assetManagerGeneric(){
		map.Empty();
		//DebugHelper::logMessage("map cleared");
	}

	void addBp(E e, T *t){
		if(t != nullptr){
			//Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
			T **found = map.Find(e);
			if(found == nullptr){
				map.Add(e, t);
			}
		}
	}

	/// @brief will try to find the type from the map
	/// @tparam E key
	/// @tparam T 
	/// @param e enum type
	T *getBp(E e){
		T **found = map.Find(e); //Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
		if(found != nullptr){
			T *pt = *found;
			if(pt != nullptr){
				return pt;
			}
		}
		return nullptr;
	}

private:
	//std::map<E, T*> map;

	UPROPERTY()
	TMap<E, T *> map;
};
