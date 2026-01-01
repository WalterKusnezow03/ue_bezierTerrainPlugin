#pragma once


#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "MaterialCompiler.h"

#include "NodeBase.generated.h"

/// @brief deriving from material expression node for materials.
UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UNodeBase : public UMaterialExpression
//UMaterialExpression
{
    GENERATED_BODY()


protected:

    


    UPROPERTY()
    TArray<FExpressionInput> expressionInputArray;

    // TArray für Outputs
    UPROPERTY()
    TArray<FExpressionOutput> expressionOutputArray;

public:
    
    //input example
    UPROPERTY()
    FExpressionInput FallbackInput;




#if WITH_EDITOR

    //setup intput array
    // CPP
    virtual void PostInitProperties() override;

    /// @brief override this method to add more inputs.
    virtual void SetupInputsOnConstruct(){
        //empty for derivation override
    }

    virtual void SetupOutputsOnConstruct(){
        //empty for derivation override
    }

    virtual void SetupInternalExpressionsOnConstruct(){
        //empty for derivation override (using internal expressions of unreal to hide inside)
    }


    // --- override this function to compile your expression ---
    //virtual int32 Compile(FMaterialCompiler *Compiler, int32 OutputIndex) override;


    virtual FString NodeName() const;

    virtual void GetCaption(TArray<FString> &OutCaptions) const override;

    virtual FString GetDescription() const override;

    ///----- intput ------

    virtual const TArray<FExpressionInput *> GetInputs() override;

    virtual FExpressionInput *GetInput(int32 index) override;

    virtual FName GetInputName(int32 index) const override;

    /// @brief compiles input or returns Constant 0
    int32 CompileInput(FMaterialCompiler *Compiler, int32 index);
    int32 CompileInput(FMaterialCompiler *Compiler, int32 index, bool &sucess);

    /// ----- output ------

    // Rückgabe aller Outputs
    virtual TArray<FExpressionOutput> &GetOutputs() override;

    //Returns the output at specified index is valid
    FExpressionOutput *GetOutput(int32 OutputIndex);

#endif
};