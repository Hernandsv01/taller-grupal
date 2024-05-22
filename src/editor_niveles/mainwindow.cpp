#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "renderizadomapa.h"

#include <qstandarditemmodel.h>
#include <QDirIterator>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      editor(this)
{

    ui->setupUi(this);

    QStandardItemModel *tiles = new QStandardItemModel();

    //QDirIterator it(":", {"*.png"} , QDir::Files);

    QDirIterator it(":/Tiles", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString file_path = it.next();

        qDebug() << it.fileName() << file_path;

        QString nombre_archivo = it.fileName();


        QStandardItem* tile_item = new QStandardItem(nombre_archivo);
        tile_item->setIcon(QIcon(file_path));

        QImage image(file_path);

        tile_item->setData(file_path);
        tile_item->setData(image ,Qt::UserRole + 2);

        Tile tile_enum;

        if (nombre_archivo == "dirt.png") {
            tile_enum = Tile::dirt;
        } else if (nombre_archivo == "stone.png") {
            tile_enum = Tile::stone;
        } else if (nombre_archivo == "water.png") {
            tile_enum = Tile::water;
        } else {
            tile_enum = Tile::air;
        }

        tile_item->setData(tile_enum, Qt::UserRole+3);

        tiles->appendRow(tile_item);

    }

    ui->listView->setModel(tiles);

    editor.add_tiles(tiles);
    editor.add_tile_selection(ui->listView);

    ui->editorContainer->layout()->addWidget(&editor);
}



MainWindow::~MainWindow()
{
    delete ui;
}



