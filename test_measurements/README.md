Test Measurements
=================

This folder contains the sample measurements to test the differential power
analysis attack against a known key.

The power traces along with the plaintext, ciphertext and key can be found
within the HDF5 file:
    traces.h5

The structure is as follows:
	
traces.h5
 |-- ciphertext   <-- data sent to the smar card to be decrypted
 |-- plaintext    <-- data received from the smar card after decryption
 |-- traces       <-- measurements during decryption
 |-- key          <-- values of the key used in the smart card