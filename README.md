# Drone

Firmware and tools for a custom multi-rotor drone. The repository contains
the **coco** platform abstraction library and application targets that build
on top of it.

## Repository layout

```
drone/
├── cmake/                      Toolchain files
│   └── arm-none-eabi.cmake     Cortex-M cross-compiler toolchain
├── coco/                       Reusable platform abstraction library
│   ├── include/coco/           Public headers (hal/, os/)
│   ├── platform/               MCU-specific implementations
│   │   └── stm32f446/
│   └── third_party/            Vendor libraries required by coco
│       ├── arm/cmsis/          ARM Cortex-M4 core headers
│       └── stm32f446/          ST CMSIS device files and LL drivers
├── esc_tester/                 Application: ESC arming and calibration tester
│   ├── linker/                 Linker script for STM32F446
│   └── main.cc
└── docs/                       Hardware reference documentation (Sphinx)
```

## Prerequisites

| Tool | Version | Notes |
|------|---------|-------|
| CMake | 4.0 or newer | |
| Ninja | any | Used by all presets |
| arm-none-eabi GCC | 15.2 | Expected at `/opt/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi` |

To use a different toolchain path, edit `cmake/arm-none-eabi.cmake` and update
`CMAKE_SYSROOT` and `CMAKE_FIND_ROOT_PATH`.

## Building

All builds are driven through CMake presets defined in `CMakePresets.json`.

### Configure

```sh
cmake --preset stm32f4-debug
```

This generates the build tree under `build/stm32f4-debug/`.

### Build

```sh
cmake --build --preset stm32f4-debug
```

The final binary is `build/stm32f4-debug/esc_tester/esc_tester.elf`.

### Available presets

| Preset | Target | Build type |
|--------|--------|------------|
| `stm32f4-debug` | STM32F446 | Debug (`-Og -g3`) |
| `stm32f4-release` | STM32F446 | Release (`-O2`) |

## Flashing

```sh
arm-none-eabi-objcopy -O binary \
    build/stm32f4-debug/esc_tester/esc_tester.elf \
    esc_tester.bin

# Flash with STLink (adjust interface as needed)
st-flash write esc_tester.bin 0x08000000
```

## coco library

`coco` is a standalone platform abstraction library. It exposes two namespaces:

- `hal::` hardware drivers (PWM, etc.)
- `os::` OS services (sleep, platform init)

Each MCU target is a self-contained subdirectory under `coco/platform/` and
`coco/third_party/`. The active platform is selected at configure time via the
`COCO_PLATFORM` cache variable, which the presets set automatically.

### Adding a new platform

1. Create `coco/platform/<name>/CMakeLists.txt` that calls `target_sources(coco ...)` and `target_link_libraries(coco ...)`.
2. Create `coco/third_party/<name>/CMakeLists.txt` that defines the vendor library target.
3. Add a new configure preset to `CMakePresets.json` with `"COCO_PLATFORM": "<name>"` and the appropriate toolchain file.

No existing CMake file needs to change.
