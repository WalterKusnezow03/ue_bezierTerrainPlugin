
#pragma once

#include "CoreMinimal.h"

/**
 * the room type enum is designed for each room.cpp file to
 * hold its type to be mapped in the room manager properly
 */
UENUM()
enum class roomtypeEnum
{
    room,
    staircase,
    staircaseTopper
};