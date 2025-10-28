#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

//Definicion de struct para guardar la configuracion actual
struct mConfig{
    float psMax;
    float psMin;
    float pdMax;
    float pdMin;
    int32_t tSample;
};

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT
signals:
    void sendPreferencesToMain(mConfig);

public:
    explicit Preferences(mConfig data, QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void on_pushButton_cerrar_clicked();

    void on_pushButton_aplicar_clicked();

private:
    Ui::Preferences *ui;
};

#endif // PREFERENCES_H
