#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "MaterialCompiler.h"
#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h"
#include "WKPixelShaderBase.generated.h"


UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKPixelShaderBase : public UWKVertexShaderBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    virtual void SetupOutputsOnConstruct() override;



    virtual FString NodeName() const override {
        return TEXT("WKPixelShaderBase");
    }

    int32 GetScreenPositionAsPixel(FMaterialCompiler *Compiler);
    int32 GetScreenPositionAsUV(FMaterialCompiler *Compiler);
    int32 GetScreenPoisitionAsUvRelativeToCenter(FMaterialCompiler *Compiler);

    int32 RadialScalarFromScreenCoordinateRelativeToCenter(FMaterialCompiler *Compiler);

    TArray<int32> SplitRGBA(FMaterialCompiler *Compiler, int32 someColor);
    int32 MakeRGBA(FMaterialCompiler *Compiler, TArray<int32> &array);

    virtual int32 Compile(FMaterialCompiler *Compiler, int32 OutputIndex) override;

    //called by compile
    virtual int32 CompilePixelShader(FMaterialCompiler *Compiler);

#endif
};