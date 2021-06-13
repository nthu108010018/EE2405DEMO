#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX, USBRX);
BufferedSerial xbee(D10, D9);

BBCar car(pin5, pin6, servo_ticker);
BufferedSerial uart(D1,D0); //tx,rx
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread thread;
void angle_cal(float angle);
char recv[1];
char angbuf[32];
int flag = 0;

int main(){
   uart.set_baud(9600);
   int i = 0;
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&pc, "r");
   FILE *devout = fdopen(&pc, "w");
   float angle = 0;
   while(1){
      
      if(uart.readable()){
            uart.read(recv, sizeof(recv));
            if(recv[0] == '\n'){
                angle = atof(angbuf);
                flag = 1;
                angle_cal(angle);
                for(int j = 0; j++; j<32){
                    angbuf[j] = '0';
                }
                i = 0;
            }
            else{
                angbuf[i] = recv[0];
                i++;
            }
            
            xbee.write(angle, 32);
            
      }
   }
}

void angle_cal(float angle){
   


    if(angle>10){
        car.turn(100, 0.3);
        ThisThread::sleep_for(500ms);
        car.stop();
        //printf("turn right\r\n");
    }
    else if(angle<-10){
        car.turn(100, -0.3);
        ThisThread::sleep_for(500ms);
        car.stop();
        //printf("turn left\r\n");
    }
    else{
        car.goStraight(100);
        ThisThread::sleep_for(1s);
        car.stop();
        //printf("go straight\r\n");
    }
   
}