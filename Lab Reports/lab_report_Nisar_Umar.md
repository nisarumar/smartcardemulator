# __Lab Report - Smartcard__ 

**Author:** Nisar, Umar

**Matr.:** ga53gax

**Group:** 03       


## __First part: Smartcard Cloning__ ##

* **Date:** *08/05/2019*
* **Tasks:**          
 * Obtain and plot Trace Data for the first time
 
* **Date:** *12/05/2019*
* **Tasks:**          
 * Setup project environment in Linux
 
* **Date:** *15/05/2019*
* **Tasks:**          
 * Added USART Module and link IOStream to USART for debugging
 
* **Date:** *18/05/2019*
* **Tasks:**          
 * Added Fifo structure in the code
 
* **Date:** *19/05/2019*
* **Tasks:**          
 * Added basic framework for Asynchronous RX/TX (ART)
 
* **Date:** *20/05/2019*
* **Tasks:**          
 * After Lots of debugging observed TX on Logic Analyser
 
* **Date:** *01/06/2019*
* **Tasks:**          
 * from last milestone to this lots of debugging 
and Bugs fixing in ART module, ART completed
 
* **Date:** *04/06/2019*
* **Tasks:**          
 * Started Working on APDU/TPDU Protocol
 
* **Date:** *06/06/2019*
* **Tasks:**          
 * Completed APDU Protocol, merged AES decryption and observed complete video
streaming the first time, first release
 
## __Second part: DPA Countermeasures and Benchmarks__ ##

* **Date:** *12/06/2019*
* **Tasks:**          
 * Started Working on Random Number Generation
 * Started exlporing options for raw entropy and pseudo number generator
 
* **Date:** *26/06/2019*
* **Tasks:**          
 * RAW entropy from Watchdog Timer Started Working
 * Meanwhile I started analysing Random Number with the script I had developed
 in python before

* **Date:** *04/07/2019*
* **Tasks:**          
 * Completed Random Number Generation
    * Incorporated fhreefish library for PRNG
    * Mechanism to generate, whiten and store raw entropy in parallel with 
    the communication
    * Finally managed Timing alongwith communication of APDU's,  *most time consuming task*


* **Date:** *XX/XX/XXXX*
* **Tasks:**          
 * Acquired Traces for DPA attacks
 * Made changes by turning on and off PRNG to facilitate DPA attacks and by moving 
trigger
 * Tried to Debug DPA attacks

