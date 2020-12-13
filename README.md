[CMU 15-213] - Introduction to Computer Systems
====

###### tags: `Linux`, `system programming`

# Note 


References: 
- [1](https://github.com/JonnyKong/CMU-15-213-Intro-to-Computer-Systems)
- Course website: [ICS @ CMU Fall 2017](http://www.cs.cmu.edu/afs/cs/academic/class/15213-f17/www/)

Color:
- <font color="#149940">Take a look at</font> reentrant SIO (Safe I/O library) from `csapp.c` in code example.


## Big overview (Ch1, Lec 1)

- Lec 1: (code not yet finished, L0 code review)
    Overview: Further courses, e.g. Networking, Compiler, OS, Distributed system, Computer Graphic, embedded system, Computer Architecture, DB(Storage Systems)!!

## Data Representation and Manipulating Information (Ch 2, Lec 2~4)

- Overview: 
    3 representation for numbers: **Unsigned integer**, **Signed integer** and **Floating-point**.
    - Something in common:
        1. Using fixed size of bits, the result after arithmetic operation may be overflow.
    - Something different: 
        1. Overflow: 
            - Integer: Producing overflow result according to **integer modular arithmetic** (Ring theory).
            - Floating-point: Overflow to positive infinity $\infty$ (or negative infinity $-\infty$).
        2. Associativity and distributivity: 
            Floating-point arithmetic doesn't follow these 2 properties. The reason compared to integer representations is that floating-point representation can encode a wide rage of values, but only approximately.
- Endianness
    - Big Endian: Least significant byte has highest address.
    - Little Endian: Least significant byte has lowest address.

    ![](https://i.imgur.com/pMZpXAB.png)

### Integer

- Representation 
    Two's Complement for **signed integer**.
    - Conversion and casting: Keep bit field and reinterpret with different representation.
        <font color="#ff0000"> **Signed** value implicitly cast int o **Unsigned**. </font> 
    - Expanding bit representation of integer number
        - Zero extension: Unsigned.
        - Signed extension: Signed.
    - Truncation
        - Unsigned: $x' = x\ mod 2^k$
        - Signed: $x' = U2T_k(x\ mod 2^k)$
- Integer Arithmetics
    - Negatioin of **Signed** integer (Two's complement) 
        $-^t_x = \begin{cases} TMin_w, & \ x = TMin_w \\ -x, & \ x>TMin_w \end{cases}$
- Shifting: 
    - Left shifting $k$: Multiplying by $2^k$.
    - Right shifting:
        - Logical shifting: Division for unsigned.
        - Arithmetic shifting: 
            - Simply shifting: Rounding to infinity.
            - Correct division by $2^k$: Rounding to 0.
                Adding bias $2^k-1$ before shifting.
                In C: `(x + (1<<k) - 1) >> k`
- Cautions about:
    - Signed or Unsigned for `char`:
        It's not specified by the standard. In practice, be sure to use `signed char` or `unsigned char`. - [CMU SEI: INT07-C. Use only explicitly char type for numeric values](https://wiki.sei.cmu.edu/confluence/display/c/INT07-C.+Use+only+explicitly+signed+or+unsigned+char+type+for+numeric+values)
    - Operation combining with **Signed** and **Unsigned** integer (casting)
        <font color="#ff0000"> **Signed** value implicitly cast int o **Unsigned**. </font> 
    - Implicit casting 
    - Returned data type by functions.

### Floating Point

- Representation (IEEE 754)
    Formula: values = $(-1)^s \times M \times 2^E$
    - Different fields:
        - $s$: signed bit.
        - $exp$: field for calculating $E$.
        - $frac_2$: field for calculating $M$.
        - $Bias$: $2^{k-1} - 1$, where $k$ is the length of $exp$ field.

    ![](https://i.imgur.com/Jq5a7ny.png)
    - 3 kinds of values: 
        - Normalized (steps getting larger while leaving 0)
            When $exp \neq 000...0\ and\ 111...1$
            $E = exp - Bias$
            $M = 1.frac_2$
        - Denormalized (equal steps)
            When $exp = 000...0$
            $E = 1 - Bias$
            $M = 0.frac_2$
        - special number
            When $exp = 111...1$
            $frac_2 = 000...0 \rightarrow$ infinity (positive or negative)
            $frac_2 =\neq 000...0 \rightarrow$ Nan (Not-a-number) 
        
        ![](https://i.imgur.com/NgzXKDF.png)
    - Rounding
      ![](https://i.imgur.com/nUPU832.png)
      - $0.RXXX > 0.5 \rightarrow$ Rounding up.
      - $0.RXXX < 0.5 \rightarrow$ Rounding down.
      - $0.RXXX = 0.5 \rightarrow$ Rounding to nearest even.

## Machine-Level Programming (x86-64, Ch3, Lec 5~9)

- ?? (Lec 5, reading not finished)
    - RISC vs. CISC, why RISC is faster than CISC?
        1. (Short) Fixed length for most of the instructions in RISC
        2. Fixed length instructions with similar working sequences that could be separated into different stagges $\rightarrow$ ease in optimization (pipelining).
    - Backward compatibility (32, 16 bit) in Hardware (x64)?
        Reference low-order 4 bytes (also low-order 1 & 2 bytes) in each register. (slide 5-17~18)

    [x86-64 guide](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
    [x86-64 programming](https://www.lri.fr/~filliatr/ens/compil/x86-64.pdf)
    [Guide to x86-64](http://web.stanford.edu/class/cs107/guide/x86-64.html)

- Control (Lec 6, reading not finished)
    - `movq`: access memory 
    - `leaq`: load effective address, not accessing memory.
    - Note that some of the instructions won't change the condition codes (conditional flag).

- Procedure (Lec 7, reading not finished)
    - x86-64 Stack
        ![](https://i.imgur.com/NkxHGTq.png)
    - Passing control
        - Procedure call: `call`,  `label`
            - Push return address on stack
            - Jump to label
        - Procedure return: `ret`
            - Pop address from stack
            - Jump to address
        -  Return address: Address of the next instruction right after call
    - Passing data: Calling convention
        - Passing arguments: 
            - First 6 arguments: `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, and `%r9`
            - Remaining: Storing in `Stack`
            - IA-32 pass all arguments in stack
        - Return value: `%rax`
    - Managing local data
        - Stack Frames: state for single procedure instantiation
            - Boundary of each stack frame: marked by `%rbp` (Base  Pointer, optional) and `%rsp` (Stack Pointer). The image shows the actual contents in stack frame.
                ![](https://i.imgur.com/K0DWmY7.png)
        - Register Saving Conventions: Why do we need to make an agreement of **caller-saved** and **callee-saved** registers? 
            Recalling that we can compile the functions (source codes) separately, generally, we don't have a full and complete picture about which registers to preserve or not. Therefore, we got to make an agreement of **caller-saved** and **callee-saved** registers.
            - **caller-saved** registers:
                - Can be altered (trampled) by Callee arbitarily, Caller should save (squirrel away) those contents in **Stack** before function call. 
                - Shall be retrieved by caller after function call.
            - **callee-saved** registers:
                - The caller has the ownership of contents in such registers, therefore, the callee should keep those contents in **Stack** before using it. 
                - Shall be restored by callee right before function return. 
    - Illustration of Recursion: Recursion (& mutual recursion) handled by normal calling conventions.
        - Can safely store values in local stack frame and in callee-saved registers.

- Data (Lec 8, reading not finished)
    -  Pointer and array 
        - Array of pointers: `int *A2[3][5]`
        - Pointer to the head of an array: `int (*A3)[3][5]`
    - Structure 
        - Structure in memory:
            1. Stored in a block of memory which could hold all the fields in structure.
            2. Fields ordered according to declaration
                ![](https://i.imgur.com/9cl6XP8.png)
                $\uparrow$ No proper alignment while programming
                $\downarrow$ Proper alignmentmay reduce the wasted memory (not saving any memory for this case)
                ![](https://i.imgur.com/ce95Ciq.png)
                ![](https://i.imgur.com/EMRAf5a.png)
            3. Compiler determines overall size + positions of fields: Smart compiler could help us reduce the wasted memory (store nothing). 
                - Offset of each structure member determined at compile time.
                - Assembly-language programs are also generated at compile phase (some of assembly instructions should be generated with the offset structure member).
                - Inserts gaps in structure to ensure correct alignment of fields.
    - Floating Point
        - Basics
            - Arguments passed in `%xmm0`, `%xmm1`, ...
            - Result returned in `%xmm0`
            - All XMM registers caller-saved.
        - FP Memory Referencing
            - Integer (and pointer) arguments passed in regular registers
            - FP values passed in XMM registers
            - Different mov instructions to move between XMM registers, and between memory and XMM registers.

- Advanced Topics (Lec 9, reading not finished, [code](http://www.cs.cmu.edu/afs/cs/academic/class/15213-f17/www/code/09-machine-advanced/) not yet finished)
    - x86-64 Linux Memory Layout
        ![](https://i.imgur.com/UmQVHlf.png)
        - Computer memory layout: 
            - Currently 48 address line (currently needed, [reference](https://stackoverflow.com/a/6716976/11612236))
            - The above figure only uses 47 bits.
            - The further upper part is used by kernel.
        - Stack:
            - The limited 8 MB aims to avoid unbounded recursion, once overacross 8 MB $\rightarrow$ Segmentation fault.
            - Storing local variables and info for returning.
        - Text: 
            - Excutable code / instruction generated by compiler and linker.
            - Readable and executable but not writeable.
        - Empty section beneath Text: used for avoiding certain attack.
    - Vulnerability
        - Buffer overflow: exceeding the memory size allocated for an array.
            - May be triggered by functions manipulating strings of arbitrary length, e.g. `gets`, `strcpy`, `strcat`, `scanf`, `fscanf`, `sscanf`
        - Attacks
            1. Stack smashing 
                Using unchecked buffer to overwrite the state of the machine, particularly the return pointer, and gain access to the system or even take control of the system.
            2. Code Injection
                Input string contains byte representation of executable code and overwrite return address A with address of buffer B. When Q executes ret, it will jump to exploit code.
            3. Return-Oriented Programming Attacks (ROP)
                - Make use of "gadgets" in text segment or segment of shared library.
                - Making a series of actions by placing a series of return addresses on stack.
                - Trigger with `ret` instruction and start executing Gadget 1, `ret` in each gadget will start next one.
        - Protection against attacks
            1. Use routines limiting string lengths (user-level)
            2. Randomized stack offsets
            3. Nonexecutable code segments
            4. Stack canaries

## Code optimization (Lec 10, reading not finished, web aside SIMD not yet read it)

- Optimization by programmer
    - Code motion: Reduce frequency of computations performed, especially moving code out of loop.
        ![](https://i.imgur.com/qfyOo8k.png)
        GCC will do this with `-O1` flag.
    - Reduction in strength: Reduce costly operation with simpler one. 
        ![](https://i.imgur.com/TcjPKkm.png)
            Integer multiplication usually takes more clock cycles than integer addition in CPU.
    - Share common subexpressions
        ![](https://i.imgur.com/pIstOfj.png)
- Optimization blockers
    - Limitations of Optimizing Compilers:
        1. Operate under fundamental constraint.
        2. Behavior that may be obvious to the programmer can be obfuscated by languages and coding styles.
        3. Most analysis is performed only within procedures: 
            Whole-program analysis is too expensive in most cases. Newer versions of GCC do interprocedural analysis within individual files, but not between code in different files.
        4. Most analysis is based only on static information: 
            Rather than **JVM JIT compiler**.
        5. Once in doubt, the compiler must be conservative. The most conservative way is not to change program behavior even though the computation would be costly.
    - Optimization Blocker #1: Procedure Calls
        - Why it is treated as blocker: Compiler may treat procedure call as a black box.
            - Procedure may have side effects: Alters global state each time called.
            - Function may not return same value for given arguments.
        - Remidedies:
            - Use of inline functions: GCC does this with `–O1` flag within single file.
            - Do your own code motion
    - Memory aliasing: Two memory references specify single location.
        The following code does memory load and store every time, because compiler assume possibility of memory aliasing:
        ![](https://i.imgur.com/qocm3xK.png)
        - Solution: Get in habit of introducing local variables
            - Accumulating within loops.
            - Your way of telling compiler not to check for aliasing.
            
            ![](https://i.imgur.com/8nglTy1.png)
- Instruction-level parallelism
    - Superscalar processor: A superscalar processor can issue and execute multiple instructions in one cycle. The instructions are retrieved from a sequential instruction stream and are usually scheduled dynamically.
    - Different arithmetic instructions take different number of CPU cycles to perform.
        ![](https://i.imgur.com/cp1Dz54.png)
        > - Latency: The number of processor clocks it takes for an instruction to have its data available for use by another instruction right after it starts executing.
        > - Throughput (Cycles/issue): Throughput is the number of processor clocks it takes for an instruction to execute or perform its calculations. An instruction with a throughput of 2 clocks would tie up its execution unit for that many cycles which prevents an instruction needing that execution unit from being executed. Only after the instruction is done with the execution unit can the next instruction enter. ==Generally speaking: It's the number of processor clocks for an instruction occupying an executing **unit** (not the whole **ALU**).==
        > 
        > Reference: [Intel dev](https://software.intel.com/content/www/us/en/develop/articles/measuring-instruction-latency-and-throughput.html)
    - Product (or sum) of elements in vector (Normal, optimized case):
        - Serial Computation (OP = * ): Normal
            ```c
            /* Product (or sum) of each element in vector (Combine4) */
            for (i = 0; i < limit; i++) {
                x = x OP d[i];
            }
            ```
            Waiting for last operation to complete.
            ![](https://i.imgur.com/EGNZrn5.png)
        - Loop Unrolling: Optimized, this optimization could only be achieved when there are multiple **ALUs** used for pipelining. 
            1. Ops in the next iteration can be started early (no dependency), breaking sequential dependency.
                ```c
                /* Combine 2 elements at a time (Unroll 2x1a) */
                for (i = 0; i < limit; i+=2) {
                    x = x OP (d[i] OP d[i+1]);
                }
                ```
                ![](https://i.imgur.com/smjRYk9.png)
            2. Two independent "streams" of operations. (from slide 10-44)
                ```c
                /* Combine 2 elements at a time (Unroll 2x2) */
                for (i = 0; i < limit; i+=2) {
                    x0 = x0 OP d[i];
                    x1 = x1 OP d[i+1];
                }
                /* Finish any remaining elements */
                for (; i < length; i++) {
                    x0 = x0 OP d[i];
                }
                x = x0 OP x1;
                ```
                ![](https://i.imgur.com/OWjIlrX.png)
        - Programming with **AVX2**, making use of Vector Instructions. E.g. SIMD (Single instruction, multiple data).
            1. Parallel operations on multiple data elements.
            2. More info: See Web Aside [OPT:SIMD](http://csapp.cs.cmu.edu/3e/waside/waside-simd.pdf) on [CS:APP web page](http://csapp.cs.cmu.edu/).
- Branch prediction: Dealing with Conditionals. We may be on the wrong way of pipelining.
    - Idea: Guess which way branch will go, begin executing instructions at predicted position, but don’t actually modify register or memory data.
    - Branch Misprediction Recovery: Reload Pipeline.
        - Multiple clock cycles on modern processor
        - Can be a major performance limiter
    - Default behavior: 
        - Backward branches are often loops $\rightarrow$ predict taken
        - Forward branches are often if $\rightarrow$ predict not taken
    - Average better than 95% accuracy
        - [Classical example](https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array) for branch prediction.

## The Memory Hierarchy (Ch 6, Lec 11~12)

- Storage technologies 
    1. RAM (Volatile Memories)
        - SRAM (Static RAM)
        - DRAM (Dynamic RAM)
            - Synchronous DRAM (SDRAM)
            - Double data-rate synchronous DRAM (DDR SDRAM)
                - Different types distinguished by size of small prefetch buffer:
                    DDR (2 bits), DDR2 (4 bits), DDR3 (8 bits)
    2. Rotating disks (Nonvolatile Memories)
    3. SSD (Nonvolatile Memories)
        - Page can be written only after its block has been erased.
        - Writing through logical block.
            The logical blocks constantly cycle through the physical blocks. It's the work of SSD controller.
- Locality
    - Temporal locality
        Recently referenced items are likely to be referenced again in the near future.
    - Spatial locality
        Items with nearby addresses tend to be referenced close together in time.
- Memory Hierarchies
    ![](https://i.imgur.com/atBH1tO.png)
    
    - Cache 
        - Each level in hierarchy serves as cache for the level below.
        - 3 Types of Cache Misses
            1. Cold (compulsory) miss: Start up the empty caches.
            2. Capacity miss: The set of active cache blocks (working set) is larger than current cache size. $\rightarrow$ Evicting victim (block to be kicked out)
            3. Conflict miss: The level k cache is large enough, but multiple data objects all map to the same level k block. (Restriction imposed by hardware)
- Cache memory organization and operation
    - Cache shipped with CPU: 
        - L1 (Instruction + data cache)
            - Instruction cache: where we store recently used instruction for fast decoding.
        - L2, L3
        - CPU chip image: lighten by tungsten halogen lamp (with good CRI and wide range of spectrum).
    - General Cache Organization
        ![](https://i.imgur.com/wtxDWsd.png)
        ![](https://i.imgur.com/tPG88wD.png)
        If tag doesn’t match (= miss): old line is evicted and replaced.
        - Direct Mapped Cache
            ![](https://i.imgur.com/u7duyrv.png)
            ![](https://i.imgur.com/P9M7NDN.png)
            If tag doesn’t match (= miss): old line is evicted and replaced.
        - E-way set associative cache (Here E / associativity = 2)
            ![](https://i.imgur.com/QDJFUU2.png)
            ![](https://i.imgur.com/19SdE1o.png)
            No match (= miss):
            - One line in set is selected for eviction and replacement
            - Replacement policies: random, least recently used (LRU), ...
        - Write-hit and Write-miss
            - What to do on a write-hit?
                - Write-through (write immediately to memory)
                - Write-back (defer write to memory until replacement of line)
                    Each cache line needs a dirty bit (line different from memory or not)
                    ![](https://i.imgur.com/mglgv1T.png)
            - What to do on a write-miss?
                - Write-allocate (load into cache, update line in cache)
                    Good if more writes to the location follow
                - No-write-allocate (writes straight to memory, does not load into cache)
            - Typical
				- Write-through + No-write-allocate
				- **Write-back + Write-allocate** $\leftarrow$ used in practice
		- Properties of Cache performance
		    - PKI (Performance Key Indicator)
		        - Miss Rate
                    - 3-10% for L1
                    - Quite small (e.g., < 1%) for L2
                - Hit Time: Addressing decoding + delivering a line in the cache to the processor.
                    - 4 clock cycle for L1
                    - 10 clock cycles for L2
                - Miss Penalty: Additional time required for main memory referencing
                    - typically 50-200 cycles.
            - 99% hits is twice as good as 97%?
                - Average access time:
					97% hits: 1 cycle + 0.03 x 100 cycles = 4 cycles
					99% hits: 1 cycle + 0.01 x 100 cycles = 2 cycles
                    $\rightarrow$ This is why “miss rate” is used instead of “hit rate”. 
            - Write cache-friendly code
                - Make the common cases go first.
                - Minimize the misses in inner loops
                    - Try to maximize **spatial locality** by reading data objects sequentially with stride-1.
                    - Try to maximize **temporal locality** by using a data object as often as possible once it’s read from memory.

- Performance impact of caches
    - Memory Mountain
        ![](https://i.imgur.com/zAELrgB.png)
    - Rearranging loops to improve spatial locality
        Reducing the number of cache miss in each iteration of inner loop (reducing overhead of memory load/store). 
        Example in slide 12-40.
    - Using blocking to improve temporal locality
        Exploiting cache memories by block multiplication of matrix multiplication. 
        Example in slide 12-49.

## Linking (Ch 7, Lec 13)

https://blog.csdn.net/baidu_30073577/article/details/89453567

> Useful command-line package (detailed usage refers to mannual): 
> - `objdump`: display information from object files.
>     ```shell
>     $ objdump -t file_path # Display the contents of the symbol table(s)
>     $ objdump -r file_path # Display the relocation entries in the file
>     $ objdump -d file_path # Display assembler contents of executable sections
>     ```
> - gcc
>     ```shell
>     $ gcc -c main2.c addvec.c multvec.c      # Compile or assemble the source files, but do not link. 
>     $ gcc -static -o prog main2.o -L. -lvec  # Link object file with static library.
>     ```
>     - `-c`: Compile or assemble the source files, but do not link. Only create relocatable object file (.o).
>     - `-Ldir`: indicating searching directory.
>     - `-l`+library_name: indicating library to link.
>     - `-fpic`: Generate position-independent code (PIC) suitable for use in a shared library. 
>     - `-static` and `-shared`
> - `ar`: create, modify, and extract from archives.
>     ```shell
>     $ ar rcs libvec.a addvec.o multvec.o # Creating archive archive file name: libvec.a; library name: vec
>     $ ar –t library_path | sort          # showing members in archive
>     ```
> - `ldd`: print shared object dependencies for an executable file.
>     If we are running it with a C program, we could find out standard C library (`libc.so`) for most of the cases. (except that we linked the executable with static libraries explicitly)
> - 

- **Compiler** and **Linker** in reality: 
    - Compiler: Generation file with `.o` extension which is **relocatable object file**.
    - Linker: Pulling all those **relocatable object files** and libraries and generating a fully linked executable object file (containing code and data	for	all	functions).
- Why linkers?
    - Modularity
    - Efficiency (separate complilation)
- Two kind of linking
    - Static linking: 
        - Executable files and running memory images contain only the library code they actually use.
    - Dynamic linking: 
        - Executable files contain no library code.
        - During execution, single copy of library code can be shared across all executing processes.
- What Do Linkers Do?
    1. Symbol resolution:
        - Symbol: Function, global var, local static var or formated string (read-only data, e.g. string constant) stored in **symbol table**.
        - Each symbol in **symbol table** contains name, size and location.
        - Three kind of symbols:
            - Global symbols: non-static functions and non-static vars.
            - External symbols: symbols defined in other modules.
            - Local symbols: static functions and static vars that can only referenced by current module.
                - Local static variable: stored in either .bss, or .data.
                - Note: Do not confuse local symbols with local variables. Local variables are allocated in stack at runtime, and have nothing to do with linker.
        - Symbol resolution
            - Program symbols are either strong or weak
                - Strong: procedures and initialized globals.
                - Weak: uninitialized globals or ones declared with specifier `extern`.
            - Resolutionto multiple symbol in global scope
                - Multiple strong symbols are not allowed.
                - Weak symbols reference to the only one strong symbol.
                - Pick a weak symbol among all the weak symbols when there is no strong symbol.
                    - May cause undefined behavior over different compilers. To fix this issue: intialize if we declare a global variable or use `static` and `explicit` extern.
                    - Linker does not do type checking.
    2. Relocation
        - Merge `text` and `data` into a single segment.
        - Relative location of symbols $\rightarrow$ absolute location in executable.
        - Updates all references to these symbols to reflect their new positions (Table check-up).
            - Relocation entry is used to aid symbol resolution. E.g. `f: R_X86_64_PC32`
- Three kinds of object files:
    - Relocatable object file (`.o` file)
        - Contains code and data in a form that can be combined with other relocatable object files to form executable object file.
        - 1 source (.c) file $\rightarrow$ 1 `.o` file. 
    - Executable object file (`a.out` file)
        - Contains code and data in a form that can be copied directly into memory and then executed.
    - Shared object file (`.so` file or Dynamic Link Libraries (DLLs) in Windows)
        - Special type of relocatable object file that can be loaded into memory and linked dynamically, at either load time or run-time.
- Executable and Linkable Format (ELF binary)
    - All 3 object files use ELF format.
    - Detailed explanation in slide 13-14~15.
- Static libraries (`.a` archive files, old-fashioned solution)
    - Concatenate related relocatable object files into a single file with an index (called an archive)
        Combining a bunch of relocatable object files (`.o`) into a single static archive (`.a`).
    - Command line order matters!
        In general, putting libraries at the end of command line.
    - Commonly used libraries: 
        - `libc.a` (the C standard library)
        - `limb.a` (the C math library)
    - Disadvantages
        - Duplication in storage, most executables needs `libc`.
        - Duplication in the running executables.
        - Bug fixes require relink each application explicitly, and it's hard to keep track of dependencies.
        
        $\rightarrow$ Solution: shared library (Dynamic linking)
- Shared libraries (`.so` or DLL, Dynamic linking, moder solution)
    - Types of dynamic linking:
        - Load-time linking
            - Handled by the dynamic linker.
            - Shared C library (`libc.so`) is usually dynamically linked. (except that we linked the executable with static libraries explicitly)
        - Run-time linking
            - Call by `dlopen()` interface in Linux.
- Library interpositioning
    - Interpositioning can occur at:
        - Compile time
        - Link time
        - Load/run time: 
            This could help us tracking the behavior of system software even if we don't have the source code of that software. (at least we need to know that the software is written by which of the language)
    - Usage: 
        - Detecting memory leaks.
        - Generating address traces.
        - Other useful software for debugging: **Valgrind**, **gdb**.

## ECF (Exception Control Flows) (Ch 8, Lec 14~15)

> Useful command-line tricks or packages
> - Running executable file with `&` at the end: execute the command but do it in the background. Return back the the shell.
> - `ps`: report a snapshot of the current processes.
> - `pstree`: display a tree of processes
> - `kill`: send a signal to a process
>     ```shell
>     $ /bin/kill –9 24818  # Send SIGKILL to process 24818
>     $ /bin/kill –9 –24817 # Send SIGKILL to every process in process group 24817
>     ```
> - Sending Signals from the Keyboard while working with terminal (shell)
>     - Pressing `ctrl+c` causes the kernel to send a **SIGINT** to every job in the **foreground process group**.
>     - Pressing `ctrl+z` causes the kernel to send a **SIGTSTP** to every job in the **foreground process group**.
    

### ECF

- In what situations we need ECF: 
    1. Data arrives from a disk or a network adapter.
    2. Instruction divides by zero.
    3. User hits Ctrl-C at the keyboard.
    4. System timer expires.
    5. Segmentation fault. 
- ECF exists at all levels of a computer system:
    - Low level mechanisms
        1. Exceptions
            Change in control flow in response to a system event (produced by hardware or OS software).
    - Higher level mechanisms
        1. Process context switch (triggered by OS software or hardware timer)
        2. Signals (triggered by OS software)
        3. Nonlocal jumps: `setjmp()` and `longjmp()` (triggered by calling C runtime library)

### Exceptions

An **exception** is a transfer of control to the OS kernel in response to some event (i.e., change in processor state)
![](https://i.imgur.com/wGd6xcS.png)
- (partial) Taxonomy of ECF:
    ![](https://i.imgur.com/lx8ZXCo.png)
    If the event occurs at the same place every time the program is executed with the same data and memory allocation, the event is **synchronous**.
    - Asynchronous Exceptions (External events of the system)
        - Caused by such as: 
            1. External timer.
            2. Arrival of a packet from a network.
            3. Mouse movement.
        - Handler returns to “next” instruction.
    - Synchronous Exceptions (Part of executing program)
        Caused by events that occur as a result of executing an instruction.
        - Traps:
            - Intentional (system calls, breakpoint traps, special instructions)
            - Returns control to “next” instruction.
        - Faults:
            - Unintentional but possibly recoverable
                - Page faults (recoverable)
                - Protection faults (unrecoverable)
                - Floating point exceptions
            - Either re-executes faulting (“current”) instruction or aborts.
        - Aborts:
            - Unintentional and unrecoverable (illegal instruction, parity error, machine check)
            - Aborts current program.

### Process and Process control

- Definition of Processes: Instances of executing programs.
    An abstract object that hsa current state of executing program.
    For comparison: 
    program $\rightarrow$ Dead, simply a collection of instructions not being executed.
    Process $\rightarrow$ Alive.
- Concurrency vs. Parallelism
    - Concurrency (Context switching rapidly)
        - Each process is a logical control flow. 
        - Two processes run **concurrently** if their flows overlap in time. Otherwise, they are sequential.
        - Context switch: Control flow passes from one process to another by **Kernel**.
    - Parallelism (Multi-cores CPU executing multiple processes)
- System Call Error Handling (slide 14-31~34)
- 3 states of a process (Programmer's perspective): scheduled by kernel
    1. Running: Executing or waiting.
    2. Stopped: Suspended and will not be scheduled until further notice.
    3. Terminated: stopped permanently
        Caused by:
        - Receving some type of signal like `SIGTERM` ([Signal(IPC) - POSIX signal](https://en.wikipedia.org/wiki/Signal_(IPC)#POSIX_signals))
        - Returning from `main()`
        - Calling `exit()` (called once but never returns)
- Creating Processes: `fork()`
    - called oncebut returns twice:
        Returns `0` to the child process, child’s `PID` to parent process.
    - Making complete copy of execution state.
    - Parent and child process execute concurrently. (Can’t predict execution order)
    - Duplicate but separate address space (independent variables).
    - Shared open files (`stdout` is the same).
    - process graph (tree structure)
        ![](https://i.imgur.com/41Eotkp.png)
        These vertices (execution of statements) would occur in an ordering that don't violate the dependencies of that graph. (topological ordering)
- Reaping Child Processes
    - Terminated child process becomes **zombie** (maintaining exit status and various OS tables), since its parent may still need to use the **exit status**.
    - Parent process uses `wait()` or `waitpid()` to reap zombie child processes. 
    - What if parent doesn’t reap?
        - If parent doesn't terminate: Never diminishes (a kind of memory waste)
        - If parent does terminate: Reaped by `init` process (pid == 1)
            So, only need explicit reaping in long-running processes. 
            E.g. shells and servers.
    - What if child process never terminates?
        Killing parent process couldn't reap the unterminated child processes, we got to kill child explicitly.
- Loading and Running Programs: `execve()` (slide 14-58~60)
    - Loads and runs in the current process
    - Overwrites code, data, and stack
    - Called onceand never returns
- Process groups
    - Can be get and set by `getpgrp()` and `setpgid()`
    - Kill all process in a group with `kill -n -<pid>`

### Signal

- Linux process hierarchy: Tree structure. 
    - Daemon: Background processes.
- Shell: an application program that runs programs on behalf of the user.
    Actually, **shell** is a typical case of using **signals**.
    - Shell contains a basic loop and an `eval()` function.
    - Two cases in `eval()`:
        - Shell built-in command
        - Not build-in, use `fork()` and `execve()`
    - Problem: How to reap those background jobs?
        - **Basic loop** and **wait()** could only reap foreground jobs, we need **signal** to help us reap the background jobs.
- Signal
    - Features
        - Akin to exceptions and interrupts.
        - Sent from the kernel (or `kill` system call invoked by another process) to a process
        - Only information in a signal is its ID and the fact that it arrived. The same signals don't queue up.
    - Sending and receiving signals
        - **Pending** and **Blocked** signal bit vector 
            Each process has its own **pending** and **blocked** bit vector
            - Pending: represents the set of pending signals
                - Kernel sets bit $k$ in **pending** $\rightarrow$ signal delivered.
                - Kernel clears bit $k$ in **pending** $\rightarrow$ signal received.
                - Signals couldn't be queued up.
            - Blocked (signal mask): represents the set of blocked signals
                - Set and cleared by using the `sigprocmask()` function.
        - Signal sent (delivered): set **pending** bit
            - Every process belongs to exactly one process group.
                - Get and change process group by `getpgrp()` and `setpgid()` respectively.
                - We can send signals to certain process or process group.
        - Signal received: clear **pending** bit and react with the following possible ways:
            - **Ignore** signal
            - **Terminate** process (with optional core dump)
            - **Stops** until restarted by a **SIGCONT** signal
            - **Catch** (executing a user-level function called **signal handler**)
    - Replacing default action by installing **signal handler**
        - Function `handler_t*signal(int signum, handler_t*handler)`
            1. signum: [Standard signals](https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html)
            2. handler:
                - `SIG_IGN`: ignore signal
                - `SIG_DFL`: revert to the default action
                - Function pointer to a user-level signal handler
        - Signals Handlers as Concurrent Flows
            - A signal handler is executed under the same process, we could think of it as a separate logical flow (not process) that runs concurrently with the main program.
                Executing under the same process $\rightarrow$ be able to access the global variable as the main program.
                ![](https://i.imgur.com/cIYMl1B.png)
                ![](https://i.imgur.com/QuVndKF.png)
        - Blocking and Non-blocking Signals
            - Nested Signal Handlers: Handlers can be interrupted by other handlers.
                ![](https://i.imgur.com/xLzXjHW.png)
            - Implicit blocking mechanism (Non-blocking mode)
                Kernel blocks any pending signals with lower priority compared to currently handled one.
            - Explicit blocking and unblocking mechanism (Blocking mode)
                Controlled by using the following functions:
                `sigprocmask()`
                `sigemptyset()` - Create empty set
                `sigfillset()` – Add every signal number to set
                `sigaddset()` – Add signal number to set
                `sigdelset()` – Delete signal number from set
        - Safe Signal Handling
            - Handlers are tricky because they are concurrent with main program and share the same global data structures. Shared data structures can become corrupted.
            - Guidelines for Writing Safe Handlers
                1. Keep handlers as simple as possible
                2. Call only `async-signal-safe` functions in handlers
                    - `async-signal-safe` functions are reentrent (access only local variables on stack), or cannot be interrupted by other signals.
                    - `_exit()`, `write()`, `wait()`, `waitpid()`, `sleep()`, `kill()` are safe.
                    - `printf()`, `sprintf()`, `malloc()`, `exit()` are **not** safe.
                    - `write()` is the only `async-signal-safe` output function. 
                        <font color="#149940">Take a look at</font> reentrant SIO (Safe I/O library) from `csapp.c` in code example.
                3. Save and restore `errno` on entry and exit
                4. Protect accesses to shared data structures by temporarily blocking all signals. 
                5. Declare global variables as `volatile` and prevent those variables from being taken account with code optimization. See [the usage of `volatile` keyword in C](https://www.keil.com/support/man/docs/armclang_intro/armclang_intro_chr1385110934192.htm) 
                6. Declare global flags as `volatile sig_atomic_t`
                    flag: variable that is only read or written (e.g. flag = 1, not flag++).
                    `volatile sig_atomic_t` are ints on most systems.
            - Correct Signal Handling
                - Must wait for all terminated child processes. Put  waitin a loop to reap all terminated children.
                - Shouldn't make any assumption of the executing order. Adequately **blocking** some signals while executing statements in **critical section**.
            - Explicitly waiting for signals: declaring `pid` as global volatile variable (<font color="#149940">Slide 15-62~66</font>)
                - Spinnig scannig: Wasting CPU resources.
                    ```c
                    while(!pid) {}
                    ```
                    
                - Pausing: Race condition, may encounter **deadlock**.
                    ```c
                    while(!pid) pause();
                    ```
                    `pause()`: forces the process to sleep until some unblocked signal arrives.
                    When the signal **SIGCHLD** arrives after the `while loop` header but before the execution of `pause()`, we would get stuck into **deadlock**.
                - Short period of sleeping: Too slow and wasting CPU resources.
                    ```c
                    while(!pid) sleep(1);
                    ```
                - Solution: `int sigsuspend(constsigset_t*mask)`
                    Equivalent to:
                    ```c
                    sigprocmask(SIG_BLOCK, &mask, &prev);
                    pause();
                    sigprocmask(SIG_SETMASK, &prev, NULL);
                    ```
### Portable Signal Handling (plz consult textbook)



### Nonlocal jumps

See <font color="#149940">Slide 15-71~</font>

Problem: Different versions of unix have different signal handling semantics.
Solution: Use `sigaction()`

## 
