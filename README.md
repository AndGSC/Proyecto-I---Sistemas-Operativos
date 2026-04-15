# Proyecto-I---Sistemas-Operativos
Para correr en Kali, desde la carpeta "terminal_carga".

Compilar:

<code>gcc -o terminal src/main.c src/camion.c src/scheduler.c src/log.c src/stats.c src/cola.c src/sync.c -I./include -lpthread</code>

Ejecutar:

<code>./terminal 1</code>
o también: <code>./terminal 2</code>