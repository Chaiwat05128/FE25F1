#include "Mapf.h"
#include "Servo.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include "CameraHandler.h"
#include <PID_v2.h>

#include <NeoSWSerial.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

extern NeoSWSerial mySerial;

Servo myservo1;
Servo myservo2;

float pvYaw;
long open_timer;

#define RX_PIN 3  // Connect to OpenMV TX
#define TX_PIN 5  // Not used in this case, but define it if needed
NeoSWSerial mySerial(RX_PIN, TX_PIN);

BlobData blob;
BlobData purple_blob1;
BlobData purple_blob2;

//  Compass Variables

float yawOffset = 0;  // Stores the zero reference


const int buzzerPin = 4;

//  Motor B
// const int E1Pin = 10;
// const int M1Pin = 11;
// const int E2Pin = 12;
// const int M2Pin = 13;

const int ENA = 11;  // Motor A speed control (PWM)
const int IN1 = 12;  // Motor A direction 1
const int IN2 = 13;  // Motor A direction 2

/**inner definition**/
typedef struct {
  byte enPin;
  byte directionPin;
} MotorContrl;

const int M1 = 0;
const int MotorNum = 1;

// const MotorContrl MotorPin[] = { { E1Pin, M1Pin }, { E2Pin, M2Pin } };

const int Forward = LOW;
const int Backward = HIGH;

//  Servos
int const STEER_SRV = 9;
int const ULTRA_SRV = 8;

//  Ultrasonic Sensor
int const ULTRA_PIN = A3;

//  Light Sensors
int const RED_SEN = A1;
int const GEEN_SEN = A2;

//  Button
int const BUTTON = A0;

char TURN = 'U';
int compass_offset = 0;
long halt_detect_line_timer;
bool found_block = false;
int lines_detect_num = 0;
int Servo_Value;

//  Field Config
char Blocks_TURN = 'U';
float avoidance_degree = 0;
long timer_block_decay;
float found_block_factor;
int x = -1;
int Y = 30;
int count = 0;
int realCount = 0;

CameraHandler camera;
// Specify the links and initial tuning parameters
PID_v2 compassPID(0.6, 0.0001, 0.02, PID::Direct);

void setup() {
  compassPID.Start(0, 0, 0);
  compassPID.SetOutputLimits(-180, 180);
  compassPID.SetSampleTime(10);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(STEER_SRV, OUTPUT);
  pinMode(ULTRA_SRV, OUTPUT);
  pinMode(ULTRA_PIN, INPUT);
  pinMode(RED_SEN, INPUT);
  pinMode(GEEN_SEN, INPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  mySerial.begin(19200);
  // initMotor();
  // pixy.changeProg("color_connected_components");
  // while (!Serial)
  //   ;
  BNOset();
  myservo1.attach(ULTRA_SRV, 600, 2500);
  myservo2.attach(STEER_SRV, 600, 2400);
  steering_servo(0);
  ultra_servo(0, 'L');
  // check_leds();
  // while (true) {
  // getIMU();
  // Serial.print("Compass : ");
  // Serial.println(pvYaw);
  // }
  // myservo1.write(50);
  // delay(100000);
  zeroYaw();
  simpleBeep();
  while (analogRead(BUTTON) > 500)
    ;
  simpleBeep();

  while (analogRead(BUTTON) <= 500)
    ;
  delay(600);
}

void loop() {
  long countdown_stop = millis();

  while (analogRead(BUTTON) > 500) {
    camera.handleIncomingData();
    getBNO();
    line_detection();
    int distance_wall = getDistance();

    // Use TURN (not Blocks_TURN) for consistency
    int steering_degree = (1 * x) * compassPID.Run((x * pvYaw) + ((distance_wall - Y)) * ((float(Blocks_TURN == 'TURN') - 0.5) * 2)) * -1;

    if (millis() - open_timer > 50) {
      avoidance_degree = calculate_avoidance();
      open_timer = millis();
    }

    int final_degree = map(max(found_block, found_block_factor), 0, 1, steering_degree, mapf(min(max(distance_wall, 5), 40), 5, 40, steering_degree, avoidance_degree * 2.5));

    if (count >= 1) {

      long timer01 = millis();
      while (millis() - timer01 < 4000) {
        camera.handleIncomingData();
        getBNO();
        // line_detection();
        int distance_wall = getDistance();

        // Use TURN (not Blocks_TURN) for consistency
        int steering_degree = (1 * x) * compassPID.Run((x * pvYaw) + ((distance_wall - Y)) * ((float(Blocks_TURN == 'TURN') - 0.5) * 2)) * -1;

        if (millis() - open_timer > 50) {
          avoidance_degree = calculate_avoidance();
          open_timer = millis();
        }

        int final_degree = map(max(found_block, found_block_factor), 0, 1, steering_degree, mapf(min(max(distance_wall, 5), 40), 5, 40, steering_degree, avoidance_degree * 2.5));
        //   getBNO();
        //   line_detection();
        //   motor_and_steer((1 * x) * compassPID.Run((x * pvYaw) + ((distance_wall - Y)) * ((float(TURN == 'TURN') - 0.5) * 2)) * -1);
        //   ultra_servo(-pvYaw, TURN);  // Use TURN here too
        ultra_servo(-pvYaw, Blocks_TURN);
        motor_and_steer(final_degree);
      }
      count = 0;
      realCount++;
      // break;
    }

    if (realCount >= 1) {
    setMotorPercent(0);
    simpleBeep();
    while (true) {
    }
    // Use TURN for servo direction (this should match the robot's movement direction)
  }


    ultra_servo(-pvYaw, Blocks_TURN);
    motor_and_steer(final_degree);
  }

  setMotorPercent(0);
  while (analogRead(BUTTON) <= 500)
    ;
  while (analogRead(BUTTON) > 500)
    ;
  while (analogRead(BUTTON) <= 500)
    ;
}