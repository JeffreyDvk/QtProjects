#include <QApplication>
#include "Win.h"

int main(int argc,char *argv[]){
    QApplication app(argc,argv);
    Win lecteur;

    lecteur.show();

    return app.exec();
}
