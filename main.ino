// Author: Miles Bowles
// Project: Light Touch
// Description: Light animations triggered by capacitive touch sensor

// Imports
// ---------------------------------------------------------------

#include <Adafruit_NeoPixel.h>

// References
// ---------------------------------------------------------------

#define buttonPin 8 // Capacitive touch sensor
#define lightPin 9 // Addressable RGB array

// Variables
// ---------------------------------------------------------------

 Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, lightPin);

// Set rgb brightness
int brightness = 60; // ~1/3 brightness

// Setup
// ---------------------------------------------------------------

void setup()
{
    // Setup addressable RGB array
    pixels.begin();
    pixels.setBrightness(0); // Set initial state as off
    pixels.show(); // Update state

    // Set button input pin
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);

    // Begin serial data transmission
    Serial.begin(9600);
}

// Main loop
// ---------------------------------------------------------------

void loop()
{
    // Get button event and act accordingly
    int button = checkButton();
    if (button == 1)
        pressEvent();
    if (button == 2)
        doublePressEvent();
    if (button == 3)
        holdEvent();
    if (button == 4)
        longHoldEvent();
}

// Events to trigger
// ---------------------------------------------------------------

void pressEvent()
{
    Serial.println("Single Tap");
    pulseAnimation();
}
void doublePressEvent()
{
    Serial.println("Double Tap");
    spinnerAnimation();
}
void holdEvent()
{
    Serial.println("Hold");
}
void longHoldEvent()
{
    Serial.println("Long Hold");
}

// Multi functional button
// ---------------------------------------------------------------
// Note - debouncing is not neccesary with capacitive touch sensor. 
// However, I kept this functionality intact so that it can work with a push button if needed.

// Button timing variables
int debounce = 20;          // (ms) Debounce period to prevent flickering when pressing or releasing the button 
int doublePressGap = 250;   // (ms) Max time between press actions for a double press event
int holdTime = 1000;        // (ms) How long to wait for press & hold event
int longHoldTime = 3000;    // (ms) How long to wait for long press & hold event

// Button variables
boolean buttonState = HIGH;         // Value read from button
boolean lastButtonState = HIGH;     // Buffered value of the button's previous state
boolean doublePressWaiting = false; // Waiting for a double press (down)
boolean doublePressOnUp = false;    // Whether to register a double press on next release
boolean singlePress = true;         // Whether to register a single click
long downTime = -1;                 // Time the button was pressed down
long upTime = -1;                   // Time the button was released
boolean ignoreUp = false;           // Whether to ignore the button release because the click & hold was triggered
boolean waitForUp = false;          // When held, whether to wait for the up event
boolean holdEventPast = false;      // Whether or not the hold event happened already
boolean longHoldEventPast = false;  // Whether or not the long hold event happened already

int checkButton()
{
    int event = 0;
    // Get button state
    buttonState = digitalRead(buttonPin);

    // Button pressed down
    if (buttonState == 1 && lastButtonState == 0 && (millis() - upTime) > debounce)
    {
        downTime = millis();
        ignoreUp = false;
        waitForUp = false;
        singlePress = true;
        holdEventPast = false;
        longHoldEventPast = false;
        if ((millis() - upTime) < doublePressGap && doublePressOnUp == false && doublePressWaiting == true)
            doublePressOnUp = true;
        else
            doublePressOnUp = false;
            doublePressWaiting = false;
    }
    // Button released
    else if (buttonState == 0 && lastButtonState == 1 && (millis() - downTime) > debounce)
    {
        if (not ignoreUp)
        {
            upTime = millis();
            if (doublePressOnUp == false)
                doublePressWaiting = true;
            else
            {
                event = 2;
                doublePressOnUp = false;
                doublePressWaiting = false;
                singlePress = false;
            }
        }
    }

    // Check for normal click event: doublePressGap expired
    if (buttonState == 0 && (millis() - upTime) >= doublePressGap && doublePressWaiting == true && doublePressOnUp == false && singlePress == true && event != 2)
    {
        event = 1;
        doublePressWaiting = false;
    }

    // Check for hold
    if (buttonState == 1 && (millis() - downTime) >= holdTime)
    {
        // Trigger normal hold
        if (not holdEventPast)
        {
            event = 3;
            waitForUp = true;
            ignoreUp = true;
            doublePressOnUp = false;
            doublePressWaiting = false;
            downTime = millis();
            holdEventPast = true;
        }

        // Trigger long hold
        if ((millis() - downTime) >= longHoldTime)
        {
            if (not longHoldEventPast)
            {
                event = 4;
                longHoldEventPast = true;
            }
        }
    }

    // Update button state
    lastButtonState = buttonState;
    return event;
}

