#define BLYNK_TEMPLATE_ID "TMPL6Xl9eZpLl"
#define BLYNK_TEMPLATE_NAME "buncha esp32"
#define BLYNK_AUTH_TOKEN "dR7I5WVupqfN98dRMJGBUDvXhntZRuMy"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Define the motor pins using GPIO numbers
#define ENA 2
#define IN1 4
#define IN2 5
#define IN3 18
#define IN4 19
#define ENB 21

BlynkTimer timer;

bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
bool lights = 0;
int Speed = 0;

// Enter your Auth token
char auth[] = BLYNK_AUTH_TOKEN;

// Enter your WIFI SSID and password
char ssid[] = "POCO X4 GT";
char pass[] = "1234567a";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Set motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

BLYNK_WRITE(V0)
{
  Speed = param.asInt();
}

BLYNK_WRITE(V1)
{
  forward = param.asInt();
}

BLYNK_WRITE(V2)
{
  backward = param.asInt();
}

BLYNK_WRITE(V3)
{
  left = param.asInt();
}

BLYNK_WRITE(V4)
{
  right = param.asInt();
}

/************** Motor movement functions *****************/
void carForward()
{
  Serial.println("Car moving forward");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward()
{
  Serial.println("Car moving backward");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carLeft()
{
  Serial.println("Car turning left");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carRight()
{
  Serial.println("Car turning right");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop()
{
  Serial.println("Car stopped");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void WIFIcar()
{
  if (forward == 1)
  {
    carForward();
    Serial.println("Car moving forward");
  }
  else if (backward == 1)
  {
    carBackward();
    Serial.println("Car moving backward");
  }
  else if (left == 1)
  {
    carLeft();
    Serial.println("Car turning left");
  }
  else if (right == 1)
  {
    carRight();
    Serial.println("Car turning right");
  }
  else
  {
    carStop();
    Serial.println("Car stopped");
  }
}

void loop()
{
  Blynk.run(); // Run Blynk library
  WIFIcar();
}