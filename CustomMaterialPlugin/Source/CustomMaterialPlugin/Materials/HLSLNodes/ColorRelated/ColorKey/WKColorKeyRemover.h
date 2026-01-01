#pragma once



#include "CoreMinimal.h"
#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h"

#include "WKColorKeyRemover.generated.h"

/// @brief deriving from material expression node for materials.
UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKColorKeyRemover : public UWKVertexShaderBase
{
    GENERATED_BODY()
public:

    UPROPERTY(Instanced)
    UMaterialExpressionCustom* customExpression = nullptr;

#if WITH_EDITOR
    virtual void SetupInputsOnConstruct() override;
    virtual void SetupOutputsOnConstruct() override;
    virtual void SetupInternalExpressionsOnConstruct() override;


    virtual FString NodeName() const override{
        return TEXT("UWKColorKeyRemover");
    }

    virtual int32 Compile(FMaterialCompiler *Compiler, int32 outputIndex) override;

    //override
    ///----- intput ------

    virtual const TArray<FExpressionInput *> GetInputs() override{
        if(customExpression){
            return customExpression->GetInputs();
        }
        return Super::GetInputs();
    }

    virtual FExpressionInput *GetInput(int32 index) override{
        if(customExpression){
            return customExpression->GetInput(index);
        }
        return Super::GetInput(index);
    }

    virtual FName GetInputName(int32 index) const override{
        if(customExpression){
            return customExpression->GetInputName(index);
        }
        return Super::GetInputName(index);
    }


    /// ----- output ------

    // Rückgabe aller Outputs
    virtual TArray<FExpressionOutput> &GetOutputs() override{
        if(customExpression){
            return customExpression->GetOutputs();
        }
        return Super::GetOutputs();
    }



private:



#endif
};
