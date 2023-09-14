# De rook bril software
Deze repository is bedoelt voor de software dat gebruikt gaat worden voor het project van de rookbril.
De IDE die gebruikt kan worden kan zowel platformio zijn als cube IDE. Als het goed is werkt voor beide IDE's de semihosting feature.

# Folder structuur
De core folder bevat alle code, inclusief de cmsis drivers die voor cube IDE gebruikt worden.
Buiten de core folder zitten nog een paar andere bestanden:
- platformio.ini is het configuratiebestand voor platformio.
- enable_semihosting.py is een script wat semihosting functionaliteit aan de platformio ide toevoegd.
- .project en .cproject zijn configuratiebestanden voor cube IDE.
- Linker file is used by cubeIDE. platformio uses a custom linker file

## Core folder
- **Startup**       This folder contains the startup file used by the cubeIDE (pio uses its own)
- **Inc**           This folder contains the includes for the project
- **Src**           This folder contains the source files for this project

- **.../Driver**    The driver folder in the Inc and Src directories are meant for peripheral drivers (uart, i2c, spi, etc...)