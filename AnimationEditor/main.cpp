#include <QApplication>
#include "AnimationEditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnimationEditor editor;
    editor.show();
    return a.exec();
}
