# sensor-data-acquisition-digitizing-and-processing
A common operation in embedded microprocessor systems is sensor data acquisition and signal conditioning. If the signal is known to be noisy it is beneficial to apply a low-pass filter to remove some of that noise. This program operates on embedded system tempearture sensors. The sensors perform data acquisition and conditioning, while providing a simple graphical output using the STM32F4-Discovery's LEDs. A supporting external circuitry of 7-segment displays has also been created and interfaced through the digital I/O pins to display the temperature numerically. 