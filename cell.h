#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QString>

class cell : public QPushButton {
    Q_OBJECT

public:
    void reset();
    void contigut();
    void bandera();

    explicit cell(QWidget *parent = nullptr);
    ~cell();

    int getValor() const { return this->valor; }
    bool esDescoberta() const { return this->descoberta; }
    bool esMarcada() const { return this->marcada; }

    void setValor(int val) { this->valor = val; }
    void setDescoberta(bool valDescoberta) { this->descoberta = valDescoberta; }

signals:
    void mouseButton(QString button);
    void banderaCanviada(bool posada);

protected Q_SLOTS:
    void mousePressEvent(QMouseEvent *e) ;

private:
    unsigned int valor;
    bool descoberta;
    bool marcada;



};

#endif // CELL_H
