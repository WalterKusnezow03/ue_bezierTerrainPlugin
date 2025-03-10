

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/MeshData.h"
#include "p2/entities/customIk/MMatrix.h"


/**
 * The data must be saved here to organize mesh actor layers for different materials 
 * and keeping code clean when applying meshes and handling more layers
 */
class P2_API wingMeshData : public MeshData
{
public:
    wingMeshData();
    ~wingMeshData() override; //destruktor überschreiben, super wird automatisch gecallt

    void setup(
        FVector &a,
        FVector &b,
        FVector &c,
        FVector &d,
        FVector &e,
        int detail
    );

    void refreshVertexA(FVector &newPos);

    void refreshVertecies(
        FVector &a,
        FVector &b,
        FVector &c,
        FVector &d,
        FVector &e
    );

protected:

    /*
    //es braucht gegebenen falls eine eigene daten struktur die von
    //mesh data erbt um den wing aus und ein zu klappen
    //der erste vertex muss immer jener sein der am bein unten liegt
    //und dann das "dreieck" ausklappt

    //man könnte die eck vertecies an den ersten drei indices plazieren
    //man muss die vertecies dazwischen irgendwie linear anordnen



    sind die flügel eines dreiecks, a,b,c,d,e sollen getrennt manipulierbar sein,
    als index gespeichert

    achtung: vertex array sollte nicht mehr manipuliert werden nach dem setup!

    a---b---c
            |
            d
            |
            e

    //dreiecke bei: abe, bde, bcd

    */
    void resetIndices();
    int indexA = 0;
    int indexB = 0;
    int indexC = 0;
    int indexD = 0;
    int indexE = 0;

    std::vector<int> logicalLineAB;
    std::vector<int> logicalLineBC;

    //die weiteren dreiecke könnten intern automatisch generiert werden!

    void fillLogicalRow(
        std::vector<int> &logicalRow,
        FVector &start,
        FVector &end,
        int detail,
        int &startIndex,
        int &endIndex,
        bool skipStart
    );

    void generateTriangles();

    void appendTriangles(
        std::vector<int> &logicalRow,
        int connectToIndex
    );

    void appendDoubleSided(
        int v0,
        int v1,
        int v2
    );

    void refreshLogicalLines();
    void refreshLogicalLine(std::vector<int> &ref);

    void refreshVertex(FVector &newpos, int index);
};