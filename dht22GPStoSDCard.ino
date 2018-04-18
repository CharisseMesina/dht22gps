#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>

//Constants
SoftwareSerial GPS_SoftSerial(15, 14);/* (Rx, Tx) */
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
TinyGPSPlus gps;      
char logFileName[13];
const int chipSelect = 4;
volatile float minutes, seconds;
volatile int degree, secs, mins;
void setup()
{
  Serial.begin(9600);
  dht.begin();
  GPS_SoftSerial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");

    return;
  }
  Serial.println("card initialized.");
}


void loop()
{

  float humidity = dht.readHumidity();

  float temperature_C = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature_C)) {

    Serial.println("Failed to read from DHT sensor!");

    return;

  } 
   smartDelay(1000); /* Generate precise delay of 1ms */
        unsigned long start;
        double lat_val, lng_val, alt_m_val;
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */

 File logFile = SD.open("datagps.txt", FILE_WRITE); // Open the log file
  if (logFile)
  { 
    logFile.println(""); 
    logFile.print(gps.location.lng(), 6);
    logFile.print(',');
    logFile.print(gps.location.lat(), 6);
    logFile.print(',');
    

    Serial.println("");
    Serial.print(gps.location.lng(), 6);
    Serial.print(',');
    Serial.print(gps.location.lat(), 6);
    Serial.print(',');
    
    logFile.print(temperature_C);
    
    logFile.print(",");
    
    logFile.print(humidity);

    logFile.close();

    // print to the serial port too:

    Serial.print(temperature_C);

    Serial.print(",");

    Serial.print(humidity);

 return 1; // Return success
  }

  return 0; // If we failed to open the file, return fail
}

  
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}


void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}

  


