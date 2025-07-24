# 📁 F - IRSensor


### **Introduction**
---
IR sensors (Infrared sensors) are widely used for various applications such obstacle avoidance, object detection, line following and much more. They work by emitting light and detecting reflections from nearby objects.

### How does an IR sensor work?
---

1. Emitter: Sends infrared light.

2. Receiver: Detects reflected light from objects.

3. Output: Often digital HIGH/LOW to indicate object presence.

### Class Design
---
The IRSensor class simplifies reading digital IR sensor data:

- `Contructor`: Takes the `pin_num` where the the sensor OUT is connected
- `init()`: Configures the pin as an input using `gpio_set_direction()`
- `is_object_detected()`:
    - Reads the digital pin status using `gpio_get_level()`.
    - Returns `true` if an object is detected (depending on sensor logic HIGH or LOW output). When an object is detected, the other led of the IR sensor is on.

### Application
---
Similar to previous applications, we will be define it inside an RTOS task using the IRSensor class in `main`:
1. Create IRSensor instance: `IRSensor ir_front(GPIO_NUM_27);`
2. Initialize sensor pin: `ir_front.init();`
3. Detect objects and serial print the detection status

Later, we will be using the IRSensor mainly to detect and avoid obstacle. Our circular TurtleBot2 will be equipped with left, right, front IT placed at the circle circonference. The detection will tell the robot to decide turning direction based on detection status.


### Final Summary
---
💡 **_Think of the IR sensor as a simple binary proximity detector that tells your robot “yes” or “no” about obstacles in its direct line of sight._**