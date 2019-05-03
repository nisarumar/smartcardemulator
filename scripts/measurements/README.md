Measurement Scripts
===================

This folder contains the measurement scripts used in the smart card lab by
TUEISEC.

In order to record power traces, connect the oscilloscope to the computer and
attach the probe on channel A to the jumper connecting to the shunt resistor of
the smart card, and the probe on the external trigger to the trigger pin on the 
smart card.

Before running a measurement please adjust the settings in
'trace_measurement.py' to your needs. All constants are defined in 'pshelper.py'

Then run a measurement by executing:
    python trace_measurement.py
