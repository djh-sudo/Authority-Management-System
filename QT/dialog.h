#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QCloseEvent>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
