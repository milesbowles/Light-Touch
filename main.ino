// ---------------------------------------------------------------
// Imports
// ---------------------------------------------------------------

#include <Adafruit_NeoPixel.h>

// ---------------------------------------------------------------
// References
// ---------------------------------------------------------------

#define buttonPin 8 // Capacitive touch sensor
#define lightPin 9 // Addressable RGB array

// Define RGB pins
int redPin = 6;
int greenPin = 5;
int bluePin = 3;

// ---------------------------------------------------------------
// Variables
// ---------------------------------------------------------------

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, lightPin);

int brightness = 40; // 1/3 brightness

// ---------------------------------------------------------------
// Setup
// ---------------------------------------------------------------

void setup()
{
    pixels.begin();
    pixels.setBrightness(0); // Set lights to off state 
    pixels.show();
    // Set button input pin
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);
    // Set LED output pins
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT); 
    // Set RGB to off state 
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);

    Serial.begin(9600);
}

// ---------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------

void loop()
{

    // Get button event and act accordingly
    int b = checkButton();
    if (b == 1)
        clickEvent();
    if (b == 2)
        doubleClickEvent();
    if (b == 3)
        holdEvent();
    if (b == 4)
        longHoldEvent();
}

// ---------------------------------------------------------------
// Events to trigger
// ---------------------------------------------------------------

void clickEvent()
{
    showPositive();
    Serial.println("Click");
}
void doubleClickEvent()
{
    Serial.println("Double Click");
}
void holdEvent()
{
    Serial.println("Hold");
}
void longHoldEvent()
{
    cleanMode();
    Serial.println("Long Hold");
}

// ---------------------------------------------------------------
// Multi functional button
// ---------------------------------------------------------------

// Button timing variables
int debounce = 20;       // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250;         // max ms between clicks for a double click event
int holdTime = 1000;     // ms hold period: how long to wait for press+hold event
int longHoldTime = 3000; // ms long hold period: how long to wait for press+hold event

// Button variables
boolean buttonVal = HIGH;          // value read from button
boolean buttonLast = HIGH;         // buffered value of the button's previous state
boolean DCwaiting = false;         // whether we're waiting for a double click (down)
boolean DConUp = false;            // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;           // whether it's OK to do a single click
long downTime = -1;                // time the button was pressed down
long upTime = -1;                  // time the button was released
boolean ignoreUp = false;          // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;         // when held, whether to wait for the up event
boolean holdEventPast = false;     // whether or not the hold event happened already
boolean longHoldEventPast = false; // whether or not the long hold event happened already

int checkButton()
{
    int event = 0;
    buttonVal = digitalRead(buttonPin);
    // Button pressed down
    if (buttonVal == 1 && buttonLast == 0 && (millis() - upTime) > debounce)
    {
        downTime = millis();
        ignoreUp = false;
        waitForUp = false;
        singleOK = true;
        holdEventPast = false;
        longHoldEventPast = false;
        if ((millis() - upTime) < DCgap && DConUp == false && DCwaiting == true)
            DConUp = true;
        else
            DConUp = false;
        DCwaiting = false;
    }
    // Button released
    else if (buttonVal == 0 && buttonLast == 1 && (millis() - downTime) > debounce)
    {
        if (not ignoreUp)
        {
            upTime = millis();
            if (DConUp == false)
                DCwaiting = true;
            else
            {
                event = 2;
                DConUp = false;
                DCwaiting = false;
                singleOK = false;
            }
        }
    }
    // Test for normal click event: DCgap expired
    if (buttonVal == 0 && (millis() - upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
    {
        event = 1;
        DCwaiting = false;
    }
    // Test for hold
    if (buttonVal == 1 && (millis() - downTime) >= holdTime)
    {
        // Trigger "normal" hold
        if (not holdEventPast)
        {
            event = 3;
            waitForUp = true;
            ignoreUp = true;
            DConUp = false;
            DCwaiting = false;
            //downTime = millis();
            holdEventPast = true;
        }
        // Trigger "long" hold
        if ((millis() - downTime) >= longHoldTime)
        {
            if (not longHoldEventPast)
            {
                event = 4;
                longHoldEventPast = true;
            }
        }
    }
    buttonLast = buttonVal;
    return event;
}


// ---------------------------------------------------------------
// Positve light animation
// ---------------------------------------------------------------

void showPositive()
{
    digitalWrite(buttonPin, LOW);
    uint16_t r, j;
    pixels.setBrightness(brightness);

    for (j = 0; j < 256 * 1; j++)
    {
        for (r = 0; r < pixels.numPixels(); r++)
        {
            pixels.setPixelColor(r, positiveWheel(((r * 256 / pixels.numPixels()) + j) & 255));
        }
        pixels.show();
        delay(5);
    }
    pixels.setBrightness(0);
    pixels.show();
    digitalWrite(buttonPin, HIGH);
}

uint32_t positiveWheel(byte WheelPos)
{
    if (WheelPos < 25)
    {
        return pixels.Color(0, 200, 0);
    }
    else if (WheelPos < 175)
    {
        WheelPos -= 25;
        return pixels.Color(0, WheelPos * 3, 0);
    }
    else
    {
        WheelPos -= 25;
        return pixels.Color(255, 255, 255);
    }
}

// ---------------------------------------------------------------
// Clean mode 
// ---------------------------------------------------------------

void cleanMode()
{
    digitalWrite(buttonPin, LOW);
    uint16_t r, j;
    pixels.setBrightness(brightness);
    
    // Set lights to purple-blue
    analogWrite(redPin, 120);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);

    // 9 animation cycles
    for (j = 0; j < 256 * 9; j++)
    { 
        for (r = 0; r < pixels.numPixels(); r++)
        {
            pixels.setPixelColor(r, cleanModeWheel(((r * 256 / pixels.numPixels()) + j) & 255));
        }
        pixels.show();
        delay(5);
    }
    // Set RGB to off state 
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    // Turn lights off
    pixels.setBrightness(0);
    pixels.show();
    digitalWrite(buttonPin, HIGH);

}

uint32_t cleanModeWheel(byte WheelPos)
{
    if (WheelPos < 40)
    {
        return pixels.Color(255, 255, 255);
    }
    else if (WheelPos < 180)
    {
        WheelPos -= 90;
        return pixels.Color(0, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 65;
        return pixels.Color(0, 0, 255 - WheelPos * 3);
    }
}

