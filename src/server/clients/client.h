#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <utility>

#include "socket.h"
#include "thread.h"
#include "queue.h"
#include "Update.h"
#include "protocol.h"
#include "ActionMessage.h"
#include "ActionType.h"

class Client_receiver: public Thread {
private:
    Protocol protocol;
    Queue<ActionMessage> inputQueue;
    bool is_running;

public:
    explicit Client_receiver(Socket& socket): protocol(socket), inputQueue(), is_running(true) {}
    ActionType get_next_action();
    void run() override;

    ~Client_receiver() override { is_running = false; }
};

class Client_sender: public Thread {
private:
    Protocol protocol;
    Queue<std::vector<Update>> outputQueue;
    bool is_running;

public:
    explicit Client_sender(Socket& socket): protocol(socket), outputQueue(), is_running(true) {}
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

class Client {
private:
    Socket skt;
    Client_receiver receiver;
    Client_sender sender;
    int player_position;

public:
    explicit Client(Socket&& socket): skt(std::move(socket)), receiver(skt), sender(skt), player_position(-1) {
        receiver.start();
        sender.start();
    }
    Client_sender& getSender() { return sender; }
    Client_receiver& getReceiver() { return receiver; }
    int get_player_position() { return player_position; }
    ~Client() {
        skt.shutdown(2);
        skt.close();

        receiver.stop();
        receiver.join();

        sender.kill();
        sender.stop();
        sender.join();
    }
};

#endif  // SERVER_CLIENT_H
