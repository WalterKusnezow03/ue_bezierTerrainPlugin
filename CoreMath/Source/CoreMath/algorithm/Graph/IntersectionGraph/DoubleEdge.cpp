#include "DoubleEdge.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"

DoubleEdge::DoubleEdge(){
    edge0 = nullptr;
    edge1 = nullptr;
}

DoubleEdge::~DoubleEdge(){
    delete edge0;
    delete edge1;
    edge0 = nullptr;
    edge1 = nullptr;
}

DoubleEdge::DoubleEdge(FVector2D &v0, FVector2D &v1){
    Setup(v0, v1);
}

void DoubleEdge::Setup(
    FVector2D &v0, 
    FVector2D &v1
){
    edge0 = new Edge(v0, v1);
    edge1 = new Edge(v1, v0);
}

bool DoubleEdge::EdgeValid(){
    return edge0 && edge1;
}

bool DoubleEdge::DoesIntersectRedirect(
    DoubleEdge *other
){
    if(directed){
        return false;
    }

    if(other && other->EdgeValid() && EdgeValid()){

        Edge *e0Other = other->edge0;
        Edge *e1Other = other->edge1;

        if(edge0->DoesIntersect(e0Other)){

            CoreMathDebugHelper::logMessage("DoubleEdge::does intersect"); //works ok but right off broken?

            //make right side flow

            //unklar ob richtig!
            if(edge0->IsRightOff(e0Other)){
                CoreMathDebugHelper::logMessage("DoubleEdge::does intersect: def right off"); //works ok but right off broken?
                Edge* E0Next = edge0->GetNext();
                Edge* E0_oNext = e0Other->GetNext();
                Edge* E1Next = edge1->GetNext();
                Edge* E1_oNext = e1Other->GetNext();

                // Cross wiring     SetNextSafe
                edge0->SetNext(E0_oNext);
                edge1->SetNext(E1_oNext);

                e0Other->SetNext(E1Next);
                e1Other->SetNext(E0Next);

                directed = true;
                return true;
            }


            //NOT TRAVERSABLE?
            //e1' right off e0
            if(edge0->IsRightOff(e1Other)){
                CoreMathDebugHelper::logMessage("DoubleEdge::does intersect: inv right off"); //works ok but right off broken?
                // apply mirrored redirection
                Edge *E0Next = edge0->GetNext();
                Edge *E0_oNext = e0Other->GetNext();
                Edge *E1_oNext = e1Other->GetNext();
                Edge *E1Next = edge1->GetNext();

                // make flow along cross to right side, from edge1 perspective
                //should be correct?
                edge0->SetNext(E1_oNext);
                edge1->SetNext(E0_oNext);

                e0Other->SetNext(E0Next); 
                e1Other->SetNext(E1Next);

                directed = true;

                return true;
            }
        }
    }
    return false;
}








bool DoubleEdge::Traversable(){
    return (edge0 && edge0->GetNext()) || (edge1 && edge1->GetNext());
}


//dissamble from this node

Edge *DoubleEdge::StartingNode(){
    if(edge0){
        if(edge0->GetNext()){
            return edge0;
        }
    }
    if(edge1){
        if(edge1->GetNext()){
            return edge1;
        }
    }
    return nullptr;
}

void DoubleEdge::Disassemble(TArray<FVector2D> &outArray, int maxIterations){

    Edge *start = StartingNode();
    Edge *current = start;
    if (start == nullptr){
        CoreMathDebugHelper::logMessage(
            FString::Printf(TEXT("DoubleEdge::START NULLPTR"))
        );
        return;
    }

    TArray<Edge *> tracked;
    TArray<FVector2D> temporaryCollected;

    int i = 0;
    while(true){
        if(i >= maxIterations){
            return;
        }

        //one direction or the other
        if(current){
            //append v1 into array
            current->AppendTarget(temporaryCollected);

            //go to next
            Edge *next = current->GetNext();
            tracked.Add(current);
            //if(next) 
            //    next->SetNextNullptr();
            current = next;

            if(tracked.Contains(next)){
                CoreMathDebugHelper::logMessage(
                    FString::Printf(TEXT("DoubleEdge::made early cirlce!: %d"), temporaryCollected.Num())
                );
                int32 index = tracked.Find(next);
                for (int j = index; j < tracked.Num(); j++){
                    outArray.Add(temporaryCollected[j]);
                }
                return;
            }

            //debug disable
            
            if(false && CanFinishByDistance(next, start)){
                if(tracked.Num() > 2){
                    CoreMathDebugHelper::logMessage(
                        FString::Printf(TEXT("DoubleEdge::finish by distance: %d"), outArray.Num())
                    );
                    return;
                }
            }

            if(!next){
                return;
            }
            if(current == start && outArray.Num() > 1){
                CoreMathDebugHelper::logMessage(
                    FString::Printf(TEXT("DoubleEdge::made real cirlce!: %d"), outArray.Num())
                );

                outArray = temporaryCollected;
                return;
            }


            if(next) 
                next->SetNextNullptr();

        }

        i++;
    }
}



bool DoubleEdge::CanFinishByDistance(Edge *a, Edge *b){
    if(a == b){
        return true;
    }

    if(a && b){
        return a->InRange(b);
    }
    return false;
}





void DoubleEdge::AppendOtherToEdge0(DoubleEdge *other){
    if(other){
        //forward connect
        Edge *e0Other = other->edge0;
        if(e0Other && edge0){
            edge0->SetNext(e0Other);
        }



        //opposite connect
        Edge *e1Other = other->edge1;
        if(e1Other && edge1){
            e1Other->SetNext(edge1);
        }
    }
}




FVector2D DoubleEdge::averageLocation(){
    if(EdgeValid()){
        return edge0->location();
    }
    return FVector2D(0,0);
}