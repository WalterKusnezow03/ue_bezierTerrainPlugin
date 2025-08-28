#include "StorageInterfaceStringArray.h"
#include "GameCore/DebugHelper.h"

StorageInterfaceStringArray::StorageInterfaceStringArray(){

}

StorageInterfaceStringArray::~StorageInterfaceStringArray(){

}

int StorageInterfaceStringArray::bytesForChars(int num){
    return sizeof(TCHAR) * num;
}

TArray<FString> StorageInterfaceStringArray::makeUniqueNonEmpty(TArray<FString> &worldNames){
    //O(n^2) but it doesnt matter, sorting order is relevant
    TArray<FString> unique;
    for (int i = 0; i < worldNames.Num(); i++){
        FString &current = worldNames[i];
        if(current.Len() > 0){
            if(!unique.Contains(current)){
                unique.Add(current);
            }
        }
    }
    return unique;
}

void StorageInterfaceStringArray::Save(FString path, TArray<FString> &Names){
    TArray<uint8> Bytes;
    
    writeStringCount(Bytes, Names);
    for (int i = 0; i < Names.Num(); i++)
    {
        AppendString(Bytes, Names[i]);
    }
    if(moreDebugLog){
        PrintBinary(Bytes, "String Array Saved");
    }


    SaveBinaryData(path, Bytes);

}

bool StorageInterfaceStringArray::Load(FString path, TArray<FString> &NamesOut){
    TArray<uint8> Bytes;
    if(!LoadBinaryData(path, Bytes)){
        FString message = FString::Printf(
            TEXT("Storage Interface Sting array: FAILED TO LOAD STRING BIN at: %s"), *path
        );
        DebugHelper::logMessage(message);
        return false;
    }
    uint8 *Ptr = Bytes.GetData();
    int countStrings = readStringCount(Ptr); //works correct

    //debug
    FString messageB = FString::Printf(
        TEXT("Storage Interface Sting array: String count loaded: %d"), countStrings
    );
    DebugHelper::logMessage(messageB);


    //debug
    //return true;

    NamesOut.SetNum(countStrings);
    for (int i = 0; i < countStrings; i++){
        NamesOut[i] = loadString(Ptr);
    }

    if(moreDebugLog){
        PrintBinary(Bytes, "String Array Loaded");
    }

    return true;
}

/// --- READ WRITE STRING COUNT (Avoid while loops) ---

///@brief writes int count into the first bytes.
void StorageInterfaceStringArray::writeStringCount(
    TArray<uint8> &Bytes,
    TArray<FString> &names
){
    int count = names.Num();
    Bytes.SetNumUninitialized(sizeof(int));
    uint8 *Ptr = Bytes.GetData();
    //FMemory::memcpy(*dest, *src, sizeT)
    FMemory::Memcpy(Ptr, &count, sizeof(int));
}

///@brief reads int count from pointer offset and increases it to next offset
int StorageInterfaceStringArray::readStringCount(
    uint8* &Ptr
){
    int count = 0;

    //FMemory::memcpy(*dest, *src, sizeT)
    FMemory::Memcpy(&count, Ptr, sizeof(int));
    Ptr += sizeof(int); //move to next offset
    return count;
}

/// --- APPEND AND LOAD STRING SECTION ---

void StorageInterfaceStringArray::AppendString(
    TArray<uint8> &Bytes,
    FString &name
){
    //binary layout: [countChars][chars]
    
    TArray<TCHAR> charArray = name.GetCharArray();
    int count = charArray.Num();

    int bytesForCharCount = sizeof(int);
    int bytesChars = bytesForChars(count);
    int bytesTotal = bytesForCharCount + bytesChars;

    int prevBytes = Bytes.Num();
    Bytes.SetNumUninitialized(prevBytes + bytesTotal);
    uint8 *Ptr = Bytes.GetData();
    Ptr += prevBytes; //move to offset!

    /*
    FMemory
    Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    
    //binary layout: [countChars][chars]
    //FMemory::memcpy(*dest, *src, sizeT)
    FMemory::Memcpy(Ptr, &count, bytesForCharCount);
    Ptr += bytesForCharCount;

    FMemory::Memcpy(Ptr, (uint8*) charArray.GetData(), bytesChars);
}



FString StorageInterfaceStringArray::loadString(
    uint8* &Ptr
){
    //binary layout: [countChars][chars]
    TArray<TCHAR> charArray;
    int count = 0;

    int bytesForCountCharacters = sizeof(int);
    
    //FMemory::memcpy(*dest, *src, sizeT)
    FMemory::Memcpy(&count, Ptr, bytesForCountCharacters);
    Ptr += bytesForCountCharacters;

    if(moreDebugLog){
        DebugHelper::logMessage(FString::Printf(TEXT("Storage Interface String array: characters(%d)"), count));
    }

    //init char array to the size for pointer to be valid
    charArray.SetNumUninitialized(count);
    int bytesChars = bytesForChars(count);

    FMemory::Memcpy((uint8*) charArray.GetData(), Ptr, bytesChars);
    Ptr += bytesChars;

    FString constructedString = TEXT("");
    for (int i = 0; i < charArray.Num(); i++){
        constructedString += charArray[i];
    }
    return constructedString;
}





// ---- TEST METHOD ----
void StorageInterfaceStringArray::Test(){
    FString path = BaseDir() + TEXT("TestStringArraySave/stringdata.bin");
    TArray<FString> array = {
        "stringA",
        "stringB",
        "stringC",
        "string0",
        "string1",
        "string2",
        "aLoooongString",
        "C++IsTheBestLanguageF*ckJavaStupidAhhhShitGCLol"
    };
    print(array, "Saving:");
    Save(path, array);

    array.Empty();

    if(!Load(path, array)){
        DebugHelper::logMessage("Storage Interface String array: FAILED TO LOAD STRING BIN");
    }
    print(array, "Loaded:");
    
}

void StorageInterfaceStringArray::print(TArray<FString> &array, FString prefixInner){
    FString prefix = FString::Printf(TEXT("Storage Interface String array %s:"), *prefixInner);
    DebugHelper::logMessage(prefix);
    for (int i = 0; i < array.Num(); i++)
    {
        FString message = prefix + array[i];
        DebugHelper::logMessage(message);
    }
}