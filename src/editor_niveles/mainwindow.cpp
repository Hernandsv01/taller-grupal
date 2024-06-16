#include "mainwindow.h"

#include <qstandarditemmodel.h>

#include <QDebug>
#include <QDialog>
#include <QDirIterator>
#include <QMetaType>
#include <QVariant>
#include <stdexcept>

#include "../common/map/map.h"
#include "./ui_mainwindow.h"
#include "renderizadomapa.h"

// cppcheck-suppress unknownMacro
Q_DECLARE_METATYPE(Block)

Block getBlockFromTextureName(QString textureName) {
    Collision collision;

    if (textureName == "dirt") {
        collision = Collision::Cube;
    } else if (textureName == "stone") {
        collision = Collision::Cube;
    } else if (textureName == "water") {
        collision = Collision::Cube;
    } else if (textureName == "spawn_enemy") {
        collision = Collision::EnemySpawn;
    } else if (textureName == "spawn_player") {
        collision = Collision::PlayerSpawn;
    } else if (textureName == "spawn_item") {
        collision = Collision::ItemSpawn;
    } else {
        throw std::runtime_error("Textura no reconocida");
    }

    return Block{collision, textureName.toStdString()};
}

Map openOrCreateMap(const char *map_path) {
    if (map_path == nullptr) {
        return Map(15, 10);
    } else {
        return Map::fromYaml(map_path);
    }
}

MainWindow::MainWindow(const char *map_path, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      map(openOrCreateMap(map_path)),
      editor(map, this) {
    // Asumo que no se pasa ninguna ruta para el mapa.

    // if (map_path == nullptr) {
    //     map = new Map(255, 255);
    // } else {
    //     map = &Map::fromYaml(map_path);
    // }

    ui->setupUi(this);

    // Creo una modelo de lista de tiles, para almacenar los distintos tipos y
    // sus texturas
    QStandardItemModel *tiles = new QStandardItemModel();

    {
        QDirIterator it(":/textures/backgrounds", QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString file_path = it.next();

            qDebug() << it.fileName() << file_path;

            QString fileName = it.fileName();

            QImage image(file_path);

            backgrounds.append(image);
        }
    }

    // itero sobre la carpeta de tiles, con el :/ para que busque en el qrc
    QDirIterator it(":/textures/tiles", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString file_path = it.next();

        qDebug() << it.fileName() << file_path;

        QString fileName = it.fileName();

        if (!fileName.endsWith(".png")) {
            throw std::runtime_error("Solo se permiten archivos .png");
        }

        QString textureName = fileName.left(fileName.length() - 4);

        // Creo el item y le asigno como icono la textura
        QStandardItem *tile_item = new QStandardItem(textureName);
        tile_item->setIcon(QIcon(file_path));

        QImage image(file_path);

        // Le seteo como la data la imagen.
        tile_item->setData(image, Qt::UserRole + 2);

        Block tile_block = getBlockFromTextureName(textureName);
        QVariant tile_block_variant = QVariant::fromValue(tile_block);

        // Le asigno como data el struct Block, para despues utilizarla en el
        // mapa.
        tile_item->setData(tile_block_variant, Qt::UserRole + 3);
        tiles->appendRow(tile_item);
    }

    on_changeBackgroundButton_clicked();

    // Agrego los items a la interfaz
    ui->listView->setModel(tiles);

    // Agrego los tiles y vista de la lista al editor, para poder obtener cual
    // tile se esta seleccionando.

    editor.addTileModel(tiles);
    editor.add_tile_selection(ui->listView);

    if (ui->editorContainer->layout() == nullptr) {
        ui->editorContainer->setLayout(new QHBoxLayout());
    }

    ui->editorContainer->layout()->addWidget(&editor);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_changeBackgroundButton_clicked() {
    QImage aBackground = backgrounds.last();
    backgrounds.pop_back();
    backgrounds.push_front(aBackground);

    editor.setBackground(aBackground);

    editor.update();
}

void MainWindow::on_saveMapButton_clicked() { editor.saveMap(); }

void MainWindow::on_loadMapEditorButton_clicked() {
    qDebug() << "Empezar a editar";

    std::string map_name = ui->mapNameInput->text().toStdString();

    map = Map::fromYaml(map_name);

    editor.update();

    ui->stackedWidget->setCurrentIndex(1);
}