#include <Adafruit_NeoPixel.h>

#define buttonPin 2 // analog input pin to use as a digital input

#define lightPin 9

#define ledPin 6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, lightPin);

int brightness = 40;

void setup()
{
    pixels.begin();
    pixels.setBrightness(0); // 1/3 brightness
    pixels.show();
    // Set button input pin
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);
    // Set LED output pins
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    Serial.begin(9600);
}

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

void clickEvent()
{
    showPositive();
    Serial.println("click");
}
void doubleClickEvent()
{

    Serial.println("double click");
}
void holdEvent()
{
    // if (digitalRead(ledPin) == LOW)
    // {
    //     digitalWrite(ledPin, HIGH);
    // }
    // else
    // {
    //     digitalWrite(ledPin, LOW);
    // }
    Serial.println("hold");
}
void longHoldEvent()
{
    showBTSync();
    Serial.println("long hold");
}

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
// Lights

void showPositive()
{
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
}

uint32_t positiveWheel(byte WheelPos)
{
    if (WheelPos < 25)
    {
        return pixels.Color(0, 200, 100);
    }
    else if (WheelPos < 110)
    {
        WheelPos -= 25;
        return pixels.Color(0, WheelPos * 3, 0);
    }
    else
    {
        WheelPos -= 25;
        return pixels.Color(0, 255 - WheelPos * 3, 0);
    }
}

void showBTSync()
{
    uint16_t r, j, i;
    pixels.setBrightness(40);

    digitalWrite(ledPin, HIGH);

    for (j = 0; j < 256 * 9; j++)
    { // 6 cycles of all colors on wheel
        for (r = 0; r < pixels.numPixels(); r++)
        {
            pixels.setPixelColor(r, BTSyncWheel(((r * 256 / pixels.numPixels()) + j) & 255));
        }
        pixels.show();
        delay(5);
    }

    pixels.setBrightness(0);
    digitalWrite(ledPin, LOW);
    pixels.show();
}

uint32_t BTSyncWheel(byte WheelPos)
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
