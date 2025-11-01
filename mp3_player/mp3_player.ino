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
// BEGIN_TRACK_TITLES
const char t0[] PROGMEM = "Abra (feat. Thyro _ Jeriko Aguilar) - Gayuma (Lyri";
const char t1[] PROGMEM = "Are you just living day to day_(MP3_70K)";
const char t2[] PROGMEM = "Ayokong Tumanda(MP3_70K)";
const char t3[] PROGMEM = "BANYO QUEEN Andrew E Lyrics(MP3_70K)";
const char t4[] PROGMEM = "Bassilyo - Lord Patawad(MP3_70K)";
const char t5[] PROGMEM = "Bill Withers  - Just The Two Of Us (Lyrics)(MP3_70";
const char t6[] PROGMEM = "Bling-Bang-Bang-Born (Official Music Video)(MP3_70";
const char t7[] PROGMEM = "DIWATA __ Abra ft. Chito Miranda (Lyrics)(MP3_70K)";
const char t8[] PROGMEM = "December Avenue - Huling Sandali (OFFICIAL LYRIC V";
const char t9[] PROGMEM = "December Avenue - Sa Ngalan Ng Pag-Ibig (OFFICIAL";
const char t10[] PROGMEM = "Earth_ Wind _ Fire - Let_s Groove (TikTok Remix) L";
const char t11[] PROGMEM = "Eraserheads - Ang Huling El Bimbo(MP3_70K)";
const char t12[] PROGMEM = "Eternal Phlegm(MP3_70K)";
const char t13[] PROGMEM = "Everyday Normal Guy 2(MP3_70K)";
const char t14[] PROGMEM = "Frieren and Shrek drive in circles to Joji (Full S";
const char t15[] PROGMEM = "GESHUOU(MP3_70K)";
const char t16[] PROGMEM = "George Michael - Careless Whisper (Official Video)";
const char t17[] PROGMEM = "Gloc 9 - Simpleng Tao (Lyrics)☁️ _ Habang tumutuno";
const char t18[] PROGMEM = "Green Day - 21 Guns (Lyrics)(MP3_70K)";
const char t19[] PROGMEM = "Hayaan Mo Sila - Ex Battalion x O.C Dawgs (Officia";
const char t20[] PROGMEM = "I LOVE YOU SO - THE WALTERS (LYRICS) __ LIRIK(MP3_";
const char t21[] PROGMEM = "IV OF SPADES - Suliranin (Official Music Video)(MP";
const char t22[] PROGMEM = "Ibang Planeta - ZILD (Music Video)(MP3_70K)";
const char t23[] PROGMEM = "Ikaw kase - Ex Battalion(MP3_70K)";
const char t24[] PROGMEM = "Itchyworms - Akin Ka Na Lang (Official Music Video";
const char t25[] PROGMEM = "Itchyworms - Beer (Official Lyric Video)(MP3_70K)";
const char t26[] PROGMEM = "Itchyworms - Di na muli (lyrics)(MP3_70K)";
const char t27[] PROGMEM = "JAWNY - Honeypie Animation MV (Full Version by Sea";
const char t28[] PROGMEM = "JoJo_s Bizarre Adventure_ Diamond is Unbreakable O";
const char t29[] PROGMEM = "Joy - Touch By Touch(MP3_70K)";
const char t30[] PROGMEM = "KICK BACK(MP3_70K)";
const char t31[] PROGMEM = "Lovers Rock - TV Girl (Lyrics)(MP3_70K)";
const char t32[] PROGMEM = "Lucky Twice - Lucky (Lyrics) _I_m So Lucky Lucky I";
const char t33[] PROGMEM = "MYMP - Eternal Flame (Official Lyric Video)(MP3_70";
const char t34[] PROGMEM = "Michael Jackson - Beat It (Official 4K Video)(MP3_";
const char t35[] PROGMEM = "Michael Jackson - Billie Jean (Official Video)(MP3";
const char t36[] PROGMEM = "Michael V. I DJ Bumbay I Official Music Video(MP3_";
const char t37[] PROGMEM = "Modern Talking - Cheri Cheri Lady (Official Video)";
const char t38[] PROGMEM = "Multo(MP3_70K)";
const char t39[] PROGMEM = "NAE NI----A ORIGINAL SONG(MP3_70K)";
const char t40[] PROGMEM = "Nadarang(MP3_70K)";
const char t41[] PROGMEM = "Nightcore _ Sweet Little Bumblebee (lyric video)(M";
const char t42[] PROGMEM = "One Day - Reggae(MP3_70K)";
const char t43[] PROGMEM = "One Direction but every line is You_re Insecure(MP";
const char t44[] PROGMEM = "Parokya Ni Edgar - Mr. Suave (Official Music Video";
const char t45[] PROGMEM = "Philippine Circulation(MP3_70K)";
const char t46[] PROGMEM = "Post Malone - Circles (Lyrics)(MP3_70K)";
const char t47[] PROGMEM = "Queen – Bohemian Rhapsody (Official Video Remaster";
const char t48[] PROGMEM = "Ricky Montgomery - Mr Loverman (Lyrics) _and I mis";
const char t49[] PROGMEM = "Ron Henley - Biglang Liko (feat. Pow Chavez)(MP3_7";
const char t50[] PROGMEM = "Soapdish - Tensionado(MP3_70K)";
const char t51[] PROGMEM = "Stupid Love - Salbakuta (Lyrics) _ Tiktok Song(MP3";
const char t52[] PROGMEM = "SunKissed Lola - Pasilyo (Official Lyric Video)(MP";
const char t53[] PROGMEM = "Sunflower - Post Malone (Spider-Man Miles Morales)";
const char t54[] PROGMEM = "Super Idol full song(MP3_70K)";
const char t55[] PROGMEM = "The Marías - Sienna (Visualizer)(MP3_70K)";
const char t56[] PROGMEM = "Wake Me Up When September Ends(MP3_70K)";
const char t57[] PROGMEM = "Yakuza OST - Baka Mitai (ばかみたい) Kiryu full version";
const char t58[] PROGMEM = "caramelldansen　ｳｯｰｳｯｰｳﾏｳﾏ(ﾟ∀ﾟ) PV(MP3_70K)";
const char t59[] PROGMEM = "iluna - panaginip(MP3_70K)";
const char t60[] PROGMEM = "in the sea(MP3_70K)";
const char t61[] PROGMEM = "mao zedong propaganda music Red Sun in the Sky(MP3";
const char t62[] PROGMEM = "Ева - винтаж - Eva - Vintage (lonely_ lonely_ I gu";
const char t63[] PROGMEM = "【Kung Fu Panda 4 OST】Baby - Justin bieber (Chinese";
const char t64[] PROGMEM = "心予報 - Eve MV(MP3_70K)";
const char t65[] PROGMEM = "米津玄師  Kenshi Yonezu - IRIS OUT(MP3_70K)";

const char* const trackTitles[] PROGMEM = {
  t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38, t39, t40, t41, t42, t43, t44, t45, t46, t47, t48, t49, t50, t51, t52, t53, t54, t55, t56, t57, t58, t59, t60, t61, t62, t63, t64, t65
};
// END_TRACK_TITLES
