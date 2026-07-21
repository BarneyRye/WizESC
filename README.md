# WizESC

- Custom STM32G431 sensorless electronic speed controller, running on [**AM32**](https://github.com/am32-firmware/am32) firmware.
- Designed to normally run from a 3S battery supply, and commanded via bi-directional DSHOT-300.

![WizESC assembled board, isometric view](WizESC_PCB/Visual%20Output%20Files/WizESC_PCB_ISO.png)

> [!WARNING]
> **This board is untested and unverified.** It has been designed and routed in full, and the
> firmware target has been derived against upstream AM32, but it has not been manufactured or
> brought up. Treat everything here as a educational design project rather than a proven product.

---

## Overview

| | |
|---|---|
| **MCU** | STM32G431CBT6 (Cortex-M4F @ 160 MHz, LQFP48) |
| **Gate driver** | TI DRV8300DPWR — 3-phase, bootstrapped high-side |
| **Power stage** | 6 × AOD4184A N-channel MOSFETs (DPAK) |
| **Input voltage** | 3S LiPo nominal (~11.1 V), up to 4S |
| **Commutation** | Sensorless BEMF via the STM32's internal comparators (COMP1/COMP2) |
| **Throttle input** | Bi-directional DSHOT-300 on PB4 (TIM3_CH1) |
| **Current sensing** | None — no shunts fitted |
| **Firmware** | AM32 2.20, stock `g431` target + one custom board target |
| **PCB** | 4-layer, 77.1 × 32.5 mm |
| **EDA tool** | Altium Designer 26.8.1 |

---

## Schematic

![WizESC schematic sheet](WizESC_PCB/Visual%20Output%20Files/WizESC_Sch_PNG.png)

Full-resolution PDF: [WizESC_Sch_PDF.pdf](WizESC_PCB/Visual%20Output%20Files/WizESC_Sch_PDF.pdf)

### Power

Battery voltage feeds the power stage directly and is stepped down to 3V3 by an
**AP63203WU-7** synchronous buck (4.7 µH inductor L1, 2 × 22 µF output). An **SMBJ16A** TVS
diode across the input clamps transients. The analogue supply is split off through a ferrite bead with its own decoupling.

### Gate drive

The DRV8300 drives all six FETs from a single 12 V-class GVDD rail derived from VBAT, with a
100 nF bootstrap capacitor per phase and 10 Ω series gate resistors on every gate

Sizing of the bootstrap capacitors, GVDD bulk capacitance and gate resistors is worked through
from the datasheet equations in
[driver-Component-calcs.md](WizESC_PCB/driver-Component-calcs.md).

### Sensing

- **Back-EMF** — each phase is divided 100k:18k into a comparator input, with the virtual
  neutral point formed by a 3 × 10k star and filtered by a 1 nF capacitor. Phase A uses COMP2,
  phases B and C share COMP1.
- **Battery voltage** — 100k:10k divider into PA6 (ADC2_IN3), giving a divider ratio of 11.
- **Temperature** — NTC divider footprint on PB1 (ADC1_IN12). No thermistor is fitted in the
  current BOM, but AM32 requires the pin to be defined.
- **Current** — not implemented. PA7 is tied to GND and `USE_CURRENT_SENSE` is left undefined.

### Debug and control

A 6-way JST **SM06B-SRSS-TB** connector (P1) carries SWD — SWCLK, SWDIO, NRST and SWO plus
power — for ST-LINK bootloader flashing. S1 is a reset button on NRST; BOOT0 is pulled through
R7.

---

## Layout

Top and bottom assembly views:

| Top | Bottom |
|---|---|
| ![Top side](WizESC_PCB/Visual%20Output%20Files/WizESC_PCB_TOP.png) | ![Bottom side](WizESC_PCB/Visual%20Output%20Files/WizESC_PCB_BOT.png) |

From left to right, sits the input pads, MCU and additional circuitry, then finally the gate driver and mosfets to the output pads.

![Top layer routing](WizESC_PCB/Visual%20Output%20Files/WizESC_PCB_PNG.png)

**Stackup:** 4 layers — Signal/GND/VBAT/Signal, with GND pours and via sticking on both signal planes.

A [3D STEP model](WizESC_PCB/Visual%20Output%20Files/WizESC_PCB_3D.step) and a
[PCB drawing PDF](WizESC_PCB/Visual%20Output%20Files/WizESC_PCB_PDF.pdf) are also included.

---

## Firmware

The board runs **stock AM32** — no motor-control code is modified. Only a board-target entry
(`WIZESC_G431`) is added to `Inc/targets.h`.

Board specific board config can be found in **[FIRMWARE_SETUP.md](FIRMWARE_SETUP.md)**. Note this does not include the full setup, which can be found in the **AM32** installation docs.

AM32 is a separate third-party open-source project (GPL-3.0). This repository is not affiliated
with or endorsed by it and contains none of its source code — only the configuration needed to
build it for this hardware.

---

## Repository layout

```
Custom_ESC/
|--FIRMWARE_SETUP.md                    AM32 board target, pin map, flashing notes
|--WizESC_PCB/
    |-- WizESC_PCB.PrjPcb               Altium project
    |--ESC_Main.SchDoc                  Schematic
    |--ESC_PCB.PcbDoc                   PCB layout
    |--WizESC_PCB.BomDoc                BOM document
    |--ESC_Gerbers_CAM.Cam              Gerber output job
    |--ESC_Drill_CAM.Cam                Drill output job
    |--driver-Component-calcs.md        DRV8300 component calculations
    |--Visual Output Files/             Renders, schematic PNG/PDF, STEP model
    |--Project Outputs for WizESC_PCB/
        |--WizESC_Gerbers.zip           Fabrication package
        |--WizESC_BOM.csv               Bill of materials (LCSC part numbers)
        |--Pick Place for ESC_PCB.csv   Assembly pick-and-place
|--README.md                            This file
```

## Manufacturing

Everything needed for fabrication (with JLCPCB) and assembly is checked in:

- **Gerbers + drill** — `Project Outputs for WizESC_PCB/WizESC_Gerbers.zip`
- **BOM** — `WizESC_BOM.csv`, with LCSC part numbers
- **Pick and place** — `Pick Place for ESC_PCB.csv`

Parts were selected for availability, good documentation and simplicity.