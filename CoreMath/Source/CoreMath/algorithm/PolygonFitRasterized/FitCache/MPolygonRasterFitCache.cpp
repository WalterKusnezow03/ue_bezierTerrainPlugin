#include "MPolygonRasterFitCache.h"

MPolygonRasterFitCache::MPolygonRasterFitCache(){
    leftSpace = 0;
    position.first = 0;
    position.second = 0;
}


MPolygonRasterFitCache::~MPolygonRasterFitCache(){

}

MPolygonRasterFitCache::MPolygonRasterFitCache(const MPolygonRasterFitCache &other){
    if(this != &other){
        *this = other;
    }
}

MPolygonRasterFitCache &MPolygonRasterFitCache::operator=(const MPolygonRasterFitCache &other){
    if(this != &other){
        leftSpace = other.leftSpace;
        position = other.position;
        cachedPolygonRaster = other.cachedPolygonRaster;
    }
    return *this;
}

void MPolygonRasterFitCache::Setup(
    int leftGaps, 
    std::pair<int, int> &pos,
    MPolygonRaster &polygon
){ //polygon for cache missing here
    leftSpace = leftGaps;
    position = pos;
    cachedPolygonRaster = polygon;
}

int MPolygonRasterFitCache::LeftSpace(){
    return leftSpace;
}

std::pair<int, int> &MPolygonRasterFitCache::GetPosition(){
    return position;
}

MPolygonRaster &MPolygonRasterFitCache::GetMPolygonRaster(){
    return cachedPolygonRaster;
}

int MPolygonRasterFitCache::FindMin(TArray<MPolygonRasterFitCache> &array){
    if (array.Num() > 0){
        int index = 0;
        int copySpace = array[0].leftSpace;
        for (int i = 1; i < array.Num(); i++){
            int currentSpace = array[i].leftSpace;
            if (currentSpace < copySpace)
            {
                index = i;
                copySpace = currentSpace;
            }
        }
        return index;
    }
    return -1;
}