# Heater Control System
#### Client Name: Brad Fairman

## Project Requirements
Desired flow is outlined below:
- Detect if the connected Head is Heater OR Rotary Cleaner by reading the NTC. The NTC will only report a value if the Heater is connected. In case of Rotary Cleaner, the NTC value will return close to 0V.
- If the **HEAD is Heater**
    - Once the operator presses button (S1), Green LED comes ON, Heater should also turn ON through the **HEATER_CTRL** signal.
    - Once the heater temperature reaches a certain INPUT threshold, Vibration Motor should turn ON through **VIBE_MOTOR_CTRL** signal.
    - The NTC feedback should ensure a constant temperature is maintained by turning on HEATERS if the temperature drops, or turn them OFF if temperature goes HIGH.
    - After 7 minutes, the unit automatically turns OFF
- If the HEAD is **Rotary Cleaner**
    - Once the operator presses button (S1), Red LED comes ON, Gear Motor should also turn ON through the **GEAR_MOTOR_CTRL** signal.
    - Long press (5 seconds) turns OFF the system.
    - After 7 minutes, the unit automatically turns OFF
