#include "mainwgt.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWgt w;
    w.setWindowTitle( " TEST _ CLIENT " );
    w.show();

    // Расположение окна
    w.setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                w.size(),
                qApp->desktop()->availableGeometry()
            )
        );

    return a.exec();
}
