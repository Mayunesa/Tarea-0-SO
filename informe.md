
## Pasos Seguidos

### 0. Antes de clonar
-Nosotras utilizamos VSCode para clonar.
-Instalar la extensión WSL (tenemos Windows)
- ctrl+shift+p ---> WSL: Connect to WSL

### 1. Clonar el repo desde WSL
- git clone https://github.com/Mayunesa/Tarea-0-SO.git
- cd Tarea-0-SO
- code .

### 2. Crear una nueva rama
- git checkout -b magdalena

### 3. Instalar de Dependencias
- sudo apt update
- sudo apt install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu

### 4. Instalar código fuente de XV6
- git remote add upstream https://github.com/mit-pdos/xv6-riscv.git
- git fetch upstream
- git branch -r: 
    origin/HEAD -> origin/main
    origin/main
    upstream/riscv
- git pull upstream riscv 
  # Problema encontrado 1: "fatal: refusing to merge unrelated histories"
    - git pull upstream riscv --allow-unrelated-histories
    # Problema encontrado 2: "fatal: empty ident name (for <lajefa@LAPTOP-MPHS697F.>) not allowed"
        - git config --global user.email "maghernandez@alumnos.uai.cl"
        - git config --global user.name "Magdalena Hernández"
- git fetch upstream riscv
- git checkout upstream/riscv -- .
- git add .
- git commit -m "Agregar código fuente de xv6"

### 5. Compilar XV6
- Comando usado: "make"
- Las últiams 2 líneas dicen:
riscv64-linux-gnu-objdump -S kernel/kernel > kernel/kernel.asm
riscv64-linux-gnu-objdump -t kernel/kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$/d' > kernel/kernel.sym

### 6. Ejecutar XV6
- Comando usado: "make qemu"
  # Problema encontrado 3: "ERROR: Need qemu version >= 7.2"
    - sudo apt remove qemu-system-misc
    - sudo apt install software-properties-common
    - sudo add-apt-repository ppa:canonical-server/server-backports
    - sudo apt update
    - sudo apt install qemu-system-misc
  - ¡Listo! Ahora, en resumen, dice: "QEMU emulator version 8.0.4"
- Nuevamente "make qemu": funciona.

### 7. Verificar la instalación
- $ ls (da una lista larga)
- $ echo "Hola xv6"
"Hola xv6"
- cat README (funciona)

## Captura de Pantalla
 - Nombres de los archivos: 
    - captura_1.jpeg
    - captura_2.jpeg