#include "RoadMakerFromGrid.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"
#include <algorithm>
#include "terrainPlugin/meshgen/generation/bezier/extendedVersion/BSpline.h"
#include "Algo/Reverse.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "DebugPlugin/DebugHelper.h"
#include "GameCore/util/FVectorUtil.h"


RoadMakerFromGrid::RoadMakerFromGrid(){

}

RoadMakerFromGrid::~RoadMakerFromGrid(){

}

FVector &RoadMakerFromGrid::CenterOfMesh(){
    return centerSaved;
}

void RoadMakerFromGrid::UpdateCenterOfMesh(){
    if(mesh.Num() > 0){
        int num = mesh.Num() * mesh[0].Num(); //same size on all arrays
        FVector centerUpdate;
        for (int i = 0; i < mesh.Num(); i++){
            TArray<FVector> &array = mesh[i];
            for (int j = 0; j < array.Num(); j++){
                centerUpdate += array[j];
            }
        }
        centerUpdate /= num;
        centerUpdate.Z = 0.0f;
        centerSaved = centerUpdate;
    }
}

void RoadMakerFromGrid::SaveOriginalProperties(
    FVector2D &size,
    float stepSize
){
    sizeSaved = size;
    stepSizeSavedBetweenMeshNodes = stepSize;
}


void RoadMakerFromGrid::ValidateSize(FVector2D &size, float &stepSize){
    size.X = std::max(1.0, std::abs(size.X));
    size.Y = std::max(1.0, std::abs(size.Y));
    stepSize = std::max(1.0f, std::abs(stepSize));
}

void RoadMakerFromGrid::CreateGrid(FVector2D size, float StepSize){
    mesh.Empty();
    ValidateSize(size, StepSize);
    SaveOriginalProperties(size, StepSize);


    int countNodesX = size.X / StepSize;
    int countNodesY = size.Y / StepSize;

    DebugHelper::logMessage(
        FString::Printf(TEXT("RoadMakerFromGrid::CreateGrid %d, %d"), countNodesX, countNodesY)
    );

    mesh.SetNum(countNodesX);
    buildedMeshQuads.SetNum(countNodesX);
    for (int i = 0; i < mesh.Num(); i++){
        float offsetX = i * StepSize;

        TArray<FVector> &current = mesh[i];
        current.SetNum(countNodesY);

        TArray<RoadQuad> &buildedMeshCurrent = buildedMeshQuads[i];
        buildedMeshCurrent.SetNum(countNodesY);

        for (int j = 0; j < current.Num(); j++){
            float offsetY = j * StepSize;
            FVector &vec = current[j];
            vec.X = offsetX;
            vec.Y = offsetY;
            vec.Z = 0.0f;
        }
    }
    UpdateCenterOfMesh();
}

void RoadMakerFromGrid::RandomizeGrid(){

}

// --- warp grid ---

void RoadMakerFromGrid::WarpCirlceRandom(){
    int iterations = 2;
    while(iterations > 0){
        float randomScalar = FVectorUtil::randomFloatNumber(0, 1);
        float maxDistance = sizeSaved.Size() * randomScalar;
        float angle = FVectorUtil::randomFloatNumber(-30, 30);

        WarpCirlceByDistanceAroundCenter(maxDistance, angle);
        iterations--;
    }
}

void RoadMakerFromGrid::WarpCirlceByDistanceAroundCenter(
    float maxDistance,
    float angle
){
    WarpCirlceByDistance(centerSaved, maxDistance, angle);
}

void RoadMakerFromGrid::WarpCirlceByDistance(
    FVector &center, 
    float maxDistance,
    float angle
){
    

    float maxDistance2 = maxDistance * maxDistance;
    if(maxDistance2 < stepSizeSavedBetweenMeshNodes){
        return;
    }
    if(std::abs(angle) < 0.1f){
        return;
    }

    MMatrix M;
    for (int i = 0; i < mesh.Num(); i++){
        TArray<FVector> &current = mesh[i];
        for (int j = 0; j < current.Num(); j++){
            FVector &currentPos = current[j];

            float currentDistance = FVector::DistSquared(currentPos, center);
            if(currentDistance <= maxDistance2){
                float skalar = currentDistance / maxDistance2; // distTarget / distAll
                float scaledAngle = skalar * angle;
                MakeRotationAroundPosition(
                    center,
                    scaledAngle,
                    M
                );
                
                currentPos = M * currentPos;
            }
        }
    }
}

void RoadMakerFromGrid::MakeRotationAroundPosition(
    FVector &position,
    float angle,
    MMatrix &outMatrix
){
    MMatrix T;
    T.setTranslation(position);
    MMatrix T1 = T.invertedTranslation();

    MMatrix R;
    R.yawRadAdd(MMatrix::degToRadian(angle));

    //M = T * R * T^-1 <-- lese richtung --
    MMatrix RT1 = R * T1;
    outMatrix = T * RT1;
}




