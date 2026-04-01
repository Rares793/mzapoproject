# Mario 1-1 — MZ_APO / Zynq-7000 (CVUT B35APO)

Partial recreation of Super Mario Bros World 1-1 in C/C++,
built for the MZ_APO embedded board as part of the
Computer Architectures course at Czech Technical University in Prague.

## Hardware
- Board: MZ_APO (MicroZed APO carrier)
- SoC: Xilinx Zynq-7000, dual-core ARM Cortex-A9 @ 866 MHz, 1GB DDR3
- Display: 480×320 TFT LCD (ILI9481), RGB565
- OS: Embedded Linux

## Implementation
- Graphics rendered via direct memory-mapped AXI bus I/O (/dev/mem + mmap)
- Tile-based level structure with basic collision detection
- Player movement and basic physics

## Tools
- GCC cross-compiler
- SSH deployment to board over Ethernet
