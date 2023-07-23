
# Bootloader Simples para Raspberry Pi
Para quem não tem J-TAG...
* Este programa, residente na área de memória próxima ao endereço 0x2000, usa a mini UART nos pinos 6, 8 e 10 para receber um arquivo no formato Intel HEX.
* Os dados do arquivo são convertidos e salvos na memória RAM.
* Após a carga completa, o programa que foi carregado é iniciado (caso seja encontrado um campo especificando o endereço de início).

## Carregando um programa com o bootloader
* O arquivo ELF gerado pelo Makefile pode ser convertido para o formato HEX com o comando--
```bash
arm-none-eabi-objcopy -O ihex kernel.elf kernel.hex
```

* A seguir, copie o arquivo no device onde está a serial:
```bash
stty -F /dev/ttyUSB0 115200
cp kernel.hex /dev/ttyUSB0
```
onde supusemos que a uart onde o RPi está conectado está no device /dev/ttyUSB0.