// --- build --- (no warp allowed anymore)

void RoadMakerFromGrid::ApplyHeightOnRawMesh(terrainCreator *creator){
    if(creator){
        for (int i = 0; i < mesh.Num(); i++){
            TArray<FVector> &current = mesh[i];
            ApplyHeightForPostions(current, creator);
        }
    }
}

void RoadMakerFromGrid::ApplyHeightForPostions(TArray<FVector> &array, terrainCreator *creator){
    float heightOffset = 30.0f;
    if (creator)
    {
        for (int j = 0; j < array.Num(); j++){
            FVector &pos = array[j];
            float zUpdate = creator->getHeightFor(pos);
            pos.Z = zUpdate + heightOffset;
        }
    }
}




void RoadMakerFromGrid::Build(
    terrainCreator *creator, 
    float _einheitsValueForBsplineStepSize, 
    float roadWidth,
    ChunkParserMap &map
){
    ApplyHeightOnRawMesh(creator);
    Build(_einheitsValueForBsplineStepSize);
    LockTerrainFromGeneratedRoadQuads(creator, roadWidth);
    ApplyHeightOnRoadQuads(creator);
    ApplyTerrain2DIndexToRoadQuadsAndRemoveTerrainOffset(creator);
    AddRoadQuadsToChunks(map);
    wasBuilded = true;
}




void RoadMakerFromGrid::Build(float _einheitsValue){
    //interpolate with bsplines
    
    //bspline extended klasse, outindex array mit 
    //index positionen im bspline wo die originalen knoten lagen

    // ---- TODO ----
    //compute all vertical and horizontal splines
    //store into map

    //build and add segments
    //build in x, x+1, x+1,y+1, y+1,x, y+1,x
    //add to ordered quad.

    ComputeAllYAxis(_einheitsValue);
    ComputeAllXAxis(_einheitsValue);
    BuildAllPolygons();
}

void RoadMakerFromGrid::ComputeAllYAxis(float _einheitsValue){
    for (int i = 0; i < mesh.Num(); i++){
        ComputeYAxis(i, _einheitsValue);
    }
}

void RoadMakerFromGrid::ComputeAllXAxis(float _einheitsValue){
    if(mesh.Num() > 0){
        //compute all x axis, along y height, which will be the same 
        //across the rectangular map.
        int yHeight = mesh[0].Num();
        for (int j = 0; j < yHeight; j++){
            ComputeXAxis(j, _einheitsValue);
        }
    }
}

void RoadMakerFromGrid::ComputeYAxis(int index, float _einheitsValue){
    if(index >= 0 && index < mesh.Num()){
        TArray<FVector> &yAxis = mesh[index];
        BSpline splineMaker;
        TArray<FVectorBSplinePosition> &outArray = GetSection(index, ERoadKeyEnum::E_yaxis);
        splineMaker.calculatecurve(
            yAxis,
            outArray,
            _einheitsValue
        );

        DebugHelper::logMessage(
            FString::Printf(
                TEXT("RoadMakerFromGrid::ComputeYAxis %d"),
                outArray.Num()
            )
        );

    }
}

void RoadMakerFromGrid::ComputeXAxis(int index, float _einheitsValue){
    if(mesh.Num() > 0 && index >= 0){
        TArray<FVector> copiedXAxis;
        for (int j = 0; j < mesh.Num(); j++){
            TArray<FVector> &yAxisArray = mesh[j];
            if(index < yAxisArray.Num()){
                copiedXAxis.Add(yAxisArray[index]); //copy in x y-vertical the desired y value for x
                /*
                0000
                xxxx <--copy--
                0000
                0000
                */
            }
        }
        BSpline splineMaker;
        TArray<FVectorBSplinePosition> &outArray = GetSection(index, ERoadKeyEnum::E_xaxis);
        splineMaker.calculatecurve(
            copiedXAxis,
            outArray,
            _einheitsValue
        );

        DebugHelper::logMessage("RoadMakerFromGrid::ComputeXAxis ", outArray.Num());
    }
}


//std::map<FRoadKey, TArray<FVectorBSplinePosition>> buildedSections;



TArray<FVectorBSplinePosition> &RoadMakerFromGrid::GetSection(int index, ERoadKeyEnum key){
    FRoadKey k(index, key);
    return GetSection(k);
}

TArray<FVectorBSplinePosition> &RoadMakerFromGrid::GetSection(FRoadKey &key){
    if(buildedSections.find(key) == buildedSections.end()){
        buildedSections[key] = TArray<FVectorBSplinePosition>();
    }
    return buildedSections[key];
}





