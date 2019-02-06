#define X_AXIS_PIN    A0  /*----(Joystick Pin connections)----*/
#define Y_AXIS_PIN    A1
#define Z_SWITCH_PIN  2

#define LED_PIN13    13  /* The on-board Pin 13 LED  */

/*-----( Declare Variables )-----*/

int  Xvalue; /*----(Values we will read from the Joystick)----*/
int  Yvalue;
int  Zvalue;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  /*----(Set up Serial Monitor Output)----*/
  Serial.println("YourDuino Joystick Test 1.02 - - 03/23/2015 -");
    Serial.println("Hold Joystick with cable to the left .");
  pinMode(LED_PIN13, OUTPUT); // Set the mode of these pins
  pinMode(Z_SWITCH_PIN, INPUT_PULLUP); // Use built-in pullup resistor

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  Xvalue = analogRead (X_AXIS_PIN); /*----(Read the X value and display it)----*/
  Serial.print ( "X " );
  Serial.print (Xvalue, DEC );

  Yvalue = analogRead (Y_AXIS_PIN);/*----(Read the Y value and display it)----*/
  Yvalue = 1023 - Yvalue;   // Reverse the numbers so it "looks right" 
  Serial.print ( " | Y " );
  Serial.print (Yvalue, DEC );

  Zvalue = digitalRead (Z_SWITCH_PIN);/*----(Read the Z value and display it)----*/
  Serial.print ( " | Z " );
  Serial.print (Zvalue, DEC );
  
  Serial.println (" ");  /*----(End of Line)----*/  
  
  if (Zvalue == 0) //The knob is pushed down
  {
    digitalWrite(LED_PIN13,HIGH);
  }
  else
  {
    digitalWrite(LED_PIN13,LOW);    
  }

  delay(250);  /*----(Wait to read the print)----*/
}/* --(end main loop )-- */

/* ( THE END ) */
