#ifndef SOCKET_H
#define SOCKET_H
#include <optional>

/*
 * TDA Socket.
 * Por simplificación este TDA se enfocará solamente
 * en sockets IPv4 para TCP.
 * */
class Socket {
   public:
    int skt;
    int number_shutdown = 0;

   private:
    bool closed;
    bool shutdown_manual = false;

    /*
     * Construye el socket pasándole directamente el file descriptor.
     * */
    explicit Socket(int skt);

    /*
     * Checkea que el file descriptor (skt) sea "valido".
     *
     * No hace un checkeo muy exhaustivo, simplemente verifica que
     * no sea -1.
     *
     * En un object RAII como este, el atributo skt esta siempre
     * bien definido (distinto de -1) **salvo** cuando es movido.
     *
     * En teoría una vez movido el objecto, este no puede ser usado
     * ya q queda invalido.
     *
     * En el caso de Socket, al moverse se setea skt a -1.
     *
     * En lenguajes como Rust usar un objeto "ya movido" falla
     * en tiempo de compilación.
     *
     * En C++, bueno, es C++ y el comportamiento es indefinido :D.
     *
     * Este check es para ayudarte a detectar ese caso y lanzar una
     * excepción. No es lo más bonito del universo pero te dará una
     * pista de que puede estar andando mal.
     * */
    void chk_skt_or_fail() const;

   public:
    /*
     * Constructores para `Socket` tanto para conectarse a un servidor
     * (`Socket::Socket(const char*, const char*)`) como para ser usado
     * por un servidor (`Socket::Socket(const char*)`).
     *
     * Muchas librerías de muchos lenguajes ofrecen una única formal de
     * inicializar los sockets y luego métodos (post-inicialización) para
     * establecer la conexión o ponerlo en escucha.
     *
     * Otras librerías/lenguajes van por tener una inicialización para
     * el socket activo y otra para el pasivo.
     *
     * Este TDA va por ese lado.
     *
     * Para `Socket::Socket(const char*, const char*)`,  <hostname>/<servname>
     * es la dirección de la máquina remota a la cual se quiere conectar.
     *
     * Para `Socket::Socket(const char*)`, buscara una dirección local válida
     * para escuchar y aceptar conexiones automáticamente en el <servname> dado.
     *
     * En caso de error los constructores lanzaran una excepción.
     * */
    Socket(const char* hostname, const char* servname);

    explicit Socket(const char* servname);

    /*
     * Deshabilitamos el constructor por copia y operador asignación por copia
     * ya que no queremos que se puedan copiar objetos `Socket`.
     *
     * Se podrían copiar?, no. Si bien un `Socket` el un `int` y un `bool` y
     * seria trivial copiarlo, conceptualmente ese `int` no es meramente un
     * número sino un file descriptor, un identificador que tiene asociado
     * varios recursos del lado del sistema operativo.
     *
     * Copiar un `int` no implica que esos recursos son copiados (es como
     * copiar un puntero y pretender que el objeto apuntado se copie
     * mágicamente).
     *
     * Más aun no tiene sentido pensar en copiar un socket. Un socket
     * representa una conexión algo q no se puede copiar.
     *
     * Lo mismo pasa con otros recursos. Un archivo referenciado por `FILE*`.
     * Copiar ese puntero no implica que realmente tendrás una copia del archivo
     * en tu disco rígido.
     *
     * Por eso deshabilitamos la copia. No tiene sentido.
     * */
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    /*
     * Hacemos que el `Socket` sea movible.
     * */
    Socket(Socket&&);
    Socket& operator=(Socket&&);

    /* `Socket::sendsome` lee hasta `sz` bytes del buffer y los envía. La
     * función puede enviar menos bytes sin embargo.
     *
     * `Socket::recvsome` por el otro lado recibe hasta `sz` bytes y los escribe
     * en el buffer (que debe estar pre-allocado). La función puede recibir
     * menos bytes sin embargo.
     *
     * Si el socket detecto que la conexión fue cerrada, se lanza la excepcion
     * ClosedConnectionError
     *
     * Retorna cuantos bytes se enviaron/recibieron.
     *
     * Si hay un error se lanza una excepción.
     *
     * Lease manpage de `send` y `recv`
     * */
    int sendsome(const void* data, unsigned int sz);
    int recvsome(void* data, unsigned int sz);

    /*
     * `Socket::sendall` envía exactamente `sz` bytes leídos del buffer, ni más,
     * ni menos. `Socket::recvall` recibe exactamente sz bytes.
     *
     * Si hay un error se lanza una excepción.
     *
     * Si se cerro la conexion (ya sea que se enviaron algunos o ningun byte) se
     * lanza la excepcion ClosedConnectionError
     *
     * En caso de éxito se retorna la misma cantidad de bytes pedidos
     * para envio/recibo, lease `sz`.
     *
     * */
    int sendall(const void* data, unsigned int sz);
    int recvall(void* data, unsigned int sz);

    /*
     * Acepta una conexión entrante y retorna un nuevo socket
     * construido a partir de ella.
     *
     * En caso de error, se lanza una excepción.
     * */
    std::optional<Socket> accept();

    // /*
    //  * Cierra la conexión ya sea parcial o completamente.
    //  * Lease manpage de `shutdown`
    //  * */
    // void shutdown(int how);

    // /*
    //  * Cierra el socket. El cierre no implica un `shutdown`
    //  * que debe ser llamado explícitamente.
    //  * */
    // int close();

    // Desconecta y cierra el socket. Por lo menos en el contexto de mi
    // programa, no tiene sentido hacer una sin la otra
    void close_and_shutdown();

    /*
     * Destruye el socket. Si aun esta conectado,
     * se llamara a `Socket::shutdown` y `Socket::close`
     * automáticamente.
     * */
    ~Socket();
};
#endif