void RoadMakerFromGrid::CopySectionTo(
    std::pair<int,int> &pos0,
    std::pair<int,int> &pos1,
    RoadQuad &quad
){
    TArray<FVectorBSplinePosition> positions = CopySection(pos0, pos1);
    quad.Append(positions);

    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("RoadMakerFromGrid::CopySectionTo (%d, %d) (%d, %d) count %d"), 
            pos0.first, 
            pos0.second,
            pos1.first, 
            pos1.second,
            positions.Num()
        )
    );*/
}

//do not paste diagonals. Not supported!
TArray<FVectorBSplinePosition> RoadMakerFromGrid::CopySection(
    std::pair<int,int> &pos0,
    std::pair<int,int> &pos1
){
    /*
    xx or (xaxis)

    x (yaxis)
    x
    */
    ERoadKeyEnum axis = pos0.first == pos1.first ? ERoadKeyEnum::E_yaxis : ERoadKeyEnum::E_xaxis;
    int axisIndex = axis == ERoadKeyEnum::E_xaxis ? pos0.second : pos0.first; //yes

    int pos0Index = axis == ERoadKeyEnum::E_xaxis ? pos0.first : pos0.second; // the other index, not axis
    int pos1Index = axis == ERoadKeyEnum::E_xaxis ? pos1.first : pos1.second; //the other index, not axis
    return CopySection(axis, axisIndex, pos0Index, pos1Index);
}

TArray<FVectorBSplinePosition> RoadMakerFromGrid::CopySection(
    ERoadKeyEnum axis,
    int axisIndex,
    int a,
    int b
){
    TArray<FVectorBSplinePosition> outArray;
    if(a != b){
        bool reverseSection = b < a; //clock wise order of sections, is flipped sometimes
        FRoadKey key(axisIndex, axis);
        TArray<FVectorBSplinePosition> &completeSection = GetSection(key); 

        //copy between a and b and reverse if needed
        int start = std::min(a, b);
        int end = std::max(a, b);

        int startFoundIndex = IndexInArrayWhereOriginalAnchorIsIndex(start, completeSection);
        int endFoundIndex = IndexInArrayWhereOriginalAnchorIsIndex(end, completeSection);
        outArray = CopySection(completeSection, startFoundIndex, endFoundIndex);
        if(reverseSection){
            Algo::Reverse(outArray);
        }
    }else{
        DebugHelper::logMessage("RoadMakerFromGrid::Could not copy section");
    }
    return outArray;
}

TArray<FVectorBSplinePosition> RoadMakerFromGrid::CopySection(
    TArray<FVectorBSplinePosition> &array,
    int i0,
    int i1
){
    TArray<FVectorBSplinePosition> copiedPart;
    if (i0 >= 0 && i1 > i0 && i1 < array.Num())
    {
        for (int i = i0; i <= i1; i++){
            copiedPart.Add(array[i]);
        }
    }
    return copiedPart;
}

///where the original array had its anchor (original grid), find that index
//inside the passed array (0,1,2,3,x<-target,...) - finds: 4
int RoadMakerFromGrid::IndexInArrayWhereOriginalAnchorIsIndex(
    int targetAnchor,
    TArray<FVectorBSplinePosition> &completeSection
){
    int innerCount = 0;
    for (int i = 0; i < completeSection.Num(); i++){
        FVectorBSplinePosition &current = completeSection[i];
        if(current.IsOriginalAnchor()){
            //is original index from raw mesh - return index in section
            if(current.IsSameIndex(targetAnchor)){
                return i;
            }
            innerCount++; //reached anchor but not the targeted one
        }
    }
    return -1;
}

//Todo: get keys in quad shape order!!
void RoadMakerFromGrid::BuildAllPolygons(){
    TArray<std::pair<int, int>> quads = GetAllQuadShapedIndices();
    for (int i = 3; i < quads.Num(); i += 4){
        //copy sections.
        //always targeted the 0,0 tl corner in
        //builded quad mesh / map
        std::pair<int, int> &p0 = quads[i - 3];
        std::pair<int, int> &p1 = quads[i - 2];
        std::pair<int, int> &p2 = quads[i - 1];
        std::pair<int, int> &p3 = quads[i];

        //add sections to p0 index
        RoadQuad &addToQuadRef = GetQuadInBuildedMesh(p0);

        //copy all sections
        CopySectionTo(p0, p1, addToQuadRef);
        CopySectionTo(p1, p2, addToQuadRef);
        CopySectionTo(p2, p3, addToQuadRef);
        CopySectionTo(p3, p0, addToQuadRef);
    }
}



RoadQuad &RoadMakerFromGrid::GetQuadInBuildedMesh(std::pair<int,int> &posXY){
    return GetQuadInBuildedMesh(posXY.first, posXY.second);
}

