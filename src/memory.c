#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Memoria de 16KiB ou 4Ki Words
#define MEM_SIZE 4096
int32_t mem[MEM_SIZE];

// carrega dados de arquivo binario na memoria de text (0x0000 - 0x1000)
void load_text(char *arq_name)
{
    FILE *arq;
    arq = fopen(arq_name, "rb");
    if(arq == NULL){
        printf("Arquivo nao existe ou nao eh possivel abrir!\n");
        exit(404);
    }

    int write_add = 0x0000;
    do{
        fread(mem + write_add, sizeof(int32_t), 1, arq);
        write_add++;
    }while(!feof(arq));

    fclose(arq);
}

// carrega dados de arquivo binario na memoria de dados (0x2000 - 0x4000)
void load_data(char *arq_name)
{
    FILE *arq;
    arq = fopen(arq_name, "rb");
    if(arq == NULL){
        printf("Arquivo nao existe ou nao eh possivel abrir!\n");
        exit(404);
    }
    
    int write_add = 0x2000>>2;
    do{
        fread(mem + write_add, sizeof(int32_t), 1, arq);
        write_add++;
    }while(!feof(arq));

    fclose(arq);
}

void dump_mem(int start, int end, char format)
{
    // Asserts
    if (start < 0 || start > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return;
    }
    if (end < 0 || end > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return;
    }
    if( start > end ){
        printf("Erro: start precisa ser menor que end!\n");
        return; // Aborta a execucao do dump
    }
    int formato;
    switch(format){
        case 'd':
        case 'D':
            formato = 0;
            break;
        case 'h':
        case 'H':
            formato = 1;
            break;

        default:
        printf("Erro: format precisa ser necessariamente 'h' ou 'd'\n");
        return;
    }

    // Corpo da funcao
    int i, accessing;
    uint32_t toPrint, remaining = end - start, actualByte = start;
    uint8_t *mem8 = (uint8_t*) mem;
    
    while(remaining > 0){
        toPrint = 0;
        accessing = actualByte/4;
        for(i = 0; i < 4 && remaining > 0; i++, remaining--, actualByte++){
            toPrint += (uint32_t)mem8[actualByte]<<(8*i);
        }
        printf(formato ? "mem[%d] = %08X\n" : "mem[%d] = %d\n", accessing, toPrint);
    }
    return;
}

// lê um inteiro alinhado - enderecos multiplos de 4
int32_t lw(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return 0;
    }
    if (add % 4)
    {
        printf("Erro: acesso nao alinhado.\nO endereco + offset de dados precisam ser multiplos de 4.\n");
        return 0;
    }

    // Corpo da funcao
    add = add>>2; // Garanto alinhamento
    int32_t dado;
    dado = (int32_t)mem[add]; // converto endereco de byte para word
    return dado;
}

// lê meia palavra, 16 bits - retorna inteiro com sinal
int32_t lh(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return 0;
    }
    if (add & 1){
        printf("Erro: acesso nao alinhado.\nO endereco + offset de dados precisam ser multiplos de 2.\n");
        return 0;
    }

    // Corpo da funcao
    add = add>>1; // Garanto alinhamento
    int16_t *mem16 = (int16_t *)mem;
    int32_t dado;
    dado = (int32_t)mem16[add]; // converto endereco de byte para half-word
    return dado;
}

// lê meia palavra, 16 bits formato inteiro sem sinal
uint32_t lhu(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return 0;
    }
    if (add & 1){
        printf("Erro: acesso nao alinhado.\nO endereco + offset de dados precisam ser multiplos de 2.\n");
        return 0;
    }

    // Corpo da funcao
    add = add>>1; // Garanto alinhamento
    uint16_t *mem16 = (uint16_t *)mem;
    uint32_t dado;
    dado = (uint32_t)mem16[add]; // converto endereco de byte para half-word
    return dado;
}

// lê um byte - retorna inteiro com sinal
int32_t lb(uint32_t address, int16_t kte)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return 0;
    }

    // Corpo da funcao
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
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return 0;
    }

    // Corpo da funcao
    uint8_t *mem8 = (uint8_t*) mem;
    uint32_t dado;
    dado = (uint32_t)mem8[add];
    return dado;
}

// escreve um inteiro alinhado na memoria - enderecos multiplos de 4
void sw(uint32_t address, int16_t kte, int32_t dado)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return;
    }
    if ( add % 4 ){
        printf("Erro: escrita nao alinhada.\nO endereco + offset de dados precisam ser multiplos de 4.\n");
        return; // Aborta a execucao do save
    }

    // Corpo da funcao
    add = ((add>>2)<<2);
    uint8_t *dado8 = (uint8_t *)&dado, *mem8 = (uint8_t *)mem;
    mem8[add + 0] = dado8[0];
    mem8[add + 1] = dado8[1];
    mem8[add + 2] = dado8[2];
    mem8[add + 3] = dado8[3];
}

// escreve meia palavra, 16 bits - enderecos multiplos de 2
void sh(uint32_t address, int16_t kte, int16_t dado)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return;
    }
    if ( add & 1 ) {
        printf("Erro: escrita nao alinhada.\nO endereco + offset de dados precisam ser multiplos de 2.\n");
        return; // Aborta a execucao do save
    }
    add = ((add >> 1) << 1);

    // Corpo da funcao
    add = (add>>1)<<1;
    uint8_t *dado8 = (uint8_t *)&dado, *mem8 = (uint8_t *)mem;
    mem8[add + 0] = dado8[0];
    mem8[add + 1] = dado8[1];
}

// escreve um byte na memoria
void sb(uint32_t address, int16_t kte, int8_t dado)
{
    int add = address + kte;
    // Asserts
    if (add < 0 || add > MEM_SIZE*4){
        printf("Erro: acesso a enderecos inexistentes!\n");
        return;
    }

    // Corpo da funcao
    uint8_t *mem8 = (uint8_t*)mem;
    mem8[add] = dado;
}