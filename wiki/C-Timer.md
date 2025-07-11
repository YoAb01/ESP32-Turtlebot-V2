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
**A timer is like an alarm clock that rings at exactly the time you set, and keeps ringing repeatedly if you tell it to.**
