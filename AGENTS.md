# indoor-climate-monitor

## Overview

`indoor-climate-monitor` is AVR firmware for an ATmega328P indoor climate monitor (ICM). It reads CO2, temperature, humidity, and sound pressure, scores each reading from 0 to 255, combines those scores into an Indoor Climate Quality (ICQ) value, fades a red/white/blue LED to show that score, and writes a 15-byte packet to an external communication module over I2C. UART prints the live readings for debugging. There is no network stack in this repository; "cloud" in the source comments means that I2C packet.

The loop in `src/main.c` is implemented, and the firmware builds with `avr-gcc`. Nothing here is wired to continuous integration. `save_icq_parameters` is empty, and the main loop never raises a network fault. The files under `test/` are manual sketches and host snippets written against an older `id_t`. The makefile does not build them.

## Repository layout

```
makefile          AVR build file
LICENSE           MIT License
src/main.c        Startup gate, sensor loop, UART log, ICQ, LED, and I2C send
src/app/          Scoring, sensor orchestration, LED colour, and the I2C payload
src/com/          I2C, UART, and SPI drivers
src/driver/       Clock and bus init, ADC, PWM, and CRC-16
src/sensor/       AM2320, CCS811, LDR, and sound-pressure drivers
test/             Manual sketches and stale host snippets
```

`obj/`, `bin/`, and `asm/` are build outputs. They are not in the tree; create them before building. Every `.c` file that should be linked is listed in `SRC` in `makefile`.

## Setup

Install the AVR toolchain and GNU Make:

- `gcc-avr` (`avr-gcc`)
- `binutils-avr` (`avr-objcopy`, `avr-objdump`, `avr-size`)
- `avr-libc`
- `make`

Flashing also needs `avrdude` and a serial port. `make format` needs `clang-format`.

On Debian or Ubuntu:

```sh
sudo apt-get install gcc-avr binutils-avr avr-libc avrdude clang-format
```

The firmware is built for an ATmega328P with `F_CPU=1000000`. `init_program` sets the clock prescaler to 16, so a 16 MHz oscillator runs at 1 MHz. Programmer defaults are `avrdude` in `arduino` mode at 57600 baud on `/dev/ttyUSB0`, using `/etc/avrdude.conf`. No environment variables or secrets are required.

## Build, run and test

Create the output directories, then build. Both commands were run successfully against this tree with avr-gcc 7.3.0:

```sh
mkdir -p obj bin asm
make all
```

`make all` writes `bin/main.elf` and `bin/main.hex` and prints an `avr-size` summary. On a fresh checkout, skipping the `mkdir` fails with `can't create obj/main.o: No such file or directory`.

| Command | Result | Verified |
| --- | --- | --- |
| `make clean` | Deletes the files in `obj/`, `bin/`, and `asm/` | Yes |
| `make assembly` | Writes `asm/*.S`. `asm/` must already exist | Yes |
| `avr-objdump -d bin/main.elf` | Disassembles the ELF produced by `make all` | Yes |
| `make format` | Rewrites sources under `src/` with `clang-format --style=LLVM` | Not run; it edits source |
| `make flash` | Builds, then programs `bin/main.hex` with `avrdude` | Not run; needs a board |
| `make fuse` | Writes low `0xFF`, high `0xDE`, extended `0x05` | Not run; needs a board |

`make disasm` stops with `No rule to make target 'main.elf'`. Its prerequisite is `main.elf` in the repo root, while the link rule writes `bin/main.elf`. Use `avr-objdump -d bin/main.elf`.

`make test` reports `make: 'test' is up to date.` Make treats the `test/` directory as that target, so the `avrdude` connectivity recipe never runs. `gcc` on `test/climatetest.c` fails because `co2` is not a member of `id_t`.

## Code style and conventions

C sources are compiled with `avr-gcc -Wall -Wextra -pedantic -O2 -DF_CPU=1000000 -mmcu=atmega328p`. The link adds `-Wl,-u,vfprintf -lprintf_flt -lm` so `sprintf` can print floats.

`make format` is the formatter. It passes `--style=LLVM` on the command line. There is no `.clang-format` file. Existing sources use two-space indentation, braces on the same line as the function, and Doxygen banners (`@file`, `@brief`, `@author`, `@param`, `@return`, `@bug`).

Names are `snake_case`. Types use a `_t` suffix (`sensor_data_t`, `scale_t`). The sensor ids in `id_t` are `CO2`, `Humidity`, `SoundPL`, and `Temperature`. Put policy in `src/app`, device code in `src/sensor`, and peripheral code in `src/com` or `src/driver`. A new `.c` file is linked only after it is added to `SRC`.

