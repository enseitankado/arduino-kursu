void setup() {
  Serial.begin(9600);
  for (byte pin = 2; pin <= 12; pin = pin + 1)
    pinMode(pin, OUTPUT);
}

void yakSondur(byte ledNo) {
  Serial.print(String(ledNo) + "yandi.");
  digitalWrite(ledNo, 1);
  delay(500); //yarım saniye bekle
  digitalWrite(ledNo, 0);
  delay(500); //yarım saniye bekle
  Serial.println(String(ledNo) + "sondu.");
}

void loop() {

  String yon;

  if (Serial.available())
    yon = Serial.readString();

  if (yon == "sag\n")
    for (byte pin = 2 ; pin <= 12; pin++)
      yakSondur(pin);

  if (yon == "sol\n")
    for (byte pin = 12; pin >= 2; pin--)
      yakSondur(pin);
}
