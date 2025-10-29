Magdalena Hernández y Vania Medic.

# Tarea 2

## Funcionamiento y lógica de la implementación.

EL lottery scheduling es un método de planificación de uso de CPU, que utliza un sistema aleatorio para ir elegiendo un proceso. Para esto, se les asignan tickets previamente a los procesos, luego el kernel genera un número aleaotrio y selecciona el proceso en espera que cuente con ese ticket. De esta forma los procesos van siendo elegidos de manera aleaotria y se benefician los que tienen más tickets.

En este caso, este sistema utilizaba un método de Round Robin el cual tuvimos que eliminar para implementar el lottery scheduling. En primer lugar la función de scheduler tuvo que ser reemplazada por una que examina todos los procesos con estado RUNNABLE y suma todos sus tickets. Luego se genera un número aleatorio entre 1 y ese total. Posteriormente se recorre la lista de procesos sumando los tickets hasta que el ticket sea igual a este número. Cuando esto sucede se ha llegado al proceso elegido por lo que se le da acceso a la CPU, donde puede ceder, ser interrumpido o finalizar. Luego de esto, el scheduler vuelve a ejecutar el loop seleccionando otro proceso mediante un nuevo número aleatorio y continúa de esta forma hasta que todos los procesos han terminado sus ejecución. Cabe destacar que si un proceso no cuenta con tickets asignados, la función por defecto le asigna 100 tickets.

Por otro lado, se creo una función llamada settickets que permite que un usuario pueda asignar la cantidad de tickets de un proceso. Además, si el valor asignado es menor a uno settickets le asigna un ticket por defecto. 

## Explicación de las modificaciones realizadas (archivos y cambios clave).
1. kernel/proc.h : Dentro de struct proc definir las variables tickets y cpu_slices

2. kernel/proc.c : 
-  En allocproc(void) agregar la inicialización  de las variables justo después de que se asigna el PID. Valor por defecto de los tickets p->tickets = 100; y el contador de uso de CPU p->cpu_slices = 0;

- Modificar la función scheduler(void) para que pase de un sistema Round-Robin a uno de lottery scheduling

-  En la función kexit( int status ) agregar un printf que muestra los tickets asignados a un proceso y su uso de CPU justo antes de que el proceso termine de ejecutarse.

3. kernel/sysproc.c : Crear la syscall settickets(int n)

4. kernel/syscall.h : Agregar la declaración del número de syscall de settickets que corresponde a 24.

5. kernel/syscall.c : Declarar la función de setticktes extern uint64 sys_settickets(void); y agregarla en la tabla de syscalls [SYS_settickets]  sys_settickets,

6. user/user.h : Agregar el systemcall int settickets(int n);

7. user/usys.pl : Agregar entry("settickets");

8. user/demo.c : Agregar el programa de prueba del funcionamiento que crea 10 procesos y les asigna tickets distintos a cada uno. Para esto se llama a la función creada anteriormente settickets.

9. Makefile: Agregar demo para poder ejecutarlo.


## Dificultades encontradas y soluciones implementadas.
- Para la contabilidad y el monitoreo de tickets y uso de CPU por proceso, se trató de implementar un printf dentro de la función scheduler en proc.c que imprimia todos los tickets y cuantas veces fue elegido. Esto generó un loop infito desde que se ejecutaba make qemu, por lo que no daba espacio para realizar pruebas y ejecutar el demo. Es por esto que se eliminó ese printf y se implemntó uno dentro de kexit justo antes de finalizar un proceso. Esto permitió terminar con el loop infinito y poder volver a utilizar la consola. Además el output que entrega es mucho más ordenado.

## Posibles problemas de este tipo de Scheduler (Lottery Scheduling)
- No garantiza equidad a corto plazo: Puede que un proceso que tenga muchos tickets asignados no salga elegido porque aleatoriamente se elegió otro con menos tickets repetidamente. Esto genera variabilidad en la respuesta y puede ser perjudicial para tareas importantes.

- No existe priorización de procesos: En esta versión no existe reajuste de tickets para procesos que han esperedo bastante tiempo en comparación a los otros, por lo que un proceso puede demorar mucho tiempo en ejecutarse. Además si un proceso tiene muy pocos tickets puede que nunca salga elegido hasta que todos los otros procesos se hayan ejecutado.

- Tiempo de procesamiento: Este tipo de planificación requiere generar un número aleatorio y recorrer todos los procesos en cada selección, lo que puede aumentar el tiempo de procesamiento del planificador.