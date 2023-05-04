//------------ OLED Display --------------------
#include <SSD1306Wire.h>
#include <string>
//#include "einlesen.h"
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  


#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height
#define MEAN_FOR_DISPLAY 4 
#define ADC_RESOLUTION 1024 // Resolution of ADC ESP
int avantiPixelX = 0;


int16_t mean(int n) // The function returns the mean of the last n values in the buffer
{
    if(n > BufferSize) { // If n is greater than the size of the buffer, the function returns 0
        return 0;
    }
    auto curIndex = bufferIndex; // The variable curIndex is the index of the last value in the buffer
    auto sumIndex = curIndex; // The variable sumIndex is the index of the value before the last value in the buffer
    int sum = 0; // The variable sum is the sum of the last n values in the buffer
    for(int i = 0; i < n; i++) // The loop iterates n times
    {
        sumIndex = curIndex-i; // The variable sumIndex is the index of the value before the last value in the buffer
        if(sumIndex >= 0) // If sumIndex is greater than or equal to 0
            sum += bufferFilteredForDisplay[sumIndex]; // The variable sum is increased by the value at the index of sumIndex
        else // If sumIndex is less than 0
            sum += bufferFilteredForDisplay[sumIndex]; // The variable sum is increased by the value at the index of+sumIndex
    }
    return sum/n; // The function returns the mean of the last n values in the buffer
}

inline void calc_value_for_display(int &y)
{

  int Signal = mean(MEAN_FOR_DISPLAY); // Filter etc.
  y =  Signal*(SCREEN_HEIGHT-1)/ADC_RESOLUTION; // Signal*y_max/Signal_max

  y = y-SCREEN_HEIGHT; 
  y = SCREEN_HEIGHT-1-y; // Since OLED coordinates go from top to bottom y_max-y

}



inline void update_OLED() // current pixel position calculate
{    
  static int oldy = 0;
  if (avantiPixelX == 0) 
    {
      display.clear();//clear Display
      calc_value_for_display(oldy);
      display.setPixel(avantiPixelX, oldy); // draw current Point
    }else if(avantiPixelX > 0){
      int cury = 0;
      calc_value_for_display(cury);
      display.drawLine(avantiPixelX-1, oldy, avantiPixelX, cury);
      oldy = cury;
    }
    display.display();//display it
    avantiPixelX++;//next x
    avantiPixelX = avantiPixelX % SCREEN_WIDTH;
  
}