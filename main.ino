
// //This value will be recorded in seconds
// int pressLength_milliSeconds = 0;
 
// int optionOne_milliSeconds = 100;
// int optionTwo_milliSeconds = 2000; 
// int capState = 0;    
// int delayResolution = 100; // The smaller this value the greater the reslolution   
// int counter = 0;


// int buttonPin = 2;
 
// int ledPin_Option_1 = 11;
// int ledPin_Option_2 = 12;
 
// void setup(){
 
//   // Initialize the pushbutton pin as an input pullup
//   // Keep in mind, when pin 2 has ground voltage applied, we know the button is being pressed
//   pinMode(buttonPin, INPUT);     
 
//   //set the LEDs pins as outputs
//   pinMode(ledPin_Option_1, OUTPUT); 
//   pinMode(ledPin_Option_2, OUTPUT); 
 
//   //Start serial communication - for debugging purposes only
//   Serial.begin(9600);                                     
 
// } // close setup
 
 
// // void loop() {
 
// //   //Record *roughly* the tenths of seconds the button in being held down
// //   while ( digitalRead(buttonPin) == 1 ){ 
 
// //     delay(delayResolution);  //if you want more resolution, lower this number 
// //     pressLength_milliSeconds = pressLength_milliSeconds + delayResolution;   
 
// //     //display how long button is has been held
// //     Serial.print("ms = ");
// //     Serial.println(pressLength_milliSeconds);

// //       //Option 2 - Execute the second option if the button is held for the correct amount of time
// //     if (pressLength_milliSeconds >= optionTwo_milliSeconds){
    
// //         digitalWrite(ledPin_Option_2, HIGH);     
        
// //     } 
    
// //     //option 1 - Execute the first option if the button is held for the correct amount of time
// //     else if(pressLength_milliSeconds >= optionOne_milliSeconds){
    
// //         digitalWrite(ledPin_Option_1, HIGH);  
        
// //     }
// //   }
 

 
 
// //   //every time through the loop, we need to reset the pressLength_Seconds counter
// //   pressLength_milliSeconds = 0;
// //   digitalWrite(ledPin_Option_2, LOW);
// //  digitalWrite(ledPin_Option_1, LOW);
// // } // close void loop

// void loop() {
//      if ( digitalRead(buttonPin) == 1) {
//          counter++;
//          delay(1000);
//      }
     
//      Serial.println(counter);
//      counter = 0;
// }