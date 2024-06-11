#include "mainwindow.h"

#include <QDebug>
#include <iostream>

#include "ui_mainwindow.h"

MainWindow::MainWindow(Lobby& lobby) : lobby(lobby), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Vacio todos estos elementos. Estan con texto inicialmente
    // para poder verlo en el editor de UI
    ui->textoErrorConectarseServidor->setText("");
    ui->textoErrorCrearPartida->setText("");
    ui->textoErrorElegirPartida->setText("");
    ui->stackedWidget->setCurrentIndex(0);
    ui->listaPartidas->clear();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::updateMatchesFromMatchList() {
    std::vector<GameMatch> matches;

    try {
        matches = lobby.getServerMatches();
    } catch (const ClosedConnectionError& e) {
        closedConnectionError();
        return;
    } catch (const std::exception& e) {
        ui->textoErrorElegirPartida->setText(e.what());
        return;
    }

    ui->listaPartidas->clear();

    for (const GameMatch& match : matches) {
        std::string name = match.name;
        uint16_t id = match.id;

        // Sacado de:
        // https://stackoverflow.com/questions/25452125/is-it-possible-to-add-a-hidden-value-to-every-item-of-qlistwidget
        auto* item = new QListWidgetItem(QString::fromStdString(name));
        QVariant variant_id;
        variant_id.setValue(id);

        item->setData(Qt::UserRole, variant_id);

        // get back the data
        // QVariant v = item->data(Qt::UserRole);
        // int id = v.value<int>();

        ui->listaPartidas->addItem(item);
    }
}

std::string get_text_or_placeholder(QLineEdit* input) {
    std::string content = input->text().toStdString();

    if (content == "") {
        content = input->placeholderText().toStdString();
    }

    return content;
}

void MainWindow::on_botonConectar_clicked() {
    std::string ip_servidor = get_text_or_placeholder(ui->IpInput);
    std::string puerto_servidor = get_text_or_placeholder(ui->PortInput);

    ui->textoErrorConectarseServidor->setText("");
    ui->textoErrorConectarseServidor->repaint();

    try {
        lobby.connectToServer(ip_servidor, puerto_servidor);
    } catch (const ClosedConnectionError& e) {
        closedConnectionError();
        return;
    } catch (const std::exception& e) {
        ui->textoErrorConectarseServidor->setText(e.what());
        return;
    }

    goToMatchSelection();
}

void MainWindow::goToMatchSelection() {
    ui->stackedWidget->setCurrentIndex(1);
    updateMatchesFromMatchList();
}

void MainWindow::on_botonUnirseAPartida_clicked() {
    ui->textoErrorElegirPartida->setText("");
    auto items_seleccionados = ui->listaPartidas->selectedItems();

    if (items_seleccionados.length() > 1) {
        ui->textoErrorElegirPartida->setText("Debe elegir una unica partida");
        return;
    }
    if (items_seleccionados.length() < 1) {
        ui->textoErrorElegirPartida->setText("Debe elegir una partida");
        return;
    }

    auto item_selecionado = items_seleccionados[0];

    QVariant v = item_selecionado->data(Qt::UserRole);
    uint16_t id = v.value<uint8_t>();

    try {
        lobby.connectToMatch(id);
    } catch (const ClosedConnectionError& e) {
        closedConnectionError();
        return;
    } catch (const std::exception& e) {
        ui->textoErrorElegirPartida->setText(e.what());
    }

    // TODO: Que tendría que hacer cuando se conecta a la partida exitosamente?
    //  -Deberia devolver el socket.
    //  -Deberia iniciar la interfaz sdl.
}

void MainWindow::on_botonIrACrearPartida_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_botonCrearPartida_clicked() {
    std::string mapa_seleccionado =
        ui->selectorMapa->currentText().toStdString();
    uint8_t cantidad_jugadores = ui->selectorCantidadJugadores->value();
    std::string nombre_partida =
        get_text_or_placeholder(ui->selectorNombrePartida);

    try {
        uint16_t id_partida = lobby.createMatch(
            mapa_seleccionado, cantidad_jugadores, nombre_partida);
        lobby.connectToMatch(id_partida);
    } catch (const ClosedConnectionError& e) {
        closedConnectionError();
        return;
    } catch (const std::exception& e) {
        ui->textoErrorCrearPartida->setText(e.what());
    }

    // TODO: Que tendría que hacer cuando se conecta a la partida exitosamente?
    //  -Deberia devolver el socket.
    //  -Deberia iniciar la interfaz sdl.
}

void MainWindow::on_botonCancelarCrearPartida_clicked() {
    goToMatchSelection();
}

void MainWindow::closedConnectionError() {
    ui->textoErrorConectarseServidor->setText(
        "La conexion con el servidor se cerró de manera inesperada");
    ui->stackedWidget->setCurrentIndex(0);
}
