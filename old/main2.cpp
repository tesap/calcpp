#include <QApplication>
#include <QListWidget>
#include <QDebug>

class MyListWidget : public QListWidget {
public:
    using QListWidget::QListWidget;

protected:
    void dropEvent(QDropEvent *event) override {
        addItem("New");
        qDebug() << "AAAAAAAAAAAAAAAAA";
        // Call the base class dropEvent to handle reordering
        QListWidget::dropEvent(event);

        // Example: output the new order after reordering
        for (int i = 0; i < count(); ++i) {
            qDebug() << i + 1 << ": " << item(i)->text();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the customized QListWidget
    MyListWidget *listWidget = new MyListWidget();

    // Add some sample items to the list
    listWidget->addItem("Task 1");
    listWidget->addItem("Task 2");
    listWidget->addItem("Task 3");
    listWidget->addItem("Task 4");
    listWidget->addItem("AAAA");

    listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Show the list widget
    listWidget->show();

    return app.exec();
}
