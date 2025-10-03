
# **Informe tarea 1**

## **A. Funcionamiento de las llamadas al sistema:**

- Las llamadas al sistema (syscalls) son funciones predefinidas que permiten al usuario solicitar servicios al kernel. De esta forma, las aplicaciones externas no acceden directamente al hardware o al sistema operativo, sino que lo hacen a través de una interfaz controlada, segura y limitada. En el caso de xv6, algunas syscalls son “SYS_fork”, “SYS_exit”, “SYS_wait”, entre otros. 
- En términos de codificación de xv6, para activar un llamado al sistema el usuario debe invocar a cualquiera de las funciones declaradas en user.h. Esto provoca que usys.S genere un ecall que llega a una trampa del kernel (trap.c) donde se reconoce que la interrupción es un syscall. Desde este punto, se ejecuta la función syscall() (definida en syscall.c), la cual obtiene el número de la función solicitada (definido en syscall.h). Este se usa para mapear el puntero a la función del kernel requerida (definidas en sysproc.c), para luego invocarla. Finalmente, se almacena su valor de retorno para devolverlo al usuario y el kernel vuelve a su estado normal de ejecución.
- En este trabajo se implementó 2 syscalls nuevas: SYS_getppid y SYS_getancestor. La primera devuelve el ID del proceso padre que lo invoca. Mientras que, SYS_getancestor retorna tanto el PID del proceso mismo, como la de su padre y su abuelo. En caso de que no existan tantos ancestros, este devuelve -1. 


## **B. Explicación de las modificaciones realizadas (con los pasos seguidos):**:

### 1. Crear nueva rama
- git checkout -b magdalena_t1

### 2. Comprobar funcionamiento, limpiar, y ver archivos antes de empezar
- En la terminal, escribir "make clean" y luego "make"
- Usar "ls -la" para revisar los archivos
- Buscar archivos en "Search" donde se encuentre "getpid" para saber dónde empezar a modificar. Estos son los encontrados:
    - kernel/syscall.c
    - kernel/syscall.h
    - kernel/sysproc.c
    - user/grind.c
    - user/user.h
    - user/usertests.c
    - user/usys.pl

### 3. Modificar los archivos necesarios:
- "kernel/syscall.h": 
    >- #define SYS_getppid 22
    >- #define SYS_getancestor 23
    - **¿Para qué?:** Para definir los números identificadores únicos para las nuevas llamadas al sistema (getppid y getancestor).
- "kernel/sysproc.c": 
    1. Al final del archivo, pusimos:\
        >uint64\
        >sys_getppid(void)\
        >{\
        >return myproc()->parent->pid;\
        >}
    2. Y luego:\
        >uint64\
        >sys_getancestor(void)\
        >{\
        >int level;\
        >if(argint(0, &level) < 0)\
            >return -1;
            
        >if(level < 0)\
            >return -1;
            
        >struct proc *p = myproc();
        
        >for(int i = 0; i < level; i++) {\
            >if(p->parent == 0)\
            >return -1;\
            >p = p->parent;\
        >}
        
        >return p->pid;\
        >}
    - **¿Para qué?:** Para implementar la lógica principal de las llamadas al sistema.
- "kernel/syscall.c": 
    - En el array extern, agregamos: 
        >1. extern uint64 sys_getppid(void);
        >2. extern uint64 sys_getancestor(void);
    - En el array "static uint64 (*syscalls[])(void)", agregamos: 
        >1. [SYS_getppid]   sys_getppid,
        >2. [SYS_getancestor]   sys_getancestor,
    - **¿Para qué?:** Para registrar las nuevas funciones en el sistema de llamadas del kernel.
- "user/user.h":
    - Dentro de las "system calls", agregar:
        >1. int getppid(void);
        >2. int getancestor(int level);
    - **¿Para qué?:** Para declarar las funciones, de modo que estén disponibles en programas de usuario.
- "user/usys.pl": 
    - Al final del archivo, abajo de las demás "entry", agregar:
        >1. entry("getppid");
        >2. entry("getancestor");
    - **¿Para qué?:** Para generar automáticamente el código que permite la transición de user a kernel.

### 4. Creación de "yosoytupadre.c"
- Crear el archivo "yosoytupadre.c" dentro de "user".
- Buscar "UPROGS" en "Makefile" y agregar "$U/_yosoytupadre\"
- Poner "make clean" y luego "make" en la terminal para actualizar lo creado, limpiar y compilar
- Terminal: "make qemu"

### 5. Probar funcionamiento
- $ yosoytupadre
- Primer resultado: "exec yosoytupadre failed" :(
- Segundo intento sí funcionó, sin cambiar nada

### 6. Hacer commit
- git add .
- git commit -m "Tarea 1 completa :)"
- git push origin magdalena_t1

## **C. Dificultades encontradas y cómo se resolvieron:**
1. En el paso 3 de la creación de "yosoytupadre.c" apareció este error:
    >"kernel/sysproc.c: In function ‘sys_getancestor’:\
    >kernel/sysproc.c:119:6: error: void value not ignored as it ought to be\
    >119 |   if(argint(0, &level) < 0)\
    >    |      ^~~~~~~~~~~~~~~~~\
    >make: *** [<builtin>: kernel/sysproc.o] Error 1"
    - Lo solucionamos cambiando la línea 119 por "argint(0, &level);"
2. En el paso 1 de "Probar funcionamiento", obtuvimos un error: todos los ancestros salían con resultado "-1".\
Para solucionarlo, agregamos esto:
    >if(level == 0)\
    >   return p->pid;

## **Pasos generales para hacerlo funcionar:**
1. make qemu (para iniciar xv6)
    - Si se desea, antes poner "make clean" y después "make", para comprobar si hay errores o si funciona bien
2. yosoytupadre 
3. Resultado que nos sale:
>Mi PID es: 3\
>El PID de mi padre es: 2\
>Ancestro nivel 0: 3\
>Ancestro nivel 1: 2\
>Ancestro nivel 2: 1
