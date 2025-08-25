void BNOset() {
  Serial.println("Initializing BNO055...");
  if (!bno.begin()) {
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
  }

  delay(1000);  // Give the sensor some time to initialize
  bno.setExtCrystalUse(true);
  Serial.println("BNO055 initialized successfully!");
}

/* Read BNO055 and compute relative yaw */
bool getBNO() {
  sensors_event_t event;
  bno.getEvent(&event);

  // Wrap raw yaw to -180 ~ 180
  float yaw = wrap_value(event.orientation.x, -179.0, 180.0);

  // Compute relative yaw using the offset
  pvYaw = yaw;
  pvYaw = wrap_value(pvYaw + compass_offset + min(max(mapf(getDistance(), 10, 25, 0, 1), 0), 1) * mapf(min(millis() - halt_detect_line_timer, 1000), 0, 1000, 25, 0) * (compass_offset >= 0 - 0.5) * 2, -179, 180);
}




int wrapValue(int value, int minValue, int maxValue) {
  int range = maxValue - minValue + 1;
  if (value < minValue) {
    value += range * ((minValue - value) / range + 1);
  }
  return minValue + (value - minValue) % range;
}

float getDistance() {
  return min(mapf(analogRead(ULTRA_PIN), 0, 1023, 0, 500), 50);
}


void ultra_servo(int degree, char mode_steer) {
  int middle_degree = 0;
  if (mode_steer == 'F') {
    middle_degree = 270;
  } else if (mode_steer == 'R') {
    middle_degree = 360;
  } else if (mode_steer == 'L' || mode_steer == 'U') {
    middle_degree = 180;
  } else {
    // beep();
  }
  Servo_Value = ((max(min(middle_degree + degree, 360), 180)) / 2);
  myservo1.write(Servo_Value);
  // myservo1.write(mapf(max(min(middle_degree + degree, 225), 45), 0, 270, 0, 180));
}

void steering_servo(int degree) {
  myservo2.write(mapf(135 + max(min(degree, 25), -25), 0, 270, 0, 180));
}

void motor_and_steer(int degree) {
  degree = max(min(degree, 35), -35);
  steering_servo(degree);
  setMotorPercent(map(abs(degree), 0, 45, 70, 70));
  // setMotorSpeed2(-100);
}

// void beep() {
//   digitalWrite(BUZZER, HIGH);
//   delay(100);
//   digitalWrite(BUZZER, LOW);
// }

void line_detection() {
  int GEEN_value = analogRead(GEEN_SEN);

  found_block_factor = min(max(mapf(millis() - timer_block_decay, 0, 1000, 1, 0), 0), 1);

  if (TURN == 'U') {
    int red_value = analogRead(RED_SEN);
    if (GEEN_value < 400 || red_value < 500) {
      int lowest_red_sen = red_value;
      long timer_line = millis();
      while (millis() - timer_line < 100) {
        int red_value = analogRead(RED_SEN);
        if (red_value < lowest_red_sen) {
          lowest_red_sen = red_value;
        }
      }
      if (lowest_red_sen > 500) {
        // Red
        TURN = 'L';
        Blocks_TURN = 'R';
        compass_offset -= 90;
        x = -1;
        Y = 16;
        // beep();
      } else {
        // Blue
        TURN = 'R';
        Blocks_TURN = 'L';
        compass_offset += 90;
        x = 1;
        Y = 28;
        // beep();
        // delay(100);
        // beep();
        // delay(100);
        // beep();
      }
      count++;
      halt_detect_line_timer = millis();
    }
  } else {
    if (millis() - halt_detect_line_timer > 800) {
      if (found_block) {
        timer_block_decay = millis();
      }
      if (GEEN_value < 400) {
        if (TURN == 'L') {
          TURN = 'L';
          x = -1;
          compass_offset -= 90;
        } else {
          TURN = 'R';
          compass_offset += 90;
          x = +1;
        }
        halt_detect_line_timer = millis();
        count++;
      }
    }
  }
}

void simpleBeep() {
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}

// void initMotor() {
//   int i;
//   for (i = 0; i < MotorNum; i++) {
//     digitalWrite(MotorPin[i].enPin, LOW);

//     pinMode(MotorPin[i].enPin, OUTPUT);
//     pinMode(MotorPin[i].directionPin, OUTPUT);
//   }
// }

// /**  motorNumber: M1, M2
// direction:          Forward, Backward **/
// void setMotorDirection(int motorNumber, int direction) {
//   digitalWrite(MotorPin[motorNumber].directionPin, direction);
// }

// /** speed:  0-100   * */
// inline void setMotorSpeed(int motorNumber, int speed) {
//   analogWrite(MotorPin[motorNumber].enPin, (int)(255.0 * ((float)speed / 100.0)));
// }

void zeroYaw() {
  sensors_event_t event;
  bno.getEvent(&event);
  float yawOffset = wrap_value(event.orientation.x, -180.0, 180.0);
}

