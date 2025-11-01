#include <AltSoftSerial.h>
#include <DFRobotDFPlayerMini.h>

#define DF_RX 8
#define DF_TX 9
#define BUTTON_UP_PIN 2
#define BUTTON_DOWN_PIN 3
#define BUSY_PIN 4

AltSoftSerial mp3Serial;
DFRobotDFPlayerMini mp3;

/* ---------- STATE ---------- */
bool isPlaying = true;
int totalTracks = 0;
int currentIdx = 0;
int playlist[255] = {0};
int volume = 15;
int btn_up = 0, btn_down = 0;
unsigned long holdPlay = 0, holdNext = 0;
bool longPlay = false, longNext = false;
const unsigned long HOLD = 500;
const unsigned long STEP = 200;
unsigned long lastStep = 0;

/* ---------- LOG ---------- */
void log(const char* msg) {
  Serial.println(msg);
}

/* ---------- BUILD SHUFFLE ---------- */
void buildShuffle() {
  totalTracks = mp3.readFileCounts();
  if (totalTracks <= 0 || totalTracks > 255) totalTracks = 66;

  for (int i = 0; i < totalTracks; i++) playlist[i] = i + 1;
  for (int i = totalTracks - 1; i > 0; i--) {
    int j = random(0, i + 1);
    int t = playlist[i]; playlist[i] = playlist[j]; playlist[j] = t;
  }
  currentIdx = 0;
}

/* ---------- PLAY NEXT WITH TRACK NUMBER ---------- */
void playNext() {
  if (currentIdx >= totalTracks) {
    log("SHUFFLE: REBUILD");
    buildShuffle();
  }

  uint8_t file = playlist[currentIdx];
  char buf[32];
  sprintf(buf, "PLAY: %04d/%04d", file, totalTracks);
  log(buf);

  mp3.play(file);
  delay(500);
  isPlaying = true;
  currentIdx++;
}

/* ============================================================= */
void setup() {
  Serial.begin(9600);
  log("=== MP3 PLAYER START ===");

  mp3Serial.begin(9600);
  delay(1000);
  if (!mp3.begin(mp3Serial)) {
    log("ERROR: DFPlayer failed!");
    while (1);
  }
  log("DFPlayer: OK");

  mp3.volume(volume);
  char v[16]; sprintf(v, "VOLUME: %d", volume); log(v);

  randomSeed(analogRead(A0));
  buildShuffle();

  currentIdx = random(0, totalTracks);
  playNext();

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUSY_PIN, INPUT);
}

void loop() {
  // === PLAY/PAUSE / VOL- ===
  if (digitalRead(BUTTON_DOWN_PIN) == LOW && !btn_down) {
    btn_down = 1; holdPlay = millis();
  }
  if (digitalRead(BUTTON_DOWN_PIN) == HIGH && btn_down) {
    if (!longPlay) {
      if (isPlaying) { mp3.pause(); log("ACTION: PAUSE"); }
      else { mp3.start(); log("ACTION: RESUME"); }
      isPlaying = !isPlaying;
    }
    btn_down = 0; holdPlay = 0; longPlay = false;
  }
  if (holdPlay && (millis() - holdPlay > HOLD) && !longPlay) {
    longPlay = true;
  }
  if (longPlay && millis() - lastStep > STEP) {
    if (volume > 0) { volume--; mp3.volume(volume); }
    char v[16]; sprintf(v, "VOLUME: %d", volume); log(v);
    lastStep = millis();
  }

  // === NEXT / VOL+ ===
  if (digitalRead(BUTTON_UP_PIN) == LOW && !btn_up) {
    btn_up = 1; holdNext = millis();
  }
  if (digitalRead(BUTTON_UP_PIN) == HIGH && btn_up) {
    if (!longNext) { log("ACTION: NEXT"); playNext(); }
    btn_up = 0; holdNext = 0; longNext = false;
  }
  if (holdNext && (millis() - holdNext > HOLD) && !longNext) {
    longNext = true;
  }
  if (longNext && millis() - lastStep > STEP) {
    if (volume < 30) { volume++; mp3.volume(volume); }
    char v[16]; sprintf(v, "VOLUME: %d", volume); log(v);
    lastStep = millis();
  }

  // === AUTO NEXT ===
  static bool wasBusy = false;
  bool busy = digitalRead(BUSY_PIN) == LOW;
  if (wasBusy && !busy && isPlaying) {
    log("EVENT: TRACK_END");
    playNext();
  }
  wasBusy = busy;

  delay(10);
}