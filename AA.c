
char  keypadPort at PORTB;
// End Keypad module connections
// LCD module connections
sbit LCD_RS at LATC4_bit;
sbit LCD_EN at LATC5_bit;
sbit LCD_D4 at LATC0_bit;
sbit LCD_D5 at LATC1_bit;
sbit LCD_D6 at LATC2_bit;
sbit LCD_D7 at LATC3_bit;

sbit LCD_RS_Direction at TRISC4_bit;
sbit LCD_EN_Direction at TRISC5_bit;
sbit LCD_D4_Direction at TRISC0_bit;
sbit LCD_D5_Direction at TRISC1_bit;
sbit LCD_D6_Direction at TRISC2_bit;
sbit LCD_D7_Direction at TRISC3_bit;
// End LCD module connections
#define HEATER PORTD.RD0
#define FAN PORTD.RD1
#define LED PORTD.RD3
#define ENTER 15
#define CLEAR 13
#define ON 1
#define OFF 0
void main() {
  unsigned short kp,Txt[14];
  unsigned int Temp_Ref;      //  Reference Temperature
  unsigned char inTemp;
  unsigned int temp;
  float mV, ActualTemp;

  ANSELC = 0;                 // Configure PORTC as digital I/O
  ANSELB = 0;                 // Configure PORTB as digital I/O
  ANSELD = 0;                 // Configure PORTD as digital I/O
  TRISA0_bit = 1;              //Configure AN0 (RA0) as input
  TRISC = 0;                  //PORTC are outputs (LCD)
  TRISD0_bit=0;               //RD0 is output (Heater)
  TRISD1_bit=0;               //RD1 is output (Fan)
  TRISD3_bit=0;               //RD3 is output (LED)
  TRISB3_bit=0;               //RB3 is output (Unconnected)
  LATB.B3=0;                 //Drive low RB3

  Keypad_Init();               // Initialize Keypad
  Lcd_Init();                  // Initialize LCD
  Sound_Init(&PORTD, 2);     // Initialize the pin RD2 for playing sound

  Lcd_Cmd(_LCD_CLEAR);                     // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);                // Cursor off
  Lcd_Out(1, 2, "--automatic--");
  Lcd_Out(2, 1, "--TempControl--");
  delay_ms(2000);                          //2s delay

  HEATER = OFF;
  FAN = OFF;

 //ON startup, read the Referance Temperature from EEPROM or the Keypad
 //Read Temp Ref from EEPROM
        Temp_Ref= EEPROM_Read(0x02); // Read data from EEPROM address 2 and store it in Temp_Ref variable
    if ((Temp_Ref > 0) & (Temp_Ref < 100))
    {
      goto START_PROGRAM ;
    }
    else {

        START:
       Lcd_Cmd(_LCD_CLEAR);                     // Clear display
       Lcd_Out(1, 1, "Enter Temp Ref");
       Temp_Ref=0;
        Lcd_Out(2, 1, "Temp Ref: ");
        while(1)
        {
        //Setup new Temp Ref
         do
         kp = Keypad_Key_Click();      // Store key code in kp variable
         while (!kp);
         if ( kp == ENTER )break;
         if (kp > 3 && kp < 8) kp = kp-1;
         if (kp > 8 && kp < 12) kp = kp-2;
         if (kp ==14)kp = 0;
         if ( kp == CLEAR )goto START;
         Lcd_Chr_Cp(kp + '0');
         Temp_Ref =(10*Temp_Ref) + kp;
    }

       }

      Lcd_Cmd(_LCD_CLEAR);          // Clear display
      Lcd_Out(1, 1, "Temp Ref: ");
      intToStr( Temp_Ref,Txt);      //Convert to String
      inTemp=Ltrim(Txt);
      Lcd_Out_CP(inTemp);          //Display Ref Temp

      EEPROM_Write(0x02,Temp_Ref); // Write Temp_Ref at address 2 of EEPROM

      Lcd_Out(2, 1, "Press # to Cont.");
      //Wait until # is pressed
      kp =0;
      while(kp!=ENTER)
      {
         do
           kp = Keypad_Key_Click();  // Store key code in kp variable
         while(!kp);
      }

      START_PROGRAM:

   Lcd_Cmd(_LCD_CLEAR);          // Clear display
   Lcd_Out(1, 1, "Temp Ref: ");
   Lcd_Chr(1,15,223);               // Different LCD displays have different                             //   char code for degree
  Lcd_Chr(1,16,'C');                // Display "C" for Celsius

  //Program loop
  while(1) {
    //Display Referance Temperature and Actual Temperature
     temp = ADC_Read(0);               //Read temperature from AN0
     mV = temp * 5000.0/1023.0;        //Convert to mV
     ActualTemp = mV/10.0 ;             // Convert to degrees Celcius
     intToStr( Temp_Ref,Txt);         //Convert to String
     inTemp=Ltrim(Txt);
     //Lcd_Out(1, 1, "Temp Ref: ");
     Lcd_Out(1, 11, inTemp);        //Display Ref Temp
     Lcd_Out(2, 1, "NOW_Temp=");
     FloatToStr(ActualTemp,Txt);    //Convert to string
     Txt[4] = 0;
     Lcd_Out(2,10,Txt);
     Lcd_Chr(2,15,223);   // char ° in lcd
     Lcd_Chr(2,16,'C');
     //Lcd_Out(2,14," ");
     //Compare ref temp with actual emp
      if (Temp_Ref >  ActualTemp)  //If Temp Ref is greater than actual Temp, Switch ON Heater
      {
      HEATER = ON,
      FAN = OFF;
      }
       if (Temp_Ref <  ActualTemp)  //If Temp Ref is less than actual Temp, Switch ON Fan
      {
         HEATER = OFF,
          FAN = ON;
      }
      if (Temp_Ref ==  ActualTemp)  //If Temp Ref is equal to actual Temp, Switch OFF Fan and Heater
      {
         HEATER = OFF,
        FAN = OFF;
      }
        //Check if the '*' key is pressed

         kp = Keypad_Key_Press(); // Store key code in kp variable
         if ( kp == CLEAR )
         {
         delay_ms(3000);          //3s delay
         kp = Keypad_Key_Press(); // Store key code in kp variable
         if ( kp == CLEAR )
         {
         goto START;
         }
         }
       //Sound the buzzer
      if (ActualTemp >=  40)  //If Temp reaches critical temperature of 40.
      {
       Sound_Play(880, 300); // Play sound at 880Hz for 300ms

      LED = ~LED ;         //Blink LED
      delay_ms(200);          //200ms delay
      }
      else
      {
      LED = OFF;
      }
  }
}