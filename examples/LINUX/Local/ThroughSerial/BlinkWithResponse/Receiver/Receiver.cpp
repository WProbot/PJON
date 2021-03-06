
// For printf used below
#include <stdio.h>
// PJON library
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define TS_RESPONSE_TIME_OUT 35000
/* Maximum accepted timeframe between transmission and synchronous
   acknowledgement. This timeframe is affected by latency and CRC computation.
   Could be necessary to higher this value if devices are separated by long
   physical distance and or if transmitting long packets. */

#define PJON_INCLUDE_TS true // Include only ThroughSerial
#include <PJON.h>
PJON<ThroughSerial> bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    printf("BLINK\n");
    bus.reply("B", 1); // Reply B to 45
  }
}

int main() {
  printf("PJON instantiation... \n");
  uint32_t baud_rate = 9600;
  printf("Opening serial... \n");
  int s = serialOpen("/dev/ttyUSB0", baud_rate);
  if(int(s) < 0) printf("Serial open fail!");
  printf("Setting serial... \n");
  bus.strategy.set_serial(s);
  bus.strategy.set_baud_rate(baud_rate);
  bus.set_receiver(receiver_function);

  printf("Opening bus... \n");
  bus.begin();
  printf("Success, initiating BlinkTest Receiver example... \n");

  while(true) {
    bus.update();
    bus.receive(1000);
  }
};
