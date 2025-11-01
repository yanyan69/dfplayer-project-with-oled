#include <AltSoftSerial.h>
#include <DFRobotDFPlayerMini.h>

#define DF_RX 8 // DFPlayer TX → Nano D8 (AltSoftSerial RX)
#define DF_TX 9 // DFPlayer RX → Nano D9 (AltSoftSerial TX)
#define BTN_NEXT 2
#define BTN_PLAY 3
#define BUSY_PIN 4 // DFPlayer BUSY → Nano D4

AltSoftSerial mp3Serial;
DFRobotDFPlayerMini mp3;

bool isPlaying = true;
int totalTracks = 0;
int currentIndex = 0;
int playlist[255]; // Supports up to 255 tracks
int volume = 15; // Starting volume (0-30)

// BEGIN_TRACK_NAMES
const char* trackNames[] = {
    "Abra (feat. Thyro _ Jeriko Aguilar) - Gayuma (Lyrics)(MP3_70K).mp3",
    "Are you just living day to day_(MP3_70K).mp3",
    "Ayokong Tumanda(MP3_70K).mp3",
    "BANYO QUEEN Andrew E Lyrics(MP3_70K).mp3",
    "Bassilyo - Lord Patawad(MP3_70K).mp3",
    "Bill Withers  - Just The Two Of Us (Lyrics)(MP3_70K).mp3",
    "Bling-Bang-Bang-Born (Official Music Video)(MP3_70K).mp3",
    "DIWATA __ Abra ft. Chito Miranda (Lyrics)(MP3_70K).mp3",
    "December Avenue - Huling Sandali (OFFICIAL LYRIC VIDEO)(MP3_70K).mp3",
    "December Avenue - Sa Ngalan Ng Pag-Ibig (OFFICIAL MUSIC VIDEO)(MP3_70K).mp3",
    "Earth_ Wind _ Fire - Let_s Groove (TikTok Remix) Lyrics _ let_s groove tonight tiktok(MP3_70K).mp3",
    "Eraserheads - Ang Huling El Bimbo(MP3_70K).mp3",
    "Eternal Phlegm(MP3_70K).mp3",
    "Everyday Normal Guy 2(MP3_70K).mp3",
    "Frieren and Shrek drive in circles to Joji (Full Song)(MP3_70K).mp3",
    "GESHUOU(MP3_70K).mp3",
    "George Michael - Careless Whisper (Official Video)(MP3_70K).mp3",
    "Gloc 9 - Simpleng Tao (Lyrics)☁️ _ Habang tumutunog ang gitara sa _kin makinig ka sana [TikTok Song](MP3_70K).mp3",
    "Green Day - 21 Guns (Lyrics)(MP3_70K).mp3",
    "Hayaan Mo Sila - Ex Battalion x O.C Dawgs (Official Music Video)(MP3_70K).mp3",
    "I LOVE YOU SO - THE WALTERS (LYRICS) __ LIRIK(MP3_70K).mp3",
    "IV OF SPADES - Suliranin (Official Music Video)(MP3_70K).mp3",
    "Ibang Planeta - ZILD (Music Video)(MP3_70K).mp3",
    "Ikaw kase - Ex Battalion(MP3_70K).mp3",
    "Itchyworms - Akin Ka Na Lang (Official Music Video)(MP3_70K).mp3",
    "Itchyworms - Beer (Official Lyric Video)(MP3_70K).mp3",
    "Itchyworms - Di na muli (lyrics)(MP3_70K).mp3",
    "JAWNY - Honeypie Animation MV (Full Version by SeanWay Studio) _animation _JAWNY(MP3_70K).mp3",
    "JoJo_s Bizarre Adventure_ Diamond is Unbreakable OST - Morioh Cho Radio(MP3_70K).mp3",
    "Joy - Touch By Touch(MP3_70K).mp3",
    "KICK BACK(MP3_70K).mp3",
    "Lovers Rock - TV Girl (Lyrics)(MP3_70K).mp3",
    "Lucky Twice - Lucky (Lyrics) _I_m So Lucky Lucky I_m So Lovely Lovely_ [TikTok Remix](MP3_70K).mp3",
    "MYMP - Eternal Flame (Official Lyric Video)(MP3_70K).mp3",
    "Michael Jackson - Beat It (Official 4K Video)(MP3_70K).mp3",
    "Michael Jackson - Billie Jean (Official Video)(MP3_70K).mp3",
    "Michael V. I DJ Bumbay I Official Music Video(MP3_70K).mp3",
    "Modern Talking - Cheri Cheri Lady (Official Video)(MP3_70K).mp3",
    "Multo(MP3_70K).mp3",
    "NAE NI----A ORIGINAL SONG(MP3_70K).mp3",
    "Nadarang(MP3_70K).mp3",
    "Nightcore _ Sweet Little Bumblebee (lyric video)(MP3_70K).mp3",
    "One Day - Reggae(MP3_70K).mp3",
    "One Direction but every line is You_re Insecure(MP3_70K).mp3",
    "Parokya Ni Edgar - Mr. Suave (Official Music Video)(MP3_70K).mp3",
    "Philippine Circulation(MP3_70K).mp3",
    "Post Malone - Circles (Lyrics)(MP3_70K).mp3",
    "Queen – Bohemian Rhapsody (Official Video Remastered)(MP3_70K).mp3",
    "Ricky Montgomery - Mr Loverman (Lyrics) _and I miss my lover man_ (Tiktok)(MP3_70K).mp3",
    "Ron Henley - Biglang Liko (feat. Pow Chavez)(MP3_70K).mp3",
    "Soapdish - Tensionado(MP3_70K).mp3",
    "Stupid Love - Salbakuta (Lyrics) _ Tiktok Song(MP3_70K).mp3",
    "SunKissed Lola - Pasilyo (Official Lyric Video)(MP3_70K).mp3",
    "Sunflower - Post Malone (Spider-Man Miles Morales) Stylish Pro Web Swinging to Music --(MP3_70K).mp3",
    "Super Idol full song(MP3_70K).mp3",
    "The Marías - Sienna (Visualizer)(MP3_70K).mp3",
    "Wake Me Up When September Ends(MP3_70K).mp3",
    "Yakuza OST - Baka Mitai (ばかみたい) Kiryu full version(MP3_70K).mp3",
    "caramelldansen　ｳｯｰｳｯｰｳﾏｳﾏ(ﾟ∀ﾟ) PV(MP3_70K).mp3",
    "iluna - panaginip(MP3_70K).mp3",
    "in the sea(MP3_70K).mp3",
    "mao zedong propaganda music Red Sun in the Sky(MP3_70K).mp3",
    "Ева - винтаж - Eva - Vintage (lonely_ lonely_ I guess I_m lonely tiktok version) (Lyrics)(MP3_70K).mp3",
    "【Kung Fu Panda 4 OST】Baby - Justin bieber (Chinese)(MP3_70K).mp3",
    "心予報 - Eve MV(MP3_70K).mp3",
    "米津玄師  Kenshi Yonezu - IRIS OUT(MP3_70K).mp3"
};
// END_TRACK_NAMES

