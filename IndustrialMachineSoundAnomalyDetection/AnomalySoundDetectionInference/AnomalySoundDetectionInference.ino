#include <anomaly-sound-detection_inferencing.h>
#include "tflite-model/tflite_learn_953451_3.h"

#include "edge-impulse-sdk/tensorflow/lite/micro/all_ops_resolver.h"
#include "edge-impulse-sdk/tensorflow/lite/micro/micro_interpreter.h"
#include "edge-impulse-sdk/tensorflow/lite/schema/schema_generated.h"

#define GREEN_LED 2
#define RED_LED 25

#define ANOMALY_THRESHOLD 0.001478f

// For Raspberry Pi UART use Serial1.
// For USB Serial testing use Serial.
#define LINK Serial1

tflite::AllOpsResolver resolver;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;

constexpr int kTensorArenaSize = 49152;
uint8_t tensor_arena[kTensorArenaSize];

float features[256];
float target[64];

float sum_mse = 0.0f;
int received_windows = 0;
int expected_windows = 0;

void setup() {
  Serial.begin(115200);
  LINK.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  delay(1000);

  Serial.println("RP2040 full-file anomaly test");

  model = tflite::GetModel(tflite_learn_953451_3);

  static tflite::MicroInterpreter static_interpreter(
    model,
    resolver,
    tensor_arena,
    kTensorArenaSize,
    nullptr,
    nullptr
  );

  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors(false) != kTfLiteOk) {
    Serial.println("AllocateTensors failed");
    while (1);
  }

  Serial.println("Model ready");
}

float run_window_mse() {
  TfLiteTensor* input = interpreter->input(0);

  float input_scale = input->params.scale;
  int input_zero = input->params.zero_point;
  int8_t* input_data = input->data.int8;

  for (int i = 0; i < 256; i++) {
    int q = round(features[i] / input_scale + input_zero);
    if (q > 127) q = 127;
    if (q < -128) q = -128;
    input_data[i] = (int8_t)q;
  }

  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Inference failed");
    return -1.0f;
  }

  TfLiteTensor* output = interpreter->output(0);

  float output_scale = output->params.scale;
  int output_zero = output->params.zero_point;
  int8_t* output_data = output->data.int8;

  float mse = 0.0f;

  for (int i = 0; i < 64; i++) {
    float pred = (output_data[i] - output_zero) * output_scale;
    float diff = pred - target[i];
    mse += diff * diff;
  }

  return mse / 64.0f;
}

void parse_window(String line) {
  char buffer[12000];
  line.toCharArray(buffer, sizeof(buffer));

  int idx = 0;
  char* token = strtok(buffer, ",");

  while (token != NULL && idx < 320) {
    float value = atof(token);

    if (idx < 256) {
      features[idx] = value;
    } else {
      target[idx - 256] = value;
    }

    token = strtok(NULL, ",");
    idx++;
  }

  if (idx != 320) {
    Serial.print("Wrong value count: ");
    Serial.println(idx);
    return;
  }

  float mse = run_window_mse();

  if (mse >= 0.0f) {
    sum_mse += mse;
    received_windows++;
  }
}

void loop() {
  if (!LINK.available()) {
    return;
  }

  String line = LINK.readStringUntil('\n');
  line.trim();

  if (line.length() == 0) {
    return;
  }

  if (line.startsWith("START")) {
    sum_mse = 0.0f;
    received_windows = 0;

    int comma = line.indexOf(',');
    expected_windows = line.substring(comma + 1).toInt();

    Serial.print("START file, expected windows: ");
    Serial.println(expected_windows);

    LINK.println("OK_START");
    return;
  }

  if (line == "END") {
    if (received_windows == 0) {
      Serial.println("No windows received");
      return;
    }

    float file_score = sum_mse / received_windows;
    bool anomaly = file_score >= ANOMALY_THRESHOLD;

    digitalWrite(GREEN_LED, anomaly ? LOW : HIGH);
    digitalWrite(RED_LED, anomaly ? HIGH : LOW);

    Serial.print("Received windows: ");
    Serial.println(received_windows);

    Serial.print("FILE_SCORE=");
    Serial.println(file_score, 8);

    Serial.println(anomaly ? "ANOMALY" : "NORMAL");

    LINK.print("FILE_SCORE=");
    LINK.println(file_score, 8);

    return;
  }

  parse_window(line);
}