#include <gesture_recognition_inferencing.h>
#include "mbed.h"

#define I2C_SDA_PIN p6
#define I2C_SCL_PIN p7
#define MPU6050_ADDR 0x68
// Left-Shifted the 7-bit address by 1 bit to obtain the 8-bit address needed by mbed::I2V object.
#define MPU6050_ADDR_8BIT 0xD1
// Who am I stores the device ID so i2c can verify that it is communicating with a real MPU6050
#define MPU6050_WHO_AM_I 0x75
//Accelerometer measurements
#define MPU6050_XOUT_H 0x3B
// Power mode register
#define MPU6050_PWR_MGMT_1 0x6B
// Accelerometer measurement range to +-2 g
#define MPU6050_ACCEL_CONFIG 0x1C
// Measurements with a fequency of 50Hz
#define FREQUENCY_HZ 50
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))
#define INTERVAL_US (INTERVAL_MS * 1000)

#define INPUT_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE // 375
float buff_sampling[INPUT_SIZE] = { 0 };
float buff_inference[INPUT_SIZE];
#define NUM_CLASSES EI_CLASSIFIER_LABEL_COUNT

mbed::I2C i2c(I2C_SDA_PIN, I2C_SCL_PIN);

rtos::Thread inference_thread(osPriorityLow); // Low priority because longer execution time due to ML inference.

class TestAndTraceFilter {
  private:
    int32_t _n {0};
    float _thr {0.0f};
    int32_t _counter {0};
    int32_t _last_idx_class {-1};
    const int32_t _num_classes {3};

  public:
    static constexpr int32_t invalid_idx_class = -1;

    TestAndTraceFilter(int32_t n, float thr) {
      _thr = thr;
      _n = n;
    }

    void reset() {
      _counter = 0;
      _last_idx_class = invalid_idx_class;
    }

    void update(size_t idx_class, float prob) {
      if(idx_class >= _num_classes || prob <= _thr) {
        reset();
      } else {
        if(prob > _thr) {
          // Incremental state
          if(idx_class != _last_idx_class) {
            _last_idx_class = idx_class;
            _counter = 0;
          }
          _counter += 1;
        } else {
          reset();
        }
      }
    }

    int32_t output() {
      if(_counter >= _n) {
        int32_t out = _last_idx_class;
        reset();
        return out;
      } else {
        return invalid_idx_class;
      }
    }
};

void inference_func() {
  // Wait until we have a full of buff_sampling
  delay((EI_CLASSIFIER_INTERVAL_MS * EI_CLASSIFIER_RAW_SAMPLE_COUNT) + 100);

  TestAndTraceFilter filter(4, 0.9f);

  while (1) {
    memcpy(buff_inference, buff_sampling, INPUT_SIZE * sizeof(float));
    signal_t signal;
    numpy::signal_from_buffer(buff_inference, INPUT_SIZE, &signal);
    ei_impulse_result_t result = { 0 };
    run_classifier(&signal, &result, false);

    size_t ix_max = 0; float pb_max = 0;
    size_t ix = 0;
    for(size_t ix = 0; ix < NUM_CLASSES; ix++) {
      if(result.classification[ix].value > pb_max) {
        ix_max = ix;
        pb_max = result.classification[ix].value;
      }
    }
    // Update filtering function
    filter.update(ix_max, pb_max);

    // Get the output
    int32_t out = filter.output();
    if(out != filter.invalid_idx_class) {
      Serial.println(result.classification[out].label);
      delay(2000);
    } else {
      delay(400);
    }
  }
}


void read_reg(int32_t addr_i2c, int32_t addr_reg, char *buf, int32_t length) {
  char data = addr_reg;
  i2c.write(addr_i2c, &data, 1);
  i2c.read(addr_i2c, buf, length);
  return;
}

// Function to write a single byte into a specific register of the MPU6050
void write_reg(int32_t addr_i2c, int32_t addr_reg, char v) {
  char data[2] = {addr_reg, v};
  i2c.write(addr_i2c, data, 2);
  return;
}

void read_accelerometer(float *x, float *y, float *z) {
  char data[6];
  read_reg(MPU6050_ADDR_8BIT, MPU6050_XOUT_H, data, 6);
  int16_t ax_i16 = (int16_t)(data[0] << 8 | data[1]);
  int16_t ay_i16 = (int16_t)(data[2] << 8 | data[3]);
  int16_t az_i16 = (int16_t)(data[4] << 8 | data[5]);

  // Converting the integer value into floating-point acceleration value.
  const float sensitivity = 16384.f;
  const float k = (1.f / sensitivity) * 9.80665f; // g stands for gravity force
  const float ax_f32 = (float)ax_i16 * k;
  const float ay_f32 = (float)ay_i16 * k;
  const float az_f32 = (float)az_i16 * k;

  *x = ax_f32;
  *y = ay_f32;
  *z = az_f32;
  return;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  i2c.frequency(400000);
  char id;
  read_reg(MPU6050_ADDR_8BIT, MPU6050_WHO_AM_I, &id, 1);
  if (id == MPU6050_ADDR) {
    Serial.println("MPU6050 found!");
  } else {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  write_reg(MPU6050_ADDR_8BIT, MPU6050_PWR_MGMT_1, 0);
  write_reg(MPU6050_ADDR_8BIT, MPU6050_ACCEL_CONFIG, 0);

  auto func_mbed = mbed::callback(&inference_func);
  inference_thread.start(func_mbed);
}

void loop() {
  mbed:: Timer timer;
  timer.start();
  float ax, ay, az;
  read_accelerometer(&ax, &ay, &az);
  //Serial.print(ax);
  //Serial.print(",");
  //Serial.print(ay);
  //Serial.print(",");
  //Serial.println(az);

  numpy::roll(buff_sampling, INPUT_SIZE, -3);
  buff_sampling[INPUT_SIZE - 3] = ax;
  buff_sampling[INPUT_SIZE - 2] = ay;
  buff_sampling[INPUT_SIZE - 1] = az;

  timer.stop();

  using std::chrono::duration_cast;
  using std::chrono::microseconds;
  
  auto t0 = timer.elapsed_time();
  auto t_diff = duration_cast<microseconds>(t0);
  uint64_t t_wait_us = INTERVAL_US - t_diff.count();
  int32_t t_wait_ms = (t_wait_us / 1000);
  int32_t t_wait_leftover_us = (t_wait_us % 1000);
  delay(t_wait_ms);
  delayMicroseconds(t_wait_leftover_us);

}
