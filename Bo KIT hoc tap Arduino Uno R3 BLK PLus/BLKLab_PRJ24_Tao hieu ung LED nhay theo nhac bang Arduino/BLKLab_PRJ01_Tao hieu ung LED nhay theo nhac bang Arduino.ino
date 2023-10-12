/************************************************************************************
 *                            BANLINHKIEN.COM                                       *
 ***********************************************************************************                           
 *            HƯỚNG DẪN TẠO HIỆU ỨNG LED NHAY THEO NHẠC BĂNG ARDUINO
 */
#include <Adafruit_NeoPixel.h>  
#ifdef __AVR__
#include <avr/power.h>   
#endif                   

#define LED_PIN   3  
#define LED_TOTAL 12  
#define LED_HALF  LED_TOTAL/2
#define VISUALS   6   
#define SAMPLE_WINDOW   10  
#define INPUT_FLOOR 50 
#define INPUT_CEILING 400 
#define PEAK_HANG 15 
#define PEAK_FALL 6 

unsigned int sample;
byte peak = 20;
byte dotCount = 0;  
byte dotHangCount = 0; 
byte  volCount  = 0;     

#define AUDIO_PIN A0  
#define KNOB_PIN  A1  
#define BUTTON_1  6   
#define BUTTON_2  5   
#define BUTTON_3  4   

