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
void car_circle(void);
void pingf(void);
char angbuf[32];
float val = 0;
float angle = 0;
void RPC_linedetect (Arguments *in, Reply *out);
RPCFunction RPC_line(&RPC_linedetect, "line");


int main(){
   uart.set_baud(9600);
   int i = 0;
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&uart, "r"); // uart
   FILE *devout = fdopen(&uart, "w"); //uart
   queue.call(pingf);
   pingthread.start(callback(&queue, &EventQueue::dispatch_forever));
   printf("demo start\r\n");
   while(1){
    if(val*17700.4f<10){
          car.stop();
          car_circle();
    }
    else if(uart.readable()){
            memset(buf, 0, 256);      // clear buffer
            for(int i=0; i<255; i++){
                char recv = fgetc(devin);
                if (recv == '\n') {
                    buf[i] = fputc(recv, devout);
                    printf("\r\n");
                    break;
                }
                buf[i] = fputc(recv, devout);
            }
            RPC::call(buf, outbuf);
        }
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
      t.stop();
      t.reset();
      //printf("ping %f\r\n", val*17700.4f);

      ThisThread::sleep_for(100ms);
   }
}


void angle_cal(float angle){
        printf("%f\r\n", angle);

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

void car_circle(void){
    printf("turn\r\n");
    car.turn(200, -0.3);
    ThisThread::sleep_for(1000ms);
    car.stop();

    car.goStraight(100);
    ThisThread::sleep_for(500ms);
    car.stop();

    car.turn(200, 0.3);
    ThisThread::sleep_for(6000ms);
    car.stop();
    return;
}

void RPC_linedetect (Arguments *in, Reply *out)   {
    angle = in->getArg<float>();
    angle_cal(angle);
    return;
}