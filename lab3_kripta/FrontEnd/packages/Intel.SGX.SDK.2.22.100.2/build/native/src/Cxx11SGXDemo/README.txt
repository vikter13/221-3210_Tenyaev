Purpose of Cxx11SGXDemo:

The project demonstrates serveral C++11 features inside the Enclave:
- lambda expressions;
- rvalue references and move semantics;
- automatic type deduction with auto and decltype;
- nullptr type;
- strongly typed enum classes;
- Range-based for statements;
- static_assert keyword for compile-time assertion;
- initializer lists and uniform initialization syntax;
- New virtual function controls: override, final, default, and delete;
- delegating constructors;
- new container classes (unordered_set, unordered_map, unordered_multiset, and unordered_multimap);
- tuple class;
- function object wrapper;
- atomic, mutexes, condition_variables;
- new smart pointer classes: shared_ptr, unique_ptr;
- new c++ algorithms: all_of, any_of, none_of;
- variadic templates;
- SFINAE;

How to build/execution:
=========
Windows:
	1. Make sure you have the Intel® Software Guard Extensions Windows SDK package installed.
	2. Enclave test key(two options):
    		a. The project will generate a test key<Enclave_private.pem> automatically when you build the project.
    		b. Rename your test key(3072-bit RSA private key) to <Enclave_private.pem> and put it under the <Enclave> folder.
	3. Open the Visual Studio solution file "SampleEnclave.sln". 
	4. Build and execute it directly.