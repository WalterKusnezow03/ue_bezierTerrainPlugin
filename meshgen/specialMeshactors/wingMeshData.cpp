
#include "wingMeshData.h"
#include "p2/meshgen/MeshData.h"
#include "CoreMinimal.h"


wingMeshData::wingMeshData(){
    resetIndices();
}

wingMeshData::~wingMeshData(){

}

void wingMeshData::resetIndices(){
    indexA = 0;
    indexB = 0;
    indexC = 0;
    indexD = 0;
    indexE = 0;
}


void wingMeshData::setup(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &e,
    int detail //should be more around 10
){
    detail = std::abs(detail);
    if(detail <= 0){
        detail = 1;
    }

    clearMesh(); //alles clearen, hier kein "Super::" weil wir nichts überschreieben
    resetIndices();

    fillLogicalRow(logicalLineAB, a, b, detail, indexA, indexB, false);
    fillLogicalRow(logicalLineBC, b, c, detail, indexB, indexC, true);

    //d,e manuell hinzufügen
    vertecies.Add(d);
    indexD = vertecies.Num() - 1;

    vertecies.Add(e);
    indexE = vertecies.Num() - 1;

    //achtung: vertecies sind nicht doppelt. Keine doppelten normalen!
    generateTriangles();
}

///@brief fills a logical row INCLUDING start and end point!
void wingMeshData::fillLogicalRow(
    std::vector<int> &logicalRow,
    FVector &start,
    FVector &end,
    int detail,
    int &startIndex,
    int &endIndex,
    bool skipStart //add the starting point to vertecies or not
){
    if(vertecies.Num() == 0){
        skipStart = false;
    }

    detail = std::abs(detail);
    if (detail <= 0){
        detail = 1;
    }

    FVector connect = end - start;
    connect /= detail;


    for (int i = 0; i <= detail; i++){ //'<=' damit (start + connect * detail = end) wieder ergibt
        FVector newVertex = start + connect * i;

        //add as new vertex or not ---> logical row will still be filled!
        if(skipStart){
            if(i != 0){
                vertecies.Add(newVertex);
            }
        }else{
            vertecies.Add(newVertex);
        }

        int indexPushed = vertecies.Num() - 1; //is valid if skip start is true and more than one in vertex buffer
        if (i == 0){
            startIndex = indexPushed; //since was added, -1, even if start was skipped
        }
        if(i == detail){
            endIndex = indexPushed; //since was added, -1
        }

        //add to logical row
        logicalRow.push_back(indexPushed); //still okay, even if skipStart
    }
}



void wingMeshData::generateTriangles(){
    /*
    a---b---c
            |
            d
            |
            e

    da vertecies indices nun vorliege, einfach triangle buffer basteln, double sided
    */
    //dreiecke bei: abe, bde, bcd ---> logical rows includieren die key vertex positionen!

    //hier jetzt die rows ab bc mit den unteren ankern verbinden wie gewünscht


    appendTriangles(logicalLineAB, indexE); //DEBUG OK WHEN RENDER
    appendTriangles(logicalLineBC, indexD); //DEBUG OK

    appendDoubleSided(
        indexE,
        indexB,
        indexD
    );
}

void wingMeshData::appendTriangles(
    std::vector<int> &logicalRow,
    int connectToIndex
){
    int v0 = connectToIndex;
    for (int i = 1; i < logicalRow.size(); i++)
    {
        int v1 = logicalRow[i - 1];
        int v2 = logicalRow[i];
        appendDoubleSided(v0, v1, v2);
    }
}




void wingMeshData::appendDoubleSided(
    int v0,
    int v1,
    int v2
){
    /*
    1 2
    0
    */
    triangles.Add(v0);
    triangles.Add(v1);
    triangles.Add(v2);

    //flip normal
    /*
    2 1 
    0 
    */
    triangles.Add(v0);
    triangles.Add(v2);
    triangles.Add(v1);
}



void wingMeshData::refreshLogicalLines(){
    refreshLogicalLine(logicalLineAB);
    refreshLogicalLine(logicalLineBC);
}

void wingMeshData::refreshLogicalLine(
    std::vector<int> &vec
){
    if(vec.size() > 1){
        int start = vec[0];
        int end = vec[vec.size() - 1];

        FVector &startVertex = vertecies[start];
        FVector &endVertex = vertecies[end];

        FVector connect = endVertex - startVertex;
        connect /= vec.size();

        //update vertex positions
        for (int i = 1; i < vec.size() - 1; i++){
            int index = vec[i];
            if(isValidVertexIndex(index)){
                vertecies[index] = startVertex + connect * i;
            }
        }
    }
}



void wingMeshData::refreshVertecies(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &e
){
    if(vertecies.Num() > 0){
        vertecies[indexA] = a;
        vertecies[indexB] = b;
        vertecies[indexC] = c;
        vertecies[indexD] = d;
        vertecies[indexE] = e;
        refreshLogicalLines();
    }
}




void wingMeshData::refreshVertexA(FVector &newPos){
    refreshVertex(newPos, indexA);
}


///@brief general method for refreshing any special key
void wingMeshData::refreshVertex(FVector &newpos, int index){
    if(vertecies.Num() > 0){
        if(isValidVertexIndex(index)){
            vertecies[index] = newpos;
            refreshLogicalLines();
        }
    }
}
