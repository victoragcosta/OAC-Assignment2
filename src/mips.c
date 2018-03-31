#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Memória de 16KiB ou 4Ki Words
#define MEM_SIZE 4096
int32_t mem[MEM_SIZE];

// Pedido: imprime o conteúdo da memória no formato hexa, palavra por palavra.
/*
    Essa função irá imprimir na tela todas as palavras contidas do endereço
    fornecido até o endereço mais o tamanho pedido.

    add: Endereço inicial em bytes. Múltiplo de 4.
    size: Tamanho do segmento para dar dump em bytes. Múltiplo de 4.
*/
void dump_mem(uint32_t add, uint32_t size)
{
    // Asserts
    if (size < 0){
        printf("Erro: tamanho deve ser maior ou igual a 0\n");
        return;
    }
    if (add < 0 || add > (MEM_SIZE - 1) * 4 || add + size > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return;
    }
    if( add & 3 || size & 3 ){
        printf("Erro: acesso não alinhado.\nO endereço e a quantidade de dados precisam ser múltiplos de 4.\n");
        return; // Aborta a execução do dump
    }
    
    // Corpo da função
    int i, accessing;
    uint32_t toPrint, remaining = size, actualByte = add;
    uint8_t *mem8 = (uint8_t*) mem;
    
    while(remaining > 0){
        toPrint = 0;
        accessing = actualByte/4;
        for(i = 0; i < 4 && remaining > 0; i++, remaining--, actualByte++){
            toPrint += (uint32_t)mem8[actualByte]<<(8*i);
        }
        printf("mem[%d] = %08X\n", accessing, toPrint);
    }
    return;
}

// lê um inteiro alinhado - endereços múltiplos de 4
int32_t lw(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return 0;
    }
    if (add & 3)
    {
        printf("Erro: acesso não alinhado.\nO endereço + offset de dados precisam ser múltiplos de 4.\n");
        return 0;
    }

    // Corpo da função
    add = add>>2; // Garanto alinhamento
    int32_t dado;
    dado = (int32_t)mem[add]; // converto endereço de byte para word
    return dado;
}

// lê meia palavra, 16 bits - retorna inteiro com sinal
int32_t lh(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return 0;
    }
    if (add & 1){
        printf("Erro: acesso não alinhado.\nO endereço + offset de dados precisam ser múltiplos de 2.\n");
        return 0;
    }

    // Corpo da função
    add = add>>1; // Garanto alinhamento
    int16_t *mem16 = (int16_t *)mem;
    int32_t dado;
    dado = (int32_t)mem16[add]; // converto endereço de byte para half-word
    return dado;
}

// lê meia palavra, 16 bits formato inteiro sem sinal
uint32_t lhu(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return 0;
    }
    if (add & 1){
        printf("Erro: acesso não alinhado.\nO endereço + offset de dados precisam ser múltiplos de 2.\n");
        return 0;
    }

    // Corpo da função
    add = add>>1; // Garanto alinhamento
    uint16_t *mem16 = (uint16_t *)mem;
    uint32_t dado;
    dado = (uint32_t)mem16[add]; // converto endereço de byte para half-word
    return dado;
}

// lê um byte - retorna inteiro com sinal
int32_t lb(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return 0;
    }

    // Corpo da função
    int8_t *mem8 = (int8_t *)mem;
    int32_t dado;
    dado = (int32_t)mem8[add];
    return dado;
}

// lê um byte - 8 bits formato inteiro sem sinal
uint32_t lbu(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return 0;
    }

    // Corpo da função
    uint8_t *mem8 = (uint8_t*) mem;
    uint32_t dado;
    dado = (uint32_t)mem8[add];
    return dado;
}

// escreve um inteiro alinhado na memória - endereços múltiplos de 4
void sw(uint32_t address, int16_t kte, int32_t dado)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return;
    }
    if ( add & 3 ){
        printf("Erro: escrita não alinhada.\nO endereço + offset de dados precisam ser múltiplos de 4.\n");
        return; // Aborta a execução do save
    }

    // Corpo da função
    add = ((add>>2)<<2);
    uint8_t *dado8 = (uint8_t *)&dado, *mem8 = (uint8_t *)mem;
    mem8[add + 0] = dado8[0];
    mem8[add + 1] = dado8[1];
    mem8[add + 2] = dado8[2];
    mem8[add + 3] = dado8[3];
}

// escreve meia palavra, 16 bits - endereços múltiplos de 2
void sh(uint32_t address, int16_t kte, int16_t dado)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return;
    }
    if ( add & 1 ) {
        printf("Erro: escrita não alinhada.\nO endereço + offset de dados precisam ser múltiplos de 2.\n");
        return; // Aborta a execução do save
    }
    add = ((add >> 1) << 1);

    // Corpo da função
    add = (add>>1)<<1;
    uint8_t *dado8 = (uint8_t *)&dado, *mem8 = (uint8_t *)mem;
    mem8[add + 0] = dado8[0];
    mem8[add + 1] = dado8[1];
}

// escreve um byte na memória
void sb(uint32_t address, int16_t kte, int8_t dado)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > (MEM_SIZE - 1) * 4){
        printf("Erro: acesso a endereços inexistentes!\n");
        return;
    }

    // Corpo da função
    uint8_t *mem8 = (uint8_t*)mem;
    mem8[add] = dado;
}