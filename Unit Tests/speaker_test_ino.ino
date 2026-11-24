

#include <ESP_I2S.h>
#include <math.h>

I2SClass i2s;

#define BCLK 26
#define LRC  25
#define DIN  22  // Connected to the amplifier's DIN

const int SAMPLE_RATE = 16000;
const int VOLUME = 3000;
const int BEAT_MS = 500;  // Increase for a slower melody

// Note frequencies in Hz
const float G4 = 392.00;
const float NOTE_A4 = 440.00;const float B4 = 493.88;
const float C5 = 523.25;
const float D5 = 587.33;
const float E5 = 659.25;
const float F5 = 698.46;
const float G5 = 783.99;

const float melody[] = {
  G4, G4, NOTE_A4, G4, C5, B4,
  G4, G4, NOTE_A4, G4, D5, C5,
  G4, G4, G5, E5, C5, B4, NOTE_A4,
  F5, F5, E5, C5, D5, C5
};

// Duration of each note in beats
const float beats[] = {
  0.75, 0.25, 1, 1, 1, 2,
  0.75, 0.25, 1, 1, 1, 2,
  0.75, 0.25, 1, 1, 1, 1, 2,
  0.75, 0.25, 1, 1, 1, 2
};

// frequency = 0 generates silence.
void playTone(float frequency, int durationMs) {
  int16_t samples[320];  // 160 stereo frames
  const int totalFrames =
      (int)((int64_t)SAMPLE_RATE * durationMs / 1000);
  const int fadeFrames = SAMPLE_RATE / 200;  // 5 ms

  float phase = 0.0f;
  const float phaseStep = 2.0f * PI * frequency / SAMPLE_RATE;

  for (int start = 0; start < totalFrames; start += 160) {
    int frames = totalFrames - start;
    if (frames > 160) frames = 160;

    for (int i = 0; i < frames; i++) {
      int position = start + i;

      // Fade in and out to reduce clicks between notes.
      float envelope = 1.0f;
      if (position < fadeFrames) {
        envelope = (float)position / fadeFrames;
      }

      int remaining = totalFrames - 1 - position;
      if (remaining < fadeFrames) {
        float fadeOut = (float)remaining / fadeFrames;
        if (fadeOut < envelope) envelope = fadeOut;
      }

      int16_t value = (int16_t)(
          VOLUME * envelope * sinf(phase));

      samples[2 * i] = value;
      samples[2 * i + 1] = value;

      phase += phaseStep;
      if (phase >= 2.0f * PI) phase -= 2.0f * PI;
    }

    size_t bytes = frames * 2 * sizeof(int16_t);
    size_t sent = 0;

    while (sent < bytes) {
      size_t written = i2s.write(
          ((uint8_t*)samples) + sent, bytes - sent);

      if (written == 0) {
        Serial.println("I2S write failed");
        while (true) delay(100);
      }

      sent += written;
    }
  }
}

void setup() {
  Serial.begin(115200);

  i2s.setPins(BCLK, LRC, DIN);

  if (!i2s.begin(I2S_MODE_STD,
                 SAMPLE_RATE,
                 I2S_DATA_BIT_WIDTH_16BIT,
                 I2S_SLOT_MODE_STEREO)) {
    Serial.println("I2S FAILED");
    while (true) delay(100);
  }

  Serial.println("I2S READY");
}

void loop() {
  Serial.println("Playing Happy Birthday!");

  const int noteCount = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < noteCount; i++) {
    int duration = (int)(beats[i] * BEAT_MS);
    int gap = 30;

    playTone(melody[i], duration - gap);
    playTone(0, gap);
  }

  Serial.println("Song finished");
  playTone(0, 3000);
}
