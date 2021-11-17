#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;
teensydmx::Sender dmxTx{Serial1};


int address = 3;
int base_midi_note = 60;

void setup() {
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleControlChange(myControlChange);

  Serial.begin(115200);
  //  Serial.begin(9600);
  //  Serial3.begin(9600);
  Serial2.begin(9600); //relay module are connected to Serial_1
  //  Serial3.write(alloff, sizeof(alloff));

  for (int i = 0; i <= 24; i++) {
    dmxTx.set(i + address, 0);
    dmxTx.begin();
    delay(10);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWriteFast(LED_BUILTIN, HIGH);
}

void loop() {
  usbMIDI.read();
}

void myNoteOn(byte channel, byte note, byte velocity) {
  uint8_t c = note - base_midi_note;
  if (c < 24) {
    dmxTx.set(c + address, velocity * 2);
    dmxTx.begin();
  }
  
  Serial.print("Note On, c=");
  Serial.print(c);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);

  Serial2.write(note | (1 << 7));
  // Serial2.println("hello");
}

void myNoteOff(byte channel, byte note, byte velocity) {
  uint8_t c = note - base_midi_note;
  if (c < 24) {
    dmxTx.set(c + address, 0);
    dmxTx.begin();
  }

  Serial.print("Note Off, c=");
  Serial.print(c);
  Serial.print(", velocity=");

  Serial2.write(note);
  // Serial3.println("bye");
}

void myControlChange(byte channel, byte control, byte value) {

}
