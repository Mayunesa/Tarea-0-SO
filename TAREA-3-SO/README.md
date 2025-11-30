## Pasos seguidos

### 1. Crear branch "magdalena_t3" y carpeta "TAREA-3-SO"
### 2. Preparaciones en la terminal: 
    - sudo apt update
    - sudo apt install -y git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
    - sudo apt --fix-broken install --> error
    - sudo rm -rf /usr/share/doc/libvirt-daemon-system-systemd
    - sudo apt clean
    - sudo apt --fix-broken install --> Se queda atascado en 94% por 4 minutos
        - ps aux | grep -i dpkg
        - sudo kill 4483
        - sudo dpkg --configure -a --> Se atasca por 3 minutos, ctrl + c
            - sudo apt update --fix-missing
            - sudo apt install -f --> Se atasca en el 33%
            - sudo dpkg --remove --force-remove-reinstreq libvirt-daemon-system libvirt-daemon-system-systemd
    - Reiniciar :D y luego: sudo apt install -y git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
        - sudo dpkg --configure -a --> se atasca
            - sudo killall dpkg
            - sudo rm /var/lib/dpkg/lock-frontend
            - sudo rm /var/lib/dpkg/lock
            - sudo apt --fix-broken install --> se atasca, y no cierra con ctrl + c
                - Cierro y escribo "wsl --shutdown" en Powershell
                - sudo rm /var/lib/dpkg/lock-frontend
                - sudo rm /var/lib/dpkg/lock
                - sudo rm /var/cache/apt/archives/lock
                - sudo dpkg --remove --force-remove-reinstreq libvirt-daemon-system
                - sudo apt update
    - sudo apt install -y git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu --> Success!!!
        - sudo apt autoremove


### 3. Clonar XV6, terminal:
    - cd ~
    - git clone https://github.com/mit-pdos/xv6-riscv.git --> no issues
    - cd xv6-riscv
    - make qemu --> "xv6 kernel is booting"
    - Salgo con ctrl + A, luego X.

### 4. Subir la rama al fork
    - git remote set-url origin https://github.com/Mayunesa/xv6-riscv.git

### 5. Agregar las syscalls
    - Entro a: kernel/syscall.h
        - Agrego "#define SYS_mrdprotect  22" y "#define SYS_munrdprotect 23"
    - Entro a: kernel/syscall.c.
        - Escribo bajo las declaraciones extern: extern uint64 sys_mrdprotect(void); y extern uint64 sys_munrdprotect(void);
        - Escribo en "static uint64 (*syscalls[])(void) = {}": [SYS_mrdprotect]   sys_mrdprotect, [SYS_munrdprotect] sys_munrdprotect,
    - Entro a: kernel/sysproc.c
        - Al final del archivo, agrego:
            uint64
            sys_mrdprotect(void)
            {
            uint64 addr;
            int len;

            argaddr(0, &addr);
            argint(1, &len);

            return mrdprotect((void*)addr, len);
            }

            uint64
            sys_munrdprotect(void)
            {
            uint64 addr;
            int len;

            argaddr(0, &addr);
            argint(1, &len);

            return munrdprotect((void*)addr, len);
            }
### 6. Incorporar funciones de protección
    - Entro a: kernel/vm.c
        - Al final del archivo, escribo las funciones: mrdprotect(void *addr, int len) y munrdprotect(void *addr, int len)
        - Agrego las declaraciones de "mrdprotect" y "munrdprotect" en "kernel/defs.h", en la parte de vm.c
        - También las agrego en "user/user.h", en // system calls
        - Las agrego en "user/usys.pl", como "entry();"

### 7. Programa de prueba
    - Crear archivo "rdprotect_test.c" en user
    - Entrar a Makefile y escribir "$U/_rdprotect_test\" arriba de "$U/_zombie\"
    - Compilar con "make qemu"
    - Escribir: rdprotect_test --> "exec rdprotect_test failed"
        - make clean
        - make qemu y rdprotect_test --> mismo error
        - Agrego sfence_vma(); a los int creados en vm.c
        - make clean y make qemu
        - Escribir "ls" --> ahora sí aparece rdprotect_test
    - rdprotect_test en XV6 kernel
    - Resultado: usertrap(): unexpected scause 0xf pid=4
                sepc=0x30 stval=0x4000
        - Revisar fallos.
        - Primero, comento "addr[0] = 'A';". make clean y make qemu --> usertrap(): unexpected scause 0xd pid=3
            sepc=0x2c stval=0x4000
