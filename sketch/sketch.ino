#define PORT_GROUP PORTB
#define PORT_NUMBER 7

//#define PORT_GROUP PORTA
//#define PORT_NUMBER 17

volatile uint32_t *setPin = &PORT->Group[PORT_GROUP].OUTSET.reg;  // Ptr to PortA Data Output Value Set register
volatile uint32_t *clrPin = &PORT->Group[PORT_GROUP].OUTCLR.reg;  // Ptr to PortA Data Output Value Clear register
volatile uint32_t *dirPin = &PORT->Group[PORT_GROUP].DIRSET.reg;  // Ptr to PortA Data Direction Set register

const uint32_t  PinMASK = (1ul << PORT_NUMBER);   // Generate bit mask, binary one PORT_NUMBER places to the left

// the setup function runs once when you press reset or power the board
void setup()
{
  
  Serial.begin(115200);
  
  // initialize digital pin as an output.
  *dirPin = PinMASK;
}

// the loop function runs over and over again forever
void loop()
{
  *setPin = PinMASK;        // turn the LED on (HIGH is the voltage level)
  delay(250);              // wait for a second
  *clrPin = PinMASK;        // turn the LED off by making the voltage LOW
  delay(250);              // wait for a second
  SerialUSB.println("cycle");
  Serial.println("cycle");
}
