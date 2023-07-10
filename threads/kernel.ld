
SECTIONS {                  /* arquivo kernel.ld para QEMU arm virt */
   /*
    * Vetor de reset
    */
   . = 0;
   .reset : { *(.reset) }

   /*
    * Segmentos text e data
    */
   . = 0x40000000; 
  .text : { *(.text) }
  .data : { *(.data) }

  /*
   * Segmento bss
   */
  inicio_bss = .;
  .bss : { *(.bss) }
  . = ALIGN(4); 
  fim_bss = .;
  heap_start = .;

  /*
   * Reserva espaço para as pilhas
   * Dois threads e o sistema.
   */
  . = . + 4096;
  . = ALIGN(8);
  stack_usr1 = .;

  . = . + 4096;
  . = ALIGN(8);
  stack_usr2 = .;

  . = . + 4096;
  . = ALIGN(8);
  stack_svr = .;
}
