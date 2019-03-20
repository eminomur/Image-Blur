#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <cstdlib>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    image = nullptr;

    // Initializion of image pointer is done here
    image_initialization();
}

MainWindow::~MainWindow()
{
    if (image != nullptr) {
        delete image;
    }
    delete ui;
}

void MainWindow::on_actionAbout_Me_triggered()
{
    QMessageBox::about(this, "About Me", "Muhammed Emin ÖMÜR\n150308007\nSoftware Engineering");
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_apply_pushButton_clicked()
{
    image->image_adjustment(ui->height_spinBox->value(), ui->width_spinBox->value(),
                            ui->xpos_spinBox->value(), ui->ypos_spinBox->value(),
                            static_cast<float>(ui->red_horizontalSlider->value()) / 100,
                            static_cast<float>(ui->green_horizontalSlider->value()) / 100,
                            static_cast<float>(ui->blue_horizontalSlider->value()) / 100);

    ui->xpos_spinBox->setMaximum(image->get_max_x_pos());
    ui->ypos_spinBox->setMaximum(image->get_max_y_pos());

    ui->image_label->setPixmap(image->get_final_pixmap());
}

void MainWindow::on_default_picture_pushButton_clicked()
{
    image->image_adjustment(image->get_height() - 100, image->get_width() - 100, 0, 0, 1, 1, 1);

    ui->height_spinBox->setValue(image->get_height() - 100);
    ui->width_spinBox->setValue(image->get_width() - 100);

    ui->xpos_spinBox->setMaximum(100);
    ui->xpos_spinBox->setValue(0);
    ui->ypos_spinBox->setMaximum(100);
    ui->ypos_spinBox->setValue(0);

    ui->red_horizontalSlider->setValue(100);
    ui->green_horizontalSlider->setValue(100);
    ui->blue_horizontalSlider->setValue(100);

    ui->image_label->setPixmap(image->get_final_pixmap());
}

void MainWindow::on_actionAbout_Program_triggered()
{
    QMessageBox::information(this, "About Program", "1) I have created this program, because I wondered how profile pictures created used in WhatsApp. "
                                                    "So don't expect something perfectly written or works program.\n"
                                                    "2) Please after selecting a proper image, select a ROI. I didn't write expection handling code so it "
                                                    "will definitely crash if you don't.\n"
                                                    "3) Even though I solved many programs in program, I believe there are still many of them.\n"
                                                    "4) If you see any issues in code please inform me. I would be appreciated.\n"
                                                    "5) Finally, don't change size of inner image and x and y positions at the same time. "
                                                    "Even though there may be no problem, it might still crash.\n");
}

void MainWindow::on_change_picture_pushButton_clicked()
{
    delete image;
    image = nullptr;

    image_initialization();
}

void MainWindow::image_initialization()
{
    // Initial image selection stage made here
    QString filter = "Image Files (*bmp *.dib *.jpeg *.jpg *.jpe *.jp2 *.png *.pbm *.pgm *.ppm *.sr *.ras *.tiff)";
    QString image_file_name = QFileDialog::getOpenFileName(this, "Select an image file", QDir::homePath(), filter);
    if (image_file_name.isEmpty()) {
        QMessageBox::critical(this, "Error", "You must select an image first");
        std::exit(EXIT_FAILURE);
    }

    image = new Image(image_file_name.toStdString());

    // Important adjustments related to selected image is done here
    ui->image_label->setPixmap(image->get_final_pixmap());

    ui->image_info_label->setText((std::string("Height: ") + std::to_string(image->get_height()) +
                                   std::string(" Width: ") + std::to_string(image->get_width())).c_str());

    // Width and height spinbox initialization is done here
    ui->height_spinBox->setMaximum(image->get_height());
    ui->height_spinBox->setValue(image->get_height() - 100);
    ui->width_spinBox->setMaximum(image->get_width());
    ui->width_spinBox->setValue(image->get_width() - 100);

    // Maximum X and Y position initialization is done here
    // Since minimum is always zero it is not necessary to explicitly specify here
    ui->xpos_spinBox->setMaximum(100);
    ui->ypos_spinBox->setMaximum(100);
}
