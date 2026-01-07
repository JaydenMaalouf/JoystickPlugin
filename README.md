[![](https://img.shields.io/github/actions/workflow/status/JaydenMaalouf/JoystickPlugin/release.yml?branch=master)](https://github.com/JaydenMaalouf/JoystickPlugin/actions/workflows/release.yml)
[![](https://img.shields.io/github/v/release/JaydenMaalouf/JoystickPlugin)](https://github.com/JaydenMaalouf/JoystickPlugin/releases/latest)

# :video_game: JoystickPlugin for Unreal Engine

JoystickPlugin provides **joystick and steering wheel support (including force feedback)** for **Unreal Engine 4 and Unreal Engine 5**, built natively for Unreal using SDL.

It integrates directly with Unreal’s **Input System** and **Subsystem architecture**, making it well-suited for simulation, racing, and hardware-driven projects that require reliable, low-level device access with minimal setup.

If you're building driving or simulation projects, JoystickPlugin pairs nicely with the
[VehicleTelemetryPlugin](https://github.com/JaydenMaalouf/VehicleTelemetryPlugin). Use JoystickPlugin
for input and force feedback, and VehicleTelemetryPlugin to stream real-time vehicle data for HUDs,
dashboards, logging, or external integrations.

---

## :blue_book: Documentation

Head to the [Wiki](https://github.com/JaydenMaalouf/JoystickPlugin/wiki) for installation steps, feature breakdowns, and usage guides.

---

## :bulb: Why JoystickPlugin?

JoystickPlugin is designed **specifically for Unreal Engine**, rather than acting as a thin wrapper over SDL. This allows it to take full advantage of engine-native systems, including:

- :electric_plug: Unreal Subsystems for lifecycle management
- :gear: Project Settings integration
- :keyboard: Enhanced Input compatibility
- :wrench: Clean editor and runtime separation

The plugin currently supports **Windows** and **Linux**.

---

## :sparkles: Key Features

- :video_game: **Native Unreal Input Integration**  
  Hooks directly into Unreal’s input pipeline, including Enhanced Input.

- :mailbox_with_mail: **Event-Driven Architecture**  
  Clean, modular callbacks for device connection and input events.

- :bust_in_silhouette: **Player & Device Mapping**  
  Explicit control over which device is assigned to which player.

- :boom: **Force Feedback / Haptics**  
  Trigger SDL-powered force feedback effects from gameplay logic.

- :satellite: **Sensor Support**  
  Access gyroscope and accelerometer data where supported by hardware.

- :bulb: **Device LED Control**  
  Dynamically control device LEDs at runtime.

- :card_index_dividers: **Device Profiles**  
  Load and apply per-device configuration presets.

- :cactus: **HID Reporting**  
  Send HID Reports directly to a joystick device.

- :gear: **Runtime Configuration**  
  Modify axis and button properties at runtime without restarting.

- :mag: **Joystick Viewer / Debugger**  
  Visual debugging tool to monitor real-time input from connected joysticks.

---

## :thinking: How does it compare to other solutions?

JoystickPlugin is not a direct port of an SDL wrapper. It is a **ground-up Unreal-native implementation**, focused on:

- :video_game: Unreal-first input handling
- :bricks: Engine subsystem lifecycle management
- :hammer_and_wrench: Editor-friendly configuration via Project Settings
- :earth_americas: Cross-platform support with minimal boilerplate

---

## :heart: Acknowledgements

This project builds on years of experimentation, forks, and community contributions across the Unreal and SDL ecosystems.

Thank you to **all developers, testers, and open-source contributors** who helped shape this plugin through code, issues, and feedback. Your work is preserved in the project history and genuinely appreciated.

---

## :art: Attribution

Controller icons by [Adib Sulthon](https://www.flaticon.com/authors/adib-sulthon)  
From [Flaticon.com](https://www.flaticon.com)
