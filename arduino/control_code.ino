#include "Prbs.h"
#include "sin.h"
#include <DueTimer.h>
#include <DuePWM.h>

#define PWM_FREQ1  1000 //1KHz
#define PWM_FREQ2  10000
DuePWM pwm( PWM_FREQ1, PWM_FREQ2 );

#define Sampling_Time 2000 // 1000usec = 1msec
//Input pins
#define FeedBack1 A0
#define Amp_prop A1
#define Acceleration A2

//output pins 
#define comm_prop_R 6
#define comm_prop_L 7
#define EnProp_R 52
#define EnProp_L 53

#define comm_dir_f 49
#define comm_dir_b 48
#define Endir_R 51
#define Endir_L 50
#define out1 44
#define out2 45
#define DAC DAC0

double command=0,Comm=0,commOld=0,commolder=0,err=0,errOld=0,errOlder=0,Iis_prop=0,Iload_prop=0,Iis_dirc_forward=0,Iload_dirc_forward=0,Iis_dirc_back=0,Iload_dirc_back=0;

volatile int FeedBack=0,FeedBack_AverageValue=0,Acc=0,Vis_prop=0,Vis_prop_average,Vis_dirc_forward=0,Vis_dirc_forward_copy=0,Vis_dirc_back=0,Vis_dirc_back_copy=0;
double Kd=12/3.3,setpoint=0.411*4095,Vis_prop_copy=0,Acc_Average_copy=0,Acc_Average=0,stepval=0;//Driver's Gain


volatile long count=0,count_copy=0;//20 ms
int n=0,count2=0,count3=0,tau=30,Tclk=14/2,n_prbs=0,FeedBack_copy=0; // n_prbs times to aplly
int Ris_prop=6800,Ris_dirc_forward=9600,Ris_dirc_back=9600;//Ohm
int Deadzone = 470; //0.1*4095;
volatile bool flagC=0,flagprbs=0,flagsqr=0,flagsin=0,flagn=0;
bool closeLoop=0,openLoop=0,ident=0,dir_f=0,dir_b=0;

int k=8500; //sens current raio of driver
int i=0;

//sin 
bool Sin=0;//flag
double fsin=1,Amp=15;//frequency
int n_sin=30;
int Tclksin=10/2;// for 10ms send

//srq
bool Sqr=0;//flag
double fsqr=0.3;
int n_sqr=20,Tclksqr=1000/(2*20*fsqr);

//step
bool step=0;

bool step1[20]={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0};


double KB=1.1,KF=1.1;

double Kp=1.15;

double K=3;
//for step
double kf=1;
double kb=1;
//for sin
//double kf=2;//0.7;
//double kb=2;//0.7;

//Moving Avarage filter
double a=0.1,b=0.9;

String receivedString="",dir="",speed="",pos="",check="",falgcase="";
char Mode;
//const long long sin[100]= {5,5.31400000000000,5.62700000000000,5.93700000000000,6.24300000000000,6.54500000000000,6.84100000000000,7.12900000000000,7.40900000000000,7.67900000000000,7.93900000000000.8.18700000000000,8.42300000000000,8.64500000000000,8.85300000000000,9.04500000000000,9.22200000000000,9.38200000000000,9.52400000000000,9.64900000000000,9.75500000000000,9.84300000000000,9.91100000000000,9.96100000000000,9.99000000000000,10	9.99000000000000,9.96100000000000,9.91100000000000,9.84300000000000,9.75500000000000,9.64900000000000,9.52400000000000,9.38200000000000,9.22200000000000,9.04500000000000,8.85300000000000,8.64500000000000,8.42300000000000,8.18700000000000,7.93900000000000,7.67900000000000,7.40900000000000,7.12900000000000,6.84100000000000,6.54500000000000,6.24300000000000,5.93700000000000,5.62700000000000,5.31400000000000,5	4.68600000000000,4.37300000000000,4.06300000000000,3.757000000000003,.45500000000000,3.15900000000000,2.87100000000000,2.59100000000000,2.32100000000000,2.06100000000000,1.81300000000000,1.57700000000000,1.35500000000000,1.14700000000000,0.955000000000000,0.778000000000000,0.618000000000000,0.476000000000000,0.351000000000000,0.245000000000000,0.157000000000000,0.0890000000000000,0.0390000000000000,0.0100000000000000,0,0.0100000000000000,0.0390000000000000,0.0890000000000000,0.157000000000000,0.245000000000000,0.351000000000000,0.476000000000000,0.618000000000000,0.778000000000000,0.955000000000000,1.14700000000000,1.35500000000000,1.57700000000000,1.81300000000000,2.06100000000000,2.32100000000000,2.59100000000000,2.87100000000000,3.15900000000000,3.45500000000000,3.75700000000000,4.06300000000000,4.37300000000000,4.68600000000000,5}


