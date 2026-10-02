# Indoor Climate Monitor

Firmware for an ATmega328P that measures the air and noise in a room, turns those readings into a single Indoor Climate Quality score, shows the score on a red/white/blue LED, and hands the data to a communication module over I2C.

The score is the point of the device. CO2, humidity, temperature, and sound pressure each become a mark from 0 to 255. The marks are combined into one ICQ value, and the LED fades from red toward white as the room scores better. The cloud path in this firmware is that I2C packet to a communication module.

## Features

- CO2 from a CCS811, and temperature and humidity from an AM2320, both on I2C.
- Sound pressure from the ADC, using a logarithmic fit in `src/sensor/spl.c`.
- An LDR reading that scales LED brightness.
- Per-sensor scores and an ICQ value, implemented in `src/app/climate_evaluator.c`.
- PWM drive for a red, white, and blue LED. A healthy score sets red to `255 - icq` and white to `icq`. A sensor fault turns the LED off. The network-fault colour in `emit_color` is blue; `main` leaves `network_fault` at 0.
- A 15-byte I2C packet to a communication module at 7-bit address `0x01`.
- UART debug text at about 4800 baud, 8N1, while the CPU runs at 1 MHz.

TVOC is stored from the CCS811 and left out of the UART log and the I2C packet. `save_icq_parameters` is empty, so the limits stay at the constants in `scale_setup`.

## Example output

Each pass, `src/main.c` writes this text to the UART, with CR LF line endings:

```
CO2: <ppm>
Temp: <degrees C>
Humd: <percent RH>
SPL: <sound pressure>
```

`src/app/visible_controller.c` then writes:

```
LDR: <brightness mask, 0 to 255>
```

The I2C packet is 15 bytes, little-endian:

| Bytes | Contents |
| --- | --- |
| 0–1 | CO2, low byte first |
| 2–5 | Humidity, IEEE-754 `float` |
| 6 | ICQ, 0–255 |
| 7–10 | Sound pressure, IEEE-754 `float` |
| 11–14 | Temperature, IEEE-754 `float` |

## Hardware

The makefile targets an ATmega328P and programs the Arduino Uno fuse bytes (low `0xFF`, high `0xDE`, extended `0x05`). `init_program` divides the clock by 16. With a 16 MHz crystal, the core runs at 1 MHz, which is what `F_CPU` is set to.

| Function | Connection in the source |
| --- | --- |
| Start gate | PD2, input with pull-up; `main` waits until the pin reads low |
| Heartbeat LED | PB5, toggled every loop. This pin is also SPI SCK |
| Red / white / blue PWM | PD5 / PD3 / PD6 |
| I2C | PC4 and PC5, internal pull-ups on. 7-bit addresses: AM2320 `0x5C`, CCS811 `0x5A`, communication module `0x01` |
| LDR | ADC channel 3, AVCC reference (`ldr.c`) |
| Sound | ADC channel 2, AVCC reference (`spl.c`) |
| UART | About 4800 baud, 8N1 |

`adc.h` describes the microphone on ADC0 and the LDR on ADC1. The drivers above use channels 2 and 3. SPI is initialised and unused.

Default comfort limits live in `scale_setup` in `src/main.c`:

| Reading | Full score | Falls to 0 |
| --- | --- | --- |
| CO2 | Below 800 ppm | At and above 1500 ppm |
| Humidity | From 30 %RH, below 50 %RH | At or below 25 %RH, and at or above 60 %RH |
| Temperature | From 20 °C, below 25 °C | At or below 10 °C, and at or above 35 °C |
| Sound pressure | Below 90 | At and above 100 |

ICQ is two successive integer square roots of the product of the four scores. Higher is better. The sound-pressure number comes from the fit in `spl.c`, which the driver comment describes as dBSPL.

## Prerequisites

- `gcc-avr`, `binutils-avr`, and `avr-libc`
- GNU Make
- `avrdude`, to talk to a board
- `clang-format`, only for `make format`
- A serial device for programming. The makefile default is `/dev/ttyUSB0`

Debian or Ubuntu:

```sh
sudo apt-get install gcc-avr binutils-avr avr-libc avrdude clang-format
```

## Install

