
/*
 * Endereços dos registradores GPIO.
 */
.set IOPMOD, 0x03ff5000
.set IOPCON, 0x03ff5004
.set IOPDATA, 0x03ff5008

/*
 * Endereços dos registradores do controlador de interrupções.
 */
 .set INTMOD, 0x03ff4000
 .set INTPND, 0x03ff4004
 .set INTMSK, 0x03ff4008

/*
 * Endereços dos registradores dos Timers.
 */
 .set TMOD, 0x03ff6000
 .set TDATA0, 0x03ff6004
 .set TDATA1, 0x03ff6008
 .set TCNT0, 0x03ff600c
 .set TCNT1, 0x03ff6010

/*
 * Endereços dos registradores da UART 0.
 */
.set ULCON0, 0x03ffd000
.set UCON0, 0x03ffd004
.set USTAT0, 0x03ffd008
.set UTXBUF0, 0x03ffd00c
.set URXBUF0, 0x03ffd010
.set UBRDIV0, 0x03ffd014
