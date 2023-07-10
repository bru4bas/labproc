
SECTIONS {                  /* arquivo kernel.ld para placa Evaluator7T */
   . = 0;
   .reset : { *(.reset) }

   . = 0x8000; 
  .text : { *(.text) }
  .data : { *(.data) }
}
