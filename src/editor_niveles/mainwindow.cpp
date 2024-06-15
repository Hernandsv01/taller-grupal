#include "mainwindow.h"

#include <qstandarditemmodel.h>

#include <QDebug>
#include <QDirIterator>

#include "./ui_mainwindow.h"
#include "renderizadomapa.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), editor(this) {
    ui->setupUi(this);

    // Creo una modelo de lista de tiles, para almacenar los distintos tipos y
    // sus texturas
    QStandardItemModel *tiles = new QStandardItemModel();

    // itero sobre la carpeta de tiles, con el :/ para que busque en el qrc
    QDirIterator it(":/Tiles", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString file_path = it.next();

        if (it.fileName() == "background_beach_world.png") {
            continue;
        }

        qDebug() << it.fileName() << file_path;

        QString fileName = it.fileName();

        // Creo el item y le asigno como icono la textura
        QStandardItem *tile_item = new QStandardItem(fileName);
        tile_item->setIcon(QIcon(file_path));

        QImage image(file_path);

        // Le seteo como la data la imagen.
        tile_item->setData(image, Qt::UserRole + 2);

        Tile tile_enum;

        if (fileName == "dirt.png") {
            tile_enum = Tile::dirt;
        } else if (fileName == "stone.png") {
            tile_enum = Tile::stone;
        } else if (fileName == "water.png") {
            tile_enum = Tile::water;
        } else if (fileName == "spawn_enemy.png") {
            tile_enum = Tile::spawn_enemy;
        } else if (fileName == "spawn_player.png") {
            tile_enum = Tile::spawn_player;
        } else if (fileName == "spawn_item.png") {
            tile_enum = Tile::spawn_item;
        } else {
            tile_enum = Tile::air;
        }

        // Le asigno como data el tipo de tile, para despues utilizarla en la
        // grilla del mapa.
        tile_item->setData(tile_enum, Qt::UserRole + 3);
        tiles->appendRow(tile_item);
    }

    // Agrego los items a la interfaz
    ui->listView->setModel(tiles);

    // Agrego los tiles y vista de la lista al editor, para poder obtener cual
    // tile se esta seleccionando.

    editor.addTileModel(tiles);
    editor.add_tile_selection(ui->listView);

    QImage *background = new QImage(":/Tiles/background_beach_world.png");
    editor.setBackground(background);

    ui->editorContainer->layout()->addWidget(&editor);
}

MainWindow::~MainWindow() { delete ui; }
