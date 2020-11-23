//0 -> Light On<->Off | 1 -> Brightnessmenu | 2 -> Deep Sleep | 3 -> Animation switch

#include <FastLED.h>
#define NUM_LEDS 36
#define DATA_PIN 15
#define TOUCH_PIN 4
#define TOUCH_MAX 30

CRGB leds[NUM_LEDS];

void callback(){
  //placeholder callback function
}

CRGB actucm[6][6] = {
  {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},
  {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},
  {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},
  {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},
  {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},
  {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}}
};
unsigned long pos = 0;
bool slow = false;
unsigned long actulength = 2000;
bool slownext = false;
unsigned long actulengthnext = 2000;
CRGB nextcm[6][6] = {
  {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},
  {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},
  {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},
  {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},
  {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},
  {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}}
};
const String B64 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
const String B16 = "0123456789abcdef";
unsigned long startMillis;
unsigned long endMillis;
unsigned long loopMillis;

bool activ = false;
String aprogr = "";
String program = "";

String programs[] = {
  "0,2000000,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99,ffff99|",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  ""
};

byte savedprograms = 1;
byte programid = 0;
unsigned int brightness = 105;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  startprogram(programs[programid]);
  if(wakeup_reason == ESP_SLEEP_WAKEUP_TOUCHPAD){
    activ = true;
    onn();
  }else{
    lightoff();
  }
  touchAttachInterrupt(T0, callback, TOUCH_MAX);
  esp_sleep_enable_touchpad_wakeup();
  addprogram("1,2000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000|0,2000,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00|");
  addprogram("1,40000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000,ff0000|1,40000,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00,00ff00|1,40000,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff,0000ff|");
  addprogram("1,150,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff|1,150,000000,000000,000000,000000,000000,0000ff,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000|1,150,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000|1,150,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000|1,150,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000|1,150,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,000000,000000,000000,000000,000000,000000,0000ff,0000ff,000000,000000,000000,000000,000000|");
  addprogram("2,100,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff|2,100,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a|2,100,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a|2,100,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f,016e8f|2,100,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8,00a1d8|2,100,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00,ff6a00|2,100,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400,e22400|2,100,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070,707070|2,100,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00,f5ec00|2,100,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57,011d57|2,100,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe|2,100,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9,fffbb9|2,100,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad|2,100,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9,0042a9|2,100,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe,b18cfe|2,100,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff|2,100,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb,ebebeb|2,100,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6,d6d6d6|2,100,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2,c2c2c2|2,100,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad,adadad|2,100,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999,999999|2,100,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a,00374a|2,100,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b,012f7b|2,100,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4,008cb4|2,100,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff|2,100,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000|2,100,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff,ffffff|2,100,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000,000000|");
  //addprogram();
  //addprogram();
  loopMillis = millis();
}

void loop() {
  if (Serial.available()>0){
     String s=Serial.readStringUntil('\n');
     Serial.println("Add Program"+addprogram(s));
  }
  int oo = touch();
  if(oo!=-1){
    Serial.println(oo);
  }
  if(oo == 0){
    activ = !activ;
    if(activ){
      onn();
      loopMillis = millis();
    }else{
      lightoff();
    }
  }
  if(oo == 1){
    if(activ){
      brigthset();
      loopMillis = millis();
    }else{
      activ=true;
      onn();
      loopMillis = millis();
    }
  }
  if(oo == 2){
    Serial.println("Deep Sleep");
    deepoff();
    while(touchRead(TOUCH_PIN) < TOUCH_MAX){
      delay(10);
    }
    esp_deep_sleep_start();
  }
  if(oo == 3){
    if(activ){
      programid = programid + 1;
      if(programid>=savedprograms){
        programid = 0;
      }
      startprogram(programs[programid]);
      loopMillis = millis();
    }else{
      activ=true;
      onn();
      loopMillis = millis();
    }
  }
  if(activ){
    unsigned int now = millis();
    pos = pos + (now-loopMillis);
    loopMillis=now;
    handelactiv();
  }else{
    loopMillis = millis();
  }
  delay(10);
}

