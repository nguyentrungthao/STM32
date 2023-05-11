# STM32F103

## Hardware BOARD V0.1

### module 7 segment with two ic 74595
1. Pin configuration.

| STM32 | 74595  |
| :---: | :---: | 
| PA4 | SER |
| PA5 | OE |
| PA6 | RCLK |
| PA7 | SRCLK |

2. Code 
high 8-bit to select a 7-segment digit 
low 8-bit is code-7-segment 
code7segment is used in this board sequence:
```
/* this code only use for this version board */
const char code7seg[] = {0xD7, 0x14, 0xCD, 0x5D, 0x1E, 0x5B, 0xDB, 0x15, 0xDF, 0x5F};
```
### 
