#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mips.h"

// Memória de 16KiB ou 4Ki Words
#define MEM_SIZE 4096

int main(int argc, char *argv[])
{

    // Iniciar testes

    printf("Testes do Professor\n\n");
    // Teste 1: Escrita (Save)
    sb(0, 0, 0x04); sb(0, 1, 0x03); sb(0, 2, 0x02); sb(0, 3, 0x01);
    sb(4, 0, 0xFF); sb(4, 1, 0xFE); sb(4, 2, 0xFD); sb(4, 3, 0xFC);
    sh(8, 0, 0xFFF0); sh(8, 2, 0x8C);
    sw(12, 0, 0xFF);
    sw(16, 0, 0xFFFF);
    sw(20, 0, 0xFFFFFFFF);
    sw(24, 0, 0x80000000);

    // Testar Estado da memória após escrita
    dump_mem(0, 28);
    /*
    deve resultar em : 
    mem[0] = 01020304 
    mem[1] = fcfdfeff
    mem[2] = 008cfff0 
    mem[3] = 000000FF 
    mem[4] = 0000FFFF 
    mem[5] = FFFFFFFF
    mem[6] = 80000000
    */

    // Testar leitura
    printf("| lb(0,0):  0x%08X | lb(0,1):  0x%08X | lb(0,2):  0x%08X | lb(0,3):  0x%08X |\n", lb(0, 0), lb(0, 1), lb(0, 2), lb(0, 3));
    printf("| lb(0,0):  %10d | lb(0,1):  %10d | lb(0,2):  %10d | lb(0,3):  %10d |\n", lb(0, 0), lb(0, 1), lb(0, 2), lb(0, 3));
    
    printf("| lb(4,0):  0x%08X | lb(4,1):  0x%08X | lb(4,2):  0x%08X | lb(4,3):  0x%08X |\n", lb(4, 0), lb(4, 1), lb(4, 2), lb(4, 3));
    printf("| lb(4,0):  %10d | lb(4,1):  %10d | lb(4,2):  %10d | lb(4,3):  %10d |\n", lb(4, 0), lb(4, 1), lb(4, 2), lb(4, 3));
    printf("| lbu(4,0): %10d | lbu(4,1): %10d | lbu(4,2): %10d | lbu(4,3): %10d |\n", lbu(4, 0), lbu(4, 1), lbu(4, 2), lbu(4, 3));

    printf("| lh(8,0):  0x%08X | lh(8,2):  0x%08X |\n", lh(8, 0), lh(8, 2));
    printf("| lh(8,0):  %10d | lh(8,2):  %10d |\n", lh(8, 0), lh(8, 2));
    printf("| lhu(8,0): %10d | lhu(8,2): %10d |\n", lhu(8, 0), lhu(8, 2));

    printf("| lw(12,0): 0x%08X |\n", lw(12, 0));
    printf("| lw(12,0): %10d |\n", lw(12, 0));
    printf("| lw(16,0): 0x%08X |\n", lw(16, 0));
    printf("| lw(16,0): %10d |\n", lw(16, 0));
    printf("| lw(20,0): 0x%08X |\n", lw(20, 0));
    printf("| lw(20,0): %10d |\n", lw(20, 0));

    printf("--------------------------------------------------------------------------------\n");
    // Fim testes do Professor

    // Escrita e leitura de bytes
    printf("Teste de Escrita e leitura de bytes\n\n");
    
    sb(28, 0, 0xF0); sb(28, 1, 0xCA); sb(28, 2, 0xF0); sb(28, 3, 0xFA);
    printf("| lb(28,0):  0x%08X | lb(28,1):  0x%08X | lb(28,2):  0x%08X | lb(28,3):  0x%08X |\n", lb(28, 0), lb(28, 1), lb(28, 2), lb(28, 3));
    printf("| lb(28,0):  %10d | lb(28,1):  %10d | lb(28,2):  %10d | lb(28,3):  %10d |\n", lb(28, 0), lb(28, 1), lb(28, 2), lb(28, 3));
    printf("| lbu(28,0): %10d | lbu(28,1): %10d | lbu(28,2): %10d | lbu(28,3): %10d |\n", lbu(28, 0), lbu(28, 1), lbu(28, 2), lbu(28, 3));

    printf("--------------------------------------------------------------------------------\n");

    // Reescrita e leitura de bytes
    printf("Teste de Reescrita e leitura de bytes\n\n");

    sb(28, 0, 0x0F); sb(28, 1, 0xAC); sb(28, 2, 0x0F); sb(28, 3, 0xAF);
    printf("| lb(28,0):  0x%08X | lb(28,1):  0x%08X | lb(28,2):  0x%08X | lb(28,3):  0x%08X |\n", lb(28, 0), lb(28, 1), lb(28, 2), lb(28, 3));
    printf("| lb(28,0):  %10d | lb(28,1):  %10d | lb(28,2):  %10d | lb(28,3):  %10d |\n", lb(28, 0), lb(28, 1), lb(28, 2), lb(28, 3));
    printf("| lbu(28,0): %10d | lbu(28,1): %10d | lbu(28,2): %10d | lbu(28,3): %10d |\n", lbu(28, 0), lbu(28, 1), lbu(28, 2), lbu(28, 3));

    printf("--------------------------------------------------------------------------------\n");

    // Teste de Limites
    printf("Teste de limites\n\n");
    printf("> Deve imprimir 8 erros e depois 10 erros alternados com zeros\n\n");
    dump_mem(-4, 4);                 // Esse endereço não existe e deve imprimir um erro
    dump_mem(MEM_SIZE * 4, 4);       // Esse endereço não existe e deve imprimir um erro
    sw(-4, 0, 0xFFFFFFFF);           // Esse endereço não existe e deve imprimir um erro
    sw(MEM_SIZE * 4, 0, 0xFFFFFFFF); // Esse endereço não existe e deve imprimir um erro
    sh(-4, 0, 0xFFFF);               // Esse endereço não existe e deve imprimir um erro
    sh(MEM_SIZE * 4, 0, 0xFFFF);     // Esse endereço não existe e deve imprimir um erro
    sb(-4, 0, 0xFF);                 // Esse endereço não existe e deve imprimir um erro
    sb(MEM_SIZE * 4, 0, 0xFF);       // Esse endereço não existe e deve imprimir um erro
    printf("%d\n", lw(-4, 0));           // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lw(MEM_SIZE * 4, 0)); // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lh(-4, 0));               // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lh(MEM_SIZE * 4, 0));     // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lhu(-4, 0));              // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lhu(MEM_SIZE * 4, 0));    // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lb(-4, 0));                 // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lb(MEM_SIZE * 4, 0));       // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lbu(-4, 0));                // Esse endereço não existe e deve imprimir um erro e retornar 0
    printf("%d\n", lbu(MEM_SIZE * 4, 0));      // Esse endereço não existe e deve imprimir um erro e retornar 0

    printf("--------------------------------------------------------------------------------\n");

    // Teste de desalinhamento
    printf("Teste de desalinhamento\n\n");
    
    printf("> Devem ocorrer 7 erros de desalinhamento\n\n");

    sh(3, 0, 0xFFFF); // Esse endereço não está alinhado e deve dar erro
    sh(2, 1, 0xFFFF); // Esse offset não está alinhado e deve dar erro
    sh(4, 3, 0xFFFF); // Esse offset não está alinhado e deve dar erro
    
    sw(3, 0, 0xFFFFFFFF); // Esse endereço não está alinhado e deve dar erro
    sw(4, 1, 0xFFFFFFFF); // Esse offset não está alinhado e deve dar erro
    sw(4, 2, 0xFFFFFFFF); // Esse offset não está alinhado e deve dar erro
    sw(4, 3, 0xFFFFFFFF); // Esse offset não está alinhado e deve dar erro

    printf("\n> Devem ocorrer 10 erros alternados de 0s\n\n");

    printf("%d\n", lh(3, 0)); // Esse endereço não está alinhado e deve dar erro
    printf("%d\n", lh(4, 1)); // Esse offset não está alinhado e deve dar erro
    printf("%d\n", lh(4, 3)); // Esse offset não está alinhado e deve dar erro
    printf("%d\n", lhu(3, 0)); // Esse endereço não está alinhado e deve dar erro
    printf("%d\n", lhu(4, 1)); // Esse offset não está alinhado e deve dar erro
    printf("%d\n", lhu(4, 3)); // Esse offset não está alinhado e deve dar erro

    printf("%d\n", lw(3, 0)); // Esse endereço não está alinhado e deve dar erro
    printf("%d\n", lw(4, 1)); // Esse offset não está alinhado e deve dar erro
    printf("%d\n", lw(4, 2)); // Esse offset não está alinhado e deve dar erro
    printf("%d\n", lw(4, 3)); // Esse offset não está alinhado e deve dar erro

    printf("--------------------------------------------------------------------------------\n");

    // Teste de alinhamento
    printf("Teste de alinhamento\n\n");
    sh(31, 1, 0x4567);      // Grava meia palavra em 32
    sh(35, -1, 0x0123);     // Grava meia palavra em 34
    sw(37, -1, 0x89ABCDEF); // Grava uma palavra em 36
    sw(39, 1, 0xAAAAAAAA);  // Grava uma palavra em 40

    dump_mem(32, 12); // Escreve esse fragmento de memória que acabamos de escrever
    /*
    deve resultar em : 
    mem[8] = 01234567 
    mem[9] = 89ABCDEF
    mem[10] = AAAAAAAA 
    */

    printf("| lh(39, 1):  0x%08X | lh(41, -1):  0x%08X |\n", lh(39, 1), lh(41, -1));
    printf("| lh(39, 1):  %10d | lh(41, -1):  %10d |\n", lh(39, 1), lh(41, -1));
    printf("| lhu(39, 1): %10d | lhu(41, -1): %10d |\n", lhu(39, 1), lhu(41, -1));

    printf("| lw(37, -1): 0x%08X |\n", lw(37, -1));
    printf("| lw(37, -1): %10d |\n", lw(37, -1));

    printf("--------------------------------------------------------------------------------\n");

    return 0;
}

