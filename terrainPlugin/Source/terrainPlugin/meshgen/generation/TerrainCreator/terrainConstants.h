#pragma once

#include "CoreMinimal.h"

class TERRAINPLUGIN_API terrainConstants {
public:

	/// @brief saves the number of vertecies in one chunk (m^2)
	static const int CHUNKSIZE = 20; //10 //20
	static const int ONEMETER = 200; //one meter constant
	static const int fractureHeightStep = 3;

	static const int MAXHEIGHT = 15000; //3000 is a good value, dont change
	static const int MINCHUNK_HILL = 5; //5x5 min hill size
	static const int MINCHUNK_LARGEHILL = 15; //15x15

	static const int CHUNKSTOCREATEATONCE = 10;

	static const int HEIGH_AVG_SNOWHILL_LOWERBOUND = 200000; //200 * 100cm
	static const int HEIGHT_MAX_OCEAN = 200; 
};