void setup() {

  // put your setup code here, to run once:
Serial.begin(115200);
Timer1.attachInterrupt(Read_ADC).start(Sampling_Time);  // attaches callback() as a timer overflow interrupt 

   // uint32_t pwm_duty = 512; // 50% duty cycle for res 10 bits
    pwm.setFreq1( PWM_FREQ1 );
    pwm.setFreq2( PWM_FREQ2 );
    pwm.pinFreq1( comm_prop_R );  // Pin 6 freq set to "pwm_freq1" on clock A
    pwm.pinFreq2( comm_prop_L );  // Pin 7 freq set to "pwm_freq2" on clock B

    //pwm.pinFreq1( comm_dir_f );  // Pin 8 freq set to "pwm_freq1" on clock A
    //pwm.pinFreq2( comm_dir_b );  // Pin 9 freq set to "pwm_freq2" on clock B

    pinMode(comm_dir_f,OUTPUT);
    pinMode(comm_dir_b,OUTPUT);

pinMode(EnProp_R,OUTPUT);
pinMode(EnProp_L,OUTPUT);
pinMode(Endir_R,OUTPUT);
pinMode(Endir_L,OUTPUT);
 
pinMode(out1,OUTPUT);//test
pinMode(out2,OUTPUT);

digitalWrite(EnProp_R,HIGH);
digitalWrite(EnProp_L,HIGH);
digitalWrite(Endir_R,HIGH);
digitalWrite(Endir_L,HIGH);


analogWriteResolution(12);
analogReadResolution(12);
//Serial.println("Send Mode 'o' or 'c' then com");



}

void Read_ADC(void) {

 flagC=1;
 count++;
    if(count2>=Tclksin)
     {
      flagsin=1;
      count2=0;
     }
      count2++;

    if(count3>=Tclksqr)
     {
      flagsqr=1;
      count3=0;
     }
      count3++;
 FeedBack= analogRead(FeedBack1); // potintiometer feedback
 FeedBack_AverageValue = (FeedBack+analogRead(FeedBack1)+analogRead(FeedBack1)+analogRead(FeedBack1))>>2;
// FeedBack_AverageValue =a*FeedBack+b*FeedBack_copy;

 Acc=analogRead(Acceleration);//Accelraction
 Acc_Average = (Acc+analogRead(Acceleration)+analogRead(Acceleration)+analogRead(Acceleration))>>2;

 Vis_prop=analogRead(Amp_prop); // Amp feedback for propotional Valve
 //Serial.println(FeedBack_copy);
 //analogWrite(DAC0,command);
 //Serial.println(String(count)  + "\t" + String(command) + "\t" + String(FeedBack_copy));
  
}

