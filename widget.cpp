#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QCursor>
#include <QPixmap>
#include <ctime>
#include <QMessageBox>
#include <qfile.h>



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);

    temps=0;
    gridLayout = new QGridLayout();
    joc=false;
    minesRestants=10;
    missatge=false ;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(actualizart()));
    connect(ui->reiniciar, SIGNAL(clicked()), this, SLOT(reniciar()));
    std::srand(std::time(nullptr));




    ui->mines->display("010");
    ui->temps->display("000");

    for(int i = 0 ; i < 9 ; i++) {
        for(int j = 0 ; j < 9 ; j++) {
            celes[i][j] = new cell(this);
             celes[i][j]->setValor(0);
            gridLayout->addWidget(celes[i][j], i, j);
            connect(celes[i][j], SIGNAL(banderaCanviada(bool)), this, SLOT(actualitzarComptadorMines(bool)));
            connect(celes[i][j], &cell::mouseButton, this, [this, i, j](QString boto) {
                if (joc==false && missatge==false) {
                    inijoc();
                }
                if (boto == "Left") {
                    mostraCela(i, j);
                } else if (boto == "Right") {
                    celes[i][j]->bandera();
                }
            });

        }
    }

    gridLayout->setSpacing(0);
    ui->verticalLayout->addLayout(gridLayout);
    ui->horizontalLayout->setAlignment(Qt::AlignCenter);
    QCursor cambiarCursor(QPixmap(":/n/christmas normal select.cur"));
    this->setCursor(cambiarCursor);
    timer->stop();




}

Widget::~Widget() {
    delete ui;
    delete timer;
    for(int i = 0 ; i < 9 ; i++) {
        for(int j = 0 ; j < 9 ; j++) {
            delete celes[i][j];
        }
    }
}
void Widget::inijoc() {
    if (joc==true) {

        return;
    }

    joc = true;
    timer->start(1000);
    posarbombes();
    comptBombes();

}





void Widget::actualizart() {
    temps++;
    ui->temps->display(QString("%1").arg(temps, 3, 10, QChar('0')));
    if (temps == 1000) {
        fijoc();
    }

}

void Widget::reniciar() {

    temps = 0;
    minesRestants = 10;
    missatge = false;
    joc = false;


    ui->temps->display(QString("%1").arg(temps, 3, 10, QChar('0')));
    ui->mines->display("010");
    timer->stop();


    ui->reiniciar->setIcon(QIcon(":/Icons/smile.png"));
    ui->reiniciar->setStyleSheet("");
    ui->mines->setStyleSheet("");
    ui->temps->setStyleSheet("");


    QFile file(":/style/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            celes[i][j]->setEnabled(true);
            celes[i][j]->reset();


        }
    }


}

void Widget::actualitzarComptadorMines(bool increment) {
    if (increment) {
        minesRestants--;
    } else {
        minesRestants++;
    }
    ui->mines->display(QString("%1").arg(minesRestants, 3, 10, QChar('0')));
}

void Widget::posarbombes(){
    int bombes = 3;


    while (bombes > 0) {
        int x = std::rand() % 9;
        int y = std::rand() % 9;

        if (celes[x][y]->getValor() != -1) {
            celes[x][y]->setValor(-1);

            bombes--;

        }
    }


}
void Widget::comptBombes(){
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (celes[i][j]->getValor() != -1) {
                int bombesAdjacents = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int adjX = i + x;
                        int adjY = j + y;
                        if (adjX >= 0 && adjX < 9 && adjY >= 0 && adjY < 9) {
                            if (celes[adjX][adjY]->getValor() == -1) {
                                bombesAdjacents++;
                            }
                        }
                    }
                }
                celes[i][j]->setValor(bombesAdjacents);
            }
        }
    }
}
void Widget::mostraCela(int x, int y) {


    if (!joc || x < 0 || x >= 9 || y < 0 || y >= 9) {

        return;
    }

    if (celes[x][y]->esDescoberta() || celes[x][y]->esMarcada()) {

        return;
    }

    celes[x][y]->setDescoberta(true);
    celes[x][y]->contigut();

    if (celes[x][y]->getValor() == -1) {

        guanyat = false;
        fijoc();
        return;
    }

    if (celes[x][y]->getValor() == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                mostraCela(x + dx, y + dy);
            }
        }
    }

    if (esGuanyat()) {
        guanyat = true;
        fijoc();
    }
}




