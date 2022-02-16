#include <SoftwareSerial.h>
#define ARDUINO_RX 12  //should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 13   //connect to RX of the module
SoftwareSerial myMP3(ARDUINO_RX, ARDUINO_TX);
static int8_t  Send_buf[6] = {0} ;
#define CMD_SEL_DEV 0X35
#define DEV_TF 0X01
#define CMD_PLAY 0X01
#define CMD_PAUSE 0X02
#define CMD_NEXT_SONG 0X03
#define CMD_PREV_SONG 0X04
#define CMD_STOP 0X0E
#define CMD_PLAY_W_INDEX 0X41
#define CMD_PLAY_FILE_NAME 0X42
#define CMD_INJECT_W_INDEX 0X43
#define CMD_SET_VOLUME 0X31
#define CMD_PLAY_W_VOL 0X31
#define CMD_SET_PLAY_MODE 0X33
#define ALL_CYCLE 0X00
#define SINGLE_CYCLE 0X01

boolean startuphum = false;
boolean off = false;

int ledcolor = 0;
int a = 500; //this sets how long the stays one color for
int red = 9; //this sets the red led pin
int green = 10; //this sets the green led pin
int blue = 11; //this sets the blue led pin
int cycleButton = 2; // cycle colors button pin
int buttonToggleCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

int button = 6;

int status = false;

void setup() { //this sets the output pins
pinMode(cycleButton, INPUT);
pinMode(button, INPUT_PULLUP);
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);
pinMode(blue, OUTPUT);
Serial.begin(9600);

myMP3.begin(9600);
delay(500);//Wait chip initialization is complete
mp3_5bytes(CMD_SEL_DEV, DEV_TF);//select the TF card  
delay(200);//wait for 200ms

}

void startupsound() {
  if(startuphum == true) {
    startuphum = false;
    mp3_6bytes(CMD_PLAY_W_VOL, 0X1E02);
    }
}

void poweroffsound() {
  if(off == true) {
    off == false;
    mp3_6bytes(CMD_PLAY_W_VOL, 0X1E01);
    }
}

void loop(){
  
  if (digitalRead(button) == true) { 
  status = !status; 
  Serial.println("butt");
    if (status == true){
    colors();
    startuphum = true;
    startupsound();
    
    } 
    else{
      off = true;
      poweroffsound();
      delay(1150);
      digitalWrite(red, LOW);
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      
    }
  
  
  while(digitalRead(button) == true); 
  delay(50); // keeps a small delay 
}
}

void colors() {
  
   buttonState = digitalRead(cycleButton);
   
   if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonToggleCounter++;
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
Serial.println(buttonToggleCounter);
 if (buttonToggleCounter == 1) {
    digitalWrite(red, LOW);
    digitalWrite(blue, HIGH);
    digitalWrite(green, LOW);  
  } 
 if (buttonToggleCounter == 2) {
    digitalWrite(red, HIGH);
    digitalWrite(blue, HIGH);
    digitalWrite(green, LOW);
    
  } 
 if (buttonToggleCounter == 3) {
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
    
  } 
if (buttonToggleCounter == 4) {
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
    
  } 
if (buttonToggleCounter == 5) {
    digitalWrite(red, LOW);
    digitalWrite(blue, HIGH);
    digitalWrite(green, LOW);
    
  } 
if (buttonToggleCounter == 6) {
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
  }

if (buttonToggleCounter == 7) {
  buttonToggleCounter = 0;
}

if (buttonToggleCounter == 0) {
digitalWrite(red, HIGH);
digitalWrite(green, HIGH);
digitalWrite(blue, HIGH);
}
}

void mp3_Basic(int8_t command)
{
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0x02; //the number of bytes of the command without starting byte and ending byte
  Send_buf[2] = command; 
  Send_buf[3] = 0xef; //
  sendBytes(4);
}
void mp3_5bytes(int8_t command, uint8_t dat)
{
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0x03; //the number of bytes of the command without starting byte and ending byte
  Send_buf[2] = command; 
  Send_buf[3] = dat; //
  Send_buf[4] = 0xef; //
  sendBytes(5);
}
void mp3_6bytes(int8_t command, int16_t dat)
{
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0x04; //the number of bytes of the command without starting byte and ending byte
  Send_buf[2] = command; 
  Send_buf[3] = (int8_t)(dat >> 8);//datah
  Send_buf[4] = (int8_t)(dat); //datal
  Send_buf[5] = 0xef; //
  sendBytes(6);
}
void sendBytes(uint8_t nbytes)
{
  for(uint8_t i=0; i < nbytes; i++)
  {
    myMP3.write(Send_buf[i]) ;
  }
}
