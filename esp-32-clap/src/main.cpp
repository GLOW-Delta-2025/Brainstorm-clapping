#include <DFRobot_MSM261/src/DFRobot_MSM261.h>

#define SAMPLE_RATE   (44100)  // Audio Sample Rate
#define I2S_SCK_IO    (25)     // I2S Clock PIN
#define I2S_WS_IO     (16)     // I2S Word Select PIN
#define I2S_DI_IO     (26)     // I2S Data In PIN
#define DATA_BIT      (16)     // Bit depth
#define MODE_PIN      (4)      // Microphone Mode PIN
#define THRESHOLD     (1500)   // Sound amplitude threshold for claps
#define DEBOUNCE_TIME (300)    // Minimum time between claps (in ms)

DFRobot_Microphone microphone(I2S_SCK_IO, I2S_WS_IO, I2S_DI_IO);
char i2sReadrawBuff[100];

int lastAmplitude = 0;         // Track previous amplitude
unsigned long lastClapTime = 0; // Track last clap time

enum ClapState { IDLE, PEAKING, FALLING_STATE };
ClapState clapState = IDLE;

void setup() {
  Serial.begin(115200);

  // Microphone setup
  pinMode(MODE_PIN, OUTPUT);
  digitalWrite(MODE_PIN, LOW);  // Set microphone to left channel

  while (microphone.begin(SAMPLE_RATE, DATA_BIT) != 0) {
    Serial.println("I2S microphone initialization failed. Retrying...");
  }
  Serial.println("I2S microphone initialization successful.");
}

// Function to handle the detected clap event
void reportClap(int strength) {
  Serial.print("Clap detected! Strength: ");
  Serial.println(strength);

  // Map strength to an action (e.g., LED brightness or count)
  // Example: Add LED lighting action here if desired
}

void loop() {
  // Read raw microphone data
  microphone.read(i2sReadrawBuff, 100);

  // Extract amplitude from raw I2S data
  // Combine two 8-bit data into a signed 16-bit int
  int16_t rawAmplitude = (int16_t)(i2sReadrawBuff[0] | (i2sReadrawBuff[1] << 8));
  int amplitude = abs(rawAmplitude);  // Get absolute amplitude value

  // Process state machine for clap detection
  switch (clapState) {
    case IDLE:
      // Detect when the sound exceeds the threshold
      if (amplitude > THRESHOLD && (millis() - lastClapTime > DEBOUNCE_TIME)) {
        clapState = PEAKING;
        Serial.println("Clap Detected! Tracking peak...");
      }
      break;

    case PEAKING:
      // Detect peak level of the clap
      if (amplitude < lastAmplitude) {  // If amplitude is falling
        clapState = FALLING_STATE; // Enter falling state
      }
      break;

    case FALLING_STATE:
      // Confirm a clap when it drops below threshold
      if (amplitude < THRESHOLD) {
        lastClapTime = millis(); // Update last clap time
        reportClap(lastAmplitude); // Process the detected clap
        clapState = IDLE;  // Reset to idle state
      }
      break;
  }

  lastAmplitude = amplitude;  // Save amplitude for next iteration
}