// Light Functions
// ---------------------------------------------------------------

// Set RGB colors
void setLEDColor(int r, int g, int b)
{
    // Set brightness
    pixels.setBrightness(brightness);
    // Id#, r value, g value, b value - Only using 3 of the 7 available rgbs
    pixels.setPixelColor(2, r, g, b);
    pixels.setPixelColor(4, r, g, b);
    pixels.setPixelColor(6, r, g, b);
    pixels.show();
}

// Light Animations
// ---------------------------------------------------------------

// Spinner animation
void spinnerAnimation() {

    uint16_t i, n;

    // Disable button
    digitalWrite(buttonPin, LOW);

    // Set rgb brightness
    pixels.setBrightness(brightness);

    // Set animation rotation speed (ms)
    int rotationSpeed = 150;

    // Set color 3 
    int r1 = 255;
    int g1 = 255; 
    int b1 = 255;

    // Set color 3 
    int r2 = 100;
    int g2 = 200; 
    int b2 = 255;

    // Set color 3 
    int r3 = 0;
    int g3 = 50; 
    int b3 = 255;

    for (i = 0; i < 30; i++) 
    {
        // Iterate through lights
        for (n = 0; n < 3; n++)
        {
            // Set rgb values for current iteration
            switch (n)
            {
                case 0:
                    pixels.setPixelColor(2, r1, g1, b1);
                    pixels.setPixelColor(4, r2, g2, b2);
                    pixels.setPixelColor(6, r3, g3, b3);
                    break;
                case 1:
                    pixels.setPixelColor(6, r1, g1, b1);
                    pixels.setPixelColor(2, r2, g2, b2);
                    pixels.setPixelColor(4, r3, g3, b3);
                    break;
                case 2:
                    pixels.setPixelColor(4, r1, g1, b1);
                    pixels.setPixelColor(6, r2, g2, b2);
                    pixels.setPixelColor(2, r3, g3, b3);
                    break;
                default:
                    break;
            }

            // Update rgb array
            pixels.show();
            // Set animation rotation speed
            delay(rotationSpeed);
        }
    }
    
    // Set rgb array to off when animation is finished
    pixels.setBrightness(0);
    pixels.show();

    // Enable button
    digitalWrite(buttonPin, HIGH);
}

// Pulse animation
void pulseAnimation()
{
    uint16_t i, n;

    // Disable button
    digitalWrite(buttonPin, LOW);

    // Set rgb brightness
    pixels.setBrightness(brightness);

    // Set pulse speed (ms)
    int pulseSpeed = 5;

    // TODO: Add logic for pulsing with any input color
    // Set color 
    // int r = 255;
    // int g = 255; 
    // int b = 255;

    for (i = 0; i < 10; i++) 
    {
        // Cycle from 0 brightness to full brightness and back
        for (n = 0; n < (255 * 2); n++)
        {
            // Adjust brightness
            if (n < 255) {
                pixels.setPixelColor(4, n, n, n);
                pixels.setPixelColor(6, n, n, n);
                pixels.setPixelColor(2, n, n, n);
            } else {
                int c = ((255 * 2) - n);
                pixels.setPixelColor(4, c, c, c);
                pixels.setPixelColor(6, c, c, c);
                pixels.setPixelColor(2, c, c, c);
            }

            // Update rgb array
            pixels.show();
            // Set pulse speed
            delay(pulseSpeed);
        }
    }
    
    // Set rgb array to off when animation is finished
    pixels.setBrightness(0);
    pixels.show();

    // Enable button
    digitalWrite(buttonPin, HIGH);
}