# electro-hydraulic-control-system

⚙️ Control of an Electro-Hydraulic Actuator System

📌 Overview

This project focuses on the design, identification, and control of an electro-hydraulic actuator system, combining hardware development, system modeling, and real-time control implementation.

🔌 Hardware & Driving Circuit

A custom driving circuit was designed to operate the electro-hydraulic system, which includes a pump, pressure valve, directional valve, and hydraulic actuator.

The system was controlled using:

1-Arduino microcontroller.

2-BTS7960 motor drivers (for valve actuation).

3-Signal conditioning circuits (low-pass filter, level shifter, anti-aliasing filter).

4-Position feedback via potentiometer.

🔬 System Identification

An experimental identification procedure was conducted to obtain a linear model of the system:

1-Closed-loop identification using PRBS excitation.

2-Data acquisition via Arduino and MATLAB.

3-Model estimation using MATLAB System Identification Toolbox.

4-Achieved model accuracy of approximately 86% fit.

⚙️ Control Design

A phase-lead controller was designed to meet specific performance requirements:

1-Overshoot < 4%

2-Settling time < 2 seconds

3-Zero steady-state error

The controller parameters were determined using Bode diagram analysis and validated through simulation in Simulink.

💻 Implementation Files

1-Arduino code for real-time control

2-MATLAB scripts for identification and simulation

3-MATLAB App Designer interface for system control

📊 Experimental Results

The controller significantly improved system performance:

1-Overshoot: reduced to 0%

2-Time constant: improved by 59%

3-Delay time: improved by 60%

4-Settling time: improved by 68%

5-Steady-state error: reduced to 0%

🏫 Academic Contribution

This project has been officially archived at the Higher Institute for Applied Science and Technology (HIAST) and is used as a reference for teaching and research purposes.

🧠 Conclusion

The project demonstrates a complete control engineering workflow, from hardware design and system identification to controller implementation and validation. The results confirm the effectiveness of the proposed approach in achieving accurate and reliable position control.