void handelactiv(){
  if(pos>=actulength){
    for(int i = 0; i < 6; i++){
      for(int j = 0; j < 6; j++){
        actucm[i][j] = nextcm[i][j];
      }
    }
    pos=0;
    slow = slownext;
    actulength = actulengthnext;
    loadnext();
  }
  unsigned int per = 100*pos/actulength;
  if(slow){
    for(int i = 0; i < 6; i++){
      for(int j = 0; j < 6; j++){
        leds[(i*6)+j] = CRGB((actucm[i][j][0] * (100-per)/100) + (nextcm[i][j][0] * per/100),(actucm[i][j][1] * (100-per)/100) + (nextcm[i][j][1] * per/100),(actucm[i][j][2] * (100-per)/100) + (nextcm[i][j][2] * per/100));
      }
    }
  }else{
    for(int i = 0; i < 6; i++){
      for(int j = 0; j < 6; j++){
        leds[(i*6)+j] = actucm[i][j];
      }
    }
  }
  FastLED.show();
}

void startprogram(String in){
  pos = 0;
  program = in;
  aprogr = in;
  String toload = aprogr.substring(0,aprogr.indexOf("|"));
  aprogr = aprogr.substring(aprogr.indexOf("|")+1);

  String now = toload.substring(0,toload.indexOf(","));
  toload = toload.substring(toload.indexOf(",")+1);
  slow = now == "1";

  now = toload.substring(0,toload.indexOf(","));
  toload = toload.substring(toload.indexOf(",")+1);
  actulength = now.toInt();

  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 6; j++){
      now = toload.substring(0,toload.indexOf(","));
      toload = toload.substring(toload.indexOf(",")+1);
      actucm[i][j] = hextocolor(toload);
    }
  }
  loadnext();
}
void loadnext(){
  String toload = aprogr.substring(0,aprogr.indexOf("|"));
  aprogr = aprogr.substring(aprogr.indexOf("|")+1);

  String now = toload.substring(0,toload.indexOf(","));
  toload = toload.substring(toload.indexOf(",")+1);
  slownext = now == "1";

  now = toload.substring(0,toload.indexOf(","));
  toload = toload.substring(toload.indexOf(",")+1);
  actulengthnext = now.toInt();
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 6; j++){
      now = toload.substring(0,toload.indexOf(","));
      toload = toload.substring(toload.indexOf(",")+1);
      nextcm[i][j] = hextocolor(toload);
    }
  }
  if(aprogr.length()<10){
    aprogr = program;
  }
}
int touch(){
  if(touchRead(TOUCH_PIN) > TOUCH_MAX){
    return -1;
  }
  startMillis = millis();
  while(touchRead(TOUCH_PIN) < TOUCH_MAX && millis()-startMillis<1500){
    delay(10);
  }
  endMillis = millis();
  if(endMillis-startMillis<20){
    return -1;
  }
  if(endMillis-startMillis<500){
    byte i = 0;
    while(touchRead(TOUCH_PIN) > TOUCH_MAX && i < 25){
      i = i + 1;
      delay(10);
    }
    if(touchRead(TOUCH_PIN) > TOUCH_MAX){
      return 0;
    }else{
      while(touchRead(TOUCH_PIN) < TOUCH_MAX){
        delay(10);
      }
      return 3;
    }
  }
  if(endMillis-startMillis<1000){
    return 1;
  }
  return 2;
}

String addprogram(String p){
  if(savedprograms>=10){
    return "full";
  }else{
    programs[savedprograms] = p;
    savedprograms = savedprograms + 1;
    return "added";
  }
}

