//SparkFun Addressable RGB LED Sound and Music Visualizer Tutorial Arduino Code by Michael Bartlett

//Libraries
#include <Adafruit_NeoPixel.h>  //Library to simplify interacting with the LED strand
#ifdef __AVR__
#include <avr/power.h>   //Includes the library for power reduction registers if your chip supports them. 
#endif                   //More info: http://www.nongnu.org/avr-libc/user-manual/group__avr__power.htlm

//Constants (change these as necessary)
#define LED_PIN   3  //Pin for the pixel strand. Can be analog or digital.
#define LED_TOTAL 12  //Change this to the number of LEDs in your strand.
#define LED_HALF  LED_TOTAL/2
#define VISUALS   6   //Số hiệu ứng)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define INPUT_FLOOR 50 //Lower range of analogRead input BEST 40
#define INPUT_CEILING 400 //Max range of analogRead input, the lower the value the more sensitive (1023 = max) BEST 400
#define PEAK_HANG 15 //Time of pause before peak dot falls
#define PEAK_FALL 6 //Rate of falling peak dot

unsigned int sample;
byte peak = 20;
byte dotCount = 0;  //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot
byte  volCount  = 0;      // Frame counter for storing past volume data

#define AUDIO_PIN A0  //Pin for the envelope of the sound detector
#define KNOB_PIN  A1  //Pin for the trimpot 10K
#define BUTTON_1  6   //Button 1 cycles color palettes
#define BUTTON_2  5   //Button 2 cycles visualization modes
#define BUTTON_3  4   //Button 3 toggles shuffle mode (automated changing of color and visual)

//////////<Globals>
//  These values either need to be remembered from the last pass of loop() or
//  need to be accessed by several functions in one pass, so they need to be global.