Adafruit_NeoPixel strand = Adafruit_NeoPixel(LED_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  

uint16_t gradient = 0; 

uint16_t thresholds[] = {1529, 1019, 764, 764, 764, 1274};

uint8_t palette = 0;  
uint8_t visual = 0;   
uint8_t volume = 0;   
uint8_t last = 0;     

float maxVol = 1;    
float knob = 1023.0;  
float avgBump = 0;    
float avgVol = 0;     
float shuffleTime = 0;  

bool shuffle = true;  
bool bump = false;     

//For Traffic() visual
int8_t pos[LED_TOTAL] = { -2};    
uint8_t rgb[LED_TOTAL][3] = {0}; 

//For Snake() visual
bool left = false;  
int8_t dotPos = 0;  
float timeBump = 0; 
float avgTime = 0;  
void setup() {    

  Serial.begin(9600); 

  pinMode(BUTTON_1, INPUT); pinMode(BUTTON_2, INPUT); pinMode(BUTTON_3, INPUT);

  digitalWrite(BUTTON_1, HIGH); digitalWrite(BUTTON_2, HIGH); digitalWrite(BUTTON_3, HIGH);

  strand.begin(); 
  strand.show();  
}


void loop() {  

  volume = (analogRead(AUDIO_PIN)-126);     
  knob = 0.9;

  if (volume < avgVol / 2.0 || volume < 15) volume = 0;

  else avgVol = (avgVol + volume) / 2.0; 
  if (volume > maxVol) maxVol = volume;

  CyclePalette();  
  CycleVisual();  
  ToggleShuffle(); 
  if (gradient > thresholds[palette]) {
    gradient %= thresholds[palette] + 1;
    maxVol = (maxVol + volume) / 2.0;
  }

  if (volume - last > 10) avgBump = (avgBump + (volume - last)) / 2.0;
  bump = (volume - last > avgBump * .9);
  if (bump) {
    avgTime = (((millis() / 1000.0) - timeBump) + avgTime) / 2.0;
    timeBump = millis() / 1000.0;
  }

  Visualize();   
  gradient++;    
  last = volume; 
  delay(16);     
}

void Visualize() {
  switch (0) {
    case 0: return VU ();
    case 1: return VUdot();
    case 2: return VU1();
    case 3: return VU2();
    case 4: return Pulse();
    case 5: return Traffic();
    
    default:
    {
      return PaletteDance();
    } 
  }
}

uint32_t ColorPalette(float num) {
  switch (palette) {
    case 0: return (num < 0) ? Rainbow(gradient) : Rainbow(num);
    case 1: return (num < 0) ? Sunset(gradient) : Sunset(num);
    case 2: return (num < 0) ? Ocean(gradient) : Ocean(num);
    case 3: return (num < 0) ? PinaColada(gradient) : PinaColada(num);
    case 4: return (num < 0) ? Sulfur(gradient) : Sulfur(num);
    case 5: return (num < 0) ? NoGreen(gradient) : NoGreen(num);
    default: return Rainbow(gradient);
  }
}

void Pulse() {

  fade(0.75);   

  if (bump) gradient += thresholds[palette] / 24;

  if (volume > 0) {
    uint32_t col = ColorPalette(-1); //Our retrieved 32-bit color

    int start = LED_HALF - (LED_HALF * (volume / maxVol)) ;
    int finish = LED_HALF + (LED_HALF * (volume / maxVol)) + strand.numPixels() % 2;

    for (int i = start; i < finish; i++) {
      float damp = sin((i - start) * PI / float(finish - start));
      damp = pow(damp, 2.0);
      uint32_t col2 = strand.getPixelColor(-1);
      uint8_t colors[3];
      float avgCol = 0, avgCol2 = 0;
      for (int k = 0; k < 3; k++) {
        colors[k] = split(col, k) * damp * knob * pow(volume / maxVol, 2);
        avgCol += colors[k];
        avgCol2 += split(col2, k);
      }
      avgCol /= 3.0, avgCol2 /= 3.0;

      if (avgCol > avgCol2) strand.setPixelColor(i, strand.Color(colors[0], colors[1], colors[2]));
    }
  }
  strand.show();
}
void VU() //Adafruit Code
{
  unsigned long startMillis = millis(); 
  float peakToPeak = 0;   

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(AUDIO_PIN)*2;
    if (sample < 1024)  
    {
      if (sample > signalMax)
      {
        signalMax = sample;  
      }
      else if (sample < signalMin)
      {
        signalMin = sample; 
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  for (int i = 0; i <= strand.numPixels() - 1; i++) {
    strand.setPixelColor(i, Wheel(map(i, 0, strand.numPixels() - 1, 10, 200)));
  }
  c = fscale(INPUT_FLOOR, INPUT_CEILING, strand.numPixels(), 0, peakToPeak, 2);

  if (c <= strand.numPixels()) { // Fill partial column with off pixels
    drawLine(strand.numPixels(), strand.numPixels() - c, strand.Color(0, 0, 0));
  }
  y = strand.numPixels() - peak;

  strand.setPixelColor(y - 1, Wheel(map(y, 0, strand.numPixels() - 1, 10, 200)));

  strand.show();

  if (dotHangCount > PEAK_HANG) { 
    if (++dotCount >= PEAK_FALL) {
      peak++;
      dotCount = 0;
    }
  }
  else {
    dotHangCount++;
  }
}


void VUdot() 
{
  unsigned long startMillis = millis();
  float peakToPeak = 0;   

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;


  
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(AUDIO_PIN)*3 ;
    if (sample < 1024)  
    {
      if (sample > signalMax)
      {
        signalMax = sample;  
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  
      }
    }
  }
  peakToPeak = signalMax - signalMin;  
  for (int i = 0; i <= strand.numPixels() - 1; i++) {
    strand.setPixelColor(i, Wheel(map(i, 0, strand.numPixels() - 1, 10, 200)));
  }
  c = fscale(INPUT_FLOOR, INPUT_CEILING, strand.numPixels(), 0, peakToPeak, 2);

  if (c < peak) {
    peak = c;        
    dotHangCount = 0;    
  }
  if (c <= strand.numPixels()) { 
    drawLine(strand.numPixels(), strand.numPixels() - c, strand.Color(0, 0, 0));
  }
  y = strand.numPixels() - peak;

  strand.setPixelColor(y - 1, Wheel(map(y, 0, strand.numPixels() - 1, 10, 200)));

  strand.show();
  if (dotHangCount > PEAK_HANG) { 
    if (++dotCount >= PEAK_FALL) { 
      peak++;
      dotCount = 0;
    }
  }
  else {
    dotHangCount++;
  }
}

void VU1()
{
  unsigned long startMillis = millis(); 
  float peakToPeak = 0;  

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;

  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(AUDIO_PIN)*3;
     Serial.println(sample);
    if (sample < 1024)
    {
      if (sample > signalMax)
      {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin;

  for (int i = 0; i <= LED_HALF- 1; i++) {
    uint32_t color = Wheel(map(i, 0, - 1, 30, 150));
    strand.setPixelColor(LED_TOTAL - i, color);
    strand.setPixelColor(0 + i, color);
  }
  c = fscale(INPUT_FLOOR, INPUT_CEILING, LED_HALF, 0, peakToPeak, 2);
  if (c < peak) {
    peak = c;       
    dotHangCount = 0;    
  }
  if (c <= strand.numPixels()) { 
    drawLine(LED_HALF, LED_HALF - c, strand.Color(0, 0, 0));
    drawLine(LED_HALF, LED_HALF + c, strand.Color(0, 0, 0));
  }

  y = LED_HALF - peak;
  uint32_t color1 = Wheel(map(y, 0, LED_HALF - 1, 30, 150));
  strand.setPixelColor(y - 1, color1);
  y =LED_HALF + peak;
  strand.setPixelColor(y, color1);
  strand.show();
  if (dotHangCount > PEAK_HANG) { 
    if (++dotCount >= PEAK_FALL) { 
      peak++;
      dotCount = 0;
    }
  }
  else {
    dotHangCount++;
  }
}

void VU2()
{
  unsigned long startMillis = millis(); 
  float peakToPeak = 0;   

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;

  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(AUDIO_PIN)*3;
    Serial.println(sample);
    if (sample < 1024)
    {
      if (sample > signalMax)
      {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin; 
  for (int i = 0; i <= LED_HALF - 1; i++) {
    uint32_t color = Wheel(map(i, 0, LED_HALF - 1, 30, 150));
    strand.setPixelColor(LED_HALF - i - 1, color);
    strand.setPixelColor(LED_HALF + i, color);
  }


  c = fscale(INPUT_FLOOR, INPUT_CEILING, LED_HALF, 0, peakToPeak, 2);

  if (c < peak) {
    peak = c;
    dotHangCount = 0;
  }

  if (c <= LED_TOTAL) {
    drawLine(LED_TOTAL, LED_TOTAL - c, strand.Color(0, 0, 0));
    drawLine(0, 0 + c, strand.Color(0, 0, 0));
  }

  y = LED_TOTAL - peak;
  strand.setPixelColor(y - 1, Wheel(map(LED_HALF + y, 0, LED_HALF - 1, 30, 150)));

  y = 0 + peak;
  strand.setPixelColor(y, Wheel(map(LED_HALF - y, 0, LED_HALF + 1, 30, 150)));

 strand.show();
  if (dotHangCount > PEAK_HANG) { 
    if (++dotCount >= PEAK_FALL) { 
      peak++;
      dotCount = 0;
    }
  }
  else {
    dotHangCount++;
  }
}

void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for (int i = from; i <= to; i++) {
    strand.setPixelColor(i, c);
  }
}

void Traffic() {
  fade(0.8);
  if (bump) {
    int8_t slot = 0;
    for (slot; slot < sizeof(pos); slot++) {
      if (pos[slot] < -1) break;
      else if (slot + 1 >= sizeof(pos)) {
        slot = -3;
        break;
      }
    }
    if (slot != -3) {
      pos[slot] = (slot % 2 == 0) ? -1 : strand.numPixels();
      uint32_t col = ColorPalette(-1);
      gradient += thresholds[palette] / 24;
      for (int j = 0; j < 3; j++) {
        rgb[slot][j] = split(col, j);
      }
    }
  }
  if (volume > 0) {
    for (int i = 0; i < sizeof(pos); i++) {
      if (pos[i] < -1) continue;
      pos[i] += (i % 2) ? -1 : 1;
      if (pos[i] >= strand.numPixels()) pos[i] = -2;
      strand.setPixelColor( pos[i], strand.Color(
                              float(rgb[i][0]) * pow(volume / maxVol, 2.0) * knob,
                              float(rgb[i][1]) * pow(volume / maxVol, 2.0) * knob,
                              float(rgb[i][2]) * pow(volume / maxVol, 2.0) * knob)
                          );
    }
  }
  strand.show(); 
}

void PaletteDance() {
  if (bump) left = !left; 
  if (volume > avgVol) {
    for (int i = 0; i < strand.numPixels(); i++) {

      float sinVal = abs(sin(
                           (i + dotPos) *
                           (PI / float(strand.numPixels() / 1.25) )
                         ));
      sinVal *= sinVal;
      sinVal *= volume / maxVol;
      sinVal *= knob;

      unsigned int val = float(thresholds[palette] + 1)
                         * (float(i + map(dotPos, -1 * (strand.numPixels() - 1), strand.numPixels() - 1, 0, strand.numPixels() - 1))
                            / float(strand.numPixels()))
                         + (gradient);

      val %= thresholds[palette]; 

      uint32_t col = ColorPalette(val); 

      strand.setPixelColor(i, strand.Color(
                             float(split(col, 0))*sinVal,
                             float(split(col, 1))*sinVal,
                             float(split(col, 2))*sinVal)
                          );
    }
    dotPos += (left) ? -1 : 1;
  }
  else  fade(0.8);
  strand.show(); //Show lights.
  if (dotPos < 0) dotPos = strand.numPixels() - strand.numPixels() / 6;
  else if (dotPos >= strand.numPixels() - strand.numPixels() / 6)  dotPos = 0;
}

void Paintball() {
  if ((millis() / 1000.0) - timeBump > avgTime * 2.0) fade(0.99);
  bleed(dotPos);
  if (bump) {
    randomSeed(micros());
    dotPos = random(strand.numPixels() - 1);
    uint32_t col = ColorPalette(random(thresholds[palette]));
    uint8_t colors[3];
    for (int i = 0; i < 3; i++) colors[i] = split(col, i) * pow(volume / maxVol, 2.0) * knob;
    strand.setPixelColor(dotPos, strand.Color(colors[0], colors[1], colors[2]));
    for (int i = 0; i < 3; i++) colors[i] *= .8;
    strand.setPixelColor(dotPos - 1, strand.Color(colors[0], colors[1], colors[2]));
    strand.setPixelColor(dotPos + 1, strand.Color(colors[0], colors[1], colors[2]));
  }
  strand.show(); //Show lights.
}


void PalettePulse() {
  fade(0.75);
  if (bump) gradient += thresholds[palette] / 24;
  if (volume > 0) {
    int start = LED_HALF - (LED_HALF * (volume / maxVol));
    int finish = LED_HALF + (LED_HALF * (volume / maxVol)) + strand.numPixels() % 2;
    for (int i = start; i < finish; i++) {
      float damp = sin((i - start) * PI / float(finish - start));
      damp = pow(damp, 2.0);

      int val = thresholds[palette] * (i - start) / (finish - start);
      val += gradient;
      uint32_t col = ColorPalette(val);

      uint32_t col2 = strand.getPixelColor(i);
      uint8_t colors[3];
      float avgCol = 0, avgCol2 = 0;
      for (int k = 0; k < 3; k++) {
        colors[k] = split(col, k) * damp * knob * pow(volume / maxVol, 2);
        avgCol += colors[k];
        avgCol2 += split(col2, k);
      }
      avgCol /= 3.0, avgCol2 /= 3.0;
      if (avgCol > avgCol2) strand.setPixelColor(i, strand.Color(colors[0], colors[1], colors[2]));
    }
  }
  strand.show();
}

void Cycle() {
  for (int i = 0; i < strand.numPixels(); i++) {
    float val = float(thresholds[palette]) * (float(i) / float(strand.numPixels())) + (gradient);
    val = int(val) % thresholds[palette];
    strand.setPixelColor(i, ColorPalette(val));
  }
  strand.show();
  gradient += 32;
}

void CyclePalette() {

  if (shuffle && millis() / 1000.0 - shuffleTime > 38 && gradient % 2) {

    shuffleTime = millis() / 1000.0; 

    palette++;
    if (palette >= sizeof(thresholds) / 2) palette = 0;
    gradient %= thresholds[palette];
    maxVol = avgVol;  
  }
}


void CycleVisual() {
  if (!digitalRead(BUTTON_2)) {
    delay(10);
    shuffle = false;
    visual++;     
    gradient = 0;
    if (visual > VISUALS) 
    {
      shuffle = true;
      visual = 0;
    }

    if (visual == 1) memset(pos, -2, sizeof(pos));

    if (visual == 2 || visual == 3) {
      randomSeed(analogRead(0));
      dotPos = random(strand.numPixels());
    }

    delay(350);

    maxVol = avgVol;
  }

  if (shuffle && millis() / 1000.0 - shuffleTime > 38&& !(gradient % 2)) {// Thời gian tự động đổi hiệu ứng

    shuffleTime = millis() / 1000.0; //Record the time this shuffle happened.

    visual++;
    gradient = 0;
    if (visual > VISUALS) visual = 0;
    if (visual == 1) memset(pos, -2, sizeof(pos));
    if (visual == 2 || visual == 3) {
      randomSeed(analogRead(0));
      dotPos = random(strand.numPixels());
    }
    maxVol = avgVol;
  }
}

void fade(float damper) {
  for (int i = 0; i < strand.numPixels(); i++) {
    uint32_t col = strand.getPixelColor(i);
    if (col == 0) continue;
    float colors[3];
    for (int j = 0; j < 3; j++) colors[j] = split(col, j) * damper;
    strand.setPixelColor(i, strand.Color(colors[0] , colors[1], colors[2]));
  }
}

void bleed(uint8_t Point) {
  for (int i = 1; i < strand.numPixels(); i++) {

    int sides[] = {Point - i, Point + i};

    for (int i = 0; i < 2; i++) {

      int point = sides[i];
      uint32_t colors[] = {strand.getPixelColor(point - 1), strand.getPixelColor(point), strand.getPixelColor(point + 1)  };

      //Sets the new average values to just the central point, not the left and right points.
      strand.setPixelColor(point, strand.Color(
                             float( split(colors[0], 0) + split(colors[1], 0) + split(colors[2], 0) ) / 3.0,
                             float( split(colors[0], 1) + split(colors[1], 1) + split(colors[2], 1) ) / 3.0,
                             float( split(colors[0], 2) + split(colors[1], 2) + split(colors[2], 2) ) / 3.0)
                          );
    }
  }
}

uint8_t split(uint32_t color, uint8_t i ) {

  //0 = Red, 1 = Green, 2 = Blue

  if (i == 0) return color >> 16;
  if (i == 1) return color >> 8;
  if (i == 2) return color >> 0;
  return -1;
}

uint32_t Rainbow(unsigned int i) {
  if (i > 1529) return Rainbow(i % 1530);
  if (i > 1274) return strand.Color(255, 0, 255 - (i % 255));   //violet -> red
  if (i > 1019) return strand.Color((i % 255), 0, 255);         //blue -> violet
  if (i > 764) return strand.Color(0, 255 - (i % 255), 255);    //aqua -> blue
  if (i > 509) return strand.Color(0, 255, (i % 255));          //green -> aqua
  if (i > 255) return strand.Color(255 - (i % 255), 255, 0);    //yellow -> green
  return strand.Color(255, i, 0);                               //red -> yellow
}

uint32_t Sunset(unsigned int i) {
  if (i > 1019) return Sunset(i % 1020);
  if (i > 764) return strand.Color((i % 255), 0, 255 - (i % 255));          //blue -> red
  if (i > 509) return strand.Color(255 - (i % 255), 0, 255);                //purple -> blue
  if (i > 255) return strand.Color(255, 128 - (i % 255) / 2, (i % 255));    //orange -> purple
  return strand.Color(255, i / 2, 0);                                       //red -> orange
}

uint32_t Ocean(unsigned int i) {
  if (i > 764) return Ocean(i % 765);
  if (i > 509) return strand.Color(0, i % 255, 255 - (i % 255));  //blue -> green
  if (i > 255) return strand.Color(0, 255 - (i % 255), 255);      //aqua -> blue
  return strand.Color(0, 255, i);                                 //green -> aqua
}

uint32_t PinaColada(unsigned int i) {
  if (i > 764) return PinaColada(i % 765);
  if (i > 509) return strand.Color(255 - (i % 255) / 2, (i % 255) / 2, (i % 255) / 2);  //red -> half white
  if (i > 255) return strand.Color(255, 255 - (i % 255), 0);                            //yellow -> red
  return strand.Color(128 + (i / 2), 128 + (i / 2), 128 - i / 2);                       //half white -> yellow
}

uint32_t Sulfur(unsigned int i) {
  if (i > 764) return Sulfur(i % 765);
  if (i > 509) return strand.Color(i % 255, 255, 255 - (i % 255));   //aqua -> yellow
  if (i > 255) return strand.Color(0, 255, i % 255);                 //green -> aqua
  return strand.Color(255 - i, 255, 0);                              //yellow -> green
}

uint32_t NoGreen(unsigned int i) {
  if (i > 1274) return NoGreen(i % 1275);
  if (i > 1019) return strand.Color(255, 0, 255 - (i % 255));         //violet -> red
  if (i > 764) return strand.Color((i % 255), 0, 255);                //blue -> violet
  if (i > 509) return strand.Color(0, 255 - (i % 255), 255);          //aqua -> blue
  if (i > 255) return strand.Color(255 - (i % 255), 255, i % 255);    //yellow -> aqua
  return strand.Color(255, i, 0);                                     //red -> yellow
}

uint32_t USA(unsigned int i) {
  if (i > 764) return USA(i % 765);
  if (i > 509) return strand.Color(0, 0, 255);      //blue
  if (i > 255) return strand.Color(128, 128, 128);  //white
  return strand.Color(255, 0, 0);                   //red
}
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strand.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strand.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strand.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
float fscale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue, float curve) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; 
  curve = pow(10, curve); 

  
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}