void brigthset(){
  displaybrightness();
  unsigned int lastmilli = millis();
  while(lastmilli+2000 > millis()){
    if(touchRead(TOUCH_PIN) < TOUCH_MAX){
      byte i = 0;
      while(touchRead(TOUCH_PIN) < TOUCH_MAX && i < 49){
        i = i + 1;
        delay(10);
      }
      if(touchRead(TOUCH_PIN) < TOUCH_MAX){
        while(touchRead(TOUCH_PIN) < TOUCH_MAX){
          brightness = brightness - 5;
          if(brightness<5){brightness=5;}
          FastLED.setBrightness(brightness);
          displaybrightness();
          delay(62);
        }
      }else{
        brightness = brightness + 20;
        if(brightness>255){brightness=255;}
        FastLED.setBrightness(brightness);
        displaybrightness();
      }
      lastmilli = millis();
    }
  }
}

void displaybrightness(){
  Serial.println(brightness);
  unsigned int brnow = brightness-5;
  unsigned int ledon = brnow/42;
  unsigned int ledhalf = brnow%42;
  int i = 0;
  while(i<ledon){
    leds[i] = CRGB(0,255,0);
    leds[i+6] = CRGB(0,255,0);
    leds[i+12] = CRGB(0,255,0);
    leds[i+18] = CRGB(0,255,0);
    leds[i+24] = CRGB(0,255,0);
    leds[i+30] = CRGB(0,255,0);
    i++;
  }
  if(i<6){
    leds[i] = CRGB(255*(42-ledhalf)/42,255*ledhalf/42,0);
    leds[i+6] = CRGB(255*(42-ledhalf)/42,255*ledhalf/42,0);
    leds[i+12] = CRGB(255*(42-ledhalf)/42,255*ledhalf/42,0);
    leds[i+18] = CRGB(255*(42-ledhalf)/42,255*ledhalf/42,0);
    leds[i+24] = CRGB(255*(42-ledhalf)/42,255*ledhalf/42,0);
    leds[i+30] = CRGB(255*(42-ledhalf)/42,255*ledhalf/42,0);
    i++;
  }
  while(i<6){
    leds[i] = CRGB(255,0,0);
    leds[i+6] = CRGB(255,0,0);
    leds[i+12] = CRGB(255,0,0);
    leds[i+18] = CRGB(255,0,0);
    leds[i+24] = CRGB(255,0,0);
    leds[i+30] = CRGB(255,0,0);
    i++;
  }
  FastLED.show();
}

byte hextonum(String hex){
  return (B16.indexOf(hex.substring(0,1))*16)+B16.indexOf(hex.substring(1,2));
}
/*CRGB hextocolor(String hex){
  return CRGB(hextonum(hex.substring(0,2)),hextonum(hex.substring(3,4)),hextonum(hex.substring(5,6)));
}*/
CRGB hextocolor(String hex){
  return CRGB(hextonum(hex.substring(0,2)),hextonum(hex.substring(2,4)),hextonum(hex.substring(4,6)));
}

