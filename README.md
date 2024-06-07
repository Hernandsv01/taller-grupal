# taller-grupal

## Como instalar

### Instalar Librerias
```bash
# Herramientas de compilacion
sudo apt install git cmake g++ make

# Librerias de SDL
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-gfx-dev

# Librerias de QT
sudo apt install qtbase5-dev qt5-qmake
```

### Clonar repositorio
```bash
git clone https://github.com/Hernandsv01/taller-grupal.git
cd taller-grupal/
```

### Compilar
```bash
cmake .
make
```

## Ejecutables
La compilacion del tp dejará 4 ejecutables:
- `client`: Que es el programa grafico donde se puede jugar la partida.
- `server`: Que es el programa que ejecuta el servidor del juego
- `editor_niveles`: Un prototipo extremadamente temprano de como se vería/usaría el editor de mapas.
- `lobby_client`: Un prototipo de como sería la interfaz grafica para conectarse a un servidor/partida.

## Como ejecutar el TP.
- ### Partida
Para ejecutar una partida, primero debe iniciar el servidor, que automaticamente empezará a escuchar en el puerto 15500.
Luego debe ejecutar el cliente, que se conectara tambien de manera automatica a la unica partida disponible.
El servidor actualmente tiene soporte para un unico jugador. No fue diseñado ni testeado para ejecutar más de un cliente.

Para cerrar el cliente, puede cerrar la ventana del cliente, y se cerrará por completo.
El servidor, debe ser cerrado enviando `q` por la terminal del mismo.

- ### Editor de niveles
El editor de niveles puede abrirse y cerrarse normalmente.

- ### Cliente GUI Lobby
El cliente lobby puede abrirse normalmente, pero para su funcionamiento tiene que poder conectarse con un servidor de lobby (todavía no programado).
Para poder testearlo, se recomienda abrir una segunda terminal con netcat, para poder "hacer de servidor".
Se puede usar el comando:
```bash
nc -l 15500
```
Siendo 15500 el puerto que el cliente lobby usa por defecto.
Por la terminal de netcat recibirá instrucciones de que debe enviar para actuar como servidor, y probar las variaciones del programa.
En particular, se le va a pedir que ingrese 4 letras para indicar el nombre de las "partidas", y luego, indicar si quiere que una solicitud de una accion sea exitosa (enviando cualquier caracter), o falle (enviando una `N`).
