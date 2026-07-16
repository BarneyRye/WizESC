# DRV8300 Gate Driver Component Calculations
### MOSFET: AOD4184A | Battery: 3S LiPo | Driver: TI DRV8300DPWR
---
## 0. Datasheets for reference
1. DRV8300DPWR: https://www.ti.com/lit/ds/symlink/drv8300.pdf
2. AD4184A: https://www.aosmd.com/res/datasheets/AOD4184A.pdf

## 1. Design values

| Parameter | Value | Source / Notes |
|---|---|---|
| V_GVDD | 12 V | Average 3S battery |
| f_SW (PWM frequency) | 20 kHz | Assumed - matches TI reference design |
| Q_G (AOD4184A, max @ V_GS=10V) | 33 nC | AOD4184A datasheet (typ 27.2 nC) |
| R_G (AOD4184A internal gate resistance) | 3.5 Ω typ (2-5 Ω range) | AOD4184A datasheet |
| V_BOOTD (bootstrap diode fwd drop, max @ 100µA) | 0.85 V | DRV8300 datasheet, Electrical Characteristics |
| V_BSTUV (falling, max) | 4.5 V | DRV8300 datasheet, Protection Circuits |
| IL_BS_TRANS (bootstrap leakage, transient, max) | 220 µA | DRV8300 datasheet, Electrical Characteristics |
| V_GHx_HI (typ @ 100 mA) | 0.6 V | DRV8300 datasheet - used to derive driver source resistance |
| V_GLx_LO (typ @ 100 mA) | 0.15 V | DRV8300 datasheet - used to derive driver sink resistance |
| Target bootstrap ripple, ΔV | 1 V | Standard design target (TI recommends 0.5-1 V) |

---

## 2. Bootstrap Capacitor (C_bstx)

**Step 1 — Available voltage headroom (Eq. 2, DRV8300 datasheet):**

```
ΔV_BSTx(avail) = V_GVDD − V_BOOTD − V_BSTUV
               = 12 − 0.85 − 4.5
               = 6.65 V
```

**Step 2 — Total charge per switching cycle (Eq. 3):**

```
Q_TOT = Q_G + (IL_BS_TRANS / f_SW)
      = 33 nC + (220 µA / 20 kHz)
      = 33 nC + 11 nC
      = 44 nC
```

**Step 3 — Minimum bootstrap capacitor (Eq. 4):**

```
C_BST(min) = Q_TOT / ΔV
           = 44 nC / 1 V
           = 44 nF
```

**C_BSTx value: 100 nF** </br>
*Chosen as the DRV8300 recommends giving headroom, and 100nF is a common value, and matches the datasheet*

---

## 3. GVDD Capacitor (C_GVDD)

**10× rule (Eq. 5, DRV8300 datasheet):**

```
C_GVDD ≥ 10 × C_BST
       = 10 × 100 nF
       = 1 µF (by this rule alone)
```

**However**, the DRV8300 pin-function table 6-2, specifies for at least a **10uF** capacitor for C_GVDD, so that has been selected. </br>
In addition I will add a single 100nF capacitor for the faster transients

---

## 4. Gate Resistors (R_GHx / R_GLx)

**Driver internal resistance, derived from electrical characteristics (@ 100 mA test current, V_GVDD = 12V):**

```
R_driver(source) = V_GHx_HI / 0.1 A = 0.6 / 0.1 = 6 Ω
R_driver(sink)   = V_GLx_LO / 0.1 A = 0.15 / 0.1 = 1.5 Ω
```

**Sizing for target peak turn-on current (~600 mA, under 750 mA typ / 1200 mA max source rating):**

```
R_ext = V_GVDD / I_peak_target − R_driver − R_G(FET)
      = 12 / 0.6 − 6 − 3.5
      = 20 − 9.5
      ≈ 10 Ω
```

**Check turn-off (sink path) with same resistor:**

```
I_peak(off) = V_GVDD / (R_ext + R_driver_sink + R_G)
            = 12 / (10 + 1.5 + 3.5)
            = 12 / 15
            = 800 mA
```
(Comfortably under 1500 mA typ / 2100 mA max sink rating.)

---

## 5. Summary Table

| Component | Calculated Min | Selected Value | Notes |
|---|---|---|---|
| C_GVDD (bulk) | 1 µF | **10 µF** ceramic (X5R/X7R) | Datasheet pin spec mandates ≥10 µF, overrides 10× rule |
| C_GVDD (HF decoupling) | — | **100 nF** ceramic | Placed at GVDD/GND pins, in addition to bulk cap |
| C_BSTx (per phase) | 44 nF | **100 nF** ceramic | Placed close to BSTx-SHx pins |
| R_GHx (high-side) | ~10 Ω | **10 Ω** | Peak source ≈600 mA @ 10 Ω |
| R_GLx (low-side) | ~10 Ω | **10 Ω**  | Peak sink ≈800 mA @ 10 Ω |

***N.B. On part selection capacitor ratings and resistor power dissipation ratings should be considered***

