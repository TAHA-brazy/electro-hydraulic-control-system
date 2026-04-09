# electro-hydraulic-control-system

⚙️ Control of an Electro-Hydraulic Actuator System

📌 Overview

This project focuses on the design, identification, and control of an electro-hydraulic actuator system, combining hardware development, system modeling, and real-time control implementation.

🔌 Hardware & Driving Circuit

A custom driving circuit was designed to operate the electro-hydraulic system, which includes a pump (supply unit), pressure valve, directional valve (control unit), and hydraulic actuator.

![Hydrulic_System](images/Hydrulic_System.png)

The system was controlled using:

1-Arduino microcontroller.

2-BTS7960 motor drivers (for valve actuation).

3-Signal conditioning circuits (low-pass filter, level shifter, anti-aliasing filter).

4-Position feedback via potentiometer.

The general driving circuit dieagram:

![drivining_circuit_diagram](images/drivining_circuit_diagram.png)

The electronic driving circuit dieagram:

![drivining_circuit_diagram2](images/drivining_circuit_diagram2.png)

The real driving circuit:

![real_driving_circuit](images/real_driving_circuit.png)

🔬 System Identification

An experimental identification procedure was conducted to obtain a linear model of the system:

1-Closed-loop identification using PRBS excitation.

![closed_loop](images/closed_loop.png)

  - The identification procedure box diagram:

    ![Identification_diagram](images/Identification_diagram.png)

2-Data acquisition via Arduino and MATLAB.

![Identification_procedure](images/Identification_procedure.png)

3-Model estimation using MATLAB System Identification Toolbox.

![Identification_Toolbox](images/Identification_Toolbox.png)

4-Achieved model accuracy of approximately 86% fit.

![es_result](images/es_result.png)


-The real work environment:

![real_work_environment](images/real_work_environment.png)

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
