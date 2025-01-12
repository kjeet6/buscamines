#include "cell.h"
#include <cstdlib>
#include <ctime>
#include <QIcon>


cell::cell(QWidget *parent) : QPushButton(parent) {

    valor=0;
    descoberta=false;
    marcada = false ;
    this->setFixedSize(QSize(37, 32));
    std::srand(std::time(nullptr));
}

cell::~cell() {}

void cell::reset() {
    descoberta = false;
    marcada = false;
    valor = 0;
    this->setIcon(QIcon());
    this->setText("");
    this->setStyleSheet("");


}

void cell::bandera() {
    if(descoberta==true){
        return;
    }

    marcada = !marcada;
    if (marcada==true) {
        int index = std::rand() % 21;
        QString icones = QString(":/Icons/%0.png").arg(index);
        QIcon icona(icones);
        if (!icona.isNull()) {
            this->setIcon(icona);
            this->setIconSize(QSize(37, 36));
        } else {
            this->setIcon(QIcon());
        }
    } else {
        this->setIcon(QIcon());
    }

    emit banderaCanviada(marcada);
}

void cell::contigut() {
    if (valor == -1) {
        this->setIcon(QIcon(":/Icons/bomba.png"));
        this->setIconSize(QSize(37, 36));
        this->setStyleSheet(
            "background-color: #000000;"
            "border: 2px solid #B71C1C;"
            "box-shadow: 2px 2px 5px #990000;"
            );
    } else if (valor > 0) {
        this->setText(QString::number(valor));
        this->setStyleSheet(
            "background-color: #FF9999;"
            "color: #000000;"
            "font-size: 16px; font-weight: bold;"
            "border: 1px solid #000000;"
            );
    } else {
        this->setText("");
        this->setStyleSheet(
            "background-color: #FFE6E6;"
            "border: 1px solid #000000;"
            );
    }
}


void cell::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        emit mouseButton("Right");
    } else if (e->button() == Qt::LeftButton && !marcada) {
        emit mouseButton("Left");
    }

}

