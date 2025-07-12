# 📁 C - Timers

### **Introduction**



## What is a timer ?
A timer is a peripheral that counts time - either up or down - at a defined rate, to:
- Trigger an action after a delay
- Repeatedly perform an action at a fixed interval time
- Measure elapsed time (e.g. pulse duration)

They are very useful, because we often need precise, reliable timing without blocking the CPU.
**Examples:**
- LED blinking at 1Hz -> Timer triggers toggle every 500ms
- Motor PID control loop -> Timer triggers PID update every 20ms
- Ultrasonic distance sensor -> Timer measures echo pulse duration
- Timeout detection -> Timer triggers a safety action after X ms

So we can simply the concept like this:

**_<< A timer is like an alarm clock that rings at exactly the time you set, and keeps ringing repeatedly if you tell it to.>>_**

---

## Types of Timers
* There are 2 main types of timers:

  1. **Hardware Timers (General Purpose Timers - GPTimer):**
      - Peripheral timers independent from CPU.
      - Accurate, ideal for precise periodic task (e.g. motor control loops, PWM generation ...).
  2. **Software Timers**:
      - Managed by RTOS (FreeRTOS) or CPU.
      - Less accurate due to scheduling delays.
      - Useful for general timeouts, UI debouncing, non-critical timing.

* As for timer operation modes:
  1. **One-shot mode**: Timer counts to its alarm value once, triggers the event, and stops.
  2. **Periodic mode**: Timer automatically reloads and restarts counting after triggering the alarm, creating a repetitive periodic event.

---

## ESP32 GPTimer
Often, to initialize an ESP32 GPTimer, we perform the following steps:
1. **Configure timer:**
    * Using `gptimer_config_t`, we can set the clock source, counting direction (up/down) and the timer tick frequency.
2. **Create timer handle:**
    * `gptimer_new_timer()` to allocate a pre-configured timer instance.
3. Set alarm period config:
    * Using `gptimer_alarm_config_t`, we can set the alarm count (when to trigger the alarm), where to restart restart counting after alarm and enable periodic mode.
4. **Register callback (ISR):**
    * Using `gptimer_register_event_callbacks()`, the function runs in interrupt context when alarm triggers.
    * ISR must be fast, non-blocking. So we need to avoid doing complex stuffs inside the ISR.
    * Usually set flags, toggle GPIOs, or notify tasks.
5. **Enable & Start timer:**
    * `gptimer_enable()` to enable timer
    * `gptimer_start()` to start timer

---

## Class design
Using the previous knowledge about timers, we can think about this initial structure.
* A `Timer` class where we can pass at constructor level the count direction, source clock, and period in us. 
  - `start()`: 
    * It configures the timer using `gptimer_config`
    * Create a new timer using with `gptimer_new_timer()` and apply the defined timer config.
    * Enable/Start the timer
  - `stop()`:
    * Stops the timer first using `gptimer_stop()`
    * For cleaning resources, it disables the `_timer_handle` using `gptimer_disable()` and delete it for proper memory management `gptimer_del_timer()`
  - `change_period()`: # Should be named alarm_config more than just changing period
    * It initializes a `gptimer_alarm_config_t` where we can adjust the period, and tell if we need to restart count of timer on alarm
    * Set alarm action for the `_timer_handle` using `gptimer_set_alarm_action`
  - `attach_callback()`:
    * Defines the callback function ISR to execute when we hit the alarm value.

## Application
To test these concepts, we can remake the blink LED to use Timer (GPTimer) instead of software timer (previously done with FreeRTOS using tasks)

So inside the `LED` class definition, we have done the following adjustments:
* `blink_timer(uint64_t period_us)`: which will be the main function to manage LED blinking with timers
  * We start by creating an instance of a `Timer` and assign it to `_blink_timer`. We used the default initialization (counting up, default source clk)
  * We attach the callback (the function to execute on alarm firing which will do the blinking)
  * We configure next the alarm using `change_period`
  * Finally, we enable and start the timer
* `_timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)`: this is our callback (ISR) function that manages blinking LED
  * We define an pointer to an LED instance with its pin number.
  * We read next the current status of that GPIO pin (high/low) with `gpio_get_level` and store it!
  * We set the level to be the inverse of the current status (if was on turn off and so on)
  * We return true to exit the ISR function. This will be called at each alarm (blink at `period_us` time interval)

### Final summary:
💡 **_Think of GPTimer as a tiny stopwatch in your microcontroller that you can program to ring a bell (trigger an interrupt) whenever you want – precisely and repeatedly – without the CPU having to keep checking the time itself._**
