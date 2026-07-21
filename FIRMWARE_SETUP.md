# WizESC — Firmware Configuration

This board runs on **AM32**, using its stock `g431` target plus one custom board-target
entry. No motor-control code is modified.

This document only covers modification of the current **AM32** setup, not the whole proccess.

### Attribution

AM32 is a separate third-party open-source project, licensed **GPL-3.0**, maintained at
<https://github.com/am32-firmware/AM32>.

This repository is **not affiliated with, endorsed by, or connected to** the AM32 project
or its maintainers. It contains no AM32 source code — only the board-specific
configuration required to build AM32 for this hardware. All credit for the firmware
belongs to its authors. Refer to the upstream repository for the toolchain, build
instructions, flashing procedure and licence terms.

Configuration below was derived against upstream commit `ad91453` (2026-07-09),
firmware version **2.20**.

---

## Board target

Target name: **`WIZESC_G431`**

Add to `Inc/targets.h`, alongside the other `*_G431` entries:

```c
#ifdef WIZESC_G431
#define FIRMWARE_NAME "WIZESC_G431 "
#define FILE_NAME "WIZESC_G431"
#define DEAD_TIME 80 // 500 ns @ 160 MHz TIM1
#define HARDWARE_GROUP_G4_D
#define TARGET_STALL_PROTECTION_INTERVAL 20000
#define TARGET_VOLTAGE_DIVIDER 110 // R14:R15 = 100k:10k -> ratio 11

#define USE_ADC_1_2
#define VOLTAGE_ADC_PIN LL_GPIO_PIN_6 // PA6 = ADC2_IN3
#define VOLTAGE_ADC_CHANNEL LL_ADC_CHANNEL_3
#define NTC_ADC_PIN LL_GPIO_PIN_1 // PB1 = ADC1_IN12
#define NTC_ADC_CHANNEL LL_ADC_CHANNEL_12
#define CURRENT_ADC_PIN LL_GPIO_PIN_7 // PA7 tied to GND, no shunt fitted
#define CURRENT_ADC_CHANNEL LL_ADC_CHANNEL_4
#define MILLIVOLT_PER_AMP 1 // placeholder; USE_CURRENT_SENSE is NOT defined

#define USE_HSE
#undef HSE_VALUE
#define HSE_VALUE 8000000 // 8 MHz crystal, Y1
#define USE_HSE_BYPASS 0
#endif
```

Two constraints worth knowing:

- **`FILE_NAME` must keep the `_G431` suffix.** Targets are discovered by grepping this
  file for `define FILE_NAME` lines containing the MCU suffix, so renaming it removes the
  build target entirely.
- **`FIRMWARE_NAME` must be exactly 12 characters** (pad with spaces) — it goes into a
  fixed-width field reported to the configurator.

Build target is then `WIZESC_G431`; artefacts are named `AM32_WIZESC_G431_2.20.bin/.hex`.

---

## Where the constants come from

| Macro | Value | Derivation |
|---|---|---|
| `HARDWARE_GROUP_G4_D` | — | The only G431 hardware group whose PWM pins match this board exactly: PA10/PB15, PA9/PB14, PA8/PB13 |
| `TARGET_VOLTAGE_DIVIDER` | 110 | `battery_voltage` is held in centivolts (cell count is derived by dividing by 370, i.e. 3.70 V/cell), and the conversion computes `mV × DIVIDER/100`. The macro is therefore **10 × the true divider ratio**. R14:R15 = 100k:10k → ratio 11 → **110** |
| `DEAD_TIME` | 80 | SYSCLK is 160 MHz (HSE 8 MHz, PLL M=1 N=40 R=2). TIM1 runs `Prescaler=0`, `CLOCKDIVISION_DIV1` → 6.25 ns/tick. 80 × 6.25 ns = **500 ns**. |
| `USE_ADC_1_2` | set | Not optional. In the single-ADC path, `VOLTAGE_ADC_PIN` is initialised on GPIOA and read on ADC1 — but ADC1's only GPIOA channels are PA0–PA3, all consumed here by the comparators and throttle. VBAT must go via ADC2 on PA6 |
| `NTC_*` / `CURRENT_*` | set | The `USE_ADC_1_2` path references both unconditionally, and `MILLIVOLT_PER_AMP` is referenced unguarded in the current-conversion maths. Omitting any of them is a compile error, regardless of whether the hardware is fitted |
| `USE_CURRENT_SENSE` | **not defined** | This is the "no current sensing" sentinel. This board has no shunts — leave it undefined and disable current limiting in the configurator |

---

## Pin map

| Pin | Port | Net | Function |
|---|---|---|---|
| 8 | PA0 | BEMF_C | COMP1_INM (IO2) — phase C sense |
| 9 | PA1 | V_NEUT | COMP1_INP (IO1) — virtual neutral |
| 11 | PA3 | V_NEUT | COMP2_INP (IO2) — virtual neutral |
| 12 | PA4 | BEMF_B | COMP1_INM (IO1) — phase B sense |
| 13 | PA5 | BEMF_A | COMP2_INM (IO1) — phase A sense |
| 14 | PA6 | VBAT_ADC | ADC2_IN3 — battery voltage |
| 15 | PA7 | GND | ADC2_IN4 — sampled but unused, no shunt |
| 17 | PB1 | NTC | ADC1_IN12 — board temperature |
| 27 | PB13 | TIM1_CH1N | phase C low |
| 28 | PB14 | TIM1_CH2N | phase B low |
| 29 | PB15 | TIM1_CH3N | phase A low |
| 30 | PA8 | TIM1_CH1 | phase C high |
| 31 | PA9 | TIM1_CH2 | phase B high |
| 32 | PA10 | TIM1_CH3 | phase A high |
| 37 / 38 | PA13 / PA14 | SWDIO / SWCLK | debug header P1 |
| 41 | PB4 | DSHOT | TIM3_CH1 — throttle input |

### Comparator scheme

| Phase | COMP | Minus (phase) | Plus (neutral) | EXTI |
|---|---|---|---|---|
| A | COMP2 | PA5 | PA3 | 22 |
| B | COMP1 | PA4 | PA1 | 21 |
| C | COMP1 | PA0 | PA1 | 21 |

---

## Board-specific flashing notes

Procedure can be found in the **AM32** docs, only board specific constants are defined in this documents

- Bootloader is flashed once via **ST-LINK over SWD** (header P1). AM32 bootloaders are
  named by throttle input pin — this board uses **PB4**.
- Application address: **`0x08001000`**
- Settings EEPROM: **`0x0800F800`** — a full chip erase resets configuration.

---

## Notes

Currently, this board is untested and unverified, as I have been unable to manufacture it due to personal constraints.

