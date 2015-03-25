const int motor = 5;
int mp3 = 12;
int spd = 255;
int fA = 0;
int state = 0; // 0 = stoji
// 1 = jede vpravo
// 2 = jede vlevo
// -1 = brzdi

//int rand = 0;

int ir1Pin = 6;
int ir2Pin = 7;

int relay = 3;
int relay_s = LOW;

int svetlo = 2;
int svetlo_s = 0;

int ir1Val = 0;
int ir2Val = 0;

unsigned long previousMillis = 0;
long interval = 1000;


void setup()  {

  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(mp3, OUTPUT);
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(svetlo, OUTPUT);

  //state = 0;

  spd = 255;
  analogWrite(motor, spd);
  Serial.println("waiting for mp3 to be paused");
  delay(8000);
  //
  mp3Ctl();
  Serial.println("mp3 should be pasued");
  //
  Serial.println("light testing");
  rozsvitit();
  zhasnout();
  rozsvitit();

}

void loop() {
  //  state = 0;
  ir1Val = digitalRead(ir1Pin);
  ir2Val = digitalRead(ir2Pin);

  if (ir1Val && ir2Val) {
    Serial.print("obje zavory jsou otevrene");
    //  int r = random(100);
    //  Serial.println(r);
    //    delay(120);
    if (state == 0) {
      // kdyz tramvaj stoji
      // tak se rozjede na pravou nebo levou stranu
      int r = random(100);
      Serial.print("random ");
      Serial.print(r);
      Serial.print("  ");

      if ( r < 50) {
        state = 1;
        mp3Ctl();
        rozsvitit();
      } else {
        state = 2;
        mp3Ctl();
        rozsvitit();
      }
    }
  }

  if (ir1Val && !ir2Val) {
    Serial.print("prava zavora je zavrena");
    if (state == 0) {
      // kdyz tramvaj stoji a ma zavrenou branu
      // rozjede se smerem od brany
      state = 2;
      delay(random(3000, 7000));
      mp3Ctl();
      rozsvitit();
    }
    if (state == 1 ) {
      // kdyz tramvaj jede a ma zavrenou branu
      // zacne brzdit
      state = -1;
    }
  }
  else if (!ir1Val && ir2Val) {
    Serial.print("leva zavora je zavrena");
    if (state == 0) {
      // kdyz tramvaj stoji a ma zavrenou branu
      // rozjede se smerem od brany
      state = 1;
      delay(random(3000, 7000));
      mp3Ctl();
      rozsvitit();
    }
    if (state == 2) {
      // kdyz tramvaj jede a ma zavrenou branu
      // zacne brzdit
      state = -1;
    }
  }

  if (state == -1) {
    // kdyz je jedna z bran zavrena a tramvaj je ve stavu brzdim
    // zpomaluji rychlost (!!! otocena logika!!!)
    spd = constrain(spd + 5, 0, 255);
    if (spd >= 255) {
      // kdyz se tramvaj uplne zastavi
      // zmeni se stav na stojim
      state = 0;
      mp3Ctl();
      zhasnout();
    }
  }
  if ((state == 1 | state == 2) && spd > 0) {
    // kdyz je tramvaj ve stavu jedu vlevo nebo vpravo
    // a rychlost neni na maximu
    // rychlost pomalu roste
    spd = constrain(spd - 5, 0, 255);
  }



  // logika smeru motoru
  if (state == 1) {
    relay_s = LOW;
  }
  if (state == 2) {
    relay_s = HIGH;
  }

  // ovladani smeru motoru
  digitalWrite(relay, relay_s);

  // ovladani rychlosti motoru
  analogWrite(motor, spd);


  Serial.print(" tramvaj je ve stavu: ");
  Serial.print(state);
  Serial.print(" rychlost: ");
  Serial.print(spd);
  Serial.println();

}

//void jede() {
//  spd = spd + fA;
//
//  // Zpomaluje
//  if (spd < 255 && ir1Val == 0 && ir2Val == 1) {
//    fA = 5;
//    delay(10);
//    analogWrite(motor, spd);
//  }
//
//  if (spd < 255 && ir1Val == 1 && ir2Val == 0) {
//    fA = 5;
//    delay(10);
//    analogWrite(motor, spd);
//  }
//  if (spd >= 255 && ir1Val == 0 && ir2Val == 0) {
//    fA = 5;
//    delay(10);
//    analogWrite(motor, spd);
//  }
//
//  // Stoji
//  if (spd >= 255 && ir1Val == 0 || spd >= 255 && ir2Val == 0) {
//    fA = 0;
//  }
//
//  // RUN
//  if (spd > 0 && ir1Val == 1 && ir2Val == 1) {
//    fA = -5;
//    delay(10);
//    analogWrite(motor, spd);
//  }
//
//  if (spd <= 0 && ir1Val == 1 && ir2Val == 1) {
//    fA = 0;
//  }
//
//  // polarity zmena
//  if (spd == 255 && ir1Val == 0 || spd == 255 && ir2Val == 0 ) {
//    if (relay_s == LOW)
//      relay_s = HIGH;
//    else
//      relay_s = LOW;
//    digitalWrite(relay, relay_s);
//    mp3Ctl();
//    delay(1000);
//    zhasnout();
//    delay(2000 + random(1000, 2000));
//    state = 0;
//  }
//
//}
//
//
//void rozjezd() {
//  spd = spd + fA;
//
//  if (spd > 0 && svetlo_s == 0) {
//    rozsvitit();
//    delay(1000);
//    mp3Ctl();
//  }
//  if (spd > 0 && svetlo_s == 1) {
//    fA = -15;
//    delay(10);
//    analogWrite(motor, spd);
//  }
//  else if (spd >= 0) {
//    fA = 0;
//    state = 1;
//  }
//}

void mp3Ctl() {
  digitalWrite(mp3, HIGH);
  delay(100);
  digitalWrite(mp3, LOW);
}

void rozsvitit() {
  digitalWrite(svetlo, LOW);
  delay(50);
  digitalWrite(svetlo, HIGH);
  delay(50);
  digitalWrite(svetlo, LOW);
  delay(50);
  digitalWrite(svetlo, HIGH);
  delay(100);
  digitalWrite(svetlo, LOW);
  svetlo_s = 1;

}

void zhasnout() {
  digitalWrite(svetlo, HIGH);
  delay(50);
  digitalWrite(svetlo, LOW);
  delay(50);
  digitalWrite(svetlo, HIGH);
  svetlo_s = 0;
}

