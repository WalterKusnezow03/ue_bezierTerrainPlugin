#include "Payload.h"

Payload::Payload(){

}

Payload::~Payload(){
    interface = nullptr;
}

Payload::Payload(const Payload &other){
    if(this != &other){
        *this = other;
    }
}

Payload &Payload::operator=(const Payload &other){
    if(this != &other){
        interface = other.interface;
        message = other.message;
    }
    return *this;
}

Payload::Payload(IPayloadInterface *interfaceIn, FString messageIn){
    Setup(interfaceIn, messageIn);
}

void Payload::Setup(IPayloadInterface *interfaceIn, FString messageIn){
    interface = interfaceIn;
    message = messageIn;
}

void Payload::Execute(){
    if(interface){
        interface->Notify(message);
    }
    interface = nullptr;
}