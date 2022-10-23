# TestGenerator
## Introduction

The solution of the programming problem is not completed after writing the code. The code needs to be compiled and tested using different sets of arguments.
During testing, the following tasks arise:
1. It is necessary to check the reaction of the program on obviously incorrect arguments.
2. It is necessary to check the program on extreme values (0, non-existent files, empty strings, ...)
3. It is necessary to check the program on arbitrary values
4. It is necessary to memorize sets of arguments we use (for rechecks)
5. It is necessary to remember the test results
6. It is necessary to limit the running time of the program.

Manual testing takes too much time and often does not guarantee the completion of any task (except, perhaps, number 3). Realizing this in time, I moved on to testing with BASH scripts.
What is a BASH script? This is a program that runs terminal commands sequentially. The simplest testing script test.sh looks like this.

```
#This is a comment in file test.sh:

#!/bin/bash
./a.out 123 string1 1.234
./a.out 456 string2 5.678
```

if we run test.sh, program a.out will be run sequentially with two different sets of arguments.

By explicitly listing all the sets of arguments we are interested in, we will be able to achieve the fulfillment of all 6 of the above points (for the 6th point we will have to use the "timeout" command).
The fulfillment of all 6 points practically guarantees the correctness of our decision, which can be considered a success. However, this way of testing takes a lot of time for two reasons:

  a) writing a script
  
  b) selection of sets of arguments

The above considerations (and lack of tests for weekly assignments in University) led to the creation of TestGenerator.


## What is it?
	
TestGenerator receives as arguments the names of config files in which, according to certain rules, can be written:
 - name of the program to test,
 - valid sets of arguments,
 - time limit for a runtime,
 - ct key (stands for clever test), responsible for 1. and 2. testing tasks.
 
Based on each file, TestGenerator creates a BASH-script test for the program specified in the file.


## What problems does TestGenerator solve?

1. Checking on numbers that go beyond the specified limits; Checking on non-existent files; However, there're no tests for the wrong number/order of arguments :(
2. Checking for 0; Checking for extreme values; Checking for empty files.
3. Checking on random numbers and strings.
4. The list of test arguments is located directly in the code of the generated script.
5. The test results are in a separate file.
6. It is possible to limit the program's runtime.

  a) You need to write a config file listing the possible types of arguments
  
  b) Sets of arguments are generated automatically

## How to use TestGenerator?

To begin with, you should write a config file with a description of the argument sets and some testing parameters.
For example, the file args.txt:

```
!out a.out
!timeout 5m
!ct

{
int
char
double
} 5

{
string
file
} 1
```

This file says following to TestGenerator:

```!out a.out``` - you need to create a test for a program named "a.out" .

```!timeout 5m``` - you need to kill the program "a.out" 5 minutes after it starts.

```!ct``` - in addition to the usual test, you need to conduct a smart test. The absence of this line would mean that there is no need to conduct a smart test.

Further it is written that the program can be run with two different sets of arguments, the sets are described in curly braces, after each set the number of normal tests for this set is indicated.
The first set consists of an integer, a symbol, and a real number. For it, you need to generate 5 regular tests.
The second set consists of a string and a file name (the difference between them is noticeable only for clever testing). For this set, you need to generate 1 regular test.

The usual test involves a simple substitution of random strings, symbols and numbers to the appropriate places.
A smart test analyzes arguments and substitutes extreme, "dangerous" values in their places. For example, a nonexistent file or 0.

Running ```./TestGenerator args.txt``` will generate something like this:

<details><summary>Click</summary>

```
#!/bin/bash

# It's a comment!
Test_Filename=a_test_results.txt
Timeout_Returned=124
Timeout_Time=5m
out=a.out
touch $Test_Filename # created file for test_results
exec &>$Test_Filename # redirected standard output from command line to output file
echo "~~~~~~~~~~~~~~~~Test starts now!~~~~~~~~~~~~~~~~"
echo
echo "-_-_-_-_-_-Clever test starts here!-_-_-_-_-_-"
touch Empty.txt
	j=1
	for Args in "-1  0.329 " "1  -0.946 " "0 �-1.84 " "40133809 \ 1.553 " "04234383 ; 0 "
	do
		echo
		echo "Test #1.$j"
		j=$(( $j + 1 ))
		echo "./$out $Args"
		timeout "$Timeout_Time" ./$out $Args
		if [[ $? -eq $Timeout_Returned ]]; then
			echo "Timeout ($Timeout_Time)!"
		fi
		echo
	done
	j=1
	for Args in "X
                       y5O$T " "�̢ P/�o. "
	do
		echo
		echo "Test #2.$j"
		j=$(( $j + 1 ))
		echo "./$out $Args"
		timeout "$Timeout_Time" ./$out $Args
		if [[ $? -eq $Timeout_Returned ]]; then
			echo "Timeout ($Timeout_Time)!"
		fi
		echo
	done
rm Empty.txt
echo
echo
echo "~_~_~_~_~_~Clever test ends here!~_~_~_~_~_~"
echo
echo
echo "*^*^*^*^*^*Normal test starts here!*^*^*^*^*^*"
echo
echo
	j=1
	for Args in "43363828  -0.792 " "33590091 W 0.508 " "91200964 + 23.469 " "-5602189 ; 0.686 " "34616253 �0.155 "
	do
		echo
		echo "Test #1.$j"
		j=$(( $j + 1 ))
		echo "./$out $Args"
		timeout "$Timeout_Time" ./$out $Args
		if [[ $? -eq $Timeout_Returned ]]; then
			echo "Timeout ($Timeout_Time)!"
		fi
		echo
	done
	j=1
	for Args in "�E\B8m H�>8m "
	do
		echo
		echo "Test #2.$j"
		j=$(( $j + 1 ))
		echo "./$out $Args"
		timeout "$Timeout_Time" ./$out $Args
		if [[ $? -eq $Timeout_Returned ]]; then
			echo "Timeout ($Timeout_Time)!"
		fi
		echo
	done
echo
echo
echo "*.*.*.*.*.*Normal test ends here!*.*.*.*.*.*"
```
</details>

## More options


The types of arguments in each set can be described more difficult, for example:
```
{
int [0; 10] # random number from 0 to 10
int const -1 # number -1
char [a; z] # random lower case letter of English alphabet
char const F # character F
double const 1e-14 # number 1e-14
string SunAug1415:41 # string SunAug1415:41
file README.md # file name - README.md
} 109
```
All possible parameters for argument types are listed above. In smart tests, boundary and out-of-bounds values (as well as empty and non-existent files) will be substituted instead of the specified values
	
## How to run?
  
After writing the file *args.txt* (the name can be any), you should run TestGenerator: 
```./TestGenerator args.txt```
If the name of the program under test was *a.out*, generated script will be named as *a_test.sh* .
Now you can run the script: 
```./a_test.sh```
At startup, the standard output and error stream are redirected from the terminal to a file *a_test_results.txt* .

## Issues

What problems does TestGenerator have?

* It's written in C. 
* Messy code in [read.c](read.c)
* Weak documentation, few comments
* The syntax of input files is checked during interpretation, this is normal, but because of the confusing code it will be difficult to find a bug
* It lacks features 
* There is no generation of real numbers in a given segment
* When analyzing a file, there may be problems with comments and empty lines.
* Clever testing does not substitute empty strings in place of string type arguments
* You cannot define multiple values for an argument
