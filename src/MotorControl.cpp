#include "MotorControl.h"



void InitMotor(){
    pinMode(MotorPin1, OUTPUT);
    pinMode(MotorPin2, OUTPUT);
}

void MotorLeftRotation(){
    digitalWrite(MotorPin1, HIGH);
    digitalWrite(MotorPin2, LOW);
}

void MotorRightRotation(){
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, HIGH);   
}

//Bremse
void MotorBreak(){
    digitalWrite(MotorPin1, HIGH);
    digitalWrite(MotorPin2, HIGH);
}

//Freilauf
void MotorFreeWheeling(){
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, LOW);
}


void RampUpMotor(uint8_t MotorSpeed){
    for (int i=0; i<MaxPWMMotor; i++) { 
        analogWrite(MotorEnable, i); 
        delay(10); 
    } 
} 

void RampDownMotor(uint8_t MotorSpeed){
    for (int i=MaxPWMMotor; i>=0; i--) {
        analogWrite(MotorEnable, i);
        delay(10);
    }
}