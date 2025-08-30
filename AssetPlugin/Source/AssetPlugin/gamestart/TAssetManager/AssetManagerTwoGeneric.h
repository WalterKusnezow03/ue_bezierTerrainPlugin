// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "assetManagerGeneric.h"
#include <typeindex>
#include <typeinfo>


/**
 * will save assets in a map and provide nessecarry methods
 * THIS CLASS WILL ONLY RETURN POINTERS AND IS DESIGNED TO HOLD THESE
 * FOR EXAMPLE FOR UCLASS* or UMATERIAL*
 * E is recommended to be an enum!
 * T is recommended to be a UClass type!
 * 
 * T must always be a UClass type
 */
template <typename E0, typename E1, typename T>
class ASSETPLUGIN_API assetManagerTwoGeneric
{

	//static_assert(std::is_enum<E0>::value, "must be an enum");
    //static_assert(std::is_enum<E1>::value, "must be an enum");
	static_assert(std::is_base_of<UObject, T>::value, "must be an UObject");

public:
    //constructor
	assetManagerTwoGeneric(){

	}

	/// @brief destructor will release all ptrs to garbage collection!
	~assetManagerTwoGeneric(){
		
	}

	void addBp(E0 enumVal0, E1 enumVal1, T *t){
		if(t != nullptr){
			//Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
			
			assetManagerGeneric<E1, T> *found = map.Find(enumVal0);
			if(found == nullptr){
				assetManagerGeneric<E1, T> instance;
				map.Add(enumVal0, instance);
				
			}
			
			found = &map[enumVal0];
            if(found != nullptr){
				found->addBp(enumVal1, t);
			}
		}
	}

	/// @brief will try to find the type from the map
	/// @tparam E key
	/// @tparam T 
	/// @param e enum type
	T *getBp(E0 enumVal0, E1 enumVal1){
		assetManagerGeneric<E1, T>* found = map.Find(enumVal0);
        if(found != nullptr){
            return found->getBp(enumVal1);
        }
		return nullptr;
	}

private:

    UPROPERTY()
	TMap<E0, assetManagerGeneric<E1, T>> map;
};