/*
1. Iniciar a memória: executar a seguinte sequência de operações de escrita.
a.sb(0, 0, 0x04); sb(0, 1, 0x03); sb(0, 2, 0x02); sb(0, 3, 0x01);
b.sb(4, 0, 0xFF); sb(4, 1, 0xFE); sb(4, 2, 0xFD); sb(4, 3, 0xFC);
c.sh(8, 0, 0xFFF0); sh(8, 2, 0x8C);
d.sw(12, 0, 0xFF);
e.sw(16, 0, 0xFFFF);
f.sw(20, 0, 0xFFFFFFFF);
g.sw(24, 0, 0x80000000);
2. Imprimir o conteúdo da memória:
a. dump_mem(0, 28);
b. deve resultar em:
mem[0] = 01020304
mem[1] = fcfdfeff
mem[2] = 008cfff0
mem[3] = 000000FF
mem[4] = 0000FFFF
mem[5] = FFFFFFFF
mem[6] = 80000000
3. Ler os dados da seguinte maneira:
Obs: lb() deve ser impresso apenas com dois dígitos em hexa, e lh() apenas
com 4.
a. lb(0,0), lb(0,1), lb(0,2) lb(0,3) : imprimir em hexa e decimal
b. lb(4,0), lb(4,1), lb(4,2) lb(4,3) : imprimir em hexa e decimal
c. lbu(4,0), lbu(4,1), lbu(4,2) lbu(4,3) : imprimir em decimal
d. lh(8,0), lh(8,2) : imprimir em hexa e decimal
e. lhu(8,0), lhu(8,2) : imprimir em decimal
f. lw(12,0), lw(16, 0), lw(20,0) : imprimir em hexa e decimal
*/