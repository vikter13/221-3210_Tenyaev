Purpose of Sgx2Enclave:
=========
The project demonstrates opt-in for Intel® Software Guard Extensions (Intel® SGX) 2.0 for an enclave and the usage of Intel® SGX 2.0 APIs. 
To opt-in to Intel® SGX 2.0 behavior, link in the sgx_tedmm.lib library and import the sgx_tedmm.edl
file.

How to build/execution:
=========
Windows:
	1. Make sure you have the Intel® SGX Windows SDK package installed.
	2. Enclave test key(two options):
    		a. The project will generate a test key<Enclave_private.pem> automatically when you build the project.
    		b. Rename your test key(3072-bit RSA private key) to <Enclave_private.pem> and put it under the <Enclave> folder.
	3. Open the Visual Studio solution file "Sgx2Enclave.sln". 
	4. Build and execute it directly.
