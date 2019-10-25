Arduino Software PWM With Timer 1
===

To change number of pin, find the code below from `PwmManager.cpp`:

```c++
  this->scheduler = new Scheduler(6);
```

and change `6` to number of pin what you want.



To change definition of pin's id, find the code below from `PwmManager.cpp`:

```c++
  uint8_t base = 3;
```

and change `3` to what your pin's id started.

