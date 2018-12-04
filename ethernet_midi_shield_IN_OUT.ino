#include <Ethernet.h>
#include <MIDI.h>  // Add Midi Library

#include "AppleMidi.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 100);
EthernetServer server(80);

unsigned long t0 = millis();
bool isConnected = false;

// Created and binds the rtpMIDI interface to the default port and EthernetUdp implementation
APPLEMIDI_CREATE_DEFAULT_INSTANCE();
// Created and binds the MIDI interface to the default hardware Serial port
//MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiBench);
 MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midiA);


// Code size:
//
// IDE 1.8.7
//
// Arduino Ethernet / Uno
// Sketch uses 26192 bytes (81%) of program storage space. Maximum is 32256 bytes.
// Global variables use 1425 bytes (69%) of dynamic memory, leaving 623 bytes for local variables. Maximum is 2048 bytes.

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

  // MyHandleNoteON is the function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
 void handleNoteOn(byte channel, byte note, byte velocity) { 
 AppleMIDI.sendNoteOn(note, velocity, channel);
}

// MyHandleNoteOFF is the function that will be called by the Midi Library
// when a MIDI NOTE OFF message is received.
// * A NOTE ON message with Velocity = 0 will be treated as a NOTE OFF message *
// It will be passed bytes for Channel, Pitch, and Velocity
 void handleNoteOff(byte channel, byte note, byte velocity) { 
  AppleMIDI.sendNoteOff(note, velocity, channel);
}

void setup()
{
  // Serial communications and wait for port to open:
  Serial.begin(31250);
  Ethernet.begin(mac, ip);
  server.begin();
  AppleMIDI.begin("test");
  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);
    midiA.setHandleNoteOn(handleNoteOn);
    midiA.setHandleNoteOff(handleNoteOff); 
    midiA.begin(MIDI_CHANNEL_OMNI);
 


  


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  // Listen to incoming notes
  AppleMIDI.run();

  // Read incoming messages
   midiA.read();
}

// ====================================================================================
// Event handlers for incoming MIDI messages
// ====================================================================================

// -----------------------------------------------------------------------------
// rtpMIDI session. Device connected
// -----------------------------------------------------------------------------
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected = true;
//  Serial.print(F("Connected to session "));
//  Serial.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected = false;
//  Serial.println(F("Disconnected"));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
    byte noteOnStatus=0x90 + (channel-1);  
    
    //Send notes to MIDI output:
    Serial.write(noteOnStatus);
    Serial.write(note);
    Serial.write(velocity);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
   byte noteOffStatus=0x80 + (channel-1);  
    
    //Send notes to MIDI output:
    Serial.write(noteOffStatus);
    Serial.write(note);
    Serial.write(velocity);

}
