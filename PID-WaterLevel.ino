#include <TimerOne.h>

const int trigPin = 9; // Chân trig
const int echoPin = 10; // Chân echo
const int in1 = 5; // Chân in1
const int in2 = 6;// Chân in2
const int ena = 3;// Chân ena

long duration;// Biến đo thời gian
float distance;// Biến đo khoảng cách
int tankHeight=25;
float waterLevel;
float T;  
double E,E1,E2;
double alpha, beta, gamma;
double Output, LastOutput; // output là ena mà mình tính ra để điều khiển cho động cơ quay
float senHeight = 23.2;


float Giatridat = 17; // Giá trị mong muốn (cm)
#define Kp 35
#define Kd 0.3
#define Ki 1

void setup() {
  pinMode(trigPin, OUTPUT); // khai báo các chân in out
  pinMode(echoPin, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);

  E = 0; E1 = 0; E2 = 0;
  Output = 0; LastOutput = 0;
  T=0.01; // thời gian lấy mẫu

  Serial.begin(9600);// baudrate 9600

  Timer1.initialize(10000);
  Timer1.attachInterrupt(PID);

}
void sr04(){
// Phát xung siêu âm
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Đo thời gian phản hồi
  duration = pulseIn(echoPin, HIGH);
  
  // Tính khoảng cách
  distance = duration /29.412 / 2;// hệ số chuyển đổi từ m sang cm

  // Chiều cao mực nước trong bình
  waterLevel = senHeight - distance;
  delay(T);
}

void PID()
{ 
  E = Giatridat - waterLevel;
  alpha = 2*T*Kp + Ki*T*T + 2*Kd;
  beta = T*T*Ki - 4*Kd - 2*T*Kp;
  gamma = 2*Kd;
  Output = (alpha*E + beta*E1 + gamma*E2 + 2*T*LastOutput)/(2*T);
  LastOutput = Output;

  E2=E1;
  E1=E;
  if(Output > 255)
    Output = 255;
  if(Output < 0)
    Output = 0;
  if(Output > 0)
  {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(ena,Output);
  }
  else 
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    analogWrite(ena,Output);
  }
}
void loop() {
  sr04();

  Serial.print(Output); Serial.print(" ");
  Serial.print(waterLevel);
  Serial.print("");Serial.println("");
    // Serial.println(waterLevel);
  // Serial.println(distance);
}