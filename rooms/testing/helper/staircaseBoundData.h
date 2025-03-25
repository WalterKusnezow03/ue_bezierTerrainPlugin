
#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/util/TTouple.h"
#include "p2/meshgen/MeshData.h"

class P2_API StaircaseBoundData{

public:
    StaircaseBoundData();
    ~StaircaseBoundData();

    void createLayout(int sizeXmeters, int sizeYmeters);

    MeshData generate(int oneMeter, int heightMeters, int maxSlopeMeters);

private:
    void clearLayout(int sizeX, int sizeY);

    class Stair{
        public:
        Stair(){
            isNone = true;
        }

        Stair(int xin, int yin, bool isFlatIn){
            isNone = false;
            posX = xin;
            posY = yin;
            isFlat = isFlatIn;
        }
        Stair &operator=(const Stair &other){
            if(this != &other){
                isNone = other.isNone;
                posX = other.posX;
                posY = other.posY;
                isFlat = other.isFlat;
                direction = other.direction;
            }
            return *this;
        }

        Stair(const Stair &other){
            *this = other;
        }

        ~Stair(){

        }
        int posX;
        int posY;
        FVector2D direction;

        bool isFlat = true;

        bool isNone = true;

        void appendData(
            int oneMeter, 
            int slopeMeter, 
            MeshData &outdata,
            FVector &heightOffset
        );

        MMatrix rotation();

        std::vector<FVector> orientedAndMovedQuad(int oneMeter);
    };

    std::vector<std::vector<Stair>> layout;

    void getIndicesClockwise(
        std::vector<TTouple<int, int>> &output
    );

    bool indexIsValid(int i, int j);

    void setFlat(int i, int j, FVector2D &dir);
    void setSlope(int i, int j, FVector2D &dir);

    void setCornersFlat();

    void printLayout();
};