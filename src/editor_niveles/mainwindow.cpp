#include "mainwindow.h"

#include <qstandarditemmodel.h>

#include <QDebug>
#include <QDialog>
#include <QDirIterator>
#include <QMetaType>
#include <QVariant>
#include <filesystem>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      map(Map(15, 10)),
      editor(this) {
    // Asumo que no se pasa ninguna ruta para el mapa.

    // if (map_path == nullptr) {
    //     map = new Map(255, 255);
    // } else {
    //     map = &Map::fromYaml(map_path);
    // }

    ui->setupUi(this);

    ui->textErrorBeginToEdit->setText("");
    ui->stackedWidget->setCurrentIndex(0);

    QMap<IdTexture, QImage> tile_textures;
    QMap<IdTexture, QImage> background_textures;

    {
        QDirIterator it(":/textures/backgrounds", QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString file_path = it.next();

            qDebug() << it.fileName() << file_path;

            QString fileName = it.fileName();

            if (!fileName.endsWith(".png") && !fileName.endsWith(".jpg"))
                throw std::runtime_error(
                    "Solo se permiten archivos .png o .jpg");

            QString textureName = fileName.left(fileName.length() - 4);

            QImage image(file_path);

            background_textures.insert(textureName.toStdString(), image);
            avaible_background_texture_ids.push_back(textureName.toStdString());
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

        // Le asigno como data el struct Block, para despues utilizarla en
        // el mapa.
        tile_item->setData(tile_block_variant, Qt::UserRole + 3);
        tiles.appendRow(tile_item);

        tile_textures.insert(textureName.toStdString(), image);
    }

    // Agrego los items a la interfaz
    ui->listView->setModel(&tiles);

    // Agrego los tiles y vista de la lista al editor, para poder obtener
    // cual tile se esta seleccionando.

    editor.addTileTextures(tile_textures);
    editor.addBackgroundTextures(background_textures);

    if (ui->editorContainer->layout() == nullptr) {
        ui->editorContainer->setLayout(new QHBoxLayout());
    }

    ui->editorContainer->layout()->addWidget(&editor);

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
                              ->data(Qt::UserRole + 3)
                              .value<::Block>();

    editor.changeSelectedTile(selected_tile);
}