Float measurements are stored by copying their IEEE-754 bits into `sensor_data_t.value` (`unsigned long`). CO2 is stored in that same field as an integer.

## CI and tooling

No CI is configured. There is no `.github/workflows` directory and no `.gitignore`.

Local tools are the makefile, the AVR toolchain, `avrdude`, and `clang-format`. Leave `obj/`, `bin/`, and `asm/` untracked.

## Working rules for agents

- Keep changes small and focused.
- Run the tests before committing. The check that builds this tree is `mkdir -p obj bin asm && make all`. `make test` stops at the `test/` directory and skips its recipe.
- Never commit secrets.
- Update the docs when behaviour changes.
- Use conventional commit messages.
- Keep `AGENTS.md`, `CLAUDE.md`, and `.github/copilot-instructions.md` in sync. `AGENTS.md` is canonical. The other two carry the same text, plus a one-line header. Copy the text; leave them as regular files.

## Gotchas

- The makefile tests the literal string `UNAME`, not `$(UNAME)`, so the Darwin and Windows `avrdude` settings never apply. The defaults always win: `sudo avrdude`, `/etc/avrdude.conf`, `/dev/ttyUSB0`, programmer `arduino`, baud `57600`.
- `make fuse` writes the Arduino Uno fuse bytes. Run it only when that clock configuration is intended.
- `main` waits in `loop_until_bit_is_clear(PIND, PIND2)` before `init()`. PD2 is an input with its pull-up enabled, so startup waits until that pin reads low.
- `live()` toggles PB5 and does not delay, even though its comment says it waits 500 ms. PB5 is also SPI SCK. SPI is initialised, and `spi_transfer` is never called.
- Sensor fault is applied one pass later. `emit_color` runs before `main` stores `vd.sensor_fault`, and that store always ORs `sd[0].fault` rather than `sd[i].fault`. Only the CO2 slot can turn the fault LED path on.
- `ccs811_read_data` returns `128` when a one-second sample is not ready. That sets the CO2 fault. Because the loop does not wait, CO2 is faulted on most passes.
- `push_data` always returns `1` and ignores I2C status. `send_data` retries only when `push_data` returns `0`, so that loop is skipped. `network_data_t.fault` stays `0`, so `main` keeps the network-fault colour (blue) unselected.
- `save_icq_parameters` has an empty body. Limits are the constants in `scale_setup`.
- Scale and sensor arrays are paired by index, not by `id_t`: 0 CO2, 1 humidity, 2 sound pressure, 3 temperature.
- Slope scores (CO2, sound pressure) are 255 below `limit[0]` and 0 at or above `limit[1]`. Plateau scores (humidity, temperature) are 255 from `limit[1]` inclusive to `limit[2]` exclusive, and 0 at or outside `limit[0]` and `limit[3]`. Defaults: CO2 800 and 1500 ppm; humidity 25, 30, 50, 60 %RH; sound pressure 90 and 100; temperature 10, 20, 25, 35 °C. ICQ is two successive integer square roots of the product of the four scores.
- `adc.h` comments name ADC0 as the microphone and ADC1 as the LDR. `spl.c` calls `adc_setup(2, 1)` and `ldr.c` calls `adc_setup(3, 1)`. The temperature sent by the firmware is the AM2320 reading. `adc_temperature` has no callers.
- Sound pressure is `(26.057 * log(splMean)) - 79.819`, and `log` is the natural logarithm. The log10 calibration constants in `spl.c` are commented out.
- AM2320 humidity and temperature are `raw / 10`. The temperature sign bit is not applied. Its address is `0x5C << 1`. The CCS811 address is `0x5A << 1`, its drive mode is one second, and `ccs811_co2()` stays at the initial 400 ppm until a read succeeds. TVOC is stored and never sent. The communication-module address is `0x01 << 1`.
- The 15-byte I2C payload is little-endian: CO2 low, CO2 high, humidity `float`, ICQ, sound-pressure `float`, temperature `float`.
- `test/cloud_data.h`, `test/climatetest.c`, and `test/sensortest.c` still use the old ids (`co2`, `lux`, `humidity`, `temperature`, `spl`). `test/uart_test.c` calls `uart_init`, which is `static` in `src/driver/init.c`. None of these files are makefile targets.
- PWM pins on port D are red PD5, white PD3, and blue PD6. Red and white targets are snapped down to a multiple of 5; each loop moves the PWM outputs 5 counts toward the target. The brightness mask is `255 - ldr_read()`.
