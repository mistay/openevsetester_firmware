# Openevsetester Firmware

## Requirements
- ✓ DEV: basic check if hardware is designed properly, hardware fixes in both schematic and board
- ✓ PP {PP1, PP2, ..PP5} switches on/of
- ✓ CP {STATE_A, STATE_B, .. STATE_E} switches on/off
- ✓ 3v3 switching on/off: power off display and CP measurement circuit
- ✓ i2c display support, thanks to https://github.com/TaraHoleInIt, taken from https://github.com/TaraHoleInIt/tarablessd1306
- ✓ rotary encoder reading, basic functionality
  - needs some improvements, does not work properly
- switch PE breaker on/off: disconnect PE manually
- single value reading: {L1, L2, L3} voltage, frequency, phasing
- display support for lager displays (spi?)
- prusa-like UI (display, rotary encoder)
- automating tests like: switch to STATE B, read CP, switch to STATE C, read CP, read L1 to L3, advice user to connect installation tester to test RCD, ...
- report generation
