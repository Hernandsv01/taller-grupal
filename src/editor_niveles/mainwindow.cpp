#include "mainwindow.h"

#include <qstandarditemmodel.h>

#include <QDebug>
#include <QDialog>
#include <QDirIterator>
#include <QMetaType>
#include <QVariant>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "../common/map/map.h"
#include "./ui_mainwindow.h"
#include "renderizadomapa.h"

#ifndef TEXTURE_PATH
#define TEXTURE_PATH ""
#endif

#define ITEM_BLOCK_TYPE_DATA (Qt::UserRole + 3)

// cppcheck-suppress unknownMacro
Q_DECLARE_METATYPE(Block)

// Define que tipo de colision tiene cada textura
Block getBlockFromTextureName(QString textureName) {
    Collision collision;

    std::map<QString, Collision> textureToCollision = {
        {"dirt", Collision::Cube},
        {"stone", Collision::Cube},
        {"water", Collision::Cube},
        {"spawn_enemy", Collision::EnemySpawn},
        {"spawn_player", Collision::PlayerSpawn},
        {"spawn_item", Collision::ItemSpawn},
        {"carrotusgrass", Collision::Cube},
        {"carrotushang", Collision::Cube},
        {"carrotusstone", Collision::Cube},
        {"castlefloor", Collision::Cube},
        {"castlelefthang", Collision::Cube},
        {"castlerighthang", Collision::Cube},
        {"castlewall", Collision::Cube},
        {"diamondusgems", Collision::Cube},
        {"diamondusgrass", Collision::Cube},
        {"diamondushang", Collision::Cube},
        {"diamondusstone", Collision::Cube},
    };

    collision = textureToCollision.at(textureName);

    return Block{collision, textureName.toStdString()};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      map(Map(0, 0)),
      editor(this) {
    ui->setupUi(this);

    ui->textErrorBeginToEdit->setText("");
    ui->stackedWidget->setCurrentIndex(0);

    QMap<IdTexture, QImage> tile_textures;
    QMap<IdTexture, QImage> background_textures;

    loadTexturesFromAndDo(
        TEXTURE_PATH "backgrounds/", background_textures,
        [&](const QString &textureName, const QImage &image) {
            avaible_background_texture_ids.push_back(textureName.toStdString());
        });

    // itero sobre la carpeta de tiles
    loadTexturesFromAndDo(TEXTURE_PATH "tiles/", tile_textures,
                          [&](const QString &textureName, const QImage &image) {
                              addTileToItemModel(textureName, image);
                          });

    // Agrego los items a la interfaz
    ui->listView->setModel(&tiles);

    // Le envio al editor las texturas que existen, junto a sus ids.
    editor.addTileTextures(tile_textures);
    editor.addBackgroundTextures(background_textures);

    // Agrego el editor a la interfaz.
    if (ui->editorContainer->layout() == nullptr) {
        ui->editorContainer->setLayout(new QHBoxLayout());
    }
    ui->editorContainer->layout()->addWidget(&editor);

    // Conecto el evento de cambio de seleccion de tile con mainwindow, para
    // poder avisarle al editor
    auto tile_selection_model = ui->listView->selectionModel();
    QObject::connect(tile_selection_model, &QItemSelectionModel::currentChanged,
                     this, &MainWindow::changed_selected_tile);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_changeBackgroundButton_clicked() {
    // Recorre todos los background disponibles, y los muestra de a uno.
    IdTexture aBackgroundTextureId = avaible_background_texture_ids.front();
    avaible_background_texture_ids.pop_front();
    avaible_background_texture_ids.push_back(aBackgroundTextureId);

    editor.changeBackground(aBackgroundTextureId);

    editor.update();
}

void MainWindow::on_saveMapButton_clicked() {
    map.contractToMin();
    editor.saveMap();
    map.expandToMax();
}

void MainWindow::on_loadMapEditorButton_clicked() {
    ui->textErrorBeginToEdit->setText("");

    std::string map_name = ui->mapNameInput->text().toStdString();

    if (map_name.empty()) {
        ui->textErrorBeginToEdit->setText(
            "No se puede crear un mapa sin nombre");
        return;
    }

    try {
        map = Map::fromYaml(map_name);
    } catch (const std::filesystem::filesystem_error &e) {
        map = Map(1, 1);
        map.set_name(map_name);
    }

    map.expandToMax();

    editor.setMap(&map);

    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_goBackButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::changed_selected_tile(const QModelIndex &new_seleccion,
                                       const QModelIndex &old_selection) {
    Block selected_tile = tiles.itemFromIndex(new_seleccion)
                              ->data(ITEM_BLOCK_TYPE_DATA)
                              .value<::Block>();

    editor.changeSelectedTile(selected_tile);
}

void MainWindow::loadTexturesFromAndDo(
    std::string path, QMap<IdTexture, QImage> &textures,
    std::function<void(const QString &, const QImage &)> action) {
    QDirIterator it(QString::fromStdString(path), QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString file_path = it.next();

        if (it.fileName() == "." || it.fileName() == "..") {
            continue;
        }

        qDebug() << it.fileName() << file_path;

        QString fileName = it.fileName();

        if (!fileName.endsWith(".png") && !fileName.endsWith(".jpg"))
            throw std::runtime_error("Solo se permiten archivos .png o .jpg");

        QString textureName = fileName.left(fileName.length() - 4);

        QImage image(file_path);

        textures.insert(textureName.toStdString(), image);

        action(textureName, image);
    }
}

void MainWindow::addTileToItemModel(const QString &texture_name,
                                    const QImage &texture) {
    // Creo el item y le asigno como icono la textura
    QStandardItem *tile_item =
        new QStandardItem(QPixmap::fromImage(texture), texture_name);

    // Le asigno como data el struct Block, para despues utilizarla en
    // el mapa.
    Block tile_block = getBlockFromTextureName(texture_name);
    QVariant tile_block_variant = QVariant::fromValue(tile_block);
    tile_item->setData(tile_block_variant, ITEM_BLOCK_TYPE_DATA);

    tiles.appendRow(tile_item);
}