Clone the repository and build `bin/main.hex`. The output directories are not checked in:

```sh
mkdir -p obj bin asm
make all
```

That command was run with avr-gcc 7.3.0. It links `bin/main.elf`, writes `bin/main.hex`, and prints flash and RAM usage. Leaving out `mkdir` fails because `obj/` does not exist yet.

## Usage

Connect an ATmega328P on `/dev/ttyUSB0` and program the hex file:

```sh
make flash
```

`make flash` depends on `make all`, so `obj/` and `bin/` still have to exist. The upload command is `sudo avrdude` with programmer `arduino`, config `/etc/avrdude.conf`, port `/dev/ttyUSB0`, and baud `57600`. The Darwin and Windows settings in the makefile compare the literal word `UNAME`, so those paths stay inactive and the defaults above are what `make` uses.

Writing fuses is separate, and it was not run here:

```sh
make fuse
```

That writes low `0xFF`, high `0xDE`, and extended `0x05`. After programming, the firmware waits until PD2 reads low, then loops: read sensors, print UART, score ICQ, update the LED, send the I2C packet.

## Configuration

Programmer, chip, and clock are macros at the top of `makefile`:

| Macro | Default | Role |
| --- | --- | --- |
| `MCU` | `atmega328p` | Device passed to `avr-gcc` and `avrdude` |
| `CLK` | `1000000` | `F_CPU` |
| `COM` | `/dev/ttyUSB0` | Serial port |
| `CONF` | `/etc/avrdude.conf` | `avrdude` config |
| `LFU`, `HFU`, `EFU` | `0xFF`, `0xDE`, `0x05` | Fuse bytes for `make fuse` |
| `SRC` | The `.c` files under `src/` | Translation units in the link |

Comfort limits are the arrays in `scale_setup` in `src/main.c`. I2C addresses are `AM2320_I2C_ADDR` and `CCS811_ADDR` in the sensor drivers, and `COMM_MODULE_ADDR` in `src/app/communication_controller.c`. UART baud is the `UBRR` value `12` in `src/driver/init.c` (about 4808 baud at 1 MHz).

Changing `CLK` without changing the prescaler in `init_program` makes the UART and I2C clocks wrong.

## Development and testing

Layout:

```
src/main.c     Application loop
src/app/       ICQ, sensors, LED, and the outbound packet
src/com/       I2C, UART, SPI
src/driver/    Init, ADC, PWM, CRC-16
src/sensor/    AM2320, CCS811, LDR, sound pressure
test/          Sketches that the makefile does not build
```

`make clean` removes generated files and was run successfully. `make assembly` writes `asm/*.S` when `asm/` exists. After a build, disassemble with:

```sh
avr-objdump -d bin/main.elf
```

`make disasm` looks for `main.elf` in the repo root, so it does not find `bin/main.elf`.

`make format` rewrites `src/` in place with LLVM style. It was not run for the documentation change.

The makefile has no software-test recipe. `make test` is satisfied by the `test/` directory, so its `avrdude` recipe is skipped. `test/climatetest.c` includes `climate_evaluator.c` and fails to compile against the current `id_t` names (`CO2`, `Humidity`, `SoundPL`, `Temperature`). The other files in `test/` are on-target sketches with their own `main`, and the makefile has no target for them.

Compile with the warning flags already in the makefile (`-Wall -Wextra -pedantic`). Add a new `.c` file to `SRC` or it will not be linked. File comments use Doxygen (`@file`, `@brief`, `@author`, `@param`, `@return`).

## Project status

The monitoring loop builds and fits the ATmega328P. The repository has no CI workflow. Programming a board, the fuse write, and the `avrdude` connectivity check need hardware and were left unrun.

`save_icq_parameters` is empty. `push_data` always returns success, so the retry in `send_data` is skipped. `network_fault` stays clear. `test/climatetest.c`, `test/sensortest.c`, and `test/cloud_data.h` still use the previous sensor-id names. The repository does not include a roadmap.

## Licence

[MIT License](LICENSE). Copyright (c) 2026 saifsal.

The header of `makefile` credits a public-domain AVR makefile template by Michael Cousins. `LICENSE` is the licence for this project.