void deepoff(){
  const String ledd = "0123456789abcdefghijklmnopqrstuvwxyz";
  String rr = "75xvp1jb8iyyl2e30a649mksms4bc6tawfldnvhq8xrdoj10zhkizuo3gepnwguq5f72trc9";
  byte rrchange = random(9);
  if(rrchange==1){rr="udwb1p0y3qljv7tjov69t8asdx2sgf794cmg5fcimrz2er6nzaqkni4lkhxhoy5eu13wp08b";}
  if(rrchange==2){rr="eqbv1b5qkssrp03tmu4g3ln5k2zgjxardhyx7oz6mc8f1ye8w6dt472hcfp09ijon9waivlu";}
  if(rrchange==3){rr="9u6ufc3yk0pqvsbtwmo4eoly9jk5s6fhna51aegwvxrpd87l70m4bc1zrg8xi2q3nthi2djz";}
  if(rrchange==4){rr="thx8f19esw3big075fcqheq4kj8namgwz6uzbl6vy9rdca3kr7sjdlot4in50pop2u1vyx2m";}
  if(rrchange==5){rr="c69o7iij32wq8orvg9ac1z4ed2v0pqsx3p1gnu7yefhdbk6urmhbn4k5yjzam0sxtwl8lf5t";}
  if(rrchange==6){rr="ejifmb68i34go6q7r0ywlfwoltahvzxna8ukm594dp1cqk0x3uhejvs179s22dzb5rtncypg";}
  if(rrchange==7){rr="osl2o7ghic0a8beis2dvtknqky6prf1x08gnd4z13jy45mfpzemrvtjqwacl53h967xuuw9b";}
  if(rrchange==8){rr="gae30grh51nsunjlwejai4io2ffqz1dktk6pqxvb3cc8x0956dswbum47rt2loyymp9h87zv";}
  if(rrchange==9){rr="j10e6ymglenu8r6kpf5fj5ntoalmzyhv4criosaq97ckdd20h9w73b4zxvsi38u1pw2xtbqg";}
  bool sw[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for(byte i = 0; i < 36*2; i++){
    byte ll = ledd.indexOf(rr.substring(i,i+1));
    if(sw[ll]){
      leds[ll] = CRGB(0,0,0);
    }else{
      leds[ll] = CRGB(random(50,255),random(50,255),random(50,255));
      sw[ll] = 1;
    }
    FastLED.show();
    delay(15);
  }
}
void lightoff(){
  for(int i = 5; i >= 0; i=i-1){
    leds[i] = CRGB(0,0,0);
    leds[i+6] = CRGB(0,0,0);
    leds[i+12] = CRGB(0,0,0);
    leds[i+18] = CRGB(0,0,0);
    leds[i+24] = CRGB(0,0,0);
    leds[i+30] = CRGB(0,0,0);
    FastLED.show();
    delay(50);
  }
}
void onn(){
  if(slow){
    unsigned int per = 100*pos/actulength;
    for(int i = 0; i < 6; i++){
      leds[i] = CRGB((actucm[0][i][0] * (100-per)/100) + (nextcm[0][i][0] * per/100),(actucm[0][i][1] * (100-per)/100) + (nextcm[0][i][1] * per/100),(actucm[0][i][2] * (100-per)/100) + (nextcm[0][i][2] * per/100));
      leds[i+6] = CRGB((actucm[1][i][0] * (100-per)/100) + (nextcm[1][i][0] * per/100),(actucm[1][i][1] * (100-per)/100) + (nextcm[1][i][1] * per/100),(actucm[1][i][2] * (100-per)/100) + (nextcm[1][i][2] * per/100));
      leds[i+12] = CRGB((actucm[2][i][0] * (100-per)/100) + (nextcm[2][i][0] * per/100),(actucm[2][i][1] * (100-per)/100) + (nextcm[2][i][1] * per/100),(actucm[2][i][2] * (100-per)/100) + (nextcm[2][i][2] * per/100));
      leds[i+18] = CRGB((actucm[3][i][0] * (100-per)/100) + (nextcm[3][i][0] * per/100),(actucm[3][i][1] * (100-per)/100) + (nextcm[3][i][1] * per/100),(actucm[3][i][2] * (100-per)/100) + (nextcm[3][i][2] * per/100));
      leds[i+24] = CRGB((actucm[4][i][0] * (100-per)/100) + (nextcm[4][i][0] * per/100),(actucm[4][i][1] * (100-per)/100) + (nextcm[4][i][1] * per/100),(actucm[4][i][2] * (100-per)/100) + (nextcm[4][i][2] * per/100));
      leds[i+30] = CRGB((actucm[5][i][0] * (100-per)/100) + (nextcm[5][i][0] * per/100),(actucm[5][i][1] * (100-per)/100) + (nextcm[5][i][1] * per/100),(actucm[5][i][2] * (100-per)/100) + (nextcm[5][i][2] * per/100));
      FastLED.show();
      delay(50);
    }
  }else{
    for(int i = 0; i < 6; i++){
      leds[i] = actucm[0][i];
      leds[i+6] = actucm[1][i];
      leds[i+12] = actucm[2][i];
      leds[i+18] = actucm[3][i];
      leds[i+24] = actucm[4][i];
      leds[i+30] = actucm[5][i];
      FastLED.show();
      delay(50);
    }
  }
}
