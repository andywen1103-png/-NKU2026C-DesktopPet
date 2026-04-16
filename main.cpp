#include "desktop_pet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DesktopPet pet;
    pet.show();
    return a.exec();
}