RoadQuad &RoadMakerFromGrid::GetQuadInBuildedMesh(int i, int j){
    if(i >= 0 && i < buildedMeshQuads.Num()){
        TArray<RoadQuad> &yAxis = buildedMeshQuads[i];
        if(j >= 0 && j < yAxis.Num()){
            return yAxis[j];
        }
    }
    return fallback;
}

//as (x,y) pairs for copying axis section parts
TArray<std::pair<int,int>> RoadMakerFromGrid::GetAllQuadShapedIndices(){
    TArray<std::pair<int,int>> outArray;
    if (mesh.Num() > 0){
        int yArrays = mesh.Num();
        int xArrays = mesh[0].Num(); //along vertical, how many horizontal (refacture for function!)
        
        // -- comment on overall objective: --
        //build and add segments
        //build in x, x+1, x+1,y+1, y+1,x, y+1,x
        /*
        0(x,y)----1(x+1,y)
        |             |
        3(x,y+1)--2(x+1,y+1)
        */

        //add to ordered quad.

        for(int i = 1; i < yArrays; i++){
            for(int j = 1; j < xArrays; j++){
                std::pair<int,int> p0(j-1, i-1);
                std::pair<int,int> p1(j, i-1);
                std::pair<int,int> p2(j, i);
                std::pair<int,int> p3(j-1, i);
                outArray.Add(p0);
                outArray.Add(p1);
                outArray.Add(p2);
                outArray.Add(p3);
            }
        }
    }
    return outArray;
}





void RoadMakerFromGrid::LockTerrainFromGeneratedRoadQuads(
    terrainCreator *creator,
    float roadWidth
){
    //DebugHelper::logMessage("RoadMakerFromGrid::LockTerrainFromGeneratedRoadQuads A");
    if(creator){
        //DebugHelper::logMessage("RoadMakerFromGrid::LockTerrainFromGeneratedRoadQuads B");
        for (int i = 0; i < buildedMeshQuads.Num(); i++)
        {
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];

                TArray<FVector> inner;
                TArray<FVector> outer;
                currentQuad.GenerateParalellRoad(
                    outer,
                    inner,
                    roadWidth
                );
                creator->lockQuadsFromParalellArrayLines(
                    outer,
                    inner
                );
            }
        }
    }
}


void RoadMakerFromGrid::ApplyHeightOnRoadQuads(terrainCreator *creator){
    if(creator){
        for (int i = 0; i < buildedMeshQuads.Num(); i++){
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];
                ApplyHeightForPostions(currentQuad.GetCirlce(), creator);
                ApplyHeightForPostions(currentQuad.GetInnerCircle(), creator);                
            }
        }

    }
}





void RoadMakerFromGrid::ApplyTerrain2DIndexToRoadQuadsAndRemoveTerrainOffset(
    terrainCreator *creator
){
    ApplyTerrain2DIndexToRoadQuads(creator);
    RemoveTerrainOffsetFromRoadQuads(creator);
}

void RoadMakerFromGrid::ApplyTerrain2DIndexToRoadQuads(
    terrainCreator *creator
){
    if(creator){
        for (int i = 0; i < buildedMeshQuads.Num(); i++){
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];

                std::pair<int, int> posIndex = creator->Index2DFromWorldPosition(currentQuad.GetCenter());
                currentQuad.UpdateChunkIndexInTerrain(posIndex);
            }
        }
    }
}

//validation needed
void RoadMakerFromGrid::RemoveTerrainOffsetFromRoadQuads(
    terrainCreator *creator
){
    if(creator){
        for (int i = 0; i < buildedMeshQuads.Num(); i++){
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];

                std::pair<int, int> &index = currentQuad.GetChunkIndex();
                chunk *chunk = creator->chunkAt(index.first, index.second);
                if(chunk){
                    FVector removeOffset = chunk->position();
                    currentQuad.RemoveOffset(removeOffset);
                }
            }
        }
    }
}




void RoadMakerFromGrid::AddRoadQuadsToChunks(ChunkParserMap &map){
    
    
    for (int i = 0; i < buildedMeshQuads.Num(); i++){
        TArray<RoadQuad> &array = buildedMeshQuads[i];
        for (int j = 0; j < array.Num(); j++){
            RoadQuad &current = array[j];
            std::pair<int, int> &indexPair = current.GetChunkIndex();

            ChunkParser &parser = map.findByIndex(indexPair.first, indexPair.second);
            AddRoadQuadToChunks(parser, current);
        }
    }



}

void RoadMakerFromGrid::AddRoadQuadToChunks(
    ChunkParser &chunk,
    RoadQuad &quad
){
    //used by meshdata saving, dont use manually if not needed
    //only public for RoadMakerFromGrid!
    bool raycastOnLayer = true;
    MeshData &data = chunk.findMeshDataReference(
        materialEnum::stoneMaterial,
        ELod::lodNear,
        raycastOnLayer
    );
    quad.AppendRoadMesh(data);
}