// float yawOffset = constrain(event.orientation.x, -180.0, 180.0);
// Serial.print("Yaw offset set to: ");
// Serial.println(yawOffset);

float wrap_value(float value, float min_value, float max_value) {
  float range_val = max_value - min_value;
  while (value < min_value) value += range_val;
  while (value > max_value) value -= range_val;
  return value;
}



void check_leds() {
  while (true) {
    Serial.print("Geen: ");
    Serial.print(analogRead(GEEN_SEN));
    Serial.print("   Red: ");
    Serial.println(analogRead(RED_SEN));
    line_detection();
  }
}

void check_BUTTON() {
  while (true) {
    Serial.print("Button: ");
    Serial.println(analogRead(BUTTON));
  }
}

/*
  Motor control with speed scale -100 to +100
  Arduino Motor Shield R3 (L293D)
  Motor A: PWM on pin 3, direction on pins 12 & 13
*/



// Function to set motor speed (-100 to +100)
// void setMotorSpeed2(int speedPercent) {
//   speedPercent = constrain(speedPercent, -100, 100);  // clamp range

//   if (speedPercent > 0) {
//     // Forward
//     digitalWrite(motorDirPin1, HIGH);
//     digitalWrite(motorDirPin2, LOW);
//     analogWrite(motorSpeedPin, map(speedPercent, 0, 100, 0, 255));
//   } else if (speedPercent < 0) {
//     // Reverse
//     digitalWrite(motorDirPin1, LOW);
//     digitalWrite(motorDirPin2, HIGH);
//     analogWrite(motorSpeedPin, map(-speedPercent, 0, 100, 0, 255));
//   } else {
//     // Stop (brake)
//     digitalWrite(motorDirPin1, LOW);
//     digitalWrite(motorDirPin2, LOW);
//     analogWrite(motorSpeedPin, 0);
//   }
// }

void setMotorPercent(int sp) {
  sp = constrain(sp, -100, 100);

  if (sp > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, map(sp, 0, 100, 0, 255));
  } else if (sp < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, map(-sp, 0, 100, 0, 255));
  } else {
    // brake (both low) OR set both high for hard brake on some boards
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}


// float calculate_avoidance() {
//   int blocks = pixy.ccc.getBlocks();


//     found_block = false;

//   if (blocks) {
//     int signature = -1;       // Signature of the object you want to detect
//     int targetHeight = 10;    // Height of the object in centimeters
//     float focalLength = 2.3;  // Focal length of the camera in centimeters
//     float cameraFOV = 80.0;   // Field of view of the camera in degrees

//     int largestBlockIndex = -1;
//     int largestBlockArea = 0;

//     for (int i = 0; i < blocks; i++) {
//       if (pixy.ccc.blocks[i].m_height > 1.33 * float(pixy.ccc.blocks[i].m_width)) {
//       int objectArea = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
//       found_block = true;
//       if (objectArea > largestBlockArea) {
//         largestBlockIndex = i;
//         largestBlockArea = objectArea;
//         signature = pixy.ccc.blocks[i].m_signature;
//         }
//       }
//     }

//     if (signature != -1) {
//       int objectHeight = pixy.ccc.blocks[largestBlockIndex].m_height;
//       float distance = (targetHeight * focalLength * 100) / objectHeight;

//       float blockCenterX = pixy.ccc.blocks[largestBlockIndex].m_x;
//       float blockCenterY = pixy.ccc.blocks[largestBlockIndex].m_y;

//       float deltaX = blockCenterX - pixy.frameWidth / 2;
//       float deltaY = blockCenterY - pixy.frameHeight / 2;

//       float detected_degree = deltaX * 40 / pixy.frameWidth;

//       float blockPositionX = distance * sin(degreesToRadians(detected_degree));
//       float blockPositionY = distance * cos(degreesToRadians(detected_degree)) - 17;

//       if (signature == 1) {
//         avoidance_degree = max(radiansToDegree(atan2(blockPositionX + 9, blockPositionY)), 5);
//         Blocks_TURN = 'R';
//       } else {
//         avoidance_degree = min(radiansToDegree(atan2(blockPositionX - 9, blockPositionY)), -5);
//         Blocks_TURN = 'L';
//       }


//       // Serial.print("Detected degree: ");
//       // Serial.print(detected_degree);
//       // Serial.println(" degree ");
//       // Serial.print("Position (X, Y): ");
//       // Serial.print(blockPositionX);
//       // Serial.print(", ");
//       // Serial.print(blockPositionY);
//       // Serial.print(" cm   Degree rotate: ");
//       // Serial.println(avoidance_degree);
//     }
//   }

//   return avoidance_degree;
// }

// float degreesToRadians(double degrees) {
//   return degrees * PI / 180.0;
// }

// float radiansToDegree(double raidans) {
//   return raidans / PI * 180.0;
// }