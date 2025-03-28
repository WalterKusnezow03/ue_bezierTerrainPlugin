// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/util/TTouple.h"
#include "p2/rooms/testing/roomBoundData.h"
#include "p2/util/FVectorUtil.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"

layoutMaker::layoutMaker()
{
}

layoutMaker::~layoutMaker()
{
    clearField();
}

void layoutMaker::clearField(){
    for (int i = 0; i < owningRoomsVec.size(); i++){
        if(owningRoomsVec[i] != nullptr){
            delete owningRoomsVec[i];
        }
    }
    pointerField.clear(); // nur clearen, das feld hat duplikat pointer! kein double deletion, wichtig!
    owningRoomsVec.clear();
}

void layoutMaker::clearAndFill(int x, int y){
    clearField();
    for (int i = 0; i < x; i++){
        std::vector<roomBoundData *> column;
        for (int j = 0; j < y; j++)
        {
            column.push_back(nullptr);
        }
        pointerField.push_back(column);
    }
}




void layoutMaker::makeLayout(
	int xMeters,
	int yMeters,
	std::vector<TTouple<int, int>> &possibleSizes, 
	std::vector<roomBoundData> &output //output will be copied in here
){
    if (possibleSizes.empty()) {
        return;
    }

    clearAndFill(xMeters, yMeters);

    int maxIterations = 10000;
    while(!fieldIsFilled() && maxIterations > 0){
        

        //create and clamp index
        int randomI = FVectorUtil::randomNumber(0, possibleSizes.size() - 1);
        if(randomI >= 0 && randomI < possibleSizes.size()){
            maxIterations--;

            TTouple<int, int> &t = possibleSizes.at(randomI);
            int xSizeWanted = t.first();
            int ySizeWanted = t.last();

            tryFit(xSizeWanted, ySizeWanted);
        }

        
    }

    createDoorsAndWindows();



    //debugPrint
    bool printLog = false;
    if(printLog){
        int counted = owningRoomsVec.size();
        FString printOut = FString::Printf(TEXT("room PRINT: %d, %d \n"), counted, (int) pointerField.at(0).size());
        DebugHelper::logMessage(printOut);
        for (int i = 0; i < pointerField.size(); i++)
        {

            FString row = FString::Printf(TEXT("room row: "));
            for (int j = 0; j < pointerField.at(i).size(); j++)
            {
                roomBoundData *current = pointerField[i][j];
                if(current != nullptr){
                    int number = current->number;
                    row.Append(FString::Printf(TEXT("%d "), number));
                }else{
                    row.Append(FString::Printf(TEXT("A")));
                }
            }
            DebugHelper::logMessage(row);
        }
    }
    
    

    //put to output
    for (int i = 0; i < owningRoomsVec.size(); i++){
        if(owningRoomsVec.at(i) != nullptr){
            roomBoundData copy = *owningRoomsVec.at(i);
            output.push_back(copy);
        }
    }
}



/**
 * 
 * room fitting section
 * 
 */


//try fit and do automatically if could
void layoutMaker::tryFit(int xSize, int ySize){
    for (int i = 0; i < pointerField.size(); i++){
        for (int j = 0; j < pointerField.at(i).size(); j++)
        {
            if(canFit(i,j, xSize, ySize)){
                int roomNumber = owningRoomsVec.size() + 1;
                roomBoundData *newRoom = new roomBoundData(i, j, xSize, ySize, roomNumber);
                owningRoomsVec.push_back(newRoom); //add to owning vector for later clean up
                fill(i, j, xSize, ySize, newRoom);
                return;
            }
        }
    }
}

//fill from to area ok?
bool layoutMaker::canFit(int fromX, int fromY, int xSize, int ySize){
    if(pointerField.size() <= 0){
        return false;
    }
    if(
        fromX + xSize >= pointerField.size() ||
        fromY + ySize >= pointerField.at(0).size()
    ){
        return false;
    }

    for (int i = fromX; i < fromX + xSize; i++){
        for (int j = fromY; j < fromY + ySize; j++)
        {
            if(pointerField.at(i).at(j) != nullptr){
                return false;
            }
        }
    }
    return true;
}


void layoutMaker::fill(int fromX, int fromY, int xSize, int ySize, roomBoundData *room){
    int toX = std::min(fromX + xSize, (int)pointerField.size());
    int toY = std::min(fromY + ySize, (int)pointerField.at(0).size());

    for (int i = fromX; i < toX; i++){
        for (int j = fromY; j < toY; j++)
        {
            pointerField.at(i).at(j) = room;
        }
    }
}

