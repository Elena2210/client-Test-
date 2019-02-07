#include "mainwgt.h"
#include "ui_mainwgt.h"

#include "network/tcpclient.h"

//===================================================================
MainWgt::MainWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWgt)
{
    ui->setupUi(this);

    // Очистка массива данных
    memset( &packP, 0, sizeof( packP ) );


    ui->host_comboBox->setCurrentText("localhost");

    ui->port_spinBox->setRange(0, 100000);
    ui->port_spinBox->setValue(33059);

    ui->btnReconnect->setText( "Вкл./Выкл." );
    ui->btnReconnect->setPalette( Qt::gray );
    ui->btnReconnect->setAutoFillBackground( true );
    ui->btnReconnect->setToolTip( "Соединение не установлено" );

    ui->Info_textEdit->setText("Соединение не установлено.");

    ui->frame->setVisible(false);

    indicators.append(ui->ind1);
    indicators.append(ui->ind2);
    indicators.append(ui->ind3);
    indicators.append(ui->ind4);

    for(int i = 0; i < 4; i++)
    {
        indicators[i]->setPixmap(circle_gray);
        indicators[i]->setAutoFillBackground(true);
    }

//============================ СВЯЗЬ =================================

    QString host = ui->host_comboBox->currentText(); //"localhost";
    int port = ui->port_spinBox->value();

    ui->Info_textEdit->append(QString("Host: %1, Port: %2").arg(host).arg(QString::number(port)));
    TcpClient * cli = new TcpClient( host, port, this );

    // Подключение
    connect( cli, &TcpClient::signalConnected,
             this, &MainWgt::slotConnect );

    // Отключение
    connect( cli, &TcpClient::signalDisconnected,
             this, &MainWgt::slotDisconnect );

    // Отключение клиента
    connect( this, &MainWgt::signalDisconnect,
              cli, &TcpClient::slotDisconnect );

    // Переподключение
    connect( this, &MainWgt::signalReconnect,
             cli, &TcpClient::slotReconnect );

    // Чтение
    connect( cli, &TcpClient::signalRecvData,
             this, &MainWgt::slotRecvData );

    // Отправка данных
    connect( this, &MainWgt::signalSendData,
             cli, &TcpClient::slotSendData);
}
//===================================================================
MainWgt::~MainWgt()
{
    delete ui;
}
//===================================================================
// Подключение
void MainWgt::slotConnect()
{
    if( ui )
    {
        ui->Info_textEdit->append( " Подключено (TCP). " );
    }

    ui->btnReconnect->setPalette( Qt::green );
    ui->btnReconnect->setToolTip( "Соединение установлено" );
    ui->btnReconnect->setAutoFillBackground( true );

    ui->frame->setVisible(true);
}
//===================================================================
// Отключение
void MainWgt::slotDisconnect()
{
    if( ui )
    {
        ui->Info_textEdit->append( " Отключено (TCP). " );
    }

    ui->btnReconnect->setPalette( Qt::red );
    ui->btnReconnect->setToolTip( "Соединение отключено" );
    ui->btnReconnect->setAutoFillBackground( true );

    ui->frame->setVisible(false);
}
//===================================================================
// Чтение данных
void MainWgt::slotRecvData(QByteArray data)
{
    ui->frame->setVisible(true);
    Info("Сообщение принято.");

    auto m = reinterpret_cast< const BaseMsg<> * >( data.data() );

    if( m->hdr == MTypeServ )
    {
        Info("Приняты данные от Serv.");
        if( m->magic == serv::MagicTest && ui )
        {
            auto s_msg = reinterpret_cast< const serv::MsgTest * >( data.data() );
            packP = s_msg->param;
            slotUpdateForm();
        }
    }
}
//===================================================================
// Вывод информация
void MainWgt::Info(QString str)
{
    ui->Info_textEdit->append( str );
}
//===================================================================
// Переподключение
void MainWgt::on_btnReconnect_clicked()
{
    QString host = ui->host_comboBox->currentText();
    int port = ui->port_spinBox->value();
    emit signalReconnect( host, port );
}
//===================================================================
// Обновление формы данных
void MainWgt::slotUpdateForm()
{
    ui->num_double->setValue(packP.num_double);
    ui->int1_spinBox->setValue(packP.num_int[0]);
    ui->int2_spinBox->setValue(packP.num_int[1]);
    ui->int3_spinBox->setValue(packP.num_int[2]);

    for(int i = 0; i < 4; i++)
    {
        if (i == packP.mode)
        {
            indicators[i]->setPixmap(circle_green);
        }
        else
        {
            indicators[i]->setPixmap(circle_yellow);
        }
    }
}
//===================================================================
// Отправка сообщения
void MainWgt::on_btnSendMsg_clicked()
{
    packC::SPack packet;

    packet.num_double = ui->num_double->value();
    packet.num_int[0] = ui->int1_spinBox->value();
    packet.num_int[1] = ui->int2_spinBox->value();
    packet.num_int[2] = ui->int3_spinBox->value();

    packet.mode = 3;

    cli::MsgJobTest msg ( packet );
    emit signalSendData( toQByteArray( msg ) );

    Info("Сообщение отправлено");
}
//===================================================================
// Отключение от сети при выключении клиента
void MainWgt::closeEvent(QCloseEvent *event)
{
    emit signalDisconnect();
    event->accept();
}
//===================================================================
