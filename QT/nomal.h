#ifndef NOMAL_H
#define NOMAL_H

#include <QDialog>

namespace Ui {
class Nomal;
}

class Nomal : public QDialog
{
    Q_OBJECT

public:
    explicit Nomal(QWidget *parent = nullptr);
    ~Nomal();

private:
    Ui::Nomal *ui;
};

#endif // NOMAL_H
