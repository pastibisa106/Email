#include <ESP8266WiFi.h>
 
WiFiClient client;

#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

String MakerIFTTT_Key;
String MakerIFTTT_Event;
char *append_str(char *here, String s) {
  int i = 0;
  while (*here++ = s[i]) {
    i++;
  }
  return here - 1;
}
char *append_ul(char *here, unsigned long u) {
  char buf[20];
  return append_str(here, ultoa(u, buf, 10));
}
char post_rqst[256];
char *p;
char *content_length_here;
char *json_start;
int compi;

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  delay(3000);
  Serial.println("Yuk kita mulai...");
  WiFi.begin("NEXIAN", "pastibisa"); // ssid, pass
  while ((!(WiFi.status() != WL_CONNECTED))) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Terhubung!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  delay(3000);
}
 
void loop() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Serial.print("Suhu : ");
    Serial.print(t);
    Serial.print(" *C\t RH : ");
    Serial.print(h);
    Serial.println("%");
   
    if (t > 27.00) {
        if (client.connect("maker.ifttt.com", 80)) {
            MakerIFTTT_Key = "hfLOps7kK17alz7UC0UYjHKYMmhhYR1mdkuuyzopwo7"; //ganti key teman-teman ifttt.com
            MakerIFTTT_Event = "alarm_detected";
            p = post_rqst;
            p = append_str(p, "GET /trigger/");
            p = append_str(p, MakerIFTTT_Event);
            p = append_str(p, "/with/key/");
            p = append_str(p, MakerIFTTT_Key);
            p = append_str(p, " HTTP/1.1\r\n");
            p = append_str(p, "Host: maker.ifttt.com\r\n");
            p = append_str(p, "Content-Type: application/json\r\n");
            p = append_str(p, "Content-Length: ");
            content_length_here = p;
            p = append_str(p, "NN\r\n");
            p = append_str(p, "\r\n");
            json_start = p;
            p = append_str(p, "{\"value1\":\"");
            p = append_str(p, "NodeMCU Alert");
            p = append_str(p, "\",\"value2\":\"");
            p = append_str(p, "Hi, Suhu aku sudah maksimal lho..");
            p = append_str(p, "\",\"value3\":\"");
            p = append_str(p, "Jangan lupa cek suhu ya");
            p = append_str(p, "\"}");
       
            compi = strlen(json_start);
            content_length_here[0] = '0' + (compi / 10);
            content_length_here[1] = '0' + (compi % 10);
            client.print(post_rqst);
        }
        Serial.println("Pesan Dikirim...............");
        Serial.println("Waspada Suhu Panas!");
        delay(10000);
        
    } else {
          Serial.println("Aman terkondisi Admin");
          
    }
    delay(5000);
}
