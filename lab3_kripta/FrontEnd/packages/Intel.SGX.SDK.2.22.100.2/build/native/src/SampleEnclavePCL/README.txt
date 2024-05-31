Purpose of SampleEnclavePCL:
=========
The project demonstrates how Intel® Software Guard Extensions Protected Code Loader (Intel® SGX PCL) 
is integrated  into an existing Intel® SGX solution. 

How to build/execution:
=========
Windows:
	1. Make sure you have the Intel® Software Guard Extensions Windows SDK package installed.
	2. Enclave test key(two options):
    		a. The project will generate a test key<Enclave_private.pem>/<Seal_private.pem> automatically when you build the project.
    		b. Rename your test key(3072-bit RSA private key) to <Enclave_private.pem>/<Seal_private.pem> and put it under the <Enclave>/<Seal> folder.
	3. Open the Visual Studio solution file "SampleEnclavePCL.sln". 
	4. Build and execute it directly.
Linux:
	1. Make sure you have the Intel® Software Guard Extensions Linux SDK package installed.
	2. Enclave test key(two options):
		a. Install openssl first, then the project will generate a test key<Enclave_private_test.pem> automatically when you build the project.
    		b. Rename your test key(3072-bit RSA private key) to <Enclave_private_test.pem> and put it under the <Enclave> folder.
	2. Build the project with the prepared Makefile:
		a. Hardware Mode, Debug build:
		    $ make SGX_MODE=HW SGX_DEBUG=1
		b. Hardware Mode, Pre-release build:
		    $ make SGX_MODE=HW SGX_PRERELEASE=1
		c. Hardware Mode, Release build:
		    $ make SGX_MODE=HW
		d. Simulation Mode, Debug build:
		    $ make SGX_DEBUG=1
		e. Simulation Mode, Pre-release build:
		    $ make SGX_PRERELEASE=1
		f. Simulation Mode, Release build:
		    $ make
	3. Execute the binary directly:
		$ ./app

