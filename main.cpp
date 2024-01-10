#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DatabaseOperations.h"
#include "StudentModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    StudentModel studentModel;

    // Expose StudentModel to QML
    engine.rootContext()->setContextProperty("studentModel", &studentModel);

    // Load the main QML file - BrowseStudents.qml
    // The home screen will be the the browse students view; App starts with this view.
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);
    //engine.load(QUrl(QStringLiteral("qrc:/MinimalExample.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
