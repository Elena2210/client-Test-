#ifndef MAINWGT_H
#define MAINWGT_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

#include "headers/client_cdgrms.h"
#include "headers/server_cdgrms.h"

namespace Ui {
class MainWgt;
}

class MainWgt : public QWidget
{
    Q_OBJECT

public:
    explicit MainWgt(QWidget *parent = nullptr);
    ~MainWgt();

signals:
    // Переподключение
    void signalReconnect( QString, ushort );

    // Отправка сообщения
    void signalSendData( QByteArray );

public slots:
    // по установке TCP соединения
    void slotConnect();
    void slotDisconnect();

    // Чтение
    void slotRecvData( QByteArray data );

    // Информация
    void Info( QString str);

private slots:
    // Слот обновления формы по приходу данных
    void slotUpdateForm();

    // Переподключение
    void on_btnReconnect_clicked();

    // Отправка данных
    void on_btnSendMsg_clicked();

private:
    Ui::MainWgt *ui;

    // Индикаторы
    QPixmap circle_red			= QPixmap( ":/image/circle_red.png" );
    QPixmap circle_green		= QPixmap( ":/image/circle_green.png" );
    QPixmap circle_gray         = QPixmap( ":/image/circle_grey.png" );
    QPixmap circle_yellow		= QPixmap( ":/image/circle_yellow.png" );
    QPixmap logo        		= QPixmap( ":/image/logo_new.png" );

    pack::SPackParam packP;

    QVector< QLabel * > indicators;
};

#endif // MAINWGT_H
