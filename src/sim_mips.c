#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"
#include "run.h"

int main(const int argc, const char *argv[])
{

    /* Carregamos um programa relativamente pequeno (do professor)
    para testar dump_mem, dump_reg e prepare_run*/

    load_text((char *)"./arqs/text.bin");
    load_data((char *)"./arqs/data.bin");
    
    printf("\nText Hexadecimal:\n");
    dump_mem(0x00000000,0x00000050, 'h'); // Text
    printf("\nText Decimal:\n");
    dump_mem(0x00000000,0x00000050, 'd'); // Text
    
    printf("\nData Hexadecimal:\n");
    dump_mem(0x00002000,0x00002060, 'h'); // Data
    printf("\nData Decimal:\n");
    dump_mem(0x00002000,0x00002060, 'd'); // Data
    
    
    prepare_run(); // Inicializa registradores
    printf("\nRegistradores Hexadecimal:\n");
    dump_reg('h'); // Checar inicialização dos registradores
    printf("\nRegistradores Decimal:\n");
    dump_reg('d'); // Checar inicialização dos registradores

    printf("\nExecucao normal do programa:\n");
    run(); // Testar execução
    printf("\n--------------------\n");

    load_text((char *)"./arqs/text1.bin"); // Carrega segmento Text
    load_data((char *)"./arqs/data1.bin"); // Carrega segmento Data
    printf("\nExecucao normal do programa de testes:\n");
    run(); // Testar execução
    printf("\n--------------------\n");

    return 0;
}