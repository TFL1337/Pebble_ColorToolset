#include <pebble.h>
// Only working in basalt
  
static Window *my_window;
static TextLayer *text_layer;

// The unique number of a color
// number = [0..63]
static int number;

// The possible properties of a single color value in RGB
// rgbValues[0..3] = {0, 85, 170, 255}
static short int rgbValues [4] = {0, 85, 170, 255};

// The color for the text_layer (frontcolor)
// rgbReturnValues [0..2] = {[0..3], [0..3], [0..3]}
// This have to be mapped through rgbValues to derive the numbers from 0 to 255
// red   =  rgbValues[rgbReturnValues[0]];
// green =  rgbValues[rgbReturnValues[1]];
// blue  =  rgbValues[rgbReturnValues[2]];
static short int rgbReturnValues [3] = {0, 0, 0};

// The color for the my_window (backgroundcolor)
static short int rgbComplementaryReturnValues [3] = {3, 3, 3};

// Calculation of a unique color per number
// using a function for transformation of decimal numbers to quaternary numbers
// e.g. 0 equivalent to 000 and 63 equivalent to 333
// result is written to rgbReturnValues
static void calculateQuaternary (const int internalNumber) {
  short int calculationNumber = 0;
  // some error handling for the input number of the function
  if ((internalNumber > -1) && (internalNumber < 64))
  {
	  calculationNumber = internalNumber;
  }  
  // main calculation
  for (int i = 0; i < 3; i++)
  {
    rgbReturnValues[i] = calculationNumber % 4;
    calculationNumber = calculationNumber / 4;
  }
} 

// Derive a complementary color from a quaternary number
// result is writen to rgbComplementaryReturnValues
static void deriveComplementaryColor (const short int color [3]) {
  for (int i = 0; i < 3; i++)
  {
    rgbComplementaryReturnValues[i] = 3 - color[i];
  }  
} 

// Calculation of a complementary color from a decimal number
// return to rgbComplementaryReturnValues
static void calculateComplementaryColor (const int internalNumber) {
  short int calculationNumber = 0;
  if ((internalNumber > -1) && (internalNumber < 64))
  {
	  calculationNumber = internalNumber;
  }  
  for (int i = 0; i < 3; i++)
  {
    rgbComplementaryReturnValues[i] = 3 - calculationNumber % 4;
    calculationNumber = calculationNumber / 4;
  }
} 

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char buffer[] = "00:00";
  // write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // increasing the number  
  if (number < 63)
    {number++;}
  else {number = 0;}
  
  // calculate the quaternary -> result rgbReturnValues
  calculateQuaternary(number);
  
  // calculate the complementary -> result rgbComplementaryReturnValues
  calculateComplementaryColor(number);
  
  // or you use the deriving function instead of line 88
  // deriveComplementaryColor(rgbReturnValues);
  
  // set the color of text_layer
  text_layer_set_text_color(text_layer, GColorFromRGB(rgbValues[rgbReturnValues[0]], rgbValues[rgbReturnValues[1]], rgbValues[rgbReturnValues[2]]));
  
   // set the color of my_window
  window_set_background_color(my_window, GColorFromRGB(rgbValues[rgbComplementaryReturnValues[0]], rgbValues[rgbComplementaryReturnValues[1]], rgbValues[rgbComplementaryReturnValues[2]]));
  
  // set time from buffer 
  text_layer_set_text(text_layer, buffer);  
}  

void handle_init(void) {
  my_window = window_create();
  
  // set the tick_timer_service
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  
  // set the first number
  number = 0;
  
  // create the layout of the app  
  window_set_background_color(my_window, GColorFromRGB(255, 255, 255)); 
  text_layer = text_layer_create(GRect(0, 115, 144, 50));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorFromRGB(0, 0, 0));
  text_layer_set_text(text_layer, "00:00");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);  
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_layer));  
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
  text_layer_destroy(text_layer);
  window_destroy(my_window);    
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
