/*
 *NeverMung - Copyright 2011 Hugh Coleman
 *
 *This file is part of NeverMung.
 *
 *NeverMung is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *NeverMung is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with NeverMung.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/interrupt.h>
#include <util/atomic.h>
int task;
int ntasks;
struct task {
int yield;
char* flag;
u_intchar sp;
uint16_t stack_max;
uint16_t stack_top;
void (*init_function)();
} tasks[8];


void save_context()
{
  asm volatile (
    "push __temp_reg__\n\t"
    "in __temp_reg__, __SREG__\n\t"
    "cli\n\t"
    "push __temp_reg__\n\t"
    "push r1\n\t"
    "clr r1\n\t"
    "push r2\n\t"
    "push r3\n\t"
    "push r4\n\t"
    "push r5\n\t"
    "push r6\n\t"
    "push r7\n\t"
    "push r8\n\t"
    "push r9\n\t"
    "push r10\n\t"
    "push r11\n\t"
    "push r12\n\t"
    "push r13\n\t"
    "push r14\n\t"
    "push r15\n\t"
    "push r16\n\t"
    "push r17\n\t"
    "push r18\n\t"
    "push r19\n\t"
    "push r20\n\t"
    "push r21\n\t"
    "push r22\n\t"
    "push r23\n\t"
    "push r24\n\t"
    "push r25\n\t"
    "push r26\n\t"
    "push r27\n\t"
    "push r28\n\t"
    "push r29\n\t"
    "push r30\n\t"
    "push r31\n\t"
    "lds r26, %0\n\t"
    "in r0, __SP_L__\n\t"
    "st x+, r0\n\t"
    "in r0, __SP_H__\n\t"
    "st x, r0\n\t"
    : : "x" (&(tasks[task].sp)));
}

void load_context()
{
  asm volatile (
  "lds r26, %0\n\t" 
  "ld  r28, x+\n\t"
  "out __SP_L__, r28\n\t"
  "ld r28, x\n\t"
  "out __SP_H__, r28\n\t"
  "pop r31\n\t"
  "pop r30\n\t"
  "pop r29\n\t"
  "pop r28\n\t"
  "pop r27\n\t"
  "pop r26\n\t"
  "pop r25\n\t"
  "pop r24\n\t"
  "pop r23\n\t"
  "pop r22\n\t"
  "pop r21\n\t"
  "pop r20\n\t"
  "pop r19\n\t"
  "pop r18\n\t"
  "pop r17\n\t"
  "pop r16\n\t"
  "pop r15\n\t"
  "pop r14\n\t"
  "pop r13\n\t"
  "pop r12\n\t"
  "pop r11\n\t"
  "pop r10\n\t"
  "pop r9\n\t"
  "pop r8\n\t"
  "pop r7\n\t"
  "pop r6\n\t"
  "pop r5\n\t"
  "pop r4\n\t"
  "pop r3\n\t"
  "pop r2\n\t"
  "pop r1\n\t"
  "pop r0\n\t"
  "out SREG, r0\n\t"
  "pop r0\n\t"
  : : "x" (&(tasks[task].sp)));
}

void uc_setup()
{
  sei(); //enable interrupts
  TCCR0A |= (1<<WGM01); //set clear timer on compare match mode
  TCCR0B |= (1<<CS02); //set timer prescaler to clk/256 (31.25KHz, with internal 8MHz clock)
  OCR0A  = 0xF; //no. clock ticks per os tick (experiment)
  TIMSK0 |= (1<<OCIE0A); //enable timer interrupt
}
void selecttask()
{
  task++;
  task%=ntasks;
  while (tasks[task].yield!=0) {
    if (tasks[task].yield>0)
      tasks[task].yield--;
    task++;
    task%=ntasks;
  }
}
ISR(TIMER0_COMPA_vect) __attribute__((signal, naked))
{
  yield();
  asm volatile( "reti" );
}
yield() __attribute__((naked))
{
  save_context();
  selecttask();
  load_context();
asm volatile("ret");
}
create_task(uint16_t padding, void (*init_function)())
{
  tasks[task].sp=tasks[task-1].stack_top - tasks[task-1].stack_max;
  asm volatile(
    "lds r26, %0\n\t"
    "ld r28, x+\n\t"
    "out __SP_L__, r28\n\t"
    "ld r28, x\n\t"
    "out __SP_H__, r28\n\t"
    : : "x" (&(tasks[task].sp)));
  init_function(); 
}
void main()
{
  ntasks=0;
  task=0;
  save_context();
  while (ntasks<TASKS)
  {
    ntasks++;
    task=ntasks;
    create_task(10,task.[ntasks]init);
    yield(-1);
  }
