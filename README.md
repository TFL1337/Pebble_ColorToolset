# Pebble_ColorToolset
## Description
A first toolset (or collection of functions) for the work with the Pebble Time - in particular with colors. Colors in SDK of the Pebble Time have constant names (e.g. `GColorPastelYellow` which is equal to `GColorFromRGB(255, 255, 170)` or `GColorFromHEX(0xFFFFAA)`).
## Problem & Solution
But sometimes it could be great to call one of the 64 colors just with a number between 0 and 63. Especially if you would like to rotate through the colors within your application. A solution could be to transfer the numbers from the decimal system to the quaternary system. This has the advantage that every decimal number from `0` to `63` is equivalent to a unique quaternary number between `000` and `333`. And this quaternary number tuple you can simple transfer to a RGB color code.

```c
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
```

## Demo
In the demo source code you also find two solutions for the calculation of complementary colors.

[![IMAGE ALT TEXT HERE](http://tobias.langkau.biz/pebble/time/colortoolset/colortoolset.png)](https://youtu.be/OyGMIi-MOBY)



