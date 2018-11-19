# Apache#34464

## Link

1. [bug link](https://bz.apache.org/bugzilla/show_bug.cgi?id=34464_)
2. [buggy](https://bz.apache.org/bugzilla/attachment.cgi?id=18328&action=diff)
3. [patch1](https://bz.apache.org/bugzilla/attachment.cgi?id=18328&action=diff)
4. [patch2](https://svn.apache.org/viewvc/ant/core/trunk/src/main/org/apache/tools/ant/taskdefs/optional/net/TelnetTask.java?view=markup&pathrev=677887)

## Desc

void waitForString(String s, Integer timeout) in TelnetTask.java
String s is a pattern string. 
The function reads chars from inputstream until the pattern matched.
If timeout is set, then the function will wait until the inputstream is available, otherwise it throws exception on timeout.
The code uses StringBuffer to store the chars from inputstream
Whether timeout is set or not, the code uses the same way to compare the StringBuffer with the pattern string.

1. The buggy code searches the entire StringBuffer for the pattern for every input char. The search function is indexOf(). When the # of input chars grows, the runtime will also grow.
2. The patch1 skips the first '''len(pattern)-1''' chars of the StringBuffer; it only performs indexOf on a minimal substring.
3. The patch2 replaces indexOf with !equals.

## Catogery

The complexity of buggy code is O(N).
The patch1 solution is to reduce compare times.
The patch2 solution also uses quicker matching functions.

## Reimplement

The original code is in Java. To adapt to ComAir, I re-implemented it with C++ (apache34464.cpp)
The primary code snippets are listed below:

1. class String: immutable after init, implements ```length, indexOf, equals``` and other helper functions.
2. class StringBuffer: exensible like std::string in C++, implements ```toString, substring, append``` and other helper functions.
3. waitForString: the buggy code, only focuses on string match parts.
4. waitForStringPatched: the patched code, only focuses on string match parts.

The usage is

```
./apache34464 PATTERN < INPUT_FILE

e.g. ./apache34464 abc < input_1000_100
```

## Input

The input files are auto-gen by gen_input.py

1. It generates pattern string with 100 chars.
2. It generates random string at incremental length (900, 1900, 2900, ... , 9900) that does not contain the pattern, then appends the pattern to the end of the random string.

## Make

To generate the original bug and the two patches

```
mkdir original
cd original
make -f ../makefile
```
The generated files apache34464.buggy, apache34464.patch1, apache34464.patch2 correspond to buggy, patch1, and patch2 respectively.

To generate the bc of the buggy code

```
mkdir build
cd build
make -f ../makefile.bc
```
The generated files apache34464.0.0.preopt.bc and apche34464_func_name_id.txt are in build/

## Instrument

Input: build/apache.0.0.preopt.bc
Output: build/apache.no.opt

```
./instrument.sh
``` 
## Run

1. runtime compare, to validate the re-implementation
```
cd run
./runtime_apache34464.sh
```
```
apache34464.buggy (ns)
3467080
13108812
23538574
21830939
27228362
38906720
52599481
67530372
86449598
107147394
apache34464.patch1 (ns)
115111
215602
314507
415024
554224
693695
768689
8pache34464.patch2 (ns)
93681
213161
265689
401596
457354
545204
643373
712306
789849
881760
42628
928068
1023762
apache34464.patch2 (ns)
93681
213161
265689
401596
457354
545204
643373
712306
789849
881760
```

2. in-house: apache34464_all_result.csv records ```func_id,rms,cost,chains,run_id```
```
cd run
./run_apache34464.sh
```
```
func_id,rms,cost,chains,run_id
0,0,0,0,1
8,16,1,0,1
20,8,1,0,1
8,16,1,0,1
15,32,2,1,1
...

```

3. Fitting

