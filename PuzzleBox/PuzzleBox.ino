/*
The code used for the Puzzle Box product
Links used in the making of this code:
http://tronixstuff.wordpress.com
http://www.electrojoystick.com/tutorial/?page_id=15
http://www.instructables.com/id/Play-the-French-Can-Can-Using-an-Arduino-and-Buzze/
http://arduino.cc/en/Tutorial/AnalogInput
http://www.instructables.com/id/RGB-LED-Tutorial-using-an-Arduino-RGBL/
 */
int latchpin = 8; // connect to pin 12 on the '595
int clockpin = 12; // connect to pin 11 on the '595
int datapin = 11; // connect to pin 14 on the '595
int buzzerpin = 9;
int sensorPin = 4;
int aa = 30; //time to wait for correct color input
float b = 0;
int c = 0;
float d = 0;
int e = 0; //b through e all control the numbers on the LED
int z = 99; //Starting number for countdown
int sensorValue = 0; //photocell input
long interval = 1000; // used to control speed of counting
long interval2 = 100; // used to control speed of counting w/o dec
long previousMillis = 0;
long previousMillis2 = 0;
int segdisp[10] = { //with decimal
 2,158,36,12,152,72,64,26,0,8 };
int segdisp2[10] = { //without decimal
 3,159,37,13,153,73,65,27,1,9 };
int cont = 0; //0 = normal playmode, 1 = game lost, 2 = game won, 3 = box moved
long randRed;
long randBlue;
long randGreen;
int t = 75; //The threshhold for the LED solution
// Init the Pins used for PWM
const int redPin = 6;
const int greenPin = 3;
const int bluePin = 5;
// Init the Pins used for 10K pots
const int redPotPin = 0;
const int greenPotPin = 1;
const int bluePotPin = 2;
// Init our Vars
int red;
int green;
int blue;

void setup()
{
 pinMode(latchpin, OUTPUT);
 pinMode(clockpin, OUTPUT);
 pinMode(datapin, OUTPUT);
 pinMode(buzzerpin, OUTPUT);
 pinMode(redPin, OUTPUT);
 pinMode(greenPin, OUTPUT);
 pinMode(bluePin, OUTPUT);
 pinMode(sensorPin, OUTPUT);
 digitalWrite(latchpin, LOW);
 shiftOut(datapin, clockpin, LSBFIRST, 0); // clears the right display
 shiftOut(datapin, clockpin, LSBFIRST, 0); // clears the left display
 digitalWrite(latchpin, HIGH);
 randomSeed(analogRead(4));
 randRed = random(255);
 randBlue = random(255);
 randGreen = random(255);// all the randColors randomize the solution for the LED
 Serial.begin(9600);
 Serial.println(randRed);
 Serial.println(randBlue);
 Serial.println(randGreen);
}
void loop()
{
 //  Count down
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();
if(cont == 0)
{
  // Read the voltage on each analog pin then scale down to 0-255 and inverting the value for common anode
  red = (255 - map( analogRead(redPotPin), 0, 1024, 0, 255 ) );
  blue = (255 - map( analogRead(bluePotPin), 0, 1024, 0, 255 ) );
  green = (255 - map( analogRead(greenPotPin), 0, 1024, 0, 255 ) ); 
// Write the color to each pin using PWM and the value gathered above
  analogWrite(redPin, red);
  analogWrite(bluePin, blue);
  analogWrite(greenPin, green);
 if (z>=0)
 {
   {
     d=z%10; // find the remainder of dividing z by 10, this will be the right-hand digit
     c=int(d); // make it an integer, c is the right hand digit
     b=z/10; // divide z by 10 - the whole number value will be the left-hand digit
     e = int(b); // e is the left hand digit
     if(red >= (randRed-t) && red <= (randRed+t) && blue >= (randBlue-t) && blue <= (randBlue+t) && green >= (randGreen-t) && green <= (randGreen+t))
       {
       if(currentMillis - previousMillis > interval) 
         {
         previousMillis = currentMillis;
         digitalWrite(latchpin, LOW); // send the digits down to the shift registers!
         shiftOut(datapin, clockpin, LSBFIRST, segdisp2[c]); 
         shiftOut(datapin, clockpin, LSBFIRST, segdisp2[e]); 
         digitalWrite(latchpin, HIGH);
         beep(1100);
         z--;
         }
      //The following waits for three seconds once solution is achieved, and then begins the "game won" part
      if (aa > 0)
       {
         if(currentMillis2 - previousMillis2 > interval2) 
         {
         previousMillis2 = currentMillis2;
         aa--;
         }
       }
       if (aa == 0)
       {
         cont = 2;
       }
      }
     //Used if solution is lost
     if(red < (randRed-t) || red > (randRed+t) || blue < (randBlue-t) || blue > (randBlue+t) || green < (randGreen-t) || green > (randGreen+t))
     {
     aa = 30;
     }
     if(currentMillis - previousMillis > interval) 
     {
     previousMillis = currentMillis;
     digitalWrite(latchpin, LOW); // send the digits down to the shift registers!
     shiftOut(datapin, clockpin, LSBFIRST, segdisp[c]); 
     shiftOut(datapin, clockpin, LSBFIRST, segdisp2[e]); 
     digitalWrite(latchpin, HIGH);
     beep(1100);
     //checks to see if box is moved
         sensorValue = analogRead(sensorPin);
         Serial.println(sensorValue);
          if (sensorValue > 110)
          {
          cont = 3; 
          }
     z--;
     }
   }
 }
 //Starts "game over"
 if (z < 0)
 {
 beep(50);
 beep(50);
 beep(50);
 cont = 1;
 }
}
//Game Over
else if (cont == 1)
 {
 c = 0;
 e = 0;
 delay(500);
 digitalWrite(latchpin, LOW); // send the digits down to the shift registers!
 shiftOut(datapin, clockpin, LSBFIRST, segdisp[c]); 
 shiftOut(datapin, clockpin, LSBFIRST, segdisp2[e]);
 digitalWrite(latchpin, HIGH);
 delay(500);
 digitalWrite(latchpin, LOW);
 shiftOut(datapin, clockpin, LSBFIRST, 255); 
 shiftOut(datapin, clockpin, LSBFIRST, 255);
 digitalWrite(latchpin, HIGH);
 }
 //Win
 else if (cont == 2)
 {
 delay(500);
 digitalWrite(latchpin, LOW); // send the digits down to the shift registers!
 shiftOut(datapin, clockpin, LSBFIRST, segdisp2[c]); 
 shiftOut(datapin, clockpin, LSBFIRST, segdisp2[e]);
 digitalWrite(latchpin, HIGH);
 delay(500);
 digitalWrite(latchpin, LOW);
 shiftOut(datapin, clockpin, LSBFIRST, 255); 
 shiftOut(datapin, clockpin, LSBFIRST, 255);
 digitalWrite(latchpin, HIGH);
 }
 //Tilt
 else if (cont == 3)
 {
 c = 8;
 e = 8;
 delay(500);
 digitalWrite(latchpin, LOW); // send the digits down to the shift registers!
 shiftOut(datapin, clockpin, LSBFIRST, segdisp[c]); 
 shiftOut(datapin, clockpin, LSBFIRST, segdisp2[e]);
 digitalWrite(latchpin, HIGH);
 delay(500);
 digitalWrite(latchpin, LOW);
 shiftOut(datapin, clockpin, LSBFIRST, 255); 
 shiftOut(datapin, clockpin, LSBFIRST, 255);
 digitalWrite(latchpin, HIGH);
 }
}
//Code for beep function
void beep(unsigned char delayms){
  analogWrite(9, 30);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(9, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  
