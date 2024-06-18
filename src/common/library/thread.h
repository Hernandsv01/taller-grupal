#ifndef THREAD_H_
#define THREAD_H_

// Para usar el gettid()
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <optional>
#include <thread>

#include "event.h"

#define gettid() syscall(SYS_gettid)

class Runnable {
   public:
    virtual void start() = 0;
    virtual void join() = 0;
    virtual void stop() = 0;
    virtual bool is_alive() const = 0;
    virtual bool keep_running() const = 0;

    virtual ~Runnable() {}
};

class Thread : public Runnable {
   private:
    std::thread thread;
    std::optional<Event*> thread_ended_event;

   protected:
    // Subclasses that inherit from Thread will have access to these
    // flags, mostly to control how Thread::run() will behave

    // Keep running es true desde el principio, y se pone en false cuando se
    // llama a stop(), o cuando termina de ejecutarse el thread.
    std::atomic<bool> _keep_running;

    // Is alive es false desde el principio, y se pone en true cuando el thread
    // nuevo inicia a ejecutarse y se pone en false cuando termina de ejecutarse
    // el thread.
    std::atomic<bool> _is_alive;

    // Indica si el thread ya fue joineado.
    std::atomic<bool> _has_been_joined;

    // Funcion que implementan quienes heredan de Thread. Es lo que se ejecuta
    // para parar forzozamente al hilo en cuestion. Esta funcion no es llamada
    // desde fuera, unicamente desde Thread
    virtual void stop_custom() {}

    // Donde se almacena la descripcion del thread
    const std::string _text_description;

    // Convertí los siguientes 3 metodos en protected, para que no se pueda, por
    // ejemplo, intentar iniciar un implementador de thread con "run()"

    // Thread se debe crear con un string que describa el thread
    Thread(const std::string& text_description)
        : _keep_running(true),
          _is_alive(false),
          thread_ended_event(),
          _text_description(text_description) {}

    void main() {
        try {
#ifndef NDEBUG
            std::cout << "Inicia hilo de " << text_description()
                      << " con PID: " << gettid() << std::endl;
#endif

            this->run();
        } catch (const std::exception& err) {
            std::cerr << "Unexpected exception in " << text_description()
                      << ": " << err.what() << "\n";
        } catch (...) {
            std::cerr << "Unexpected exception in " << text_description()
                      << ": <unknown>\n";
        }
        _keep_running = false;
        _is_alive = false;

        // Si alguien agregó un evento de terminado, notificarlo.
        if (thread_ended_event.has_value())
            thread_ended_event.value()->notify_all();
    }

    virtual void run() = 0;

   public:
    void start() override final {
        _is_alive = true;
        _keep_running = true;
        thread = std::thread(&Thread::main, this);
    }

    void join() override final {
        thread.join();
        _has_been_joined = true;

#ifndef NDEBUG
        std::cout << "Termina hilo de " << text_description()
                  << " con PID: " << gettid() << std::endl;
#endif
    }

    // Agrega la opcion de que alguien agrege al thread un evento que notifique
    // cuando el thread terminó.
    void add_thread_ended_event(Event* event) {
        thread_ended_event = std::optional<Event*>(event);

        if (has_ended()) event->notify_all();
    }

    // Note: it is up to the subclass to make something meaningful to
    // really stop the thread. The Thread::run() may be blocked and/or
    // it may not read _keep_running.

    // Implementar la parte custom en stop_custom()
    void stop() override final {
        // Si ya terminó, no hago nada
        if (has_ended()) return;

        // Pongo para que deje de ejecutarse
        _keep_running = false;

        // Hago yield para darle la oportunidad de que se detenga.
        std::this_thread::yield();

        // Luego del yield checkeo si el thread ya terminó, si no terminó,
        // ejecuto la funcion stop_custom, que debería considerar que el hilo
        // pueda estar bloqueado en un recurso, y pararlo forzosamente
        if (_is_alive) stop_custom();
    }

    // El thread ya se lanzó, pero todavía no terminó.
    bool is_alive() const override { return _is_alive; }

    // El thread todavía no se le pidio terminar, ni terminó.
    bool keep_running() const override { return _keep_running; }

    // El thread ya no se está ejecutando.
    bool has_ended() const { return !_keep_running && !_is_alive; }

    // El thread ya fue joineado.
    bool has_been_joined() const { return _has_been_joined; }

    // Devuelve la descripcion del thread
    const std::string text_description() const { return _text_description; }

    virtual ~Thread() {
        // Si el thread ya no esta vivo y fue joineado. no hago nada.
        if (has_been_joined()) return;

            // Si esta en modo debug, aviso que un thread se stopea y joinea por
            // estar fuera de scope. Convierte el objeto en RAII.
            // Tal vez no debería ser necesario el aviso, pero lo agrego
            // temporalmente para poder ver si hay algun thread que no estemos
            // joineando correctamente.
#ifndef NDEBUG
        std::cout << "ALERTA: Thread " << text_description()
                  << " fue destruido mientras todavía se esta ejecutando.\n"
                  << "Se procede a forzar la terminación del thread."
                  << std::endl;
#endif

        this->stop();
        this->join();
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) = delete;
    Thread& operator=(Thread&& other) = delete;
};

#endif
