#include <ESP8266WiFi.h>    //Required to Connect the module with WiFi
#include "WiFiUdp.h"        //Required to Open the WebServer 
#include "NTPClient.h"      //Required to Access real Time-Date

int s1 = 5;               //Please put the GPIO number to which sensor is connected here 
int s2 = 4;               //Please put the GPIO number to which sensor is connected here
int s3 = 2;               //Please put the GPIO number to which sensor is connected here
int v1,v2,v3,v4,v5,v6;    //Declaring the variables that we've used in the program



const char* ssid = "Your_WiFi_name";           // Please put your wifi name here
const char* password = "Your_WiFi_Password";   //Please put your wifi password here
const char* host = "maker.ifttt.com";          //to access a webserver


const long utcOffsetInSeconds = 19800;         //making the time in sync with IST

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

/*The above code is to access real time from server(NTP client) */


void setup() {// begin the hardware setup part
    Serial.begin(9600);                 //setting up serial monitor bod-rate
    pinMode(s1, INPUT_PULLUP);          //declaring pin modes
    pinMode(s2, INPUT_PULLUP);          //declaring pin modes
    pinMode(s3, INPUT_PULLUP);          //declaring pin modes

    Serial.print("Connecting to ");     //Starting the WiFi module
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);         //Module begins to connect to the Given WiFi credentials
    
    while (WiFi.status() != WL_CONNECTED) {//cheching whether WiFi is connected or not
      delay(500);
      Serial.print("..");
    }
  
    Serial.println("");
    Serial.println("IP address: ");       //Confirming the connection
    Serial.println(WiFi.localIP());  

    timeClient.begin();                 //beginning the Time client
              
}


void loop() {  
    
            WiFiClient client;             //Declaring the WiFi Client
            const int httpPort = 80;       //Assigning a httpPort

            timeClient.update();                   //updating the TimeClient

            Serial.print(daysOfTheWeek[timeClient.getDay()]);
            Serial.print(", ");
            Serial.print(timeClient.getHours());
            Serial.print(":");
            Serial.print(timeClient.getMinutes());
            Serial.print(":");
            Serial.println(timeClient.getSeconds());
/*  These Statments mentioned above Prints the current time and day in the Serial monitor */        
            
          v1=digitalRead(s1);        //This line instructs the Sensors read the state usually sensors read "1" when there is an obstracle and "0" when nothing is present
          v3=digitalRead(s2);
          v5=digitalRead(s3);
          delay(10);                //This delay is nessary because sometimes the sensors read the input too fast so it takes the same value twice this is done to improve the accuracy of the model
          v2=digitalRead(s1);       //This line instructs the Sensors read the state once again to improve the Accuracy
          v4=digitalRead(s2);
          v6=digitalRead(s3);
            
            if ((timeClient.getHours())== 3 &&(timeClient.getMinutes())== 04)    // Set the time at which sensors should start reading for changes 
             /*You can Switch ON the Sensors multiple times a day just by repeating the above sub-routine*/
                 {
                    if(v1==0&&v2==1||v1==1&&v2==0||v3==0&&v4==1||v3==1&&v4==0||v6==0&&v5==1||v6==1&&v5==0)  //checking whether the Medicine is taken or not
                    {
                        String url = "Your_Url_Obtained_from_following README.md Steps";      //Note there are changes to be made here
              
                        Serial.print("Requesting URL: ");
                        Serial.println(url);
                     
                         client.print(String("GET ") + url + " HTTP/1.1\r\n" + 
                                        "Host: " + host + "\r\n" +   
                                               "Connection: close\r\n\r\n");  //this Triggers the Web-hook
                    }
                  }  
               delay(10);              //this delay is to improve the accuracy of the model
           
      
}
