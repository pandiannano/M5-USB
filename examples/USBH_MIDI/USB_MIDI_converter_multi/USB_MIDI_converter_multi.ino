/*
 *******************************************************************************
 * USB-MIDI to Legacy Serial MIDI converter
 * Copyright (C) 2012-2021 Yuuichi Akagawa
 *
 * Idea from LPK25 USB-MIDI to Serial MIDI converter
 *   by Collin Cunningham - makezine.com, narbotic.com
 *
 * This is sample program. Do not expect perfect behavior.
 *******************************************************************************
 */

#include <usbh_midi.h>
#include <m5_usbhub.h>

#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#else
#define _MIDI_SERIAL_PORT Serial
#endif

// Set to 1 if you want to wait for the Serial MIDI transmission to complete.
// For more information, see https://github.com/felis/USB_Host_Shield_2.0/issues/570
#define ENABLE_MIDI_SERIAL_FLUSH 0

//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USB Usb;
USBHub  Hub1(&Usb);
USBH_MIDI  Midi1(&Usb);
USBH_MIDI  Midi2(&Usb);

void MIDI_poll(USBH_MIDI &Midi);

void setup()
{
  _MIDI_SERIAL_PORT.begin(31250);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  Usb.Task();

  if ( Midi1 ) {
    MIDI_poll(Midi1);
  }
  if ( Midi2 ) {
    MIDI_poll(Midi2);
  }
  //delay(1ms) if you want
  //delayMicroseconds(1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll(USBH_MIDI &Midi)
{
  uint8_t outBuf[ 3 ];
  uint8_t size;

  do {
    if ( (size = Midi.RecvData(outBuf)) > 0 ) {
      //MIDI Output
      _MIDI_SERIAL_PORT.write(outBuf, size);
#if ENABLE_MIDI_SERIAL_FLUSH
      _MIDI_SERIAL_PORT.flush();
#endif
    }
  } while (size > 0);
}

// Delay time (max 16383 us)
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime)
{
  uint32_t t3;

  t3 = t2 - t1;
  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}