void loop() {
  // put your main code here, to run repeatedly:
   FeedBack_copy=4095-FeedBack_AverageValue-40; // offset ..feedback range 0 ~ 4000
  count_copy=count;
  Vis_prop_copy=Vis_prop_average;
  Vis_prop_copy=Vis_prop_copy*3.3/4095;
  Acc_Average_copy=Acc_Average/4095*3.3;

  // calculation current for the Valves
   Iload_prop=0.6677*Vis_prop_copy*Vis_prop_copy*Vis_prop_copy*Vis_prop_copy*Vis_prop_copy- 3.775*Vis_prop_copy*Vis_prop_copy*Vis_prop_copy*Vis_prop_copy + 7.911*Vis_prop_copy*Vis_prop_copy*Vis_prop_copy- 7.814*Vis_prop_copy*Vis_prop_copy + 4.685*Vis_prop_copy + 0.1956;
    if(Iload_prop > 0.7)
    Iload_prop=Iload_prop-0.05;
    if(Iload_prop < 0.21)
    Iload_prop=0;

  if (Serial.available() > 0) {
    receivedString = Serial.readStringUntil('\n');  // قراءة الجملة المستقبلة حتى نهاية السطر
      Mode = receivedString.charAt(0);
      //Serial.println(receivedString);


if(Mode == 'o') //open loop
  {  
    openLoop=1;
    closeLoop=0;

    int firstSpaceIndex = receivedString.indexOf(' ');
    int secondSpaceIndex = receivedString.indexOf(' ', firstSpaceIndex + 1);
    dir = receivedString.substring(firstSpaceIndex + 1,secondSpaceIndex);
    speed = receivedString.substring(secondSpaceIndex + 1);
    if(dir=="Forward")
      {
        dir_f=1;
        dir_b=0;
      }
      else {
        dir_f=0;
        dir_b=1;
      }
    command=speed.toInt(); //per %
    command=command/100*4095;//digital
  }
  if(Mode == 'c') //close loop
  {  
    openLoop=0;
    closeLoop=1;

    int firstSpaceIndex = receivedString.indexOf(' ');
    int secondSpaceIndex = receivedString.indexOf(' ', firstSpaceIndex + 1);
    int thirdSpaceIndex = receivedString.indexOf(' ', secondSpaceIndex + 1);
    int forthSpaceIndex = receivedString.indexOf(' ', thirdSpaceIndex + 1);
    int fifthSpaceIndex = receivedString.indexOf(' ', forthSpaceIndex + 1);


      falgcase = receivedString.substring(firstSpaceIndex + 1,secondSpaceIndex);

    //setpoint=setpoint/100*4095;//digital
    //step=check.toInt();
    if(falgcase == "step")
    {
          step=1;
          Sin=0;
          Sqr=0;
          pos = receivedString.substring(secondSpaceIndex + 1,thirdSpaceIndex);
          setpoint = pos.toInt()*4000/74; //per %
        //Serial.println(setpoint);
    }
    else if(falgcase == "sin")
    {
        Sin=1;
        step=0;
        Sqr=0;
        Amp = receivedString.substring(secondSpaceIndex + 1,thirdSpaceIndex).toInt()*100/74;
        fsin = receivedString.substring(thirdSpaceIndex + 1,forthSpaceIndex).toInt();

    
    } 
    else if(falgcase == "Sqr")
    {
      Sqr=1;
      step=0;
      Sin=0;
    }
  }
    // تفريغ المخزن المؤقت بعد القراءة
    while (Serial.available() > 0) {
      Serial.read();
    }
  
  }
  if(Mode == 's') // stop
{
    openLoop=0;
    closeLoop=0;
    command=0;
    pwm.pinDuty( comm_prop_R, 0);  // Write 25% duty With 1KHz
    pwm.pinDuty( comm_prop_L, 0);  // Write 50% duty With 1KHz
    digitalWrite(comm_dir_f,0);
    digitalWrite(comm_dir_b,0);
}

if(closeLoop)
    if (flagC)
  {
    
    errOlder=errOld;
    errOld = err;
    commolder=commOld ;
    commOld= Comm; 
    
    err = setpoint - (FeedBack_copy);// error
    
    if(abs(err) < 50)
     {
      err=0;
      errOld=0;
     } 

    if(err>0)
    {

    Comm = 0.9725*commOld + KF * (err - 0.993*errOld);//good controller      
        command = Comm + Deadzone ;
      
      //command=kf*err/Kd +Deadzone;
      dir_f = 1; //Forward
      dir_b = 0;
    }
    else {

       Comm = 0.9725*commOld +  KB *(err - 0.993*errOld);//good controller
          command = Comm - Deadzone ;

      //command=kb*err/Kd -Deadzone;
      dir_f = 0; //Backward
      dir_b = 1;
         }


         if(abs(err) < 50)
       {
        command=0;
        Comm=0;
       }

    if( abs(command) > 3000)
    command=3000;
    ///// propotional Valve 
    pwm.pinDuty( comm_prop_R, abs(command) );  // Write 
    pwm.pinDuty( comm_prop_L, 0 );  // Write 
    //analogWrite(DAC0,abs(command));
   
    analogWrite(DAC0,setpoint);

    ////  Directional Valve
    digitalWrite(comm_dir_f,dir_f);
    digitalWrite(comm_dir_b,dir_b);

  //if(count_copy %10 ==0)
        Serial.println(String(setpoint/4000*74)  + "\t" + String(command/4000*12) + "\t" + String(FeedBack_copy/4000*74) +  "\t" + String(Iload_prop));

    if(Sqr)
    { 
        if(flagsqr)
     {    
        if(step1[i])
         {
          setpoint=0.5*4000 + 0.2*4000;
          //digitalWrite(out1,0);      
         }
        else
         {  
          setpoint=0.5*4000 - 0.2*4000;
          //digitalWrite(out1,1);
         }
         i++;
         flagsqr=0;
         if (i>19)
          i=0;       
      }

    }
    if(Sin)
     {
      
      if(flagsin)
      {
       if(fsin == 1)
        {
           //KB=1,KF=1;
           setpoint=(Amp*sin1[i]+47)/100*4000;
           i++;
           if (i>Len1-1)
           {
            i=0;
           n++;
           } 
        }
        else if(fsin == 1.5)
        {
           //KB=0.9,KF=0.9;
           setpoint=(Amp*sin15[i]+47)/100*4000;
           i++;
           if (i>Len15-1)
            i=0;
           
        }
        else if(fsin == 2)
        {
           //KB=0.8,KF=0.8;
           setpoint=(Amp*sin2[i]+47)/100*4000;
           i++;
           if (i>Len2-1)
            i=0;
        }
        else if (fsin == 2.5)
            {
           //KB=0.8,KF=0.8;
           setpoint=(Amp*sin25[i]+47)/100*4000;
           i++;
           if (i>Len25-1)
            i=0;
             }
        else if (fsin == 3)
            {
           //KB=0.8,KF=0.8;
           setpoint=(Amp*sin3[i]+47)/100*4000;
           i++;
           if (i>Len3-1)
            i=0;
             }
      flagsin=0;
     }
      }



    
    flagC=0;

  }
if(openLoop)
  {
    ///// propotional Valve 
    pwm.pinDuty( comm_prop_R, command);  // Write 
    pwm.pinDuty( comm_prop_L, 0);  // Write

    ////  Directional Valve
        digitalWrite(comm_dir_f,dir_f);
        digitalWrite(comm_dir_b,dir_b);

  }
 

}
