#ifndef WIDGET_H
#define WIDGET_H

#include "cell.h"
#include <QWidget>
#include <QTimer>
#include <QGridLayout>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots :
    void actualizart();
    void reniciar();
    void posarbombes();
    void comptBombes();
    void mostraCela(int x , int y);
    void fijoc();
    void actualitzarComptadorMines(bool increment);
    bool esGuanyat();
    void inijoc();



private:
    Ui::Widget *ui;
    QTimer *timer;
    unsigned int temps ;
    QGridLayout *gridLayout;
    cell *celes[9][9];
    bool joc ;
    int minesRestants;
    bool guanyat ;
    bool missatge;


};



#endif // WIDGET_H
