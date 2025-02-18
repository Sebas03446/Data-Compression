#include <Arduino_APDS9960.h>

#define NUM_SAMPLES 200         // 10 seconds * 20Hz = 200 samples
#define KEYFRAME_INTERVAL 3     // Every 3rd sample is a keyframe (absolute value)

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for serial port to connect

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
    while (1); // Stop execution if sensor fails to initialize
  }
}

void loop() {
  // Arrays to hold raw sensor data and delta encoded data for each color channel.
  int rawRed[NUM_SAMPLES];
  int rawGreen[NUM_SAMPLES];
  int rawBlue[NUM_SAMPLES];

  int encodedRed[NUM_SAMPLES];
  int encodedGreen[NUM_SAMPLES];
  int encodedBlue[NUM_SAMPLES];

  // Sampling for 10 seconds (~200 samples at 20Hz)
  unsigned long startTime = millis();
  int sampleCount = 0;
  while ((millis() - startTime < 10000) && (sampleCount < NUM_SAMPLES)) {
    // Wait until a new color reading is available
    while (!APDS.colorAvailable()) {
      delay(1);
    }
    
    int r, g, b;
    APDS.readColor(r, g, b);
    
    // Convert sensor readings to integers (simulate conversion from float by multiplying by 100)
    rawRed[sampleCount]   = r * 100;
    rawGreen[sampleCount] = g * 100;
    rawBlue[sampleCount]  = b * 100;
    
    sampleCount++;
    delay(50);  // 50ms delay for ~20Hz sampling rate
  }
  

  // --- Delta Encoding ---
  // For each channel, the first sample is stored as an absolute value.
  // Every sample with an index that is a multiple of KEYFRAME_INTERVAL is stored as a keyframe (absolute value).
  // Other samples are stored as the difference from the previous sample.
  if (sampleCount > 0) {
    // Red channel
    encodedRed[0] = rawRed[0];
    for (int i = 1; i < sampleCount; i++) {
      if (i % KEYFRAME_INTERVAL == 0) {
        encodedRed[i] = rawRed[i];  // Keyframe: store absolute value
      } else {
        encodedRed[i] = rawRed[i] - rawRed[i - 1];  // Delta difference
      }
    }
    
    // Green channel
    encodedGreen[0] = rawGreen[0];
    for (int i = 1; i < sampleCount; i++) {
      if (i % KEYFRAME_INTERVAL == 0) {
        encodedGreen[i] = rawGreen[i];
      } else {
        encodedGreen[i] = rawGreen[i] - rawGreen[i - 1];
      }
    }
    
    // Blue channel
    encodedBlue[0] = rawBlue[0];
    for (int i = 1; i < sampleCount; i++) {
      if (i % KEYFRAME_INTERVAL == 0) {
        encodedBlue[i] = rawBlue[i];
      } else {
        encodedBlue[i] = rawBlue[i] - rawBlue[i - 1];
      }
    }
  }

  // --- Send Entire Data as One JSON Object ---

Serial.print("{");

// "raw" object
Serial.print("\"raw\":{");

// Red channel raw data
Serial.print("\"red\":[");
for (int i = 0; i < sampleCount; i++) {
  Serial.print(rawRed[i]);
  if (i < sampleCount - 1)
    Serial.print(",");
}
Serial.print("],");

// Green channel raw data
Serial.print("\"green\":[");
for (int i = 0; i < sampleCount; i++) {
  Serial.print(rawGreen[i]);
  if (i < sampleCount - 1)
    Serial.print(",");
}
Serial.print("],");

// Blue channel raw data
Serial.print("\"blue\":[");
for (int i = 0; i < sampleCount; i++) {
  Serial.print(rawBlue[i]);
  if (i < sampleCount - 1)
    Serial.print(",");
}
Serial.print("]");

// End of "raw" object
Serial.print("},");

// "delta" object
Serial.print("\"delta\":{");

// Red channel delta encoded data
Serial.print("\"red\":[");
for (int i = 0; i < sampleCount; i++) {
  Serial.print(encodedRed[i]);
  if (i < sampleCount - 1)
    Serial.print(",");
}
Serial.print("],");

// Green channel delta encoded data
Serial.print("\"green\":[");
for (int i = 0; i < sampleCount; i++) {
  Serial.print(encodedGreen[i]);
  if (i < sampleCount - 1)
    Serial.print(",");
}
Serial.print("],");

// Blue channel delta encoded data
Serial.print("\"blue\":[");
for (int i = 0; i < sampleCount; i++) {
  Serial.print(encodedBlue[i]);
  if (i < sampleCount - 1)
    Serial.print(",");
}
Serial.print("]");

// End of "delta" object
Serial.print("}");

// End of JSON object
Serial.println("}");
}
