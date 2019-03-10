/*************************
 * This program illustrates the use of interrupts on the Arduino Uno.
 * This code is based on an example by Dronebot Workshop: https://youtu.be/oQQpAACa3ac
 * 
 * Facts about Arduino Interrupts:
 * The Arduino Uno has two pins that can handle interrupt pulses from external circuitry (INT0 and INT1). These pins are mapped to digital pins 2 and 3 respectively.
 * When an interrupt occurs, the Arduino will break out of the main loop and execute code related to the interrupt. Then the main loop resumes execution.
 * The code perfomed by an interrupt is called an Interrupt Service Routine, or ISR. An ISR cannot have any parameters, and they shouldn’t return anything.
 * For best results, ISRs should be short and fast pieces of code. There are also some additional restrictions on the type of code that can be handled by an ISR. 
 * For example, functions that depend on the internal Arduino timer (e.g. delay(), tone(), millis(), etc.) will not work in an ISR. 
 * For additional restrictions and best practices, see Arduino reference here: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
 * In addition to external interrupts, the Arduino Uno can also handle internal interrupts triggered by the internal timer. To use these timer based interrupts, you must include the TimerOne Library.
 ************************/
#include "TimerOne.h"

const byte MOTOR1 = 2;  //assign hardware interrupt pins to each motor
const byte MOTOR2 = 3;
//IMPORTANT: Variables that need to be modified within an ISR MUST be declared as "volatile". See Arduino reference listed above.
volatile int encoder1 = 0; //variables to count encoder disc "ticks" each time the encoder circuitry sends an interrupt pulse. 
volatile int encoder2 = 0;

float slots = 20.00; //Number of slots in encoder disc. Used to track rotation and calculate speed.

/**************INTERRUPT SERVICE ROUTINES******************/
//Motor1 pulse count ISR
void ISR_encoder1() {
  encoder1++;
}

//Motor2 pulse count ISR
void ISR_encoder2() {
  encoder2++;
}

//TimerOne ISR
void ISR_timerone() {
  Timer1.detachInterrupt();   //Stop Timer1 to allow time for serial print out
  Serial.print("Motor Speed 1: ");
  float rotation1 = (encoder1 / slots) * 60.00;   //Calculate RPM. Since Timer1 is set for 1 second, we get the number of slot ticks per second. Divide this by number of slots on the encoder disc to get number of revolutions per second. Then, multiply by 60 to get revolutions per minute.
  Serial.print(rotation1);
  Serial.print(" RPM - ");
  encoder1 = 0;
  Serial.print("Motor Speed 2: ");
  float rotation2 = (encoder2 / slots) * 60.00;
  Serial.print(rotation2);
  Serial.print(" RPM - ");
  encoder2 = 0;
  Timer1.attachInterrupt(ISR_timerone);
}
 /*************END INTERRUPT SERVICE ROUTINES*************/


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MOTOR1, INPUT);
  pinMode(MOTOR2, INPUT);
  attachInterrupt(digitalPinToInterrupt(MOTOR1), ISR_encoder1, RISING);   //Attach interrupt service routines to hardware interrupt pins and set trigger mode.
  attachInterrupt(digitalPinToInterrupt(MOTOR2), ISR_encoder2, RISING);
  Timer1.initialize(1000000);   //Timer1 accepts parameter in microseconds. Set to one million for 1 second.
  Timer1.attachInterrupt(ISR_timerone);   //Enable the timer
}

void loop() {
  // put your main code here, to run repeatedly:

}
