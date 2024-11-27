#include <QApplication>
#include <QQuickView>
#include <QWidget>
#include <QVBoxLayout>
#include <QQmlEngine>
#include <QQuickItem>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Ensure QML is loaded from the correct path
    QCoreApplication::addLibraryPath("/usr/lib/x86_64-linux-gnu/qt5/qml");

    // Create a QQuickView to display QML content
    QQuickView *view = new QQuickView;
    
    // Set the source QML file (ensure it exists in the resources)
    view->setSource(QUrl(QStringLiteral("qrc:/resources/main.qml")));

    // Ensure the QQuickView resizes its root object to match the view's size
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    // Create a QWidget container for the QQuickView
    QWidget *container = QWidget::createWindowContainer(view);
    
    // Set minimum size and focus policy for the container
    container->setMinimumSize(800, 600);
    container->setFocusPolicy(Qt::StrongFocus);

    // Create the main window and set up layout
    QWidget mainWindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);
    layout->addWidget(container);
    mainWindow.setWindowTitle("Map Viewer");

    // Show the main window with the QQuickView inside
    mainWindow.show();

    return app.exec();
}
