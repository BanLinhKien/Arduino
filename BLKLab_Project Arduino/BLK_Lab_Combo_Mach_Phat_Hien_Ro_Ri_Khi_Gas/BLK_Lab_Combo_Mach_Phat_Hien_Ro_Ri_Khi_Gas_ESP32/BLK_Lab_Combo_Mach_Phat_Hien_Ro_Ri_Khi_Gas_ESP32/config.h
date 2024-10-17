// Your WiFi credentials.
//char ssid[] = "Tplink 2.4 ghz";
//char pass[] = "hoianhson";

#define ssid      "ESP32"             //AP SSID
#define pass      ""                //AP Password

String  Essid = "";                 //EEPROM Network SSID
String  Epass = "";                 //EEPROM Network Password
String  Etoken = "";                //EEPROM Network token

String  sssid = "";                 //Read SSID From Web Page
String  passs = "";                 //Read Password From Web Page
String  token = "";                 //Read token From Web Page
