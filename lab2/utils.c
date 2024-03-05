#include <lcom/lcf.h>

#include <stdint.h>

// LSB -> Less Significant Bits
int util_get_LSB (uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; 
  *lsb = 0xFF & val;         // Coloca no apontador os 8 bits menos significativos do valor
  return 0;
}

// MSB -> Most Significant Bits
int util_get_MSB (uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1; // O apontador deve ser válido
  *msb = (val >> 8) & 0xFF;  // Coloca no apontador os 8 bits mais significativos do valor
  return 0;
}

// Transform 32 bit output in 8 bit output. Error prevention.
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;    // o apontador deve ser válido
  uint32_t val;                   // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val);  // val ficará com o valor lido de port
  *value = 0xFF & val;            // value ficará apenas com os primeiros 8 bits do resultado
  return ret;
}
