#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

//#define NUM_LEDS 150
#define NUM_LEDS 150
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
int brightness = 25;
boolean countingup = true;

#define ROW1(n) (150-(n+1))
#define ROW2(n) (n+100)
#define ROW3(n) (100 - (n+1))
#define ROW4(n) (n+50)
#define ROW5(n) (50 - (n+1))
#define ROW6(n) (n+0)


#define RED    255,0,0
#define ORANGE 255,60,0
#define YELLOW 255,188,0
#define GREEN  0,255,0
#define BLUE   0,0,255
#define PURPLE 200,40,255


uint8_t _brightnesses[NUM_LEDS];
int16_t _timings[NUM_LEDS];

int coord(int row, int col) {
	int res = 0;
	switch(row) {
		case 1:
			res = 150 - (col+1);
			break;
		case 2:
			res = (col+100);
			break;
		case 3:
			res = (100 - (col+1));
			break;
		case 4:
			res = (col+50);
			break;
		case 5:
			res = 50 - (col+1);
			break;
		case 6:
			res = col+0;
			break;
		default:
			res = col+0;
	}
	//return (res) ? res -1 : res;
	return res;
}

void setBrightnessAndColor(int n, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
	// Stored brightness value is different than what's passed.
	// This simplifies the actual scaling math later, allowing a fast
	// 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
	// adding 1 here may (intentionally) roll over...so 0 = max brightness
	// (color values are interpreted literally; no scaling), 1 = min
	// brightness (off), 255 = just below max brightness.
	//uint8_t newBrightness = brightness + 1;
	// Brightness has changed -- re-scale existing data in RAM
	//uint8_t  c,
			 //oldBrightness = _brightnesses[n] - 1; // De-wrap old brightness value
	//uint16_t scale;


	//if(oldBrightness == 0) scale = 0; // Avoid /0
	//else if(brightness == 255) scale = 65535 / oldBrightness;
	//else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;

	strip.setPixelColor(n,
			((r * brightness) >> 8),
			((g * brightness) >> 8),
			((b * brightness) >> 8));
	_brightnesses[n] = brightness;
}

void setstrip(int row, int number, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
	for (int i=0; i < number; i++) {
		setBrightnessAndColor(coord(row, i), brightness, r, g, b);
	}
}

void clear() {
	for (int i=0; i<NUM_LEDS; i++){
		strip.setPixelColor(i, 0,0,0);
	}
}

void setup() {
	for (int i=0; i <NUM_LEDS; i++) {
		_brightnesses[i] = 0;
	}
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() {
	fade_LtR();
	strip.show();
	delay(5000);
	tetris_inb();
	delay(5000);
	clear();
	strip.show();
}
void tetris_in() {
	for (int j = 1; j <=6; j++) {
		setstrip(j,25, 200, PURPLE);
		strip.show();
		delay(100);
	}
	for (int j = 1; j <=5; j++) {
		setstrip(j,25, 200, BLUE);
		strip.show();
		delay(100);
	}
	for (int j = 1; j <=4; j++) {
		setstrip(j,25, 200, GREEN);
		strip.show();
		delay(100);
	}
	for (int j = 1; j <=3; j++) {
		setstrip(j,25, 200, YELLOW);
		strip.show();
		delay(100);
	}
	for (int j = 1; j <=2; j++) {
		setstrip(j,25, 200, ORANGE);
		strip.show();
		delay(100);
	}
	for (int j = 1; j <=1; j++) {
		setstrip(j,25, 200, RED);
		strip.show();
		delay(100);
	}
}
void tetris_inb() {
	for (int j = 1; j <=6; j++) {
		setstrip(j,25, 200, PURPLE);
		strip.show();
		delay(100);
		setstrip(j, 25, 0, 0, 0, 0);
	}
	setstrip(6,25, 200, PURPLE);
	for (int j = 1; j <=5; j++) {
		setstrip(j,25, 200, BLUE);
		strip.show();
		delay(100);
		setstrip(j, 25, 0, 0, 0, 0);
	}
	setstrip(5,25, 200, BLUE);
	for (int j = 1; j <=4; j++) {
		setstrip(j,25, 200, GREEN);
		strip.show();
		delay(100);
		setstrip(j, 25, 0, 0, 0, 0);
	}
	setstrip(4,25, 200, GREEN);
	for (int j = 1; j <=3; j++) {
		setstrip(j,25, 200, YELLOW);
		strip.show();
		delay(100);
		setstrip(j, 25, 0, 0, 0, 0);
	}
	setstrip(3,25, 200, YELLOW);
	for (int j = 1; j <=2; j++) {
		setstrip(j,25, 200, ORANGE);
		strip.show();
		delay(100);
		setstrip(j, 25, 0, 0, 0, 0);
	}
	setstrip(2,25, 200, ORANGE);
	for (int j = 1; j <=1; j++) {
		setstrip(j,25, 200, RED);
		strip.show();
		delay(100);
		setstrip(j, 25, 0, 0, 0, 0);
	}
	setstrip(1,25, 200, RED);
}

void fade_LtR() {
	for(uint8_t i=0; i < 25 ; i++)
	{
		setBrightnessAndColor(ROW1(i), i * 10, RED);
		setBrightnessAndColor(ROW2(i), i * 10, ORANGE);
		setBrightnessAndColor(ROW3(i), i * 10, YELLOW);
		setBrightnessAndColor(ROW4(i), i * 10, GREEN);
		setBrightnessAndColor(ROW5(i), i * 10, BLUE);
		setBrightnessAndColor(ROW6(i), i * 10, PURPLE);
	}
}