Adafruit_NeoPixel strand = Adafruit_NeoPixel(LED_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //LED strand objetcs

uint16_t gradient = 0; //Used to iterate and loop through each color palette gradually

//IMPORTANT:
//  This array holds the "threshold" of each color function (i.e. the largest number they take before repeating).
//  The values are in the same order as in ColorPalette()'s switch case (Rainbow() is first, etc). This is simply to
//  keep "gradient" from overflowing, the color functions themselves can take any positive value. For example, the
//  largest value Rainbow() takes before looping is 1529, so "gradient" should reset after 1529, as listed.
//     Make sure you add/remove values accordingly if you add/remove a color function in the switch-case in ColorPalette().
uint16_t thresholds[] = {1529, 1019, 764, 764, 764, 1274};

uint8_t palette = 0;  //Holds the current color palette.
uint8_t visual = 0;   //Holds the current visual being displayed.
uint8_t volume = 0;   //Holds the volume level read from the sound detector.
uint8_t last = 0;     //Holds the value of volume from the previous loop() pass.

float maxVol = 1;    //Holds the largest volume recorded thus far to proportionally adjust the visual's responsiveness.
float knob = 1023.0;  //Holds the percentage of how twisted the trimpot is. Used for adjusting the max brightness.
float avgBump = 0;    //Holds the "average" volume-change to trigger a "bump."
float avgVol = 0;     //Holds the "average" volume-level to proportionally adjust the visual experience.
float shuffleTime = 0;  //Holds how many seconds of runtime ago the last shuffle was (if shuffle mode is on).

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTE: The reason "average" is quoted is because it is not a true mathematical average. This is because I have
//      found what I call a "sequenced average" is more successful in execution than a real average. The difference
//      is that the sequenced average doesn't use the pool of all values recorded thus far, but rather averages the
//      last average and the current value received (in sequence). Concretely:
//
//          True average: (1 + 2 + 3) / 3 = 2
//          Sequenced: (1 + 2) / 2 = 1.5 --> (1.5 + 3) / 2 = 2.25  (if 1, 2, 3 was the order the values were received)
//
//      All "averages" in the program operate this way. The difference is subtle, but the reason is that sequenced
//      averages are more adaptive to changes in the overall volume. In other words, if you went from loud to quiet,
//      the sequenced average is more likely to show an accurate and proportional adjustment more fluently.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool shuffle = true;  //Toggles shuffle mode.
bool bump = false;     //Used to pass if there was a "bump" in volume

//For Traffic() visual
int8_t pos[LED_TOTAL] = { -2};    //Stores a population of color "dots" to iterate across the LED strand.
uint8_t rgb[LED_TOTAL][3] = {0};  //Stores each dot's specific RGB values.

//For Snake() visual
bool left = false;  //Determines the direction of iteration. Recycled in PaletteDance()
int8_t dotPos = 0;  //Holds which LED in the strand the dot is positioned at. Recycled in most other visuals.
float timeBump = 0; //Holds the time (in runtime seconds) the last "bump" occurred.
float avgTime = 0;  //Holds the "average" amount of time between each "bump" (used for pacing the dot's movement).

//////////</Globals>


//////////<Standard Functions>

void setup() {    //Like it's named, this gets ran before any other function.

  Serial.begin(9600); //Sets data rate for serial data transmission.

  //Defines the buttons pins to be input.
  pinMode(BUTTON_1, INPUT); pinMode(BUTTON_2, INPUT); pinMode(BUTTON_3, INPUT);

  //Write a "HIGH" value to the button pins.
  digitalWrite(BUTTON_1, HIGH); digitalWrite(BUTTON_2, HIGH); digitalWrite(BUTTON_3, HIGH);

  strand.begin(); //Initialize the LED strand object.
  strand.show();  //Show a blank strand, just to get the LED's ready for use.
}


void loop() {  //This is where the magic happens. This loop produces each frame of the visual.

  volume = (analogRead(AUDIO_PIN)-126);     //Record the volume level from the sound detector
  //Serial.println(volume);
  //knob = analogRead(KNOB_PIN) / 1023.0; //Record how far the trimpot is twisted
  knob = 0.9;

  //Sets a threshold for volume.
  //  In practice I've found noise can get up to 15, so if it's lower, the visual thinks it's silent.
  //  Also if the volume is less than average volume / 2 (essentially an average with 0), it's considered silent.
  if (volume < avgVol / 2.0 || volume < 15) volume = 0;

  else avgVol = (avgVol + volume) / 2.0; //If non-zeo, take an "average" of volumes.

  //If the current volume is larger than the loudest value recorded, overwrite
  if (volume > maxVol) maxVol = volume;

  //Check the Cycle* functions for specific instructions if you didn't include buttons in your design.
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  CyclePalette();  //Changes palette for shuffle mode or button press.

  CycleVisual();   //Changes visualization for shuffle mode or button press.

  ToggleShuffle(); //Toggles shuffle mode. Delete this if you didn't use buttons.
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  //This is where "gradient" is modulated to prevent overflow.
  if (gradient > thresholds[palette]) {
    gradient %= thresholds[palette] + 1;

    //Everytime a palette gets completed is a good time to readjust "maxVol," just in case
    //  the song gets quieter; we also don't want to lose brightness intensity permanently
    //  because of one stray loud sound.
    maxVol = (maxVol + volume) / 2.0;
  }

  //If there is a decent change in volume since the last pass, average it into "avgBump"
  if (volume - last > 10) avgBump = (avgBump + (volume - last)) / 2.0;

  //If there is a notable change in volume, trigger a "bump"
  //  avgbump is lowered just a little for comparing to make the visual slightly more sensitive to a beat.
  bump = (volume - last > avgBump * .9);

  //If a "bump" is triggered, average the time between bumps
  if (bump) {
    avgTime = (((millis() / 1000.0) - timeBump) + avgTime) / 2.0;
    timeBump = millis() / 1000.0;
  }

  Visualize();   //Calls the appropriate visualization to be displayed with the globals as they are.

  gradient++;    //Increments gradient

  last = volume; //Records current volume for next pass

  delay(16);     //Paces visuals so they aren't too fast to be enjoyable
}
//////////</Standard Functions>


//////////<Visual Functions>

//This function calls the appropriate visualization based on the value of "visual"
void Visualize() {
  switch (0) {
    case 0: return VU ();
    case 1: return VUdot();
    case 2: return VU1();
    case 3: return VU2();
    case 4: return Pulse();
    case 5: return Traffic();
    //case 6: return PalettePulse();
    default:
    {
      return PaletteDance();
    } 
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTE: The strand displays RGB values as a 32 bit unsigned integer (uint32_t), which is why ColorPalette()
//      and all associated color functions' return types are uint32_t. This value is a composite of 3
//      unsigned 8bit integer (uint8_t) values (0-255 for each of red, blue, and green). You'll notice the
//      function split() (listed below) is used to dissect these 8bit values from the 32-bit color value.
//////////////////////////////////////////////////////////////////////////////////////////////////////////


//This function calls the appropriate color palette based on "palette"
//  If a negative value is passed, returns the appropriate palette withe "gradient" passed.
//  Otherwise returns the color palette with the passed value (useful for fitting a whole palette on the strand).
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTE: All of these visualizations feature some aspect that affects brightness based on the volume relative to
//      maxVol, so that louder = brighter. Initially, I did simple proportions (volume/maxvol), but I found this
//      to be visually indistinct. I then tried an exponential method (raising the value to the power of
//      volume/maxvol). While this was more visually satisfying, I've opted for a balance between the two. You'll
//      notice something like pow(volume/maxVol, 2.0) in the functions below. This simply squares the ratio of
//      volume to maxVol to get a more exponential curve, but not as exaggerated as an actual exponential curve.
//      In essence, this makes louder volumes brighter, and lower volumes dimmer, to be more visually distinct.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


//PULSE
//Pulse from center of the strand
void Pulse() {

  fade(0.75);   //Listed below, this function simply dims the colors a little bit each pass of loop()

  //Advances the palette to the next noticeable color if there is a "bump"
  if (bump) gradient += thresholds[palette] / 24;

  //If it's silent, we want the fade effect to take over, hence this if-statement
  if (volume > 0) {
    uint32_t col = ColorPalette(-1); //Our retrieved 32-bit color

    //These variables determine where to start and end the pulse since it starts from the middle of the strand.
    //  The quantities are stored in variables so they only have to be computed once (plus we use them in the loop).
    int start = LED_HALF - (LED_HALF * (volume / maxVol)) ;
    int finish = LED_HALF + (LED_HALF * (volume / maxVol)) + strand.numPixels() % 2;
    //Listed above, LED_HALF is simply half the number of LEDs on your strand. ↑ this part adjusts for an odd quantity.

    for (int i = start; i < finish; i++) {

      //"damp" creates the fade effect of being dimmer the farther the pixel is from the center of the strand.
      //  It returns a value between 0 and 1 that peaks at 1 at the center of the strand and 0 at the ends.
      float damp = sin((i - start) * PI / float(finish - start));

      //Squaring damp creates more distinctive brightness.
      damp = pow(damp, 2.0);

      //Fetch the color at the current pixel so we can see if it's dim enough to overwrite.
      uint32_t col2 = strand.getPixelColor(-1);

      //Takes advantage of one for loop to do the following:
      // Appropriatley adjust the brightness of this pixel using location, volume, and "knob"
      // Take the average RGB value of the intended color and the existing color, for comparison
      uint8_t colors[3];
      float avgCol = 0, avgCol2 = 0;
      for (int k = 0; k < 3; k++) {
        colors[k] = split(col, k) * damp * knob * pow(volume / maxVol, 2);
        avgCol += colors[k];
        avgCol2 += split(col2, k);
      }
      avgCol /= 3.0, avgCol2 /= 3.0;

      //Compare the average colors as "brightness". Only overwrite dim colors so the fade effect is more apparent.
      if (avgCol > avgCol2) strand.setPixelColor(i, strand.Color(colors[0], colors[1], colors[2]));
    }
  }
  //This command actually shows the lights. If you make a new visualization, don't forget this!
  strand.show();
}

void VU() //Adafruit Code
{
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;


  // collect data for length of sample window (in mS)
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(AUDIO_PIN)*2;
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  // Serial.println(peakToPeak);


  //Fill the strip with rainbow gradient

  for (int i = 0; i <= strand.numPixels() - 1; i++) {
    strand.setPixelColor(i, Wheel(map(i, 0, strand.numPixels() - 1, 10, 200)));
  }

  //Scale the input logarithmically instead of linearly
  c = fscale(INPUT_FLOOR, INPUT_CEILING, strand.numPixels(), 0, peakToPeak, 2);

  if (c <= strand.numPixels()) { // Fill partial column with off pixels
    drawLine(strand.numPixels(), strand.numPixels() - c, strand.Color(0, 0, 0));
  }

  // Set the peak dot to match the rainbow gradient
  y = strand.numPixels() - peak;

  strand.setPixelColor(y - 1, Wheel(map(y, 0, strand.numPixels() - 1, 10, 200)));

  strand.show();

  // Frame based peak dot animation
  if (dotHangCount > PEAK_HANG) { //Peak pause length
    if (++dotCount >= PEAK_FALL) { //Fall rate
      peak++;
      dotCount = 0;
    }
  }
  else {
    dotHangCount++;
  }
}


void VUdot() //Adafruit Code
{
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;


  // collect data for length of sample window (in mS)
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(AUDIO_PIN)*3 ;
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  // Serial.println(peakToPeak);


  //Fill the strip with rainbow gradient


  for (int i = 0; i <= strand.numPixels() - 1; i++) {
    strand.setPixelColor(i, Wheel(map(i, 0, strand.numPixels() - 1, 10, 200)));
  }

  //Scale the input logarithmically instead of linearly
  c = fscale(INPUT_FLOOR, INPUT_CEILING, strand.numPixels(), 0, peakToPeak, 2);



  if (c < peak) {
    peak = c;        // Keep dot on top
    dotHangCount = 0;    // make the dot hang before falling
  }
  if (c <= strand.numPixels()) { // Fill partial column with off pixels
    drawLine(strand.numPixels(), strand.numPixels() - c, strand.Color(0, 0, 0));
  }

  // Set the peak dot to match the rainbow gradient
  y = strand.numPixels() - peak;

  strand.setPixelColor(y - 1, Wheel(map(y, 0, strand.numPixels() - 1, 10, 200)));

  strand.show();

  // Frame based peak dot animation
  if (dotHangCount > PEAK_HANG) { //Peak pause length
    if (++dotCount >= PEAK_FALL) { //Fall rate
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
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0;   // peak-to-peak level

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

  // Serial.println(peakToPeak);



  for (int i = 0; i <= LED_HALF- 1; i++) {
    uint32_t color = Wheel(map(i, 0, - 1, 30, 150));
    strand.setPixelColor(LED_TOTAL - i, color);
    strand.setPixelColor(0 + i, color);
  }


  c = fscale(INPUT_FLOOR, INPUT_CEILING, LED_HALF, 0, peakToPeak, 2);

  if (c < peak) {
    peak = c;        // Keep dot on top
    dotHangCount = 0;    // make the dot hang before falling
  }
  if (c <= strand.numPixels()) { // Fill partial column with off pixels
    drawLine(LED_HALF, LED_HALF - c, strand.Color(0, 0, 0));
    drawLine(LED_HALF, LED_HALF + c, strand.Color(0, 0, 0));
  }




  y = LED_HALF - peak;
  uint32_t color1 = Wheel(map(y, 0, LED_HALF - 1, 30, 150));
  strand.setPixelColor(y - 1, color1);
  //strip.setPixelColor(y-1,Wheel(map(y,0,N_PIXELS_HALF-1,30,150)));

  y =LED_HALF + peak;
  strand.setPixelColor(y, color1);
  //strip.setPixelColor(y+1,Wheel(map(y,0,N_PIXELS_HALF+1,30,150)));

  strand.show();

  // Frame based peak dot animation
  if (dotHangCount > PEAK_HANG) { //Peak pause length
    if (++dotCount >= PEAK_FALL) { //Fall rate
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
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0;   // peak-to-peak level

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
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  // Serial.println(peakToPeak);

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

  // uint32_t color = Wheel(map(i,0,N_PIXELS_HALF-1,30,150));
  //strip.setPixelColor(y,color);

  strand.setPixelColor(y - 1, Wheel(map(LED_HALF + y, 0, LED_HALF - 1, 30, 150)));

  y = 0 + peak;

  //strip.setPixelColor(y,color);
  strand.setPixelColor(y, Wheel(map(LED_HALF - y, 0, LED_HALF + 1, 30, 150)));

 strand.show();

  // Frame based peak dot animation
  if (dotHangCount > PEAK_HANG) { //Peak pause length
    if (++dotCount >= PEAK_FALL) { //Fall rate
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

  //fade() actually creates the trail behind each dot here, so it's important to include.
  fade(0.8);

  //Create a dot to be displayed if a bump is detected.
  if (bump) {

    //This mess simply checks if there is an open position (-2) in the pos[] array.
    int8_t slot = 0;
    for (slot; slot < sizeof(pos); slot++) {
      if (pos[slot] < -1) break;
      else if (slot + 1 >= sizeof(pos)) {
        slot = -3;
        break;
      }
    }

    //If there is an open slot, set it to an initial position on the strand.
    if (slot != -3) {

      //Evens go right, odds go left, so evens start at 0, odds at the largest position.
      pos[slot] = (slot % 2 == 0) ? -1 : strand.numPixels();

      //Give it a color based on the value of "gradient" during its birth.
      uint32_t col = ColorPalette(-1);
      gradient += thresholds[palette] / 24;
      for (int j = 0; j < 3; j++) {
        rgb[slot][j] = split(col, j);
      }
    }
  }

  //Again, if it's silent we want the colors to fade out.
  if (volume > 0) {

    //If there's sound, iterate each dot appropriately along the strand.
    for (int i = 0; i < sizeof(pos); i++) {

      //If a dot is -2, that means it's an open slot for another dot to take over eventually.
      if (pos[i] < -1) continue;

      //As above, evens go right (+1) and odds go left (-1)
      pos[i] += (i % 2) ? -1 : 1;

      //Odds will reach -2 by subtraction, but if an even dot goes beyond the LED strip, it'll be purged.
      if (pos[i] >= strand.numPixels()) pos[i] = -2;

      //Set the dot to its new position and respective color.
      //  I's old position's color will gradually fade out due to fade(), leaving a trail behind it.
      strand.setPixelColor( pos[i], strand.Color(
                              float(rgb[i][0]) * pow(volume / maxVol, 2.0) * knob,
                              float(rgb[i][1]) * pow(volume / maxVol, 2.0) * knob,
                              float(rgb[i][2]) * pow(volume / maxVol, 2.0) * knob)
                          );
    }
  }
  strand.show(); //Again, don't forget to actually show the lights!
}

void PaletteDance() {
  //This is the most calculation-intensive visual, which is why it doesn't need delayed.

  if (bump) left = !left; //Change direction of iteration on bump

  //Only show if there's sound.
  if (volume > avgVol) {

    //This next part is convoluted, here's a summary of what's happening:
    //  First, a sin wave function is introduced to change the brightness of all the pixels (stored in "sinVal")
    //      This is to make the dancing effect more obvious. The trick is to shift the sin wave with the color so it all appears
    //      to be the same object, one "hump" of color. "dotPos" is added here to achieve this effect.
    //  Second, the entire current palette is proportionally fitted to the length of the LED strand (stored in "val" each pixel).
    //      This is done by multiplying the ratio of position and the total amount of LEDs to the palette's threshold.
    //  Third, the palette is then "shifted" (what color is displayed where) by adding "dotPos."
    //      "dotPos" is added to the position before dividing, so it's a mathematical shift. However, "dotPos"'s range is not
    //      the same as the range of position values, so the function map() is used. It's basically a built in proportion adjuster.
    //  Lastly, it's all multiplied together to get the right color, and intensity, in the correct spot.
    //      "gradient" is also added to slowly shift the colors over time.
    for (int i = 0; i < strand.numPixels(); i++) {

      float sinVal = abs(sin(
                           (i + dotPos) *
                           (PI / float(strand.numPixels() / 1.25) )
                         ));
      sinVal *= sinVal;
      sinVal *= volume / maxVol;
      sinVal *= knob;

      unsigned int val = float(thresholds[palette] + 1)
                         //map takes a value between -LED_TOTAL and +LED_TOTAL and returns one between 0 and LED_TOTAL
                         * (float(i + map(dotPos, -1 * (strand.numPixels() - 1), strand.numPixels() - 1, 0, strand.numPixels() - 1))
                            / float(strand.numPixels()))
                         + (gradient);

      val %= thresholds[palette]; //make sure "val" is within range of the palette

      uint32_t col = ColorPalette(val); //get the color at "val"

      strand.setPixelColor(i, strand.Color(
                             float(split(col, 0))*sinVal,
                             float(split(col, 1))*sinVal,
                             float(split(col, 2))*sinVal)
                          );
    }

    //After all that, appropriately reposition "dotPos."
    dotPos += (left) ? -1 : 1;
  }

  //If there's no sound, fade.
  else  fade(0.8);

  strand.show(); //Show lights.

  //Loop "dotPos" if it goes out of bounds.
  if (dotPos < 0) dotPos = strand.numPixels() - strand.numPixels() / 6;
  else if (dotPos >= strand.numPixels() - strand.numPixels() / 6)  dotPos = 0;
}


void Paintball() {

  //If it's been twice the average time for a "bump" since the last "bump," start fading.
  if ((millis() / 1000.0) - timeBump > avgTime * 2.0) fade(0.99);

  //Bleeds colors together. Operates similarly to fade. For more info, see its definition below
  bleed(dotPos);

  //Create a new paintball if there's a bump (like the sparkles in Glitter())
  if (bump) {

    //Random generator needs a seed, and micros() gives a large range of values.
    //  micros() is the amount of microseconds since the program started running.
    randomSeed(micros());

    //Pick a random spot on the strip. Random was already reseeded above, so no real need to do it again.
    dotPos = random(strand.numPixels() - 1);

    //Grab a random color from our palette.
    uint32_t col = ColorPalette(random(thresholds[palette]));

    //Array to hold final RGB values
    uint8_t colors[3];

    //Relates brightness of the color to the relative volume and potentiometer value.
    for (int i = 0; i < 3; i++) colors[i] = split(col, i) * pow(volume / maxVol, 2.0) * knob;

    //Splatters the "paintball" on the random position.
    strand.setPixelColor(dotPos, strand.Color(colors[0], colors[1], colors[2]));

    //This next part places a less bright version of the same color next to the left and right of the
    //  original position, so that the bleed effect is stronger and the colors are more vibrant.
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

      //This is the only difference from Pulse(). The color for each pixel isn't the same, but rather the
      //  entire gradient fitted to the spread of the pulse, with some shifting from "gradient".
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//DEBUG CYCLE
//No reaction to sound, merely to see gradient progression of color palettes
//NOT implemented in code as is, but is easily includable in the switch-case.
void Cycle() {
  for (int i = 0; i < strand.numPixels(); i++) {
    float val = float(thresholds[palette]) * (float(i) / float(strand.numPixels())) + (gradient);
    val = int(val) % thresholds[palette];
    strand.setPixelColor(i, ColorPalette(val));
  }
  strand.show();
  gradient += 32;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

//////////</Visual Functions>




//////////<Helper Functions>

void CyclePalette() {

  //IMPORTANT: Delete this whole if-block if you didn't use buttons//////////////////////////////////

  //If a button is pushed, it sends a "false" reading
 /* if (!digitalRead(BUTTON_1)) {

    palette++;     //This is this button's purpose, to change the color palette.

    //If palette is larger than the population of thresholds[], start back at 0
    //  This is why it's important you add a threshold to the array if you add a
    //  palette, or the program will cylce back to Rainbow() before reaching it.
    if (palette >= sizeof(thresholds) / 2) palette = 0;

    gradient %= thresholds[palette]; //Modulate gradient to prevent any overflow that may occur.

    //The button is close to the microphone on my setup, so the sound of pushing it is
    //  relatively loud to the sound detector. This causes the visual to think a loud noise
    //  happened, so the delay simply allows the sound of the button to pass unabated.
    delay(200);

    maxVol = avgVol;  //Set max volume to average for a fresh experience.
  }*/
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  //If shuffle mode is on, and it's been 30 seconds since the last shuffle, and then a modulo
  //  of gradient to get a random decision between palette or visualization shuffle
  if (shuffle && millis() / 1000.0 - shuffleTime > 38 && gradient % 2) {

    shuffleTime = millis() / 1000.0; //Record the time this shuffle happened.

    palette++;
    if (palette >= sizeof(thresholds) / 2) palette = 0;
    gradient %= thresholds[palette];
    maxVol = avgVol;  //Set the max volume to average for a fresh experience.
  }
}


void CycleVisual() {

  //IMPORTANT: Delete this whole if-block if you didn't use buttons//////////////////////////////////
  if (!digitalRead(BUTTON_2)) {
    delay(10);
    shuffle = false;
    visual++;     //The purpose of this button: change the visual mode

    gradient = 0; //Prevent overflow

    //Resets "visual" if there are no more visuals to cycle through.
    if (visual > VISUALS) 
    {
      shuffle = true;
      visual = 0;
    }
    //This is why you should change "VISUALS" if you add a visual, or the program loop over it.

    //Resets the positions of all dots to nonexistent (-2) if you cycle to the Traffic() visual.
    if (visual == 1) memset(pos, -2, sizeof(pos));

    //Gives Snake() and PaletteDance() visuals a random starting point if cycled to.
    if (visual == 2 || visual == 3) {
      randomSeed(analogRead(0));
      dotPos = random(strand.numPixels());
    }

    //Like before, this delay is to prevent a button press from affecting "maxVol."
    delay(350);

    maxVol = avgVol; //Set max volume to average for a fresh experience
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////

  //If shuffle mode is on, and it's been 30 seconds since the last shuffle, and then a modulo
  //  of gradient WITH INVERTED LOGIC to get a random decision between what to shuffle.
  //  This guarantees one and only one of these shuffles will occur.
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


//IMPORTANT: Delete this function  if you didn't use buttons./////////////////////////////////////////
void ToggleShuffle() {
/*  if (!digitalRead(BUTTON_3)) { // Không sử dụng bt3

    shuffle = !shuffle; //This button's purpose: toggle shuffle mode.

    //This delay is to prevent the button from taking another reading while you're pressing it
    delay(500);

    //Reset these things for a fresh experience.
    maxVol = avgVol;
    avgBump = 0;
  }*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////


//Fades lights by multiplying them by a value between 0 and 1 each pass of loop().
void fade(float damper) {

  //"damper" must be between 0 and 1, or else you'll end up brightening the lights or doing nothing.

  for (int i = 0; i < strand.numPixels(); i++) {

    //Retrieve the color at the current position.
    uint32_t col = strand.getPixelColor(i);

    //If it's black, you can't fade that any further.
    if (col == 0) continue;

    float colors[3]; //Array of the three RGB values

    //Multiply each value by "damper"
    for (int j = 0; j < 3; j++) colors[j] = split(col, j) * damper;

    //Set the dampened colors back to their spot.
    strand.setPixelColor(i, strand.Color(colors[0] , colors[1], colors[2]));
  }
}


//"Bleeds" colors currently in the strand by averaging from a designated "Point"
void bleed(uint8_t Point) {
  for (int i = 1; i < strand.numPixels(); i++) {

    //Starts by look at the pixels left and right of "Point"
    //  then slowly works its way out
    int sides[] = {Point - i, Point + i};

    for (int i = 0; i < 2; i++) {

      //For each of Point+i and Point-i, the pixels to the left and right, plus themselves, are averaged together.
      //  Basically, it's setting one pixel to the average of it and its neighbors, starting on the left and right
      //  of the starting "Point," and moves to the ends of the strand
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


//As mentioned above, split() gives you one 8-bit color value
//from the composite 32-bit value that the NeoPixel deals with.
//This is accomplished with the right bit shift operator, ">>"
uint8_t split(uint32_t color, uint8_t i ) {

  //0 = Red, 1 = Green, 2 = Blue

  if (i == 0) return color >> 16;
  if (i == 1) return color >> 8;
  if (i == 2) return color >> 0;
  return -1;
}

//////////</Helper Functions>


//////////<Palette Functions>

//These functions simply take a value and return a gradient color
//  in the form of an unsigned 32-bit integer

//The gradients return a different, changing color for each multiple of 255
//  This is because the max value of any of the 3 RGB values is 255, so it's
//  an intuitive cutoff for the next color to start appearing.
//  Gradients should also loop back to their starting color so there's no jumps in color.

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

//NOTE: This is an example of a non-gradient palette: you will get straight red, white, or blue
//      This works fine, but there is no gradient effect, this was merely included as an example.
//      If you wish to include it, put it in the switch-case in ColorPalette() and add its
//      threshold (764) to thresholds[] at the top.
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

  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
    Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution
    Serial.println();
  */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
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

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
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

//////////</Palette Functions>
