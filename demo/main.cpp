#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"
#include "string.h"

Ticker servo_ticker;
Timer t;
DigitalInOut ping(D11);

PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX, USBRX);
BufferedSerial xbee(D10, D9);

BBCar car(pin5, pin6, servo_ticker);
BufferedSerial uart(D1,D0); //tx,rx
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread pingthread(osPriorityNormal);
void angle_cal(float angle);
void pingf(void);
char recv[1];
char angbuf[32];
float val = 0;
float angle = 0;

int main(){
   uart.set_baud(9600);
   int i = 0;
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&pc, "r");
   FILE *devout = fdopen(&pc, "w");
   //float angle = 0;
   queue.call(pingf);
   pingthread.start(callback(&queue, &EventQueue::dispatch_forever));
   while(1){
      
      if(uart.readable()){
            uart.read(recv, sizeof(recv));
            if(recv[0] == '\n'){
                angle = 0;
                angle = atof(angbuf);
                //xbee.write(angbuf, sizeof(angbuf));
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
            
            
      }
      else{
          car.stop();
      }
   }
}

void angle_cal(float angle){
   


    if(angle>10){
        car.turn(100, -0.3);
        ThisThread::sleep_for(250ms);
        car.stop();
        //printf("turn right\r\n");
    }
    else if(angle<-10){
        car.turn(100, 0.3);
        ThisThread::sleep_for(250ms);
        car.stop();
        //printf("turn left\r\n");
    }
    else{
        car.goStraight(100);
        ThisThread::sleep_for(1000ms);
        car.stop();
        //printf("go straight\r\n");
    }
   
}

void pingf(void){

  while(1) {

      ping.output();
      ping = 0; wait_us(200);
      ping = 1; wait_us(5);
      ping = 0; wait_us(5);

      ping.input();
      while(ping.read() == 0);
      t.start();
      while(ping.read() == 1);
      val = t.read();
      //printf("Ping = %lf\r\n", val*17700.4f);
      char val_buf[40];
      sprintf(val_buf,"r :%f  theta: %f\r\n",val*17700.4f , angle);
      
      xbee.write(val_buf, 40);
      t.stop();
      t.reset();

      ThisThread::sleep_for(1s);
   }
}