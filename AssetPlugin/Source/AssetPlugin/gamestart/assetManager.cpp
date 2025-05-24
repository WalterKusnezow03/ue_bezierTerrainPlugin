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

assetManager::assetManager()
{
}

assetManager::~assetManager()
{
}


// --- entity ---
UClass *assetManager::findBp(entityEnum type){
    UClass *found = entityAssets.getBp(type);
    return found;
}

void assetManager::addBp(entityEnum type, UClass *uclass){
    if(uclass != nullptr){
        entityAssets.addBp(type, uclass);
    }
}


// --- weapons ---
UClass *assetManager::findBp(weaponEnum type){
    UClass *found = weaponAssets.getBp(type);
    return found;
}

void assetManager::addBp(weaponEnum type, UClass *uclass){
    if(uclass != nullptr){
        weaponAssets.addBp(type, uclass);
    }
}

// --- throwables ---
UClass *assetManager::findBp(throwableEnum type){
    UClass *found = throwableAssets.getBp(type);
    return found;
}

void assetManager::addBp(throwableEnum type, UClass *uclass){
    if(uclass != nullptr){
        throwableAssets.addBp(type, uclass);
    }
}

// --- particles ---
UClass *assetManager::findBp(particleEnum type){
    UClass *found = particleAssets.getBp(type);
    if(found != nullptr){
        return found;
    }
    return nullptr;
}

void assetManager::addBp(particleEnum type, UClass *uclass){
    if(uclass != nullptr){
        particleAssets.addBp(type, uclass);
    }
}


// --- weapon assets ---
UClass *assetManager::findBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment){
    if(weaponAttachmentAssets.find(weapon) != weaponAttachmentAssets.end()){
        assetManagerGeneric<weaponAttachmentEnum, UClass> &manager = weaponAttachmentAssets[weapon];
        return manager.getBp(weaponAttachment);
    }
    return nullptr;
}

void assetManager::addBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment, UClass *uclass){
    if(uclass != nullptr){
        if(weaponAttachmentAssets.find(weapon) != weaponAttachmentAssets.end()){
            assetManagerGeneric<weaponAttachmentEnum, UClass> &manager = weaponAttachmentAssets[weapon];
            manager.addBp(weaponAttachment, uclass);
        }else{
            assetManagerGeneric<weaponAttachmentEnum, UClass> newManager;
            weaponAttachmentAssets[weapon] = newManager;
            assetManagerGeneric<weaponAttachmentEnum, UClass> &ref = weaponAttachmentAssets[weapon];
            ref.addBp(weaponAttachment, uclass);
        }
    }
}



// --- room assets ---







// --- other assets will follow here... ---


// --- material assets ---
UMaterialInterface *assetManager::findMaterial(materialEnum type){
    UMaterialInterface* material = materialAssets.getBp(type);
    return material;
}

void assetManager::addMaterial(materialEnum type, UMaterialInterface *material){
    if(material != nullptr){
        materialAssets.addBp(type, material);
    }
}



// --- ui ---
UClass *assetManager::uiBp(){
    return uiBpPointer;
}

void assetManager::addUiBp(UClass *uclassin){
    uiBpPointer = uclassin;
}


UTexture2D *assetManager::findTexture(textureEnum type){
    UTexture2D *found = textureAssets.getBp(type);
    if(found != nullptr){
        return found;
    }
    return nullptr;
}
void assetManager::addTexture(textureEnum type, UTexture2D *texture){
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