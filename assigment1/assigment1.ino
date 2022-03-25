//Bouncng Ball
#include <avr/sleep.h>
#include <EnableInterrupt.h>

int Ls = 6; //red led's pin (PWM)
byte ButtonPin[] = {2, 3, 4, 5};
byte ledPin[] = {8, 9, 10, 11}; //L1,L2, L3, L4
int pot = A0; //potenziometro
unsigned long tempoIniziale;
unsigned long tempoCorrente;

byte S = 100;
byte direzione = 1;
byte ledCorrente = 0;

unsigned long tempoTrascorso;
int brightness = 0; 
int fadeAmount = 5; 

int tempoTurno;
int time1;
int time2 = 1500;
int timer = 0;
int playState = 0;
int score;
int difficulty;
int diffPrev;
int p;
float f;
int winner = 0;

void WakeUp(){}

void SleepMode(){
      Serial.println("sleep");
      delay(200); //piccolo delay per fare arrivare i dati
      digitalWrite(Ls, LOW);//inserito per sicurezza, a volte va in sleep con ls HIGH
      set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
      sleep_enable();
      sleep_mode(); 
      Serial.println("WAKE UP"); //riparte da qui quando premi T1 
      sleep_disable(); 
}
void ReadDifficulty(){
    pot = analogRead(A0);
  difficulty = map(pot,1, 1023, 1, 8);
  if(diffPrev!=difficulty){
      Serial.print("Difficulty: ");
      Serial.println(difficulty);
      diffPrev = difficulty;
  }
  
}

void setup() {
  // put your setup code here, to run once:
  enableInterrupt(ButtonPin[0], WakeUp,FALLING);
  enableInterrupt(ButtonPin[1], WakeUp,FALLING);
  enableInterrupt(ButtonPin[2], WakeUp,FALLING);
  enableInterrupt(ButtonPin[3], WakeUp,FALLING);
  for (byte x=0; x<4; x++) {
    pinMode(ledPin[x], OUTPUT);
  }
  for (byte x=0; x<4; x++) {
    pinMode(ButtonPin[x], INPUT);
  }
  pinMode(Ls, OUTPUT);
  
  time1 = random(3000,5000);
  Serial.begin(9600);
  Serial.println("Welcome to the Catch the Bouncing Led Ball Game. \nPress Key T1 to Start");
}

int c = 0;
void loop() {
  analogWrite(Ls, brightness); 
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading
  if (brightness == 0 || brightness == 255) {
   fadeAmount = -fadeAmount ; 
   }  
  delay(50);
  //leggo pot
  if(score == 0){
      ReadDifficulty();  
  }
  int T1State = digitalRead(ButtonPin[0]);
  if(T1State == HIGH){
    playState = 1;
  }
  if(playState == 1){
    if(c == 0){
      score = 0;
      Serial.println("GO");
      c++;
    }
    //GIOCO
    tempoIniziale = millis();
    tempoCorrente = millis();
    while((tempoCorrente-tempoIniziale) < time1){
      tempoCorrente = millis();
      p = cambiaStatoLed();
      delay(S); 
    }
    for (byte x=0; x<4; x++) {
    digitalWrite(ledPin[x], LOW);
    }
    tempoIniziale = millis();
    tempoCorrente = millis();
    while((tempoCorrente-tempoIniziale) < time2){
      tempoCorrente = millis();
      digitalWrite(ledPin[p], HIGH);
      //qui If di controllo
      winner = 0;
      if(digitalRead(ButtonPin[p])== HIGH){
        score = score + 1;
        S = S - difficulty;
        //time2 = time2 - (difficulty* 35);
        f = 1-(0.05*difficulty);
        time2 = time2 * f;
        winner = 1;
        Serial.print("score: ");
        Serial.println(score);
        break;
      }
    }
     digitalWrite(ledPin[p], LOW);
     if(winner == 0){
        Serial.println("GAME OVER"); 
        Serial.print("Il tuo score era: ");
        Serial.println(score);
         
        playState = 0;  
        score = 0;
        S = 100;
        time2 = 1500; 
        c = 0;   
     }
  }
  else{
    timer++;
    if(timer == 200){ //50 * 200 = 10000ms
      timer = 0;
      SleepMode();
    }
  }
}

int cambiaStatoLed() {
  // spegne tutti i LED
  for (byte x=0; x<4; x++) {
    digitalWrite(ledPin[x], LOW);
  }
  // accende il LED corrente
  digitalWrite(ledPin[ledCorrente], HIGH);
  // incrementa la variabile direzione
  ledCorrente += direzione;
  // cambia la direzione se si arriva alla fine
  if (ledCorrente == 3) {
    direzione = -1;
  }
  if (ledCorrente == 0) {
    direzione = 1;
  }
  return ledCorrente;
}
