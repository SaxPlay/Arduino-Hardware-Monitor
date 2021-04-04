#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  //SPI oled display library

// SPI OLED STUFF HERE 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
// END SPI OLED STUFF 

int Y=0;                  //For OLED cursor positioning
int MaxCPUtemp =0;        //Will store maximun CPU temp
int MaxCPUuse=0;          //Will store maximun CPU Usage
int MaxGPUtemp =0;        //Will store maximun GPU temp
int MaxGPUuse =0;         //Will store maximun GPU Usage
String SerialString = ""; //Will store de data from the serial port. 
String SubStr = "";
void setup()  
{   
  // Check for the OLED display
  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // END Oled stuff
  
  display.clearDisplay();             // Clear display 
  display.setTextColor(WHITE, BLACK); //(background color, foreground color) This fill the background with black to erase the previous value.
  display.setTextSize(1);
  display.setCursor(0,0);             // (X,Y) in pixels
  display.println(F("PC HARDWARE MONITOR"));
  //Let's show the static information
  for (Y=16; Y <= 56; Y+=10) {
    display.setCursor(0,Y);
    switch(Y) {
      case 16:
        display.println(F("CPU TEMP. :"));
        break;
      case 26:
        display.println(F("CPU USAGE :"));
        break;
      case 36:
        display.println(F("GPU TEMP. :"));
        break; 
      case 46:
        display.println(F("GPU USAGE :"));
        break;
      case 56:
        display.println(F("FREE RAM  :"));
        break;
    }
  }
  display.display(); //Shows all the above
  Serial.begin(9600);//you can raise this up to 57600 in Nano and 115200 in Uno. (but there is no need for that)
}  
   
void loop()  
{   
  if(Serial.available() > 0)  //Are there any characters to read from the serial port?
  {          
    SerialString = Serial.readString(); //Read data from the serial port
    if(SerialString != "")   {          //Is there any data?
      //Let's show the dynamic information
      for (Y=16; Y <= 56; Y+=10) {      
        display.setCursor(67,Y);
        switch(Y) {
          case 16:
            SubStr = SerialString.substring(0, 3);  //SubStr stores the current value (if you have any doubt about how, please read the comment at the end)
            if (SubStr.toInt() > MaxCPUtemp)        //If the current value is greater than the previous value 
              MaxCPUtemp = SubStr.toInt();          //I assign the current value to the variable that stores the maximum value
            display.println(SubStr+" C  "+String(MaxCPUtemp));
            break;
          case 26:
            SubStr = SerialString.substring(4, 7);
            if (SubStr.toInt() > MaxCPUuse)
              MaxCPUuse = SubStr.toInt();
            display.println(SubStr+" %  "+String(MaxCPUuse));
            break;
          case 36:
            SubStr = SerialString.substring(8, 11);
            if (SubStr.toInt() > MaxGPUtemp)
              MaxGPUtemp = SubStr.toInt();
            display.println(SubStr+" C  "+String(MaxGPUtemp));
            break; 
          case 46:
            SubStr = SerialString.substring(12, 15);
            if (SubStr.toInt() > MaxGPUuse)
              MaxGPUuse = SubStr.toInt();
            display.println(SubStr+" %  "+String(MaxGPUuse));
            break;
          case 56:
            display.println(SerialString.substring(16, 20)+"GB");
            break;
        }
      }
      display.display(); //Shows all the above
    }
  }   
  delay(50);  //let's give the display some time to refresh the information
}

/*
I am sending the different values from C# in concatenated blocks of four characters each, this is to make it simpler to differentiate each value.
The first block of four characters contains the CPU usage, then using SubStr.substring(0, 3) I assign those four characters to the variable SubStr, the next four with SubStr.substring(4, 7) and so on
For more information on how substring() works please follow the link below.
https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/substring/
 */
