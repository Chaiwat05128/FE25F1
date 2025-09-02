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
  myservo2.write(mapf(135 + max(min(degree, 35), -35), 0, 270, 0, 180));
}

void motor_and_steer(int degree) {
  degree = min(max(degree, -40), 40);
  steering_servo(degree);
  setMotorPercent(map(abs(degree), 0, 45, 50, 60));
  // setMotorSpeed2(-100);
}

void line_detection() {
  int GEEN_value = analogRead(GEEN_SEN);

  found_block_factor = min(max(mapf(millis() - timer_block_decay, 0, 1000, 1, 0), 0), 1);

  if (TURN == 'U') {
    int red_value = analogRead(RED_SEN);
    if (GEEN_value < 400 || red_value < 400) {
      simpleBeep();
      int lowest_red_sen = red_value;
      long timer_line = millis();
      while (millis() - timer_line < 100) {
        int red_value = analogRead(RED_SEN);
        if (red_value < lowest_red_sen) {
          lowest_red_sen = red_value;
        }
      }
      if (lowest_red_sen > 400) {
        // Red line - turn left (counterclockwise)
        TURN = 'L';
        Blocks_TURN = 'L';  // Keep them synchronized
        // compass_offset -= 90;
        x = -1;
        Y = 26;
        while (millis() - timer_line < 2000) {
          int GEEN_value = analogRead(GEEN_SEN);
          if (GEEN_value < 400) {
            simpleBeep();
            break;
          }
          getBNO();
          motor_and_steer(pvYaw);
        }

        compass_offset -= 90;
        while (millis() - timer_line < 2000) {
          getBNO();
          steering_servo(-pvYaw);
          setMotorPercent(-70);
        }
        TURN = 'L';
        setMotorPercent(0);
        // compass_offset += 90;
      } else {
        // Blue line - turn right (clockwise)
        TURN = 'R';
        Blocks_TURN = 'R';  // Keep them synchronized

        x = 1;
        Y = 26;
        long timer_line = millis();

        while (millis() - timer_line < 2000) {
          int GEEN_value = analogRead(GEEN_SEN);
          if (GEEN_value < 400) {
            simpleBeep();
            break;
          }
          getBNO();
          motor_and_steer(pvYaw);
        }

        compass_offset += 90;
        while (millis() - timer_line < 2000) {
          getBNO();
          steering_servo(-pvYaw);
          setMotorPercent(-70);
        }
        TURN = 'R';
        setMotorPercent(0);
        // compass_offset += 90;
      }
      count++;
      halt_detect_line_timer = millis();
    }
    // } else {
    //   if (millis() - halt_detect_line_timer > 1000) {
    //     if (found_block) {
    //       timer_block_decay = millis();
    //     }
    //     if (GEEN_value < 400) {
    //       if (TURN == 'L') {
    //         // Continue counterclockwise
    //         compass_offset -= 90;
    //         x = -1;
    //       } else {
    //         // // Continue clockwise
    //         compass_offset += 90;
    //         x = 1;
    //       }
    //       halt_detect_line_timer = millis();
    //       count++;
    //     }
    //   }
  }
}


void simpleBeep() {
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}

void zeroYaw() {
  sensors_event_t event;
  bno.getEvent(&event);
  float yawOffset = wrap_value(event.orientation.x, -180.0, 180.0);
}

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

float calculate_avoidance() {
  camera.handleIncomingData();

  found_block = false;  //TODO: I wrote a bug and it works.

  if (camera.isBlockFound()) {
    BlobData currentBlob = camera.getBlobData();

    int signature = -1;       // Signature of the object you want to detect
    int targetHeight = 10;    // Height of the object in centimeters
    float focalLength = 2.8;  // Focal length of the camera in centimeters
    float cameraFOV = 70.0;   // Field of view of the camera in degrees

    // Check if the detected blob meets the height/width ratio criteria
    if (currentBlob.height > 1.33 * float(currentBlob.width)) {
      found_block = true;
      signature = currentBlob.signature;

      if (signature != -1) {
        int objectHeight = currentBlob.height;
        float distance = (targetHeight * focalLength * 100) / objectHeight;

        float blockCenterX = currentBlob.x;
        float blockCenterY = currentBlob.y;

        // Assuming frame dimensions (you may need to adjust these values)
        int frameWidth = 320;   // Typical Pixy2 frame width
        int frameHeight = 150;  // Typical Pixy2 frame height

        float deltaX = blockCenterX - frameWidth / 2;
        float deltaY = blockCenterY - frameHeight / 2;

        float detected_degree = deltaX * 40 / frameWidth;

        float blockPositionX = distance * sin(degreesToRadians(detected_degree));
        float blockPositionY = distance * cos(degreesToRadians(detected_degree)) - 17;

        if (signature == 1) {
          avoidance_degree = min(radiansToDegree(atan2(blockPositionX + 9, blockPositionY)), -5);
          Blocks_TURN = 'R';
          x = 1;
        } else {
          avoidance_degree = max(radiansToDegree(atan2(blockPositionX - 9, blockPositionY)), 5);
          Blocks_TURN = 'L';
          x = -1;
        }

        // Serial.print("Detected degree: ");
        // Serial.print(detected_degree);
        // Serial.println(" degree ");
        // Serial.print("Position (X, Y): ");
        // Serial.print(blockPositionX);
        // Serial.print(", ");
        // Serial.print(blockPositionY);
        // Serial.print(" cm   Degree rotate: ");
        // Serial.println(avoidance_degree);
      }
    }
  }

  return avoidance_degree;
}

float degreesToRadians(double degrees) {
  return degrees * PI / 180.0;
}

float radiansToDegree(double raidans) {
  return raidans / PI * 180.0;
}