// Include the ESP8266 Library. This library is automatically provided by the ESP8266 Board Manager and does not need to be installed manually.
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char index_html[] PROGMEM = R"rawliteral(
HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8

<!DOCTYPE html>
<html>
<head>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
<link href="https://fonts.googleapis.com/css?family=Fredoka+One&display=swap" rel="stylesheet">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta charset="UTF-8">
<title>Is my house on fire?</title>
<style>
.centerDiv { display: flex; justify-content: center; align-items: center; text-align: center; }
.btn { margin-left: 1rem; margin-right: 1rem; }
#data { text-align: center; margin-top: 0.25rem; }
#error { text-align: center; color: #D8000C; background-color: #FFD2D2; font-size: 25px; }
body { background-color: #e63c3c; color: #fff; }
.BBQ { background-color: #eed202; }
.FIRE { background-color: #3ac427; }
h1 { font-family: 'Fredoka One', cursive, sans-serif; text-align: center; margin-top: 0.25rem; }
.csans { font-family: 'Comic Sans MS', sans-serif; }
#words { font-size: 5rem; }
</style>
</head>
<body>
<h1>Is my house on fire?</h1>
<div id="timeValue" class="centerDiv ms-3 csans">Last updated at %timeplaceholder%</div>
<br>
<p id="error"></p>
<br>
<h1 id="words">loading</h1>
<div id="svg" class="centerDiv">loading</div>
<br>
<p id="data" class="centerDiv fs-4 csans">MQ-9 Sensor: %mqsensor%</p>
<script>
const svgElem = document.querySelector('#svg');
const wordElem = document.querySelector('#words');

setInterval(() => {
  let xhttp = new XMLHttpRequest();
  xhttp.timeout = 2000;
  xhttp.addEventListener("load", () => {
    if (!xhttp.status === 200) return;
    //console.log(this);
    let response = JSON.parse(xhttp.responseText);
    let d = new Date();

    document.querySelector("#error").textContent = "";
    document.querySelector("#timeValue").textContent = `Last updated at ${d.getHours()}:${d.getMinutes() < 10 ? "0" : ""}${d.getMinutes()}`;
    document.querySelector("#data").textContent = `MQ-9 Sensor: ${response.mq} (${Math.round((response.mq / 1024 * 5) * 1000) / 1000}V)`; // gets the voltage sent by mq sensor and rounds to 3 decimal places

    if (response.mq > 500) {
      document.body.classList.add('FIRE');
      wordElem.textContent = response.mq > 1000 ? "IT'S TOO LATE" : 'YES';
      svgElem.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" version="1.2" viewBox="0 0 256 256" width="256" height="256"><style> .s0 { fill: #fff; }</style><g><path class="s0" d="m9.2 153.2c-6-5.7-9-13.3-9.2-21-0.1-7.7 2.6-15.4 8.4-21.4 5.7-6 13.3-9 21-9.2 7.7-0.2 15.4 2.6 21.4 8.3l51.6 49.7 102.7-130.5 3.7 2.8-3.7-2.8q0.4-0.6 1-1c6.1-5.3 13.8-7.6 21.3-7.2h0.3c7.6 0.5 14.9 3.8 20.4 9.9 5.5 6.2 8 14 7.6 21.7v0.3c-0.5 7.4-3.8 14.7-9.6 20.1l-123.3 150q-0.3 0.5-0.7 0.8c-5.8 5.1-13.1 7.6-20.4 7.4-7.3-0.1-14.6-2.9-20.2-8.3z"/></g></svg>`;
    } else if (response.mq > 100) {
      document.body.classList.add('BBQ');
      wordElem.innerHTML = '♨️ MAYBE ♨️<br>Check your stove and barbecue';
      svgElem.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="256" height="256" viewBox="0 0 256 256" xml:space="preserve"><g style="stroke: none; stroke-width: 0; stroke-dasharray: none; stroke-linecap: butt; stroke-linejoin: miter; stroke-miterlimit: 10; fill: #fff; fill-rule: nonzero; opacity: 1;" transform="translate(1.4065934065934016 1.4065934065934016) scale(2.81 2.81)"><path d="M 45 66.364 L 45 66.364 c -2.979 0 -5.429 -2.345 -5.56 -5.321 l -5.132 -49.519 C 33.841 5.304 38.762 0 45 0 h 0 c 6.238 0 11.159 5.304 10.692 11.524 L 50.56 61.043 C 50.429 64.019 47.979 66.364 45 66.364 z" style="stroke: none; stroke-width: 1; stroke-dasharray: none; stroke-linecap: butt; stroke-linejoin: miter; stroke-miterlimit: 10; fill: #fff; fill-rule: nonzero; opacity: 1;" transform=" matrix(1 0 0 1 0 0) " stroke-linecap="round"/><circle cx="44.998000000000005" cy="81.128" r="8.868" style="stroke: none; stroke-width: 1; stroke-dasharray: none; stroke-linecap: butt; stroke-linejoin: miter; stroke-miterlimit: 10; fill: #fff; fill-rule: nonzero; opacity: 1;" transform="matrix(1 0 0 1 0 0) "/></g></svg>`;
    } else {
      document.body.classList.remove('FIRE');
      document.body.classList.remove('BBQ');
      wordElem.textContent = 'NO';
      svgElem.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="256" height="256" viewBox="0 0 256 256" xml:space="preserve"><g style="stroke: none; stroke-width: 0; stroke-dasharray: none; stroke-linecap: butt; stroke-linejoin: miter; stroke-miterlimit: 10; fill: #fff; fill-rule: nonzero; opacity: 1;" transform="translate(1.4065934065934016 1.4065934065934016) scale(2.81 2.81)"><path d="M 11 90 c -2.815 0 -5.63 -1.074 -7.778 -3.222 c -4.295 -4.296 -4.295 -11.261 0 -15.557 l 68 -68 c 4.297 -4.296 11.26 -4.296 15.557 0 c 4.296 4.296 4.296 11.261 0 15.557 l -68 68 C 16.63 88.926 13.815 90 11 90 z" style="stroke: none; stroke-width: 1; stroke-dasharray: none; stroke-linecap: butt; stroke-linejoin: miter; stroke-miterlimit: 10; fill: #fff; fill-rule: nonzero; opacity: 1;" transform=" matrix(1 0 0 1 0 0) " stroke-linecap="round"/><path d="M 79 90 c -2.815 0 -5.63 -1.074 -7.778 -3.222 l -68 -68 c -4.295 -4.296 -4.295 -11.261 0 -15.557 c 4.296 -4.296 11.261 -4.296 15.557 0 l 68 68 c 4.296 4.296 4.296 11.261 0 15.557 C 84.63 88.926 81.815 90 79 90 z" style="stroke: none; stroke-width: 1; stroke-dasharray: none; stroke-linecap: butt; stroke-linejoin: miter; stroke-miterlimit: 10; fill: #fff; fill-rule: nonzero; opacity: 1;" transform=" matrix(1 0 0 1 0 0) " stroke-linecap="round"/></g></svg>`;
    }
  });
  xhttp.addEventListener("timeout", () => {
    document.querySelector("#error").textContent = "Lost connection to server.";
  });
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 750);
</script>
</body>
</html>
)rawliteral";

const long utcOffsetInSeconds = -18000;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// WiFi Router Login - change these to your router settings
const char* ssid = "HWDSB-GUEST";
const char* password = "Education";

String request;

unsigned long delayLength = 500; // blink length when mq is 501-1000

const int MQ = A0;
const int redLED = D0;
const int yellowLED = D1;
const int greenLED = D3;

int timeout = 0;

// Create the ESP Web Server on port 80
WiFiServer WebServer(80);
// Web Client
WiFiClient client;

void setup() {
  Serial.begin(19200);

  //setup pin modes
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Connect to WiFi network
  Serial.println();
  WiFi.disconnect();
  WiFi.hostname("William's ESP8266");
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_LIGHT_SLEEP);   //added to keep wifi awake other options are WIFI_NONE_SLEEP WIFI_LIGHT_SLEEP and WIFI_MODEM_SLEEP
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");

  timeClient.begin();

  // Start the Web Server
  WebServer.begin();
  Serial.println("Web Server started");

  // Print the IP address
  Serial.print("You can connect to the ESP8266 at this URL: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
}

String format() {
  String stringified = String(index_html);
  
  timeClient.update();
  String timeString = String(timeClient.getHours()) + ":" + (timeClient.getMinutes() < 10 ? "0" : "") + String(timeClient.getMinutes());
  
  stringified.replace("%timeplaceholder%", timeString);
  stringified.replace("%mqsensor%", String(analogRead(MQ)));
  return stringified;
}

String getJSON(String key, String value, bool comma) {
  return "  \"" + key + "\": " + value + (comma ? ", " : "") + "\n";
}

bool onState = LOW;
unsigned long previousMillis = 0;
void loop() {
  int mqValue = analogRead(MQ);

  // LEDs
  if (mqValue > 500 && mqValue < 1001) { // 501 - 1000
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= delayLength) { // uses time to check so it doesn't prevent the web server from responding
      // save the last time LED blinked
      previousMillis = currentMillis;

      onState = !onState;
      
      digitalWrite(redLED, onState);
      digitalWrite(yellowLED, !onState);
    }
  } else {
    digitalWrite(redLED, mqValue > 1000); // red led = 1000 - 1024
    digitalWrite(yellowLED, mqValue > 100 && mqValue < 501); // yellow led = 101 - 500
    digitalWrite(greenLED, mqValue < 101); // green led = 0 - 100
  }
  
  // Check if a user has connected
  client = WebServer.available();
  if (!client) //restart loop
    return delay(300);

  // Wait until the user sends some data
  Serial.println("New User");
  while (!client.available()) {
    delay(1);
    timeout++;
    if (timeout > 3000) {
      Serial.println("INFINITE LOOP BREAK!");
      break;
    }
  }
  timeout = 0;

  timeClient.update();
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());

  // Read the first line of the request
  request = client.readStringUntil('\r\n');
  Serial.println(request);
  client.flush();

  // Process the request:
  if (request.indexOf("/state") != -1) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json; charset=UTF-8");
    client.println();
    client.print("{\n");
    client.print(getJSON("mq", String(mqValue), false));
    client.print("}");
    return;
  }

  // Return the response
  client.println(format());
  Serial.println("------------------------------------");
}
