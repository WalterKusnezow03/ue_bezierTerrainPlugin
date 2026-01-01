#include "Edge.h"

Edge::Edge(){
    next = nullptr;

}

Edge::~Edge(){
    next = nullptr;
    
}

Edge::Edge(FVector2D &v0In, FVector2D &v1In){
    v0 = v0In;
    v1 = v1In;
}

Edge::Edge(const Edge &other){
    if(this != &other){
        *this = other;
    }
}

Edge &Edge::operator=(const Edge &other){
    if(this != &other){
        next = other.next;

        v0 = other.v0;
        v1 = other.v1;
    }
    return *this;
}



void Edge::SetNext(Edge *other){
    next = other;
}

void Edge::SetNextSafe(Edge *other){
    if(other){
        next = other;
    }
}

void Edge::SetNextNullptr(){
    next = nullptr;
}

Edge* Edge::GetNext(){
    return next;
}


void Edge::AppendTarget(TArray<FVector2D> &array){
    array.Add(v1);
}

bool Edge::DoesIntersect(Edge *other){
    if(other){
        FVector ignored;
        return DoesIntersect(
            v0,
            v1,
            other->v0,
            other->v1,
            ignored
        );
    }
    return false;
}

bool Edge::DoesIntersect(
    const FVector2D &v0In, 
    const FVector2D &v1In,
    const FVector2D &d0,
    const FVector2D &d1,
    FVector &IntersectionPoint
){
    //FVector IntersectionPoint;
    //FMath::SegmentIntersection2D(Segment1Start, Segment1End, Segment2Start, Segment2End, IntersectionPoint);
    return FMath::SegmentIntersection2D(
        FVector(v0In, 0.0),
        FVector(v1In, 0.0),
        FVector(d0, 0.0),
        FVector(d1, 0.0),
        IntersectionPoint
    );
}




bool Edge::IsRightOff(Edge *other){
    if(other){

        return RightOffTest(v0, v1, other->v1);
        /*FVector2D connect = v1 - v0;
        FVector2D dirCheck = other->v1 - v0; //AB = B - A
        FVector2D normal(connect.Y, connect.X * -1.0f);

        dirCheck = dirCheck.GetSafeNormal();
        normal = normal.GetSafeNormal();
        float dot = dirCheck.X * normal.X + dirCheck.Y * normal.Y;
        return dot >= 0.0f;*/
    }
    return false;
}



bool Edge::RightOffTest(FVector2D &a0, FVector2D &a1, FVector2D &check){
    FVector2D connect = a1 - a0;
    FVector2D dirCheck = check - a0; //AB = B - A

    //normaldir check
    //return (connect.X * dirCheck.Y) - (connect.X * dirCheck.Y) >= 0.0f;

    FVector2D normal(connect.Y, -connect.X);

    if(bLeftOff){
        normal *= -1.0f;
    }

    float dot = dirCheck.X * normal.X + dirCheck.Y * normal.Y;
    return dot >= 0.0f;


    /*
    dirCheck = dirCheck.GetSafeNormal();
    normal = normal.GetSafeNormal();
    float dot = dirCheck.X * normal.X + dirCheck.Y * normal.Y;
    return dot >= 0.0f;*/
}




bool Edge::IsRightOffForReplacement(Edge *other){
    if(other && next){
        if(RightOffTest(v0, next->v0, other->v0)){
            return true;
        }
    }
    
    
    
    if(IsRightOff(other)){ //default test
        
        //must be right off and dot next smaller

        //wenn das skalar produkt zweier vektoren 0 ergibt
        //sind sie orthogonal zu einander

        //wenn das skalar produkt zweier normalisierter
        //vektoren 1 ergibt sind sie paralell zu einander

        //hier: v0->nextCurrent
        //und: v0->nextPotential
        //wenn (v0->nextCurrent \cdot v0->nextPotential) < 1.0f,
        //und right off that edge (v0nextcurrent): other is a better choice
        if(
            RightOffTest(
                v1, next->v1, other->v1
            )
        ){
            return true;
        }
        return true;
    }
    return false;
}

bool Edge::InRange(Edge *other){
    if(other){
        
        float dist2 = FVector2D::DistSquared(v0, v1) / 10.0f; //based on this edge

        return 
        FVector2D::DistSquared(other->v1, v0) <= dist2 ||
        FVector2D::DistSquared(v1, other->v0) <= dist2 ;
    }
    return false;
}