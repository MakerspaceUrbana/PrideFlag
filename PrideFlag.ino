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
#define NUM_LEDS 149
#define TWINKLE_THRESHOLD 1
#define TWINKLE_PERIOD_MAX 100
#define TWINKLESTART 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
int brightness = 25;
boolean countingup = true;

typedef struct {
	uint8_t brightness;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

typedef struct {
	uint16_t period : 8;
	uint16_t step : 8;
} Twinkle;

Color *setBrightness(uint8_t brightness, Color *color);
void setPixel(int n, Color *color);
void setstrip(int row, int number, Color *color);

Color _red     = {255, 255,   0,   0};
Color _orange  = {255, 255,  60,   0};
Color _yellow  = {255, 255, 188,   0};
Color _green   = {255,   0, 255,   0};
Color _blue    = {255,   0,   0, 255};
Color _purple  = {255, 200,  40, 255};
Color _off     = {  0,   0,   0,   0};
Color _tmp;

#define RED    &_red
#define ORANGE &_orange
#define YELLOW &_yellow
#define GREEN  &_green
#define BLUE   &_blue
#define PURPLE &_purple
#define OFF    &_off


Color *color_row[6] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
Twinkle _timings[NUM_LEDS];

Color *setBrightness(uint8_t brightness, Color *color) {
	_tmp.brightness = brightness;
	_tmp.red = color->red;
	_tmp.green = color->green;
	_tmp.blue = color->blue;
	return &_tmp;
}

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

int num_to_row(int n) {
	if(n < 25) return 6;
	if(n < 50) return 5;
	if(n < 75) return 4;
	if(n < 100) return 3;
	if(n < 125) return 2;
	return 1;
}

void setPixel(int n, Color *color) {
	// brightness calc magic - based off the NeoPixel lib
	strip.setPixelColor(n,
			((color->red   * color->brightness) >> 8),
			((color->green * color->brightness) >> 8),
			((color->blue  * color->brightness) >> 8));
}

void setstrip(int row, int number, Color *color) {
	for (int i=0; i < number; i++) {
		setPixel(coord(row, i), color);
	}
	strip.show();
}

void clear() {
	for (int i=0; i<NUM_LEDS; i++){
		setPixel(i, OFF);
	}
	strip.show();
}

void setup() {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() {
	fade_in(0, TWINKLESTART);
	twinkle(1000);
	alt_tetris_in();
	delay(1000);
	fade_out(255);
	delay(500);
}

void alt_tetris_in() {
	for (int i=6; i>0; i--) {
		for (int j=1; j<=i; j++) {
			setstrip(j, 25, color_row[i-1]);
			delay(100);
			setstrip(j, 25, OFF);
		}
		setstrip(i, 25, color_row[i-1]);
	}
}


int twinkle_period() {
  int maximum = TWINKLE_PERIOD_MAX;
  int minimum = TWINKLE_PERIOD_MAX/2;
  return random(minimum,maximum);
}

void start_one_twink() {
	while (1) {
		int row = random(1, 7);
		int col = random(1,25);
		int target = coord(row, col);
		if (_timings[target].period == 0) {
			_timings[target].period = twinkle_period();
			_timings[target].step = 0;
			break;
		}
	}
}

void twinkle(int n){
	solid(TWINKLESTART);
	//reset twinkles
	for (int i = 0; i < NUM_LEDS; i++) {
		_timings[i].period = 0;
		_timings[i].step = 0;
	}
	while(n >0) {
		if (random(1, 10) > TWINKLE_THRESHOLD) {
			start_one_twink();
		}
		for (int i=0; i<NUM_LEDS; i++) {
			if(_timings[i].period == 0) {
				continue;
			} else if (_timings[i].period == _timings[i].step) {
				setPixel(i, setBrightness(TWINKLESTART,color_row[num_to_row(i) - 1]));
				_timings[i].period = 0;
			} else {
				int adjust;
				double dstep = (double)(_timings[i].step);
				double dperiod = (double)(_timings[i].period);
				double bright_percent = dstep/dperiod;
				bright_percent = sin(bright_percent * -2 * PI);
				if (bright_percent < 0) {
					adjust = (int) ((double)TWINKLESTART * bright_percent);
					_timings[i].step+=2;
				} else {
					adjust = (int) ((double)(255-TWINKLESTART) * bright_percent);
					_timings[i].step++;
				}
				setPixel(i, setBrightness(TWINKLESTART + adjust,
								color_row[num_to_row(i) -1]));
			}
		}
		strip.show();
		delay(10);
		n--;
	}
	fade_out(TWINKLESTART);
}

void solid(uint8_t bright) {
	for(int i=1; i <= 6; i++) {
		setstrip(i, 25, setBrightness(bright, color_row[i-1]));
	}
}

void fade_LtR() {
	for(uint8_t i=0; i < 25 ; i++)
	{
		setPixel(coord(1,i), setBrightness(i * 10, RED));
		setPixel(coord(2,i), setBrightness(i * 10, ORANGE));
		setPixel(coord(3,i), setBrightness(i * 10, YELLOW));
		setPixel(coord(4,i), setBrightness(i * 10, GREEN));
		setPixel(coord(5,i), setBrightness(i * 10, BLUE));
		setPixel(coord(6,i), setBrightness(i * 10, PURPLE));
	}
	strip.show();
}

void fade_out(uint8_t b) {
	while (b > 0) {
		solid(b);
		b--;
		delay(15);
	}
}
void fade_in(uint8_t start, uint8_t end) {
	for(int i=start; i < end; i++) {
		solid(i);
		delay(15);
	}
}
