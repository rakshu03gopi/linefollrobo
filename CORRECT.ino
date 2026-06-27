// === IR SENSOR PINS (HIGH = BLACK) ===
#define SENSOR_PIN_L A0
#define SENSOR_PIN_M A1
#define SENSOR_PIN_R A2

// === MOTOR DRIVER PINS ===
#define LEFT_EN   6
#define RIGHT_EN  11
#define LEFT_IN1  10
#define LEFT_IN2  9
#define RIGHT_IN1 8
#define RIGHT_IN2 7

// === BUZZER ===
#define BUZZER    12

// === STATE VARIABLES ===
bool ls, ms, rs;
byte pattern;
int lastDir = 0;
int nodeCount = 0;
int targetNode = 0;
bool commandReceived = false;
bool returning = false;
bool onNode = false;

const int finalMaxNode = 8;

// Turn logic: index = node number
bool leftTurnNode[10] = {
  false,  // 0 (unused)
  false,  // Node 1
  false,  // Node 2
  false,  // Node 3
  true,   // Node 4 → Turn left ✅
  true,   // Node 5 → Turn left ✅
  false,  // Node 6
  false,  // Node 7
  false   // Node 8
};

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_IN1, OUTPUT);  pinMode(LEFT_IN2, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT); pinMode(RIGHT_IN2, OUTPUT);
  pinMode(LEFT_EN, OUTPUT);   pinMode(RIGHT_EN, OUTPUT);

  pinMode(SENSOR_PIN_L, INPUT);
  pinMode(SENSOR_PIN_M, INPUT);
  pinMode(SENSOR_PIN_R, INPUT);
  pinMode(BUZZER, OUTPUT);

  analogWrite(LEFT_EN, 255);
  analogWrite(RIGHT_EN, 255);

  Serial.println(F("🔧 Ready. Send node 1–8 from GUI."));
}

void loop() {

  // Step 0: wait for user command
  if (!commandReceived) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c >= '1' && c <= '8') {
        targetNode = c - '0';
        commandReceived = true;
        returning = false;
        nodeCount = 0;
        Serial.print(F("📦 Target Node: ")); Serial.println(targetNode);
        beep(2);
      }
    }
    return;
  }

  // Step 1: follow line
  readSensors();

  if (pattern == 0b111) {  // full black node
    if (!onNode) {
      onNode = true;
      nodeCount++;
      Serial.print(F("📍 Node #")); Serial.println(nodeCount);

      // Perform turn if needed (forward only)
      if (!returning && nodeCount <= finalMaxNode && leftTurnNode[nodeCount]) {
        Serial.println(F("↩  Turning Left"));
        beep(1);
        hardLeft();
        delay(500);  // Adjust for your chassis
        stopBot();
        delay(200);
      }

      // Final destination reached
      if (nodeCount == targetNode && !returning) {
        Serial.println(F("✅ Reached Destination"));
        stopBot();
        beep(2);
        delay(1000);

        Serial.println(F("↪  Turning 180° to return"));
        hardRight();  // U-turn
        delay(650);   // Adjust for 180°
        stopBot();
        delay(200);

        returning = true;
        nodeCount = 0;
      }

      // Returned to start
      else if (returning && nodeCount == targetNode) {
        Serial.println(F("🏁 Returned to Start (Node 1)"));
        stopBot();
        beep(3);
        commandReceived = false;
        targetNode = 0;
      }
    }
  } else {
    onNode = false;

    // Line-following logic
    switch (pattern) {
      case 0b010: moveForward(); lastDir = 0; break;
      case 0b110:
      case 0b100: moveForward(); lastDir = -1; break;
      case 0b011:
      case 0b001: moveForward(); lastDir = 1; break;
      case 0b101: moveForward(); lastDir = 0; break;
      case 0b000:
        (lastDir == -1) ? hardLeft() : hardRight();
        break;
      default: moveForward(); break;
    }
  }

  delay(30);
}

// === Helper Functions ===
void readSensors() {
  ls = digitalRead(SENSOR_PIN_L);
  ms = digitalRead(SENSOR_PIN_M);
  rs = digitalRead(SENSOR_PIN_R);
  pattern = (ls << 2) | (ms << 1) | rs;
}

void moveForward() {
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW);
  digitalWrite(RIGHT_IN1, HIGH); digitalWrite(RIGHT_IN2, LOW);
}

void moveBackward() {
  digitalWrite(LEFT_IN1, LOW); digitalWrite(LEFT_IN2, HIGH);
  digitalWrite(RIGHT_IN1, LOW); digitalWrite(RIGHT_IN2, HIGH);
}

void stopBot() {
  digitalWrite(LEFT_IN1, LOW); digitalWrite(LEFT_IN2, LOW);
  digitalWrite(RIGHT_IN1, LOW); digitalWrite(RIGHT_IN2, LOW);
}

void hardLeft() {
  digitalWrite(LEFT_IN1, LOW); digitalWrite(LEFT_IN2, HIGH);
  digitalWrite(RIGHT_IN1, HIGH); digitalWrite(RIGHT_IN2, LOW);
}

void hardRight() {
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW);
  digitalWrite(RIGHT_IN1, LOW); digitalWrite(RIGHT_IN2, HIGH);
}

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER, HIGH); delay(100);
    digitalWrite(BUZZER, LOW);  delay(100);
  }
}