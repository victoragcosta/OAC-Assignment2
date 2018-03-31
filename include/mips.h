#ifndef MIPS_H_
#define MIPS_H_
#include <stdint.h>

// imprime o conteúdo da memória no formato hexa, palavra por palavra.
void dump_mem(uint32_t add, uint32_t size);

// lê um inteiro alinhado - endereços múltiplos de 4
int32_t lw(uint32_t address, int16_t kte);

// lê meia palavra, 16 bits - retorna inteiro com sinal
int32_t lh(uint32_t address, int16_t kte);

// lê meia palavra, 16 bits formato inteiro sem sinal
uint32_t lhu(uint32_t address, int16_t kte);

// lê um byte - retorna inteiro com sinal
int32_t lb(uint32_t address, int16_t kte);

// lê um byte - 8 bits formato inteiro sem sinal
int32_t lbu(uint32_t address, int16_t kte);

// escreve um inteiro alinhado na memória - endereços múltiplos de 4
void sw(uint32_t address, int16_t kte, int32_t dado);

// escreve meia palavra, 16 bits - endereços múltiplos de 2
void sh(uint32_t address, int16_t kte, int16_t dado);

// escreve um byte na memória
void sb(uint32_t address, int16_t kte, int8_t dado);

#endif /*MIPS_H_*/