//muss man noch umschreiben
bool layoutMaker::fieldIsFilled(){
    if(pointerField.size() <= 0){
        return true;
    }
    for (int i = pointerField.size() - 1; i >= 0; i--){
        for (int j = pointerField.at(i).size() - 1; j >= 0; j--){
            if(i >= 0 && j >= 0){
                if(pointerField[i][j] == nullptr){
                    return false;
                }
            }
            
        }
    }
    return true;
}

/**
 * 
 * neighboring section
 * 
 */
void layoutMaker::createDoorsAndWindows(){
    makeDoors();
    makeWindows();
}

void layoutMaker::makeDoors(){
    //alle räume durchgehen
    //alle seiten abtasten, jede nur eine tür

    for (int i = 0; i < owningRoomsVec.size(); i++){
        roomBoundData *current = owningRoomsVec.at(i);
        if(current != nullptr){
            //nur die nächsten befüllen
            for (int j = i + 1; j < owningRoomsVec.size(); j++){
                roomBoundData *compared = owningRoomsVec.at(j);
                if(compared != nullptr){
                    //current->connectTo(compared);
                    current->connect(*compared);
                }
            }
        }
        
    }
}


void layoutMaker::makeWindows(){
    //go through all rooms and check
    for (int i = 0; i < owningRoomsVec.size(); i++){
        roomBoundData *current = owningRoomsVec.at(i);

        if(current != nullptr){

            std::vector<FVector> potentialWindows;

            int xcopy = current->xpos();//lower bounds
            int ycopy = current->ypos();

            int xTo = current->xMax();
            int yTo = current->yMax();

            //checkFree(xcopy - 1, ycopy - 1, xTo + 1, yTo + 1, potentialWindows);
            checkFree(xcopy, ycopy, xTo, yTo, potentialWindows);

            bool doorwasAlreadyPlaced = false;
            for (int pos = 0; pos < potentialWindows.size(); pos++)
            {
                int num = (int)(FVectorUtil::randomNumber(0, 10));
                bool create = true;
                if (create)
                {
                    bool placeDoor = (!doorwasAlreadyPlaced && num > 5);

                    FVector currentWindow = potentialWindows[pos];
                    if(!placeDoor){
                        current->addWindowPosition(currentWindow);
                    }else{
                        current->addDoorPosition(currentWindow);
                        doorwasAlreadyPlaced = true;
                    }
                }
            }
        }
    }
}

/// @brief checks if an index is min or max
/// @param i index to check
/// @return true or false 
bool layoutMaker::isOuterEdge(int i){
    bool b = (i <= 0 || i >= pointerField.size() - 1);
    return b;
}

bool layoutMaker::isOuterEdge(int i, int j){
    return (i <= 0 || i >= pointerField.size() - 1 || 
            j <= 0 || j >= pointerField[i].size() - 1);
}


/// @brief checks any area for being an edge area, also below or beyond limits
/// when checking for window positions for a room, decrease lower bound by one, increase higher 
/// bound by one to scan for neighbors / potential windows properly
/// @param xf x from
/// @param yf y from
/// @param xto x to
/// @param yto y to
/// @param output output positions for potential windows. Was either edge of array or was nullptr
void layoutMaker::checkFree(int xf, int yf, int xto, int yto, std::vector<FVector> &output){

    //alle achsen einmal abgehen
    //oben und unten
    
    for (int i = xf; i < xto; i++){
        if(canBeWindow(i, yf-1)){
            output.push_back(FVector(i, yf, 0));
        }
        if(canBeWindow(i, yto+1)){
            output.push_back(FVector(i, yto, 0));
        }
    }


    for (int i = yf; i < yto; i++){
        if(canBeWindow(xf-1, i)){
            output.push_back(FVector(xf, i, 0));
        }
        if(canBeWindow(xto+1, i)){
            output.push_back(FVector(xto, i, 0));
        }
    }

}

/// @brief soll eine rand position von einem raum sein, also aktiv im index daneben,
/// wenn edge, bounds oder null, dann dort mögliche window position
/// @param i 
/// @param j 
/// @return bool is a window position 
bool layoutMaker::canBeWindow(int i, int j){
    //edge or out of bounds
    if(i <= 0 || i >= pointerField.size() - 1)
        return true;
    if(j <= 0 || j >= pointerField.at(i).size() - 1)
        return true;

    if(i >= 0 && j >= 0 && i < pointerField.size() && j < pointerField.at(i).size()){
        return pointerField.at(i).at(j) == nullptr;
    }

    return false;
}

