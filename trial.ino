#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>
#include <Servo.h>   



  //create servo pin
Servo myservo;
// Declare the Servo pin 
int servoPin = 3; 

int lightSensor = 0;

// defines pins numbers for distancesensor
const int trigPin = 9;
const int echoPin = 10;

// defines variables
long duration;
int distance;


int pos = 0; 


//rfid
char tag[11] ="$000622499\0"; // Replace with your own Tag ID
char input[11];        // A variable to store the Tag ID being presented
int count = 0;        // A counter variable to navigate through the input[] character array
boolean flag = 0;     // A variable to store the Tag match status


byte userIdentifier[4];
byte auxID[4];
byte card = -1;


int flag1 = 0;
int flag2 = 0;
int finalflag = 0;
 
void setup() 
{
  Serial.begin(9600);
  myservo.attach(servoPin);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.flush();
}

 
 
 
void loop() 
{
     
     
     //LIGHTPART
     int valueFromLightSensor = analogRead(lightSensor);
     Serial.print("light");
     Serial.println(valueFromLightSensor);
     
          
    
     //DISTANCE PART
     
     // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Time: ");
    Serial.println(second());
    Serial.print("Flag1: ");
    Serial.println(flag1);
    Serial.print("Flag2: ");
    Serial.println(flag2);
    Serial.print("Flag3: ");
    Serial.println(flag3);

    delay(1000);
    Serial.flush();

    //RFID tags
  
    if (second()>50)
      {
        finalflag = 0;
        flag1 = 0;
        flag2 = 0;
      }
      
 if(Serial.available())// Check if there is incoming data in the RFID Reader Serial Buffer.
 {
    count = 0; // Reset the counter to zero
    /* Keep reading Byte by Byte from the Buffer till the RFID Reader Buffer is empty 
       or till 12 Bytes (the ID size of our Tag) is read */
    while(Serial.available() && count < 10) 
    {
      input[count] = Serial.read(); // Read 1 Byte of data and store it in the input[] variable
      delay(1);
      //Serial.print(input[count]);
      count++; // increment counter
      //delay(5);;
    }
    input[10] == '\0';

   
    Serial.println(input);
    Serial.println(tag);
    
    /* When the counter reaches 12 (the size of the ID) we stop and compare each value 
        of the input[] to the corresponding stored value */
    Serial.println(strlen(input));
    Serial.println(strlen(tag));
    
    if(strcmp(input,tag)==0) 
    {
      flag=1;
    }

    if (second()>50)
      {
        finalflag = 0;
        flag1 = 0;
        flag2 = 0;
      }
    
    if(flag == 1) // If flag variable is 1, then it means the tags match
    {
      
      
      //delay(5000);
      myservo.attach(servoPin);

      if ((second()>=0) && (second()<=20 ) && flag1 == 0)
      {
        finalflag = 1;
        flag1 = 1;
      }

      if ((second()>=40) && (second()<=50 ) && flag2 == 0)
      {
        finalflag = 1;
        flag2 = 1;
      }

    
      
     if(valueFromLightSensor > 600)
     {
      finalflag = 0;
     }
      if (finalflag)
        
               {
                finalflag = 0;
                 if (distance>=10)
                 {
                     for(pos = 130; pos>=80; pos-=1)     
                       {
                        myservo.write(pos);              
                        delay (50);
                       }
                       
                     for(pos = 130; pos < 180; pos += 1)  
                       {                                  
                         myservo.write(pos);              
                         delay(50);                      
                       } 
                     
                  }
                  
                  delay(1000);  
               }
      myservo.detach();
     
    }
    
    else
    {
      Serial.println("Access Denied"); //Incorrect Tag Message
      //digitalWrite(WARNLEDPIN,HIGH);
      //delay(1000);
      //digitalWrite(WARNLEDPIN,LOW);
    }
   
 
 
 flag=0;    

//}

}

               
       
 

