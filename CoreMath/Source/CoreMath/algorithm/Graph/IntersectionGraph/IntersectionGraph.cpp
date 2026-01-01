#include "IntersectionGraph.h"


IntersectionGraph::IntersectionGraph(){

}

IntersectionGraph::~IntersectionGraph(){

    //for safetey reasons track pointers (adresses which already were deleted)
    TArray<DoubleEdge *> deleted;
    for (int i = 0; i < edges.Num(); i++){
        DoubleEdge *ptr = edges[i];
        if(!deleted.Contains(ptr)){
            delete ptr;
            deleted.Add(ptr);
        }
    }
}

void IntersectionGraph::AddEdgesFromConnectedArray(TArray<FVector2D> &interpolated){
    AddEdgesFromConnectedArray(interpolated, 1);
}

void IntersectionGraph::AddEdgesFromConnectedArray(TArray<FVector2D> &interpolated, int reduceDeail){
    TArray<DoubleEdge *> created;

    reduceDeail = std::max(reduceDeail, 1);

    DoubleEdge *latest = nullptr;
    for (int i = reduceDeail; i < interpolated.Num(); i += reduceDeail)
    {
        FVector2D &v0 = interpolated[i - reduceDeail];
        FVector2D &v1 = interpolated[i];
        DoubleEdge *edgeNew = new DoubleEdge(v0, v1);
        if(latest){
            latest->AppendOtherToEdge0(edgeNew);
        }
        created.Add(edgeNew);
        latest = edgeNew;
    }
    AddAll(created);
}

DoubleEdge *IntersectionGraph::findClosest(DoubleEdge *other){
    if(other){
        bool bInit = false;
        FVector2D location;
        float prevDistance = 0.0f;
        DoubleEdge *found = nullptr;

        for (int i = 0; i < edges.Num(); i++){
            DoubleEdge *current = edges[i];
            if(current){
                FVector2D temp = current->averageLocation();
                float compare = FVector2D::DistSquared(temp, other->averageLocation());
                if(bInit){
                    if (
                        compare <
                        prevDistance)
                    {
                        location = temp;
                        found = current;
                        prevDistance = compare;
                    }
                }else{
                    location = temp;
                    found = current;
                    prevDistance = compare;
                }
            }
        }
        return found;
    }
    return nullptr;
}

bool IntersectionGraph::AddAll(TArray<DoubleEdge*> &heapArray){
    int currentCount = edges.Num();
    bool flag = false;
    for (int i = 0; i < heapArray.Num(); i++)
    {
        if(AddEdge(heapArray[i], currentCount)){
            flag = true;
        }
    }
    return flag;
}

bool IntersectionGraph::AddEdge(DoubleEdge *edge, int limit){
    if(edge){

        //find first intersection
        int upper = std::min(limit, edges.Num());
        for (int i = 0; i < upper; i++)
        {
            DoubleEdge *current = edges[i];
            if(current){
                if(current->DoesIntersectRedirect(edge)){
                    edges.Add(edge);
                    countIntersections++;
                    return true;
                }
            }
        }
        edges.Add(edge);
    }
    return false;
}



#include "CoreMath/Debug/CoreMathDebugHelper.h"
void IntersectionGraph::CreatePolygons(TArray<TArray<FVector2D>> &traversedOutPolygons){
    CoreMathDebugHelper::logMessage(
        FString::Printf(TEXT("IntersectionGraph---- LOG START DISSAMBLE ----"))
    );
    RemoveDeadEdges();

    int maxIterations = edges.Num();

    CoreMathDebugHelper::logMessage(
        FString::Printf(TEXT("IntersectionGraph::nodes: %d, intersections %d"), maxIterations, countIntersections)
    );

    for (int i = 0; i < edges.Num(); i++){
        DoubleEdge *current = edges[i];
        if(current && current->Traversable()){
            TArray<FVector2D> collected;
            current->Disassemble(collected, maxIterations);

            if(IsValidPolygon(collected)){
                //mark start
                FVector2D end = collected.Last() + FVector2D(0, -3000);
                collected.Add(end);

                traversedOutPolygons.Add(collected);
                CoreMathDebugHelper::logMessage(
                    FString::Printf(TEXT("IntersectionGraph::collected: %d"), collected.Num())
                );
            }

            //both side traversal!
            collected.Empty();
            current->Disassemble(collected, maxIterations);
            if(IsValidPolygon(collected)){
                //mark start
                FVector2D end = collected.Last() + FVector2D(0, -3000);
                collected.Add(end);

                traversedOutPolygons.Add(collected);
                CoreMathDebugHelper::logMessage(
                    FString::Printf(TEXT("IntersectionGraph::collected: %d"), collected.Num())
                );
            }

        }
    }

    CoreMathDebugHelper::logMessage(
        FString::Printf(TEXT("IntersectionGraph::made polygons: %d"), traversedOutPolygons.Num())
    );
}


bool IntersectionGraph::IsValidPolygon(TArray<FVector2D> &array){
    if(array.Num() > 2){
        return true;
    }

    return false;
}

bool IntersectionGraph::IsValidPolygon(TArray<FVector2D> &array, int epsilon){
    if(array.Num() > 2){ //at least a triangle
        FVector first(array[0], 0.0);
        FVector last(array.Last(), 0.0);
        return FVector::Dist(first, last) <= epsilon;
    }
    return false;
}





void IntersectionGraph::RemoveDeadEdges(){

}