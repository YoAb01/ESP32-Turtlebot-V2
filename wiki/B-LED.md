
# 📁 B - LED

### **Introduction**
In this section, we introduce basic ESP-IDF embedded programming concepts. The goal is to implement LED control with a class-based approach using FreeRTOS tasks to handle asynchronous blinking. This will form the foundation for structured, scalable embedded software design.

---

## Class Design
Throughout this project, we adopt a class-based approach to ensure:
- Modularity: encapsulating each component logic
- Reusability: easy integration of multiple LEDs or peripherals
- Clean architecture: simpler maintenance and extension over time

For this implementation, we created:
- An `LED` class with:
  - `init()`: Configures the pin number, set mode to OUTPUT and reset any previous states
  - `set_on()` and `set_off()`: Triggers the pin to HIGH or LOW to turn on/off the LED
  - `blink()`: basic LED application
  - FreeRTOS task that manages blinking functionality

--- 

## Code Explanation

### Introduction to FreeRTOS
FreeRTOS is a Real-Time Operating System kernel for embedded devices, enabling:
- Concurrent tasks with controlled priorities
- Deterministic scheduling for critical applications
- ...and a lot of other functionalities.

For now, we will start by covering the concept of a **task** in FreeRTOS.

---

### Task
A task in FreeRTOS is like a mini-program running inside your main program. It allows the ESP32 to:
- Do multiple things “at the same time” (multitasking)
- Run each task for a certain time slice, then switch to the next (context switching)
-  Assign priority levels so critical tasks run first

A task in FreeRTOS **MUST** follow this function signature
```
void task(void *pvParameters);
```
Where the `pvParameters` is a generic pointer to pass data to the task (e.g a pointer to an LED instance)

* **Key Concepts**
  - The task function is an infinite loop where we define what the task does repeatedly (in a while true loop).
  - xTaskCreate(): This function is often define at main level. It creates and starts the task. The function accepts the following arguments:
  ```
    xTaskCreate(
      task_function,   // Function pointer to your task
      "TaskName",      // Task name for debugging
      stack_size,      // Stack size in words (e.g. 2048)
      parameters,      // Data passed to the task (pvParameters)
      priority,        // Priority (higher = more urgent)
      handle           // Task handle (optional)
    );
  ```
**Note:** Concerning the task function, in our implementation, we define a statuc wrapper function `_blink_task_w` to respect the FreeRTOS's C-Style function signature (since `this` pointer will be passed implicitly to the signature which will cause build error). This calls the class method `blink_task()`

---

### Wiring
We have used the internal LED at GPIO2 (included in ESP32)

---

### Testing
1. Connect ESP32 to your setup device
2. Navigate to the project directory
```
cd $PATH_TO_PROJECT
```
3. Build and flash
```
idf.py build
idf.py flash
idf.py monitor
```
4. LED blinks continuously at configured intervals.

**Note:** In order to see the ESP_LOGI serial logs, we can run this:
```
idf.py monitor
```
