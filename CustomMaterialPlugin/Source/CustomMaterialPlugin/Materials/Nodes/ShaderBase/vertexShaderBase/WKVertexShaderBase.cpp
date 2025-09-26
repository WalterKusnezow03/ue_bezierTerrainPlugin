#include "WKVertexShaderBase.h"

void UWKVertexShaderBase::SetupInternalExpressionsOnConstruct(){
    Super::SetupInternalExpressionsOnConstruct();
    ceilExpression = NewObject<UMaterialExpressionCeil>(this);

    
}

int32 UWKVertexShaderBase::MakeConstant(FMaterialCompiler *compiler, float num){
    return compiler->Constant(num);
}

int32 UWKVertexShaderBase::MakeConstant2D(FMaterialCompiler *Compiler, float x, float y){
    return Compiler->Constant2(x,y);
}

int32 UWKVertexShaderBase::MakeConstant3D(FMaterialCompiler *Compiler, float x, float y, float z){
    return Compiler->Constant3(x,y,z);
}

int32 UWKVertexShaderBase::MakeConstant4D(FMaterialCompiler *Compiler, float x, float y, float z, float h){
    return Compiler->Constant4(x,y,z,h);
}


int32 UWKVertexShaderBase::GetUV(
    FMaterialCompiler* Compiler, 
    int32 UVIndex, 
    bool bFractional, 
    bool bUnMirror //idk what that is for
)
{
    return Compiler->TextureCoordinate(UVIndex, bFractional, bUnMirror);
}




int32 UWKVertexShaderBase::GetWorldPosition(
    FMaterialCompiler* Compiler, 
    EWorldPositionIncludedOffsets WorldPositionMode
)
{
    return Compiler->WorldPosition(WorldPositionMode);
}


int32 UWKVertexShaderBase::GetLocalPosition(FMaterialCompiler* Compiler){
    //FMaterialCompiler::ObjectWorldPosition
    int32 objectWorld = GetObjectPosition(Compiler);
    int32 vertexWorld = GetWorldPosition(Compiler);
    return Compiler->Sub(vertexWorld, objectWorld);
}

int32 UWKVertexShaderBase::MakeVector2D(FMaterialCompiler *Compiler, int32 X, int32 Y){
    int32 XY = Compiler->AppendVector(X, Y); // 2D Vector: (x, y)
    return XY;
}

int32 UWKVertexShaderBase::MakeVector3D(FMaterialCompiler *Compiler, int32 X, int32 Y, int32 Z){ //as layers.
    int32 XY = Compiler->AppendVector(X, Y); // 2D Vector: (x, y)
    int32 XYZ = Compiler->AppendVector(XY,Z); // 3D Vector: (x, y, z)
    return XYZ;
}





int32 UWKVertexShaderBase::WorldPosX(FMaterialCompiler *Compiler){
    int32 worldPos = GetWorldPosition(Compiler);
    int32 x = Compiler->ComponentMask(worldPos, true, false, false, false); // x,y,z,h
    return x;
}

int32 UWKVertexShaderBase::WorldPosY(FMaterialCompiler *Compiler)
{
    int32 worldPos = GetWorldPosition(Compiler);
    int32 y = Compiler->ComponentMask(worldPos, false, true, false, false); // x,y,z,h
    return y;
}

int32 UWKVertexShaderBase::WorldPosZ(FMaterialCompiler *Compiler){
    int32 worldPos = GetWorldPosition(Compiler);
    int32 z = Compiler->ComponentMask(worldPos, false, false, true, false); // x,y,z,h
    return z;
}

int32 UWKVertexShaderBase::QuadraticHorizontalDistanceFromOrigin(FMaterialCompiler *Compiler){
    int32 worldPos = GetWorldPosition(Compiler);
    int32 x = WorldPosX(Compiler);
    int32 y = WorldPosY(Compiler);

    return SizeVector2D(Compiler, x, y);
    /*
    int32 x2 = Compiler->Mul(x, x);
    int32 y2 = Compiler->Mul(y, y);

    int32 sum = Compiler->Add(x2, y2);
    int32 distance = Compiler->SquareRoot(sum);

    return distance;*/
}

int32 UWKVertexShaderBase::SizeVector2D(FMaterialCompiler *Compiler, int32 X, int32 Y){
    int32 x2 = Compiler->Mul(X, X);
    int32 y2 = Compiler->Mul(Y, Y);
    int32 sum = Compiler->Add(x2, y2);
    int32 distance = Compiler->SquareRoot(sum);
    return distance;
}



int32 UWKVertexShaderBase::GetObjectPosition(FMaterialCompiler* Compiler)
{
    return Compiler->ObjectWorldPosition();
}

int32 UWKVertexShaderBase::GetVertexColor(FMaterialCompiler* Compiler)
{
    return Compiler->VertexColor();
}

int32 UWKVertexShaderBase::GetVertexNormal(FMaterialCompiler* Compiler)
{
    return Compiler->VertexNormal();
}

// Vertex-Tangent
int32 UWKVertexShaderBase::GetVertexTangent(FMaterialCompiler* Compiler)
{
    return Compiler->VertexTangent();
}

// Spielzeit
int32 UWKVertexShaderBase::GetTime(FMaterialCompiler* Compiler, bool bPeriodic, bool bRealTime)
{
    return Compiler->GameTime(bPeriodic, bRealTime);
}

//scaled time
int32 UWKVertexShaderBase::ScaledTime(FMaterialCompiler *Compiler, float scalar){
    return Compiler->Mul(
        GetTime(Compiler), //t,t
        MakeConstant(Compiler, scalar)
    );
}



// Kamerarichtung
int32 UWKVertexShaderBase::GetCameraVector(FMaterialCompiler* Compiler)
{
    return Compiler->CameraVector();
}




//operators
int32 UWKVertexShaderBase::Less(FMaterialCompiler *Compiler, int32 A, int32 B){
    int32 r = Compiler->Max(A, B);
    UE_LOG(LogTemp, Warning, TEXT("UWKVertexShaderBase::Less A=%d, B=%d R=%d"), A, B, r);
    return r == A ? MakeConstant(Compiler, 0.0f) : MakeConstant(Compiler, 1.0f);

    /*
    FMaterialCompiler::    
    If ( 
    int32 A,
    int32 B,
    int32 AGreaterThanB,
    int32 AEqualsB,
    int32 ALessThanB,
    int32 Threshold
    )
    
    return Compiler->If(
        A, 
        B, 
        MakeConstant(Compiler, 0.0f), 
        MakeConstant(Compiler, 0.0f),
        MakeConstant(Compiler, 1.0f),//int32 ALessThanB
        MakeConstant(Compiler, 0.0f)
    );*/
}



int32 UWKVertexShaderBase::Ceil(FMaterialCompiler *Compiler, FExpressionInput *inputOverride){
    if(ceilExpression && inputOverride){

        //UMaterialExpression *	Expression	
        //Material expression that this input is connected to, or NULL if not connected.

        //tmp swap out parent which should not exsist anyway.
        UMaterialExpression *ExpressionParentPrev = ceilExpression->Input.Expression;
        UMaterialExpression *tmpOverride = inputOverride->Expression;
        if(tmpOverride){
            ceilExpression->Input.Expression = tmpOverride;
            int32 result = ceilExpression->Compile(Compiler, 0);
            ceilExpression->Input.Expression = ExpressionParentPrev;
            return result;
        }


        
    }

    return INDEX_NONE;
}