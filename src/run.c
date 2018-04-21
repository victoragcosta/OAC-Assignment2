#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "memory.h"

#define REG_BANK_SIZE 32

enum REG_NAMES
{
    zero, at, v0, v1, a0, a1, a2, a3, t0, t1,
    t2, t3, t4, t5, t6, t7, s0, s1, s2, s3,
    s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra
};
int32_t reg[REG_BANK_SIZE];
uint32_t pc = 0, ri = 0, hi, lo;

int8_t end_prog = 0;

/*
Formatos de Instrucao
            | 6 bits | 5 bits | 5 bits | 5 bits | 5 bits | 6 bits|
    TIPO-R: | opcode | rs     | rt     | rd     | shamnt | funct |
    TIPO-I: | opcode | rs     | rt     |           Imm           |
    TIPO-J: | opcode |                  Endereco                 |
*/

uint8_t opcode; //6 bits
uint8_t rs;     //5 bits
uint8_t rt;     //5 bits
uint8_t rd;     //5 bits
int8_t shamnt;  //5 bits
uint8_t funct;  //6 bits
int16_t k16;    //16 bits
uint32_t k26;   //26 bits

enum OPCODES
{ // Considere apenas os primeiros 6 bits alinhados a direita
    EXT = 0x00,  LW = 0x23,    LB = 0x20,   LBU = 0x24,  LH = 0x21,
    LHU = 0x25,  LUI = 0x0F,   SW = 0x2B,   SB = 0x28,   SH = 0x29,
    BEQ = 0x04,  BNE = 0x05,   BLEZ = 0x06, BGTZ = 0x07, ADDI = 0x08,
    ADDIU = 0x09,
    SLTI = 0x0A, SLTIU = 0x0B, ANDI = 0x0C, ORI = 0x0D,  XORI = 0x0E,
    J = 0x02,    JAL = 0x03
};

enum FUNCTS
{
    ADDU = 0x21,
    ADD = 0x20, SUB = 0x22, MULT = 0x18, DIV = 0x1A,     AND = 0x24,
    OR = 0x25,  XOR = 0x26, NOR = 0x27,  SLT = 0x2A,     JR = 0x08,
    SLL = 0x00, SRL = 0x02, SRA = 0x03,  SYSCALL = 0x0c, MFHI = 0x10,
    MFLO = 0x12
};

enum SYSCALLS
{
    PRINT_INT = 1,
    PRINT_STRING = 4,
    END = 10
};

void fetch()
{
    ri = lw(pc, 0);
    pc += 4;
}

void decode()
{
    opcode = ri >> (32 - 6);
    rs = (ri >> (32 - 6 - 5 * 1)) & 0x1F;
    rt = (ri >> (32 - 6 - 5 * 2)) & 0x1F;
    rd = (ri >> (32 - 6 - 5 * 3)) & 0x1F;
    shamnt = (ri >> (32 - 6 - 5 * 4)) & 0x1F;
    funct = ri & 0x3F;
    k16 = ri & 0x0000FFFF;
    k26 = ri & 0x03FFFFFF;
}

