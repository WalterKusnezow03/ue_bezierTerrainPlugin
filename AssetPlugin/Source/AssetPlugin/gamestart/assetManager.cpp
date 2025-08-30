// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetPlugin/gamestart/TAssetManager/assetManagerGeneric.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "CoreMinimal.h"


template class assetManagerGeneric<entityEnum, UClass>;
template class assetManagerGeneric<weaponEnum, UClass>;
template class assetManagerGeneric<throwableEnum, UClass>;
template class assetManagerGeneric<particleEnum, UClass>;
template class assetManagerGeneric<materialEnum, UMaterial>;
template class assetManagerGeneric<weaponAttachmentEnum, UClass>;
template class assetManagerTwoGeneric<weaponEnum, weaponAttachmentEnum, UClass>;


assetManager *assetManager::instancePointer = nullptr;


void assetManager::EndGame(){
    if(assetManager* ptr = instance()){
        delete ptr;
        assetManager::instancePointer = nullptr;
    }
}

/// @brief you are not allowed to delete this pointer!
/// @return instance pointer
assetManager* assetManager::instance(){
    if(assetManager::instancePointer == nullptr){
        assetManager::instancePointer = new assetManager();
    }
    return assetManager::instancePointer;
}



void assetManager::setupDefaultTracker(){
    Track<entityEnum>();
    Track<weaponEnum>();
    Track<throwableEnum>();
    Track<particleEnum>();
    Track<materialEnum>();
    Track<textureEnum>();
}

void assetManager::Clear(){
    for(auto &pair : TrackedEnums){
        FNamedEnumBase *ptr = pair.second;
        if(ptr){
            delete ptr;
            pair.second = nullptr;
        }
    }
    TrackedEnums.clear();


    for(auto &pair : TrackedMap){
        assetManagerGeneric<FString, UObject> *ptr = pair.second;
        if(ptr){
            delete ptr;
            pair.second = nullptr;
        }
    }
    TrackedMap.clear();

    for(auto &pair : TrackedDoubleKeyMap){
        assetManagerTwoGeneric<FString, FString, UObject> *ptr = pair.second;
        if(ptr){
            delete ptr;
            pair.second = nullptr;
        }
    }
    TrackedDoubleKeyMap.clear();
}

assetManager::assetManager()
{
    setupDefaultTracker();
}

assetManager::~assetManager()
{
    Clear();
}

// --- entity ---
UClass *assetManager::findBp(entityEnum type){
    if(useFakeRTTI){
        return Find<entityEnum, UClass>(type);
    }

    UClass *found = entityAssets.getBp(type);
    return found;
}

void assetManager::addBp(entityEnum type, UClass *uclass){
    if(useFakeRTTI){
        Add<entityEnum, UClass>(type, uclass);
        return;
    }
    
    if(uclass != nullptr){
        entityAssets.addBp(type, uclass);
    }
}


// --- weapons ---
UClass *assetManager::findBp(weaponEnum type){
    if(useFakeRTTI){
        return Find<weaponEnum, UClass>(type);
    }

    UClass *found = weaponAssets.getBp(type);
    return found;
}

void assetManager::addBp(weaponEnum type, UClass *uclass){
    if(useFakeRTTI){
        Add<weaponEnum, UClass>(type, uclass);
        return;
    }
    
    if(uclass != nullptr){
        weaponAssets.addBp(type, uclass);
    }
}

// --- throwables ---
UClass *assetManager::findBp(throwableEnum type){
    if(useFakeRTTI){
        return Find<throwableEnum, UClass>(type);
    }

    UClass *found = throwableAssets.getBp(type);
    return found;
}

void assetManager::addBp(throwableEnum type, UClass *uclass){
    if(useFakeRTTI){
        Add<throwableEnum, UClass>(type, uclass);
        return;
    }

    if(uclass != nullptr){
        throwableAssets.addBp(type, uclass);
    }
}

// --- particles ---
UClass *assetManager::findBp(particleEnum type){
    if(useFakeRTTI){
        return Find<particleEnum, UClass>(type);
    }

    UClass *found = particleAssets.getBp(type);
    if(found != nullptr){
        return found;
    }
    return nullptr;
}

void assetManager::addBp(particleEnum type, UClass *uclass){
    if(useFakeRTTI){
        Add<particleEnum, UClass>(type, uclass);
        return;
    }
    
    if(uclass != nullptr){
        particleAssets.addBp(type, uclass);
    }
}


// --- weapon assets ---
UClass *assetManager::findBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment){
    if(useFakeRTTI){
        return Find<weaponEnum, weaponAttachmentEnum, UClass>(weapon, weaponAttachment);
    }

    return weaponAttachmentAssets.getBp(weapon, weaponAttachment);
}

void assetManager::addBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment, UClass *uclass){
    if(useFakeRTTI){
        Add<weaponEnum, weaponAttachmentEnum, UClass>(weapon, weaponAttachment, uclass);
        return;
    }

    if(uclass != nullptr){
        weaponAttachmentAssets.addBp(weapon, weaponAttachment, uclass);
    }
}



// --- room assets ---







// --- other assets will follow here... ---


// --- material assets ---
UMaterialInterface *assetManager::findMaterial(materialEnum type){
    if(useFakeRTTI){
        return Find<materialEnum, UMaterialInterface>(type);
    }

    UMaterialInterface* material = materialAssets.getBp(type);
    return material;
}

void assetManager::addMaterial(materialEnum type, UMaterialInterface *material){
    if(useFakeRTTI){
        Add<materialEnum, UMaterialInterface>(type, material);
        return;
    }

    if(material != nullptr){
        materialAssets.addBp(type, material);
    }
}



// --- ui ---



UTexture2D *assetManager::findTexture(textureEnum type){
    if(useFakeRTTI){
        return Find<textureEnum, UTexture2D>(type);
    }

    return textureAssets.getBp(type);
}
void assetManager::addTexture(textureEnum type, UTexture2D *texture){
    if(useFakeRTTI){
        Add<textureEnum, UTexture2D>(type, texture);
        return;
    }
    if(texture != nullptr){
        textureAssets.addBp(type, texture);
    }
}





void assetManager::setDebugCubeBp(UClass *uclassin){
    if(uclassin != nullptr){
        debugCubePointer = uclassin;
    }
}
UClass *assetManager::debugCubeBp(){
    return debugCubePointer;
}