=====================================
XXD HW30A ESC: Technical Reference
=====================================

.. contents:: Table of Contents
   :depth: 3
   :local:

--------

Overview
========

The **XXD HW30A** is a brushless Electronic Speed Controller (ESC) designed for
multi-rotor aircraft and fixed-wing RC models. It regulates the power delivered
from the LiPo battery to a brushless motor, translating throttle commands into
precise motor speed control.

.. note::
   This document covers wiring, PWM protocol, arming/calibration procedures,
   and bench-testing without a flight controller.

--------

Specifications
==============

.. list-table::
   :widths: 35 65
   :header-rows: 1

   * - Parameter
     - Value
   * - Continuous current
     - 30 A
   * - Burst current
     - 40 A (≤ 10 s)
   * - Input voltage
     - 2S – 3S LiPo (7.4 V – 11.1 V)
   * - BEC output
     - 5 V / 2 A (linear BEC)
   * - Signal input
     - Standard PWM (1000 – 2000 µs @ 50 Hz)
   * - Motor type
     - Brushless (3-phase)
   * - Weight
     - ≈ 30 g

.. warning::
   Do **not** exceed the 3S input voltage limit. Connecting a 4S or higher
   battery may permanently damage the ESC.

--------

How It Works
============

The ESC sits between the battery and the motor, acting as a controllable
power gate. The motor requires 3-phase AC power to spin, but the LiPo
battery provides DC. The ESC handles this conversion internally.

Current and Throttle
--------------------

The motor does not draw a fixed amount of current. It draws as much as it
needs depending on the load placed on it:

.. code-block:: text

   Low throttle / light load   →  low current draw   (e.g.  5 A)
   High throttle / heavy load  →  high current draw   (e.g. 25 A)
   Full throttle / stall       →  peak current draw   (e.g. 40 A)

The **30 A continuous** rating is the maximum current the ESC can sustain
indefinitely without overheating. The **40 A burst** rating allows short
peaks above that limit (up to 10 seconds), for example during a sudden
throttle increase.

If the motor consistently demands more than 30 A, because it is too
powerful or the propeller is too large, the ESC will overheat and
eventually fail.

DC to 3-Phase Conversion
-------------------------

A brushless motor needs 3-phase AC power to spin. The ESC generates this
internally by rapidly switching the DC from the battery across the three
motor phase wires in a timed sequence. Changing the timing and duty cycle
of these switches is what controls the motor speed.

.. code-block:: text

   LiPo (DC)  →  ESC internal switching  →  3-phase AC  →  Motor spins

The throttle signal from the flight controller (or servo tester) tells the
ESC how fast to run this switching sequence, which directly controls the
motor RPM.

--------

Connector Pinout
================

The ESC has two sets of connectors:

Power Side
----------

.. code-block:: text

   Battery input
   ┌────────────────────────────┐
   │  RED  (+) ── Battery +    │
   │  BLACK(-) ── Battery −    │
   └────────────────────────────┘

   Motor output (3-phase)
   ┌────────────────────────────┐
   │  BLUE ── Motor phase A    │
   │  BLUE ── Motor phase B    │
   │  BLUE ── Motor phase C    │
   └────────────────────────────┘

.. note::
   Motor wire colours may vary between units. To reverse the motor spin
   direction, swap any **two** of the three motor phase wires.

Signal Side (3-pin JST / Dupont)
---------------------------------

.. code-block:: text

   Physical connector (left to right):

   ┌──────────────────────────────────────┐
   │  ORANGE ── PWM Signal               │
   │  RED    ── +5 V (BEC)               │  ← center pin
   │  BROWN  ── GND                      │
   └──────────────────────────────────────┘

The BEC provides regulated **5 V** that can power a flight controller or
RC receiver; no separate power supply is needed for low-current electronics.

Power Flow Diagram
------------------

The **LiPo battery** is the only power source in the system. The ESC's
internal BEC steps the battery voltage down to a regulated 5 V to power
downstream electronics. The servo tester (or flight controller) only sends
a signal back; it does not provide power to the ESC.

.. code-block:: text

                        LiPo battery (e.g. 11.1 V)
                               │
                               ▼
                   ┌───────────────────────┐
                   │          ESC          │
                   │                       │
                   │  BEC: 11.1V → 5V     │
                   └───────────────────────┘
                          │         │
             ┌────────────┘         └─────────────────┐
             │                                         │
             ▼                                         ▼
   thick RED / BLACK wires                   3-pin signal connector
   (raw battery voltage)                              │
             │                          ┌─────────────┼──────────────┐
             ▼                          │             │              │
         MOTOR                      ORANGE          RED           BROWN
   (spins at commanded            (PWM signal     (5 V out      (GND)
      throttle)                  FROM tester)   TO tester)

.. note::
   The **orange** wire carries the control signal **into** the ESC.
   The **red** wire carries 5 V power **out of** the ESC to the servo
   tester or flight controller. The servo tester provides no power to
   the ESC. All motor power comes directly from the LiPo.

--------

PWM Control Protocol
====================

The XXD HW30A uses the **standard RC PWM** protocol (sometimes called
"traditional PWM" or "analog PWM").

Signal Characteristics
-----------------------

.. list-table::
   :widths: 35 65
   :header-rows: 1

   * - Parameter
     - Value
   * - Frequency
     - 50 Hz
   * - Period
     - 20 ms
   * - Minimum pulse (0% throttle)
     - 1000 µs (1 ms)
   * - Maximum pulse (100% throttle)
     - 2000 µs (2 ms)
   * - Logic voltage
     - 3.3 V or 5 V

