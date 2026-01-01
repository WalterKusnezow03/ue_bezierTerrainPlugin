#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"

#include "Materials/MaterialExpressionCeil.h"

#include "MaterialCompiler.h"
#include "CustomMaterialPlugin/Materials/Nodes/NodeBase.h"

//#include "Materials/MaterialExpressionLocalPosition.h"

#include "WKVertexShaderBase.generated.h"

/// @brief base class for any expression, has a lot of helper functions
UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKVertexShaderBase : public UNodeBase
{
    GENERATED_BODY()

public:


#if WITH_EDITOR
    virtual void SetupInternalExpressionsOnConstruct() override;


    virtual FString NodeName() const override {
        return TEXT("WKVertexShaderBase");
    }

    //convert constants
    int32 MakeConstant(FMaterialCompiler *compiler, float num);

    // UV-Koordinaten
    int32 GetUV(
        FMaterialCompiler *Compiler,
        int32 UVIndex = 0,
        bool bFractional = false,
        bool bUnMirror = false // idk what that is for
    );

    //World Vertex Location
    int32 GetWorldPosition(
        FMaterialCompiler *Compiler,
        EWorldPositionIncludedOffsets WorldPositionMode = WPT_Default
    );

    //Local Vertex Location, does not include rotation or scaling right now!
    int32 GetLocalPosition(FMaterialCompiler *Compiler);

    //make vertex
    int32 MakeVector2D(FMaterialCompiler *Compiler, int32 X, int32 Y);
    int32 MakeVector3D(FMaterialCompiler *Compiler, int32 X, int32 Y, int32 Z);

    //make const vector
    int32 MakeConstant2D(FMaterialCompiler *Compiler, float x, float y);
    int32 MakeConstant3D(FMaterialCompiler *Compiler, float x, float y, float z);
    int32 MakeConstant4D(FMaterialCompiler *Compiler, float x, float y, float z, float h);

    int32 WorldPosX(FMaterialCompiler *Compiler);
    int32 WorldPosY(FMaterialCompiler *Compiler);
    int32 WorldPosZ(FMaterialCompiler *Compiler);

    int32 QuadraticHorizontalDistanceFromOrigin(FMaterialCompiler *Compiler);
    int32 SizeVector2D(FMaterialCompiler *Compiler, int32 X, int32 Y);

    // Objektposition
    int32 GetObjectPosition(FMaterialCompiler *Compiler);

    // Vertex-Farbe
    int32 GetVertexColor(FMaterialCompiler *Compiler);

    // Vertex-Normal
    int32 GetVertexNormal(FMaterialCompiler *Compiler);

    // Vertex-Tangent
    int32 GetVertexTangent(FMaterialCompiler *Compiler);

    //RunTime
    int32 GetTime(FMaterialCompiler *Compiler, bool bPeriodic = false, bool bRealTime = true);
    
    //scaled time
    int32 ScaledTime(FMaterialCompiler *Compiler, float scalar);

    // Kamerarichtung
    int32 GetCameraVector(FMaterialCompiler *Compiler);




    //operators
    int32 Less(FMaterialCompiler *Compiler, int32 A, int32 B);
#endif

};