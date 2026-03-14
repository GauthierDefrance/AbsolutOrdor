#include <QApplication>
#include <QWidget>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.resize(400, 200);

    QPushButton button("Clique moi", &window);
    button.move(150, 80);

    window.show();

    return app.exec();
}