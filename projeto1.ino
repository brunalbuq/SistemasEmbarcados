#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int trig = 10;
int echo = 9;
int duracao;
float distancia;


void setup()
{
  pinMode(trig,OUTPUT);
  pinMode (trig,INPUT);
  lcd.begin(16,2); 
  lcd.setCursor(0,0);
  lcd.print("Distancia:");
}

void loop()
{
  digitalWrite(trig,HIGH);
  delay(1);
  digitalWrite(trig,LOW);
  duracao = pulseIn(echo,HIGH);
  distancia = duracao/58.2;
  lcd.setCursor(0,1);
  lcd.print(distancia + String(" cm"));
  
}
