# PWM Servomotor Demo Code
## 7-20-21

- Program will boot up and position servomotor at 90 degrees location.
- Pressing S1 on launchpad will decrement angle by approximately 1.7 degrees
	- Servomotor cannot decrement below 90 degrees.
- Pressing S2 on launchpad will increment angle by approximately 1.7 degrees
  - Servomotor cannot increment past approximately 100 degrees.
  
  ## To minimize button debouncing:
- Press button quickly and allow a short amount of time between consecutive button presses
