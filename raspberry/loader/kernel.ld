
SECTIONS {                  /* arquivo kernel.ld */
  .init 0x2000 : {          /* O bootloader ficará no endereço 0x2000 */
    *(.init)
  }
  .text : { *(.text) }

  . = ALIGN(0x8);            /* ou ALIGN para 4096 para uma página */
  rodata_begin = .;
  .rodata : { *(.rodata) }
  rodata_end = .;
  
  . = ALIGN(0x8);
  data_begin = .;
  .data : { *(.data) }

  . = ALIGN(0x8);
  inicio_bss = .;
  .bss : { *(.bss*) }
  fim_bss = .;
}
