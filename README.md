# TP2_SO
Trabajo practico 2

Pre-entrega

Para esta preentrega dejamos las features que se pedían funcionando, esto incluye:
 - 2 implementaciones de memory manager:
    - Buddy de: https://github.com/evanw/buddy-malloc
    - FreeList (Heap4) de: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/master/portable/MemMang/heap_4.c

Se puede intercambiar entre cada una agregando el flag -D BUDDY en el make file del kernel o sacandolo paea compilar con Heap4 (rdb\Kernel\Makefile)

Para tester la meoria se puede usar:
 - run testmm & (para correr el test que nos pasaste vos en bg)
 - test mem (es una serie de 5 tests commentados y explicados en rdb\Userland\SampleCodeModule\commands.c)

Se puede utilizar el comando mem para mostrar el estado de la memoria.

 - el scheduler, context switching, con priority based round robin
Para testear el funcionamiento de los procesos se puede usar:
 - run (para correr uno de los 4 procesos disponibles) (a, b, loop, testmm) seguido o no de & para correr en bg/fg
    - a printea en pantalla una vez cada masomenos dos segundos, hasta 3
    - b printea en pantalla cada vez que llega a contar hasta un numero divisible por 100 (util para probar prioridades)
    - loop printea por siempre igual que a 
    - testmm corre tu test de la memoria
 - kill (para matar a un proceso dado su pid)
 - ps (para mostrar los procesos corriendo)
 - nice (para cambiar la prioridad de un proceso dado si pid)
 - block (para pasar de bloqueado a ready y viceversa dado su pid)

Para testear la prioridad recomendamos correr 2 veces al proceso b, cambiandole a la segunda vez la prioridad. se podra observar como el proceso con mayor prioridad avanza mas rapido.

Los comandos con los que cuenta nuestra shell se muestran con sus descripciones ejecutando el comando "help".
Se encuentran tambien los comandos creados en Arquitectura pero no se garantiza su funcionamiento.