// Button timing variables
unsigned long btnPlayPressTime = 0;
unsigned long btnNextPressTime = 0;
bool btnPlayLongPressed = false;
bool btnNextLongPressed = false;
const unsigned long holdTime = 500; // ms to detect long press
const unsigned long adjustInterval = 200; // ms between volume changes while holding
unsigned long lastAdjustTime = 0;

// === SMART SHUFFLE FUNCTION ===
void buildSmartShuffle() {
  totalTracks = mp3.readFileCounts(); // Get total number of mp3 files
  if (totalTracks == -1) totalTracks = 10; // fallback if not detected
  for (int i = 0; i < totalTracks; i++) playlist[i] = i + 1;
  // Fisher-Yates shuffle
  for (int i = totalTracks - 1; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = playlist[i];
    playlist[i] = playlist[j];
    playlist[j] = temp;
  }
  currentIndex = 0;
}

// === PLAY NEXT SONG IN SMART ORDER ===
void playNextSmart() {
  if (currentIndex >= totalTracks) {
    buildSmartShuffle(); // Rebuild when all songs played
  }
  // Use the track name from the array (playlist[currentIndex] is 1-based, array is 0-based)
  Serial.print("Playing track: "); Serial.println(trackNames[playlist[currentIndex] - 1]);
  mp3.play(playlist[currentIndex]);
  delay(200); // Added delay to allow DFPlayer to process and start playing, preventing potential skips
  isPlaying = true; // Ensure state is updated
  currentIndex++;
}

void setup() {
  Serial.begin(9600); // Debug output
  Serial.println("Starting setup...");
  
  mp3Serial.begin(9600);
  delay(1000);
  if (!mp3.begin(mp3Serial)) {
    Serial.println("DFPlayer init failed!");
    while (true); // DFPlayer not found
  }
  Serial.println("DFPlayer initialized.");
  
  mp3.volume(volume);
  Serial.print("Initial volume: "); Serial.println(volume);
  
  randomSeed(analogRead(A1)); // Seed randomness (A1 should be floating)
  buildSmartShuffle();
  // Start at a random position in the shuffled playlist for the first song
  currentIndex = random(0, totalTracks);
  Serial.print("Starting first track: "); Serial.println(trackNames[playlist[currentIndex] - 1]);
  mp3.play(playlist[currentIndex]);
  delay(200); // Added delay for initial play
  isPlaying = true; // Ensure state is updated
  currentIndex++; // Prepare for next
  
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BUSY_PIN, INPUT);
  Serial.println("Setup complete.");
}

void loop() {
  // Handle BTN_PLAY (short: play/pause, long: volume down)
  if (digitalRead(BTN_PLAY) == LOW) {
    if (btnPlayPressTime == 0) {
      btnPlayPressTime = millis(); // Record press time
      btnPlayLongPressed = false;
      Serial.println("Play button pressed.");
    }
    if (millis() - btnPlayPressTime > holdTime && !btnPlayLongPressed) {
      btnPlayLongPressed = true; // Enter long press mode
      Serial.println("Play button long press detected.");
    }
    if (btnPlayLongPressed && millis() - lastAdjustTime > adjustInterval) {
      if (volume > 0) {
        volume--;
        mp3.volume(volume);
        Serial.print("Volume down: "); Serial.println(volume);
      }
      lastAdjustTime = millis();
    }
  } else {
    if (btnPlayPressTime != 0) {
      if (!btnPlayLongPressed) { // Short press: toggle play/pause
        if (isPlaying) {
          Serial.println("Sending pause command.");
          mp3.pause();
        } else {
          Serial.println("Sending start command.");
          mp3.start();
        }
        isPlaying = !isPlaying;
      }
      btnPlayPressTime = 0; // Reset
      delay(50); // Debounce
    }
  }

  // Handle BTN_NEXT (short: next, long: volume up)
  if (digitalRead(BTN_NEXT) == LOW) {
    if (btnNextPressTime == 0) {
      btnNextPressTime = millis(); // Record press time
      btnNextLongPressed = false;
      Serial.println("Next button pressed.");
    }
    if (millis() - btnNextPressTime > holdTime && !btnNextLongPressed) {
      btnNextLongPressed = true; // Enter long press mode
      Serial.println("Next button long press detected.");
    }
    if (btnNextLongPressed && millis() - lastAdjustTime > adjustInterval) {
      if (volume < 30) {
        volume++;
        mp3.volume(volume);
        Serial.print("Volume up: "); Serial.println(volume);
      }
      lastAdjustTime = millis();
    }
  } else {
    if (btnNextPressTime != 0) {
      if (!btnNextLongPressed) { // Short press: next song
        Serial.println("Sending next track command.");
        playNextSmart();
      }
      btnNextPressTime = 0; // Reset
      delay(50); // Debounce
    }
  }

  // === AUTOPLAY NEXT WHEN SONG ENDS USING BUSY PIN ===
  static bool wasPlayingPhysically = false; // Tracks previous BUSY state (true if was playing)
  bool isPlayingPhysically = (digitalRead(BUSY_PIN) == LOW); // LOW = playing
  if (wasPlayingPhysically && !isPlayingPhysically && isPlaying) {
    // Detected end of track (transition from playing to stopped), and intent is to play
    Serial.println("Track ended, autoplaying next.");
    playNextSmart();
  }
  wasPlayingPhysically = isPlayingPhysically;
}