bool Widget::esGuanyat() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (celes[i][j]->getValor() != -1 && !celes[i][j]->esDescoberta()) {
                return false;
            }
        }
    }
    return true;
}
void Widget::fijoc() {
    timer->stop();
    joc = false;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            celes[i][j]->setEnabled(false);
            if(guanyat==true){
                if (celes[i][j]->getValor() == -1 && !celes[i][j]->esMarcada()) {
                    QIcon bombaIcon(":/Icons/bomb.png");
                    celes[i][j]->setIcon(bombaIcon);
                    celes[i][j]->setIconSize(QSize(37, 36));
                    celes[i][j]->setStyleSheet("background-color: white;");
                }
            }else{
                if (celes[i][j]->esMarcada()) {
                    if (celes[i][j]->getValor() != -1) {
                        celes[i][j]->setStyleSheet("background-color: white;" "border: 1px solid #000033;");
                    }
                    celes[i][j]->setDescoberta(false);
                } else {
                    celes[i][j]->setDescoberta(true);
                    celes[i][j]->contigut();
                }

                if (celes[i][j]->getValor() == -1) {
                    QIcon bombaIcon(":/Icons/bomb.png");
                    celes[i][j]->setIcon(bombaIcon);
                    celes[i][j]->setIconSize(QSize(37, 36));
                    celes[i][j]->setStyleSheet("background-color: #DCDCDC  ; border: 1px solid #FF0000;");


                } else if (celes[i][j]->getValor() > 0) {
                    celes[i][j]->setStyleSheet(
                        "background-color: white;"
                        "color: black;"
                        "border: 1px solid #000033;"
                        " box-shadow: 2px 2px 5px #000033;"
                        );
                } else {
                    celes[i][j]->setStyleSheet(
                        "background-color: #1D8CEC;"
                        "color: black;"
                        "border: 1px solid #000000;"
                        "box-shadow: 2px 2px 5px #000033;"
                        );
                }

            }
        }
    }


    if (guanyat == true && missatge == false) {
        missatge = true;

        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("Fi del joc");
        msgBox->setText("Enhorabona! Bon Nadal i Feliç Any nou 🎄 !  ");
        msgBox->setIconPixmap(QPixmap(":/Icons/clinking-glasses.png"));
        ui->reiniciar->setIcon(QIcon(":/Icons/party-face.png"));

        QPushButton* closeButton = msgBox->addButton(("Tancar"), QMessageBox::AcceptRole);
        closeButton->setIcon(QIcon(":/f/futbol/santandreu.png"));

        msgBox->exec();

    }

    if (guanyat == false) {
        this->setStyleSheet("background-color: #99CCFF; font-size: 18px; font-weight: bold; border: 1px solid #A0C0D0; color: #000000 ; ");
        ui->reiniciar->setIcon(QIcon(":/Icons/loose.png"));
        ui->reiniciar->setStyleSheet("background-color: white; border: 2px solid #4169E1; color: black;");
        ui->mines->setStyleSheet("background-color: white; border: 2px solid #4169E1; color: black;");
        ui->temps->setStyleSheet("background-color: white; border: 2px solid #4169E1; color: black;");

    }

    if (temps == 1000) {
        this->setStyleSheet("background-color: #F5DEB3;");
        ui->reiniciar->setIcon(QIcon(":/Icons/timeout.png"));
        ui->reiniciar->setStyleSheet("background-color: white; border: 2px solid #FFC400 ; color: black;");
        ui->mines->setStyleSheet("background-color: white; border: 2px solid #FFC400 ; color: black;");
        ui->temps->setStyleSheet("background-color: white; border: 2px solid #FFC400 ; color: black;");
    }
}




