#ifndef RUN_H_
#define RUN_H_

#define REG_BANK_SIZE 32

void fetch();  /* Copy 0(pc) to ri and increments pc by 4 */
void decode();  /* Extracts all instructions parameters possible from ri */
void execute();  /* Runs the instruction on ri (uses parameters decoded) */
void step();  /* runs one fetch, decode, execute cycle */
void run();  /* runs step until the text segment ends or syscall to stop happens */

void dump_reg(char format);  /* prints registers contents to the screen */

void prepare_run();  /* prepares all globals for a simulation */

#endif /*RUN_H_*/