------------------------
Purpose of SealUnseal
------------------------
The project demonstrates sealing/unsealing data between two enclaves.

------------------------------------
How to Build/Execute the Sample Code
------------------------------------
	1. Make sure you have the Intel® Software Guard Extensions Windows SDK package installed.
	2. Enclave test key(two options):
    		a. The project will generate a test key<Enclave_Seal_private.pem> automatically when you build the project.
    		b. Rename your test key(3072-bit RSA private key) to <Enclave_Seal_private.pem> and put it under the <Enclave_Seal> folder.
	3. Open the Visual Studio solution file "SealUnseal.sln". 
	4. Build and execute it directly.