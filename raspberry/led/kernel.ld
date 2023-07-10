
SECTIONS {                  /* arquivo kernel.ld para raspberry pi 2*/
  .init 0x8000 : {          /* endereço de carga do bootloader */
    *(.init)
  }
  .text : { *(.text) }

  . = ALIGN(0x8);
  rodata_begin = .;
  .rodata : { *(.rodata) }
  rodata_end = .;
  
  . = ALIGN(0x8);
  data_begin = .;
  .data : { *(.data) }

  . = ALIGN(0x8);
  bss_begin = .;
  .bss : { *(.bss*) }
  bss_end = .;
}
