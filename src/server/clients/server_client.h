#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <utility>

#include "ActionType.h"
#include "Update.h"
#include "protocol.h"
#include "queue.h"
#include "socket.h"
#include "thread.h"

class Client_receiver : public Thread {
   private:
    Protocol protocol;
    Queue<ActionType> inputQueue;
    bool is_running;

   public:
    explicit Client_receiver(Socket& socket)
        : Thread("Client_reciever server"),
          protocol(socket),
          inputQueue(),
          is_running(true) {}
    ActionType get_next_action();
    void run() override;

    ~Client_receiver() override { is_running = false; }
};

class Client_sender : public Thread {
   private:
    Protocol protocol;
    Queue<std::vector<Update>> outputQueue;
    bool is_running;

   public:
    explicit Client_sender(Socket& socket)
        : Thread("Client_sender server"),
          protocol(socket),
          outputQueue(),
          is_running(true) {}
    void run() override;
    void addToQueue(std::vector<Update> const& result);
    void kill() {
        std::vector<Update> result;
        bool has_stuff;
        do {
            has_stuff = outputQueue.try_pop(result);
        } while (has_stuff);
        outputQueue.close();
    }

    ~Client_sender() override { is_running = false; }
};

class Server_Client {
   private:
    Socket skt;
    Client_receiver receiver;
    Client_sender sender;
    int player_position;

   public:
    explicit Server_Client(Socket&& socket)
        : skt(std::move(socket)),
          receiver(skt),
          sender(skt),
          player_position(0) {
        receiver.start();
        sender.start();
    }
    Client_sender& getSender() { return sender; }
    Client_receiver& getReceiver() { return receiver; }
    int get_player_position() { return player_position; }
    ~Server_Client() {
        skt.close_and_shutdown();

        receiver.stop();
        receiver.join();

        sender.kill();
        sender.stop();
        sender.join();
    }
};

#endif  // SERVER_CLIENT_H
