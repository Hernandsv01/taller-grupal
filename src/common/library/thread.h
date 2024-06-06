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

    // Funcion que implementan quienes heredan de Thread. Es lo que se ejecuta
    // para parar forzozamente al hilo en cuestion. Esta funcion no es llamada
    // desde fuera, unicamente desde Thread
    virtual void stop_custom() {}

    // Funcion que implementan quienes heredan de Thread. Se usa para, si se
    // genera una excepcion, tener una descripcion de texto de cual tipo de
    // Thread devolvió el error. (vease main)
    virtual std::string text_description() = 0;

    // Convertí los siguientes 3 metodos en protected, para que no se pueda, por
    // ejemplo, intentar iniciar un implementador de thread con "run()"
    Thread() : _keep_running(true), _is_alive(false), thread_ended_event() {}

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

    void join() override final { thread.join(); }

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

    virtual ~Thread() {}

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) = delete;
    Thread& operator=(Thread&& other) = delete;
};

#endif