void execute()
{
    switch (opcode)
    {
    case EXT:
        switch (funct)
        {
        /* TIPO-R: |op|rs|rt|rd|shamnt|funct| */
        case ADDU:
        case ADD:
            reg[rd] = reg[rs] + reg[rt];
            break;
        case SUB:
            reg[rd] = reg[rs] - reg[rt];
            break;
        case MULT:
        {
            int32_t *p;
            int64_t aux = ((int64_t)reg[rs]) * ((int64_t)reg[rt]);
            p = (int32_t*)&aux;
            lo = *(p + 0);
            hi = *(p + 1);
        }
        break;
        case DIV:
            lo = reg[rs] / reg[rt];
            hi = reg[rs] % reg[rt];
            break;
        case AND:
            reg[rd] = reg[rs] & reg[rt];
            break;
        case OR:
            reg[rd] = reg[rs] | reg[rt];
            break;
        case XOR:
            reg[rd] = reg[rs] ^ reg[rt];
            break;
        case NOR:
            reg[rd] = ~(reg[rs] | reg[rt]);
            break;
        case SLT:
            reg[rd] = reg[rs] < reg[rt] ? 1 : 0;
            break;
        case JR: /* PC <- rs */
            pc = reg[rs];
            break;
        case SLL:
            reg[rd] = reg[rt] << shamnt;
            break;
        case SRL:
            reg[rd] = (int32_t) ( ((uint32_t) reg[rt]) >> shamnt);
            break;
        case SRA:
            reg[rd] = reg[rt] >> shamnt;
            break;
        case SYSCALL:
            switch(reg[v0]){
                case PRINT_INT:
                    printf("%d", reg[a0]);
                    break;
                case PRINT_STRING:
                {
                    int count = 0;
                    char get;
                    get = lb(reg[a0], count++);
                    while (get != '\0')
                    {
                        printf("%c", get);
                        get = lb(reg[a0], count++);
                    }
                }
                break;
                case END:
                    end_prog = 1;
                    break;
                default:
                    printf("Syscall nao implementada ou nao existe!\n");
                    exit(404);
            }
            break;
        case MFHI:
            reg[rd] = hi;
            break;
        case MFLO:
            reg[rd] = lo;
            break;
        default:
            printf("Erro: funct invalido!");
            exit(404);
        }
        break;
    case LW: //TIPO-I
        //printf("lw $%d, %d($%d)\n", rt,k16,rs);
        reg[rt] = lw(reg[rs], k16);
        break;
    case LH:
        reg[rt] = lh(reg[rs], k16);
        break;
    case LHU:
        reg[rt] = lhu(reg[rs], k16);
        break;
    case LB:
        reg[rt] = lb(reg[rs], k16);
        break;
    case LBU:
        reg[rt] = lbu(reg[rs], k16);
        break;
    case LUI:
        reg[rt] = (k16 << 16) & 0xFFFF0000;
        break;
    case SW:
        sw(reg[rs], k16, reg[rt]);
        break;
    case SH:
        sh(reg[rs], k16, reg[rt]);
        break;
    case SB:
        sb(reg[rs], k16, reg[rt]);
        break;
    case BEQ: /* beq rt, rs, imm  =>  |op|rs|rt|imm| */
        if (reg[rt] - reg[rs] == 0)
            pc += ((int32_t)k16) << 2;
        break;
    case BNE: /* bne rt, rs, imm  =>  |op|rs|rt|imm| */
        if (reg[rt] - reg[rs] != 0)
            pc += ((int32_t)k16) << 2;
        break;
    case BLEZ: /* blez rs, imm => |op|rs|0|imm| */
        if (reg[rs] <= 0)
            pc += ((int32_t)k16) << 2;
        break;
    case BGTZ: /* blez rs, imm => |op|rs|0|imm| */
        if (reg[rs] >= 0)
            pc += ((int32_t)k16) << 2;
        break;
    case ADDIU:
    case ADDI: /* addi rt, rs, imm => |op|rs|rt|imm| */
        reg[rt] = reg[rs] + k16;
        break;
    case SLTI: /* slti rt, rs, imm => |op|rs|rt|imm| */
        reg[rt] = (reg[rs] < k16) ? 1 : 0;
        break;
    case SLTIU: /* sltiu rt, rs, imm => |op|rs|rt|imm| */
        reg[rt] = (reg[rs] < ((uint32_t)((int32_t)k16))) ? 1 : 0;
        break;
    case ANDI: /* andi rt, rs, imm => |op|rs|rt|imm| */
        reg[rt] = reg[rs] & ((uint16_t)k16);
        break;
    case ORI: /* ori rt, rs, imm => |op|rs|rt|imm| */
        reg[rt] = reg[rs] | ((uint16_t)k16);
        break;
    case XORI: /* xori rt, rs, imm => |op|rs|rt|imm| */
        reg[rt] = reg[rs] ^ ((uint16_t)k16);
        break;
    case J: /* j imm => |op|imm| -> 4 bits de pc seguido de k26 e mais 2 0 */
        pc = ((pc & 0xF0000000) + (k26 << 2)) & 0xFFFFFFFC;
        break;
    case JAL: /* jal imm => |op|imm| -> 4 bits dde pc seguido de k26 e mais 2 0 */
        reg[ra] = pc;
        pc = ((pc & 0xF0000000) + (k26 << 2)) & 0xFFFFFFFC;
        break;
    default:
        printf("Erro: instrucao invalida! Codigo 1");
        exit(404);
    }
}

void step()
{
    fetch();
    decode();
    execute();
}

void prepare_run()
{
    end_prog = 0;
    reg[zero] = 0x0000;
    reg[sp] = 0x3FFC;
    reg[gp] = 0x1800;
    pc = 0;
}

void run()
{
    prepare_run();
    while (pc <= 0x0FFC && !end_prog)
    {
        step();
        //printf("[ri = %08X]\n", ri);
    }
}

void dump_reg(char format)
{
    int i;
    for (i = 0; i < REG_BANK_SIZE; i++)
    {
        printf(((format == 'h') | (format == 'H')) ? "$%d = %08X\n" : "$%d = %d\n", i, reg[i]);
    }
    printf(((format == 'h') | (format == 'H')) ? "pc = %08X\n" : "pc = %d\n", pc);
    printf(((format == 'h') | (format == 'H')) ? "ri = %08X\n" : "ri = %d\n", ri);
    printf(((format == 'h') | (format == 'H')) ? "hi = %08X\n" : "hi = %d\n", hi);
    printf(((format == 'h') | (format == 'H')) ? "lo = %08X\n" : "lo = %d\n", lo);
}
