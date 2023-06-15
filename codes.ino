#include <LiquidCrystal.h>
#include <MsTimer2.h>
// LCD pim bağlantıları
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Buzzer pim bağlantısı
const int buzzerPin = 7;

// Buton pim bağlantısı
const int buttonPin = 1;
bool buttonState = HIGH; // Buton durumunu saklamak için değişken


// Geri sayım değişkenleri
int remainingTime = 1;
int waterCount = 0;
bool buzzerState = false;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Kalan Sure:");
  lcd.print(remainingTime);
  lcd.print("dk   ");
  lcd.setCursor(0, 1);
  lcd.print("Su Bardagi: ");
  lcd.print(waterCount);
  lcd.print("    ");
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  
  // 1 dakikada bir timer kesmesi
  MsTimer2::set(60000, timerISR);
  MsTimer2::start();
}

void loop() {
  // Buton durumunu kontrol etme
  bool buttonStateNew = digitalRead(buttonPin);

  // Buton durumu değiştiyse
  if (buttonState != buttonStateNew) {
    delay(10); // Debouncing için kısa bir bekleme süresi ekle
    buttonStateNew = digitalRead(buttonPin); // Buton durumunu tekrar oku

    if (buttonState != buttonStateNew && buttonStateNew == LOW) {
      if (remainingTime > 0) {
        remainingTime = 120;
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        waterCount++;
      } else {
        remainingTime = 120;
        waterCount++;
        buzzerState = false;
        digitalWrite(buzzerPin, LOW);
      }
      
      lcd.setCursor(0, 0);
      lcd.print("Kalan Sure:");
      lcd.print(remainingTime);
      lcd.print("dk   ");
      
      lcd.setCursor(0, 1);
      lcd.print("Su Bardagi: ");
      lcd.print(waterCount);
      lcd.print("    ");
    }

    buttonState = buttonStateNew; // Buton durumunu sakla
  }
}

//Geri sayımı azaltma ve ekran güncelleme
void timerISR() {
  if (remainingTime > 0) {
    remainingTime--;
    lcd.setCursor(0, 0);
    lcd.print("Kalan Sure:");
    lcd.print(remainingTime);
    lcd.print("dk   ");
  }
  
  if (remainingTime == 0) {
    if (!buzzerState) {
      digitalWrite(buzzerPin, HIGH);
      buzzerState = true;
    }
  }
}

void buttonISR() {
  if (buzzerState) {
    digitalWrite(buzzerPin, LOW);
    buzzerState = false;
  }
}
