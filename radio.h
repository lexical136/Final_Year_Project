#ifndef RADIO_H_
#define RADIO_H_

#define RADIO_MAX_LEN 128

void radio_init(unsigned short macaddr);
void radio_start();
unsigned char radio_tx_done();
void radio_send(unsigned char *buffer, unsigned char len, unsigned short dst);
unsigned char radio_rxed();
unsigned char radio_recv(unsigned char *buffer, unsigned char len,
	unsigned short *dst, unsigned short *src,
	unsigned char *valid_crc, unsigned char *energy, unsigned char *lqi);
unsigned char radio_raw_recv(unsigned char *buffer, unsigned char len,
	unsigned char *valid_crc, unsigned char *energy, unsigned char *lqi);
void radio_raw_send(unsigned char *buffer, unsigned char len);

// level   actual power
// -----   ------------
//     0   3.0 dBm
//     1   2.8 dBm
//     2   2.3 dBm
//     3   1.8 dBm
//     4   1.3 dBm
//     5   0.7 dBm
//     6   0.0 dBm
//     7  -1   dBm
//     8  -2   dBm
//     9  -3   dBm
//    10  -4   dBm
//    11  -5   dBm
//    12  -7   dBm
//    13  -9   dBm
//    14 -12   dBm
//    15 -17   dBm
void radio_set_power(unsigned char level);

unsigned char radio_rssi();
char radio_max_rssi();

unsigned char radio_latest_energy();
unsigned char radio_latest_lqi();

// 0=250kbps, 1=500, 2=1Mbps, 3=2Mbps
void radio_set_hdr(unsigned char rate);

// 11-26 are supported
void radio_set_channel(unsigned char chan);

#endif /* RADIO_H_ */
