#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include "mconfigdata.h"

//Definicion de struct para guardar la configuracion actual
// struct mConfig{
//     float psMax;
//     float psMin;
//     float pdMax;
//     float pdMin;
//     int32_t tSample;
// };

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT
signals:
    // void sendPreferencesToMain(mConfig);
    void sendPreferencesToMain(MConfigData);
public:
    // explicit Preferences(mConfig data, QWidget *parent = nullptr);
    explicit Preferences(MConfigData data, QWidget *parent = nullptr);
    ~Preferences();
    void readSatus(bool status);

private slots:
    void on_pushButton_cerrar_clicked();

    void on_pushButton_aplicar_clicked();

private:
    Ui::Preferences *ui;
    bool m_status;
};

#endif // PREFERENCES_H