Throttle to Pulse Width Mapping
---------------------------------

.. code-block:: text

   Throttle    Pulse width
   ─────────── ───────────
     0  %       1000 µs
    10  %       1100 µs
    25  %       1250 µs
    50  %       1500 µs
    75  %       1750 µs
   100  %       2000 µs

Timing Diagram
--------------

.. code-block:: text

   Full period = 20 ms
   │◄────────────────────── 20 ms ──────────────────────►│

   0% throttle (1000 µs):
   ┌──┐
   │  │
   ┘  └────────────────────────────────────────────────────
   │1ms│◄──────────────── 19 ms ───────────────────────►│

   50% throttle (1500 µs):
   ┌───┐
   │   │
   ┘   └───────────────────────────────────────────────────
   │1.5ms│◄────────────── 18.5 ms ─────────────────────►│

   100% throttle (2000 µs):
   ┌────┐
   │    │
   ┘    └──────────────────────────────────────────────────
   │ 2ms │◄──────────────── 18 ms ───────────────────────►│

--------

Arming Sequence
===============

The ESC will **not drive the motor** until it has been armed. This is a safety
feature to prevent accidental spin-up on power-on.

.. code-block:: text

   Step 1: Apply 1000 µs signal (low throttle) BEFORE connecting battery
   Step 2: Connect the LiPo battery
   Step 3: Wait for the ESC startup beeps (melody)
   Step 4: ESC is now ARMED ✓
   Step 5: Slowly increase pulse width to spin the motor

.. warning::
   If the ESC detects a pulse width **above 1000 µs** at power-on, it will
   refuse to arm and emit a repeated error beep. Always start with low
   throttle.

Beep Codes
----------

.. list-table::
   :widths: 40 60
   :header-rows: 1

   * - Beep pattern
     - Meaning
   * - Short melody on power-on
     - ESC initialising
   * - Single beep after melody
     - Armed and ready
   * - Continuous repeated beeps
     - No signal detected / throttle too high
   * - Three beeps on power-on
     - Number of LiPo cells detected (e.g. 3 beeps = 3S)

--------

Throttle Range Calibration
===========================

Calibration teaches the ESC the exact minimum and maximum pulse widths of
your transmitter or signal source. It must be performed before the very
first arming sequence and is typically a one-time bench setup step; the
ESC stores the endpoints in non-volatile memory and retains them across
power cycles.

Because the ESC needs to see the maximum throttle signal at the moment it
receives power, the controller must be outputting 2000 µs before the ESC
boots. When both share the same battery (via the BEC), power the controller
separately (e.g. via USB) so it is already running and outputting maximum
throttle before the ESC is powered on.

.. note::
   The arming sequence does not apply during calibration. The ESC enters a
   dedicated calibration mode and does not check for the low-throttle hold
   that arming normally requires.

.. code-block:: text

   Step 1: Power the controller separately (e.g. via USB)
   Step 2: Set signal to 2000 µs (full throttle)
   Step 3: Connect the LiPo battery
   Step 4: Wait for confirmation beeps (usually 2 short beeps)
   Step 5: Set signal to 1000 µs (low throttle)
   Step 6: Wait for confirmation beeps
   Step 7: Calibration complete; normal arming applies from this point on

.. note::
   Calibration only needs to be repeated if you change your transmitter,
   servo tester, or signal source.

--------

Bench Testing Without a Flight Controller
==========================================

The XXD HW30A can be fully tested using a **servo tester**, a small and
inexpensive device (~5 USD) that generates a 50 Hz PWM signal via a dial.

Required Hardware
-----------------

- XXD HW30A ESC
- Brushless motor (propeller **not** required)
- LiPo battery (2S or 3S)
- Servo tester

Wiring Diagram
--------------

.. code-block:: text

   LiPo battery
       (+) ─────────── ESC battery RED  (+)
       (−) ─────────── ESC battery BLACK(−)
                            │
                        ESC unit
                            │
            ┌───────────────┼───────────────┐
           BLUE            BLUE            BLUE
            │               │               │
         Phase A          Phase B         Phase C
            │               │               │
         Motor A          Motor B         Motor C

   Servo tester
       Signal ────────── ESC ORANGE (signal wire)
       RED    ────────── ESC RED    (+5 V BEC, powers the servo tester)
       GND    ────────── ESC BROWN  (GND wire)

Procedure
---------

1. Connect motor phase wires to ESC output.
2. Connect servo tester signal wire to ESC signal connector.
3. Set servo tester dial to **minimum position** (= 1000 µs).
4. Connect LiPo battery.
5. Wait for ESC to arm (startup beeps).
6. Slowly turn servo tester dial to increase throttle.
7. Motor should spin smoothly.

.. warning::
   Without a propeller the motor has **no load** and can reach very high RPM.
   Keep throttle low during bench tests. Do not run at full throttle for
   extended periods, as it can damage motor bearings.


--------

Safety Guidelines
=================

.. warning::

   - Always connect the **signal wire before** the battery.
   - Keep the motor **secured** to a surface during bench tests.
   - Never short the LiPo leads (fire risk).
   - Disconnect the battery **before** touching motor or wiring.
   - Use a **LiPo-safe bag** during charging and storage.
   - Do not exceed the rated **3S (11.1 V)** input voltage.

--------

References
==========

- `AliExpress product page`_
- `Servo control wiki page`_

.. _AliExpress product page: https://it.aliexpress.com/item/1005006102604074.html?spm=a2g0o.order_list.order_list_main.32.18ff3696AOJksA&gatewayAdapt=glo2ita
.. _Servo control wiki page: https://en.wikipedia.org/wiki/Servo_control
