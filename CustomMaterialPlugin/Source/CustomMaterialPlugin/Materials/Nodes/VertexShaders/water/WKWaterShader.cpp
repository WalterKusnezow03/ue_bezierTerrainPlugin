#include "WKWaterShader.h"


void UWKWaterShader::SetupOutputsOnConstruct(){
    Super::SetupOutputsOnConstruct();
    FExpressionOutput vertexOutput;
    vertexOutput.OutputName = FName(TEXT("VertexOutput"));
    expressionOutputArray.Add(vertexOutput);

    FExpressionOutput normalOutput;
    normalOutput.OutputName = FName(TEXT("NormalOutput"));
    expressionOutputArray.Add(normalOutput);
}

FString UWKWaterShader::GetDescription() const {
    return TEXT("First output: Vertex Displacement, Second output: Normals!");
}



// ----- Shader -----

#if WITH_EDITOR

//convert to layer codes
int32 UWKWaterShader::AmplitudeCode(FMaterialCompiler *Compiler){
    return MakeConstant(Compiler, Amplitude);
}

int32 UWKWaterShader::Frequency(FMaterialCompiler *Compiler){
    //float frac = 100.0f / DistanceBetweenWaves;
    float f = 2.0f * M_PI / DistanceBetweenWaves; // 1 Welle pro DistanceBetweenWaves Meter
    return MakeConstant(Compiler, f);
}


//// ---- t * v part ----

int32 UWKWaterShader::RunTime(FMaterialCompiler *Compiler){
    int32 TimeNow = GetTime(Compiler); //t,t
    return TimeNow;
}
int32 UWKWaterShader::Speed(FMaterialCompiler *Compiler){
    return MakeConstant(Compiler, IntervallSlowDown);
}








FString UWKWaterShader::NodeName() const {
    return TEXT("WKWaterShader");
}

int32 UWKWaterShader::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{

    //return CompileVertexShader(Compiler);

    if(OutputIndex == vertexOutputLayer){
        return CompileVertexShader(Compiler);
    }
    if(OutputIndex == normalOutputLayer){
        return CompileNormalShader(Compiler);
    }
    return INDEX_NONE; // oder ein Fallback
}

int32 UWKWaterShader::CompileVertexShader(
    FMaterialCompiler *Compiler
){
    int32 x_frequency = Compiler->Mul(WorldPosX(Compiler), Frequency(Compiler));
    int32 y_frequency = Compiler->Mul(WorldPosY(Compiler), Frequency(Compiler));

    //int32 time = RunTime(Compiler);
    //int32 speed = Speed(Compiler);

    //shaderRunningTime * speed
    int32 scaledTime = ScaledTime(Compiler, IntervallSlowDown);

    int32 x = Compiler->Add(x_frequency, scaledTime);
    int32 y = Compiler->Add(y_frequency, scaledTime);

    int32 sinX = Compiler->Sine(x);
    int32 cosY = Compiler->Cosine(y);

    int32 waveSumAndOffset = Compiler->Add(sinX, cosY);
    int32 zResult = Compiler->Mul(waveSumAndOffset, AmplitudeCode(Compiler));

    //int32 MakeVector3D(FMaterialCompiler *compiler, int32 X, int32 Y, int32 Z)
    int32 result = MakeVector3D(
        Compiler, 
        MakeConstant(Compiler, 0.0f),
        MakeConstant(Compiler, 0.0f),
        zResult
    );
    return result;

    /*
    (Shader from terrain plugin i used for water.)
    /// @brief apply vertex shader to the given vertex
    /// @param vertex vertex to move
    void AcustomWaterActor::applyShaderToVertex(FVector &vertex){
        FVector actorLocation = GetActorLocation();
        float distXAll = vertex.X + actorLocation.X;
        float distYAll = vertex.Y + actorLocation.Y;

        float frequency = 0.01f; // Wellenbreite
        float amplitude = 10.0f; // Wellenhöhe
        float speed = 1.0f; // Wellengeschwindigkeit //1.0;
        float wave = sin(distXAll * frequency + shaderRunningTime * speed) + 
                    cos(distYAll * frequency + shaderRunningTime * speed);

        vertex.Z = wave * amplitude;

    }
    */


}



int32 UWKWaterShader::CompileNormalShader(FMaterialCompiler *Compiler){
    //wave height as scalar value between -2 and 2 (at max?)
    int32 f = Frequency(Compiler);

    //from vertex shader
    int32 x_frequency = Compiler->Mul(WorldPosX(Compiler), Frequency(Compiler));
    int32 y_frequency = Compiler->Mul(WorldPosY(Compiler), Frequency(Compiler));

    int32 time = RunTime(Compiler);
    int32 speed = Speed(Compiler);

    //shaderRunningTime * speed
    int32 scaledTime = Compiler->Mul(time, speed);

    int32 x = Compiler->Add(x_frequency, scaledTime);
    int32 y = Compiler->Add(y_frequency, scaledTime);





    //ableiten um tangente aufzustellen
    //die ableitung ist die steigung der funktion.
    //f(x,y) = sin((x * f) + (t * v)) + cos((y * f) +(t * v));
    //f'(x) = cos((x * f) + (t * v)) * f
    //f'(y) = -sin((x * f) + (t * v)) * f

    int32 dZdX = Compiler->Cosine(x);
    int32 dZdYAbs = Compiler->Sine(y);
    int32 dZdY = Compiler->Mul(dZdYAbs, MakeConstant(Compiler, -1.0f));

    //cdot F
    int32 dZdXf = Compiler->Mul(dZdX, f);
    int32 dZdYf = Compiler->Mul(dZdY, f);


    //tangente aufstellen als vektor richtung x und z
    int32 TangentX = MakeVector3D(
        Compiler, 
        MakeConstant(Compiler, 1.0f),
        MakeConstant(Compiler, 0.0f),
        dZdXf
    );

    int32 TangentY = MakeVector3D(
        Compiler, 
        MakeConstant(Compiler, 0.0f),
        MakeConstant(Compiler, 1.0f),
        dZdYf
    );

    int32 cross = Compiler->Cross(TangentX, TangentY);
    return Compiler->Normalize(cross);
}





#endif