// Fill out your copyright notice in the Description page of Project Settings.


#include <algorithm>
#include <map>
#include "p2/meshgen/MeshData.h"
#include "p2/meshgen/foliage/helper/ParallellShapeMerger.h"
#include "p2/rooms/testing/roomBoundData.h"

roomBoundData::roomBoundData(int xin, int yin, int xscalein, int yscalein, int numberIn)
{
    x = xin;
    y = yin;
    xscale = xscalein;
    yscale = yscalein;
    number = numberIn;
}

roomBoundData::~roomBoundData()
{
    connectedNeighbors.clear();
}

int roomBoundData::xpos(){
    return x;
}
int roomBoundData::ypos(){
    return y;
}

FVector roomBoundData::positionInMeterSpace(int onemeter){
    return FVector(
        xpos() * onemeter,
        ypos() * onemeter,
        0
    );
}

int roomBoundData::xScale(){
    return xscale;
}
int roomBoundData::yScale(){
    return yscale;
}

int roomBoundData::xMax(){
    return x + xscale;
}
int roomBoundData::yMax(){
    return y + yscale;
}

void roomBoundData::connect(roomBoundData &other){

    //überlappende kante finden

    //brauche bool für doOverlap
    //brauche methode für finden der overlap zahlen
    //finde mitte
    //brauche für den porzess noch achsen unterscheidung

    if(hasNeighbor(&other)){
        return;
    }

        //für x, für y xy tauschen
    //entweder bei xmax und ymax ein abziehen 
    defineOverlap(
        other,
        other.xpos(),
        other.ypos(),
        other.xMax() -1,
        other.yMax() -1,
        xpos(),
        ypos(),
        xMax() -1,
        yMax() -1,
        false
    );

    defineOverlap(
        other,
        other.ypos(),
        other.xpos(),
        other.yMax() -1,
        other.xMax() -1,
        ypos(),
        xpos(),
        yMax() -1,
        xMax() -1,
        true
    );
}

void roomBoundData::defineOverlap(
    roomBoundData &other,
    int otherX,
    int otherY,
    int otherXmax,
    int otherYmax,
    int thisX,
    int thisY,
    int thisXmax,
    int thisYmax,
    bool xyFlipped
)
{

    bool flip = otherX < thisXmax;

    if (oneApart(thisXmax, otherX, flip) || oneApart(otherXmax, thisX, flip))
    {
        //vertical overlap
        int a = -1;
        int b = -1;
        // should be correct, one overlap of 2 possible
        a = std::max(thisY, otherY);
        b = std::min(otherYmax, thisYmax);
        if(b < a){ //no overlap
            return;
        }

        if(a != -1 && b != -1){
            int middle = (a + b) / 2.0f;

            
            if(flip){
                if(!xyFlipped){
                    //using only the larger coordinate works better with clamping
                    addDoorPosition(thisX, middle);
                    other.addDoorPosition(thisX, middle);
                }
                else
                {
                    addDoorPosition(middle, thisX);
                    other.addDoorPosition(middle, thisX);
                }
            }else{
                if(!xyFlipped){
                    addDoorPosition(otherX, middle);
                    other.addDoorPosition(otherX, middle);
                }else{
                    addDoorPosition(middle, otherX);
                    other.addDoorPosition(middle, otherX);
                }
            }

            //add neighbors each
            connectedNeighbors.push_back(&other);
            other.connectedNeighbors.push_back(this);

        }

        //oneApart(thisXmax, otherX, flip) || oneApart(otherXmax, thisX, flip)
        FString printing = FString::Printf(
            TEXT("neighbor found a %d, a1 %d OR b %d b1 %d"),
            thisXmax,
            otherX,
            otherXmax,
            thisX
        );
        //DebugHelper::logMessage(printing);

        return;
    }else{
        //oneApart(thisXmax, otherX, flip) || oneApart(otherXmax, thisX, flip)
        FString printing = FString::Printf(
            TEXT("neighbor not found a %d, a1 %d OR b %d b1 %d"),
            thisXmax,
            otherX,
            otherXmax,
            thisX
        );
        //DebugHelper::logMessage(printing);
    }
}

bool roomBoundData::oneApart(int a, int b, bool &flip){
    /*if(order.size() < 2){
        return false;
    }*/
    if(a == b - 1){
        return true;
    }
    if(b == a - 1){ //b ist also grösser, x
        //flip = true;
        return true;
    }
    return false;

    //return a == b - 1 || b == a - 1;
}



void roomBoundData::addWindowPosition(FVector &other){
    addWindowPosition(other.X, other.Y);
}

/// @brief adds a window poisiton at x and y in global space
/// @param xIn x global
/// @param yIn y global
void roomBoundData::addWindowPosition(int xIn, int yIn){
    //make local
    int xLocal = xIn - x;
    int yLocal = yIn - y;

    //clamp?
    FVector pos(xLocal, yLocal, 0);
    clamp(pos);

    if(!contains(windowPositions, pos)){    
        windowPositions.push_back(pos);
    }
}



void roomBoundData::addDoorPosition(FVector &other){
    addDoorPosition(other.X, other.Y);
}

/// @brief adds a door poisiton at x and y in global space
/// @param xIn x global
/// @param yIn y global
void roomBoundData::addDoorPosition(int xIn, int yIn){
    //make local
    int xLocal = xIn - x;
    int yLocal = yIn - y;

    //clamp?
    FVector doorpos(xLocal, yLocal, 0);
    //clamp(doorpos);

    if(!contains(doorPositions, doorpos)){    
        doorPositions.push_back(doorpos);
    }
}

bool roomBoundData::contains(std::vector<FVector> &other, int xin, int yin){
    return contains(other, FVector(xin, yin, 0));
}

bool roomBoundData::contains(std::vector<FVector> &other, FVector pos){
    pos.X = (int)pos.X;
    pos.Y = (int)pos.Y;

    for (int i = 0; i < other.size(); i++){
        FVector &current = other.at(i);
        if((int)current.X == pos.X && (int)current.Y == pos.Y){
            return true;
        }
    }
    return false;
}

void roomBoundData::clamp(FVector &other){
    
    int xLocal = other.X;
    int yLocal = other.Y;

    xLocal = std::max(0, xLocal);
    yLocal = std::max(0, yLocal);
    xLocal = std::min(xscale, xLocal);
    yLocal = std::min(yscale, yLocal);

    other.X = xLocal;
    other.Y = yLocal;
}

std::vector<FVector> roomBoundData::relativeDoorPositionsCm(int onemeter){
    std::vector<FVector> output;
    for (int i = 0; i < doorPositions.size(); i++){
        FVector copy = doorPositions.at(i);
        output.push_back(copy * onemeter);
    }
    return output;
}

std::vector<FVector> roomBoundData::relativeWindowPositionsCm(int onemeter){
    std::vector<FVector> output;
    for (int i = 0; i < windowPositions.size(); i++){
        FVector copy = windowPositions.at(i);
        output.push_back(copy * onemeter);
    }
    return output;
}







bool roomBoundData::hasNeighbor(roomBoundData *other){
    if(other == nullptr){
        return false;
    }
    for (int i = 0; i < connectedNeighbors.size(); i++){
        if(other == connectedNeighbors[i]){
            return true;
        }
    }
    return false;
}




//new 

///@brief finds the largest possible bounding box bl and tr 
void roomBoundData::staircasePossibleBounds(FVector2D &start, FVector2D &end){
    if(!stairBoundsFound){
        stairBoundsFound = true;
        findStairBounds(3,3);
    }
    start = startStairBounds;
    end = endStairBounds;
}

void roomBoundData::staircasePossbileBoundsInMeters(FVector2D &start, FVector2D &end, int onemeter){
    staircasePossibleBounds(start, end);
    start *= onemeter;
    end *= onemeter;
}

void roomBoundData::staircasePossbileBoundsInMetersQuad(
    std::vector<FVector> &verteciesOut,
    int onemeter
){
    FVector2D bl;
    FVector2D tr;
    staircasePossbileBoundsInMeters(bl, tr, onemeter);

    float xmin = bl.X < tr.X ? bl.X : tr.X;
    float xmax = bl.X > tr.X ? bl.X : tr.X;
    float ymin = bl.Y < tr.Y ? bl.Y : tr.Y;
    float ymax = bl.Y > tr.Y ? bl.Y : tr.Y;

    FVector v0(xmin, ymin, 0);
    FVector v1(xmin, ymax, 0);
    FVector v2(xmax, ymax, 0);
    FVector v3(xmax, ymin, 0);

    verteciesOut.push_back(v0);
    verteciesOut.push_back(v1);
    verteciesOut.push_back(v2);
    verteciesOut.push_back(v3);
}

void roomBoundData::findStairBounds(int scaleX, int scaleY){
    //startStairBounds , endStairBounds

    //simple case
    if(doorPositions.size() <= 0){
        startStairBounds = FVector2D(0, 0);
        endStairBounds = FVector2D(xscale, yscale);
    }

    std::vector<FVector> boundsToCheck = {
        FVector(scaleX, scaleY, 0),
        FVector(scaleX, -scaleY, 0),
        FVector(-scaleX, scaleY, 0),
        FVector(-scaleX, -scaleY, 0)
    };

    float largestArea = 0.0f;
    for (int i = 0; i < doorPositions.size(); i++){
        //distanz zum rand
        FVector &current = doorPositions[i];
        for (int j = 0; j < boundsToCheck.size(); j++){
            FVector check = current + boundsToCheck[j];
            if(!exceedsBounds(check)){
                startStairBounds = FVector2D(current.X, current.Y);
                endStairBounds = FVector2D(check.X, check.Y);
                return;
            }
        }
    }
}

bool roomBoundData::exceedsBounds(FVector &other){
    return other.X < 0.0f || other.Y < 0.0f ||
           other.X > xscale || other.Y > yscale;
}





bool roomBoundData::doorIndexIsValid(int index){
    return index >= 0 && index < doorPositions.size();
}





/// ---- helper for mesh gen ----
void roomBoundData::appendBottomOrTopClosed(
    MeshData &appendTo, 
    bool gapForStairs, 
    int onemeter,
    float heightOffset
){
    std::vector<FVector> quad = MeshData::create2DQuadVertecies(xScale() * onemeter, yScale() * onemeter);
    FVector offset(0, 0, heightOffset);
	for (int i = 0; i < quad.size(); i++){
		quad[i] += offset;
	}
    
    if(!gapForStairs){
        appendTo.appendDoublesided(quad[0], quad[1], quad[2], quad[3]);
        return;
    }else{

        std::vector<FVector> innerQuad;
        staircasePossbileBoundsInMetersQuad(
            innerQuad,
            onemeter
        );
        for (int i = 0; i < innerQuad.size(); i++){
            innerQuad[i] += offset;
        }

        for (int i = 0; i < quad.size(); i++){
            int next = (i + 1) % quad.size();

            FVector &v0 = quad[i];
            FVector &v1 = quad[next];
            FVector &v2 = innerQuad[next];
            FVector &v3 = innerQuad[i];

            appendTo.appendDoublesided(v0, v1, v2);
            appendTo.appendDoublesided(v0, v2, v3);

        }


        processFoundStairBounds(innerQuad, onemeter);
    }
}

void roomBoundData::processFoundStairBounds(
    std::vector<FVector> &ref,
    int onemeter
){
    stairBoundsQuad = ref;
    for (int i = 0; i < stairBoundsQuad.size(); i++){
        stairBoundsQuad[i] /= onemeter;
        stairBoundsQuad[i].Z = 0.0f;
    }
}


///@brief returns the bounds in INDEX space, Z height removed!
std::vector<FVector> &roomBoundData::stairBoundsQuadInIndexSpaceRef(int onemeter){

    if (stairBoundsQuad.size() == 0)
    {
        std::vector<FVector> innerQuad;
        staircasePossbileBoundsInMetersQuad(
            innerQuad,
            onemeter
        );
        processFoundStairBounds(innerQuad, onemeter);
    }
    return stairBoundsQuad;
}



