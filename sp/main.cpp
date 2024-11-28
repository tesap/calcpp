#include <QListView>
#include <QAbstractListModel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPainter>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>

class SmoothListModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit SmoothListModel(QObject *parent = nullptr) : QAbstractListModel(parent) {
        items = {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5"};
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return items.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= items.size()) return QVariant();
        if (role == Qt::DisplayRole) {
            return items.at(index.row());
        }
        return QVariant();
    }

    void moveItem(int from, int to) {
        if (from < 0 || from >= items.size() || to < 0 || to >= items.size() || from == to) return;

        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to > from ? to + 1 : to);
        items.move(from, to);
        endMoveRows();
    }

private:
    QStringList items;
};

class SmoothListView : public QListView {
    Q_OBJECT
public:
    explicit SmoothListView(QWidget *parent = nullptr) : QListView(parent) {
        setSelectionMode(QAbstractItemView::SingleSelection);
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::InternalMove);
        setDefaultDropAction(Qt::MoveAction);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        draggedIndex = indexAt(event->pos());
        if (!draggedIndex.isValid()) {
            QListView::mousePressEvent(event);
            return;
        }

        QPixmap pixmap(viewport()->size());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect itemRect = visualRect(draggedIndex);
        itemRect.moveTopLeft(itemRect.topLeft() - viewport()->rect().topLeft());
        render(&painter, QPoint(0, 0), QRegion(itemRect));
        painter.end();

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->pos() - itemRect.topLeft());
        drag->exec(Qt::MoveAction);
    }

void dragMoveEvent(QDragMoveEvent *event) override {
    event->accept();
    auto newIndex = indexAt(event->pos());
    if (!newIndex.isValid() || !draggedIndex.isValid() || newIndex == draggedIndex) {
        return;
    }

    auto *model = qobject_cast<SmoothListModel *>(this->model());
    if (model) {
        model->moveItem(draggedIndex.row(), newIndex.row());
        draggedIndex = newIndex;
        update();  // Redraw the view to reflect changes.
    }
}

    void dropEvent(QDropEvent *event) override {
        event->accept();
        draggedIndex = QModelIndex();
    }

private:
    QModelIndex draggedIndex;

void animateItemMove(int from, int to) {
    auto *model = qobject_cast<SmoothListModel *>(this->model());
    if (!model) return;

    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);

    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i);
        QRect startRect = visualRect(index);
        QRect endRect = visualRect(model->index(i == from ? to : i));

        QPropertyAnimation *animation = new QPropertyAnimation(viewport(), "geometry");
        animation->setDuration(300);
        animation->setStartValue(startRect);
        animation->setEndValue(endRect);
        animationGroup->addAnimation(animation);
    }

    connect(animationGroup, &QParallelAnimationGroup::finished, animationGroup, &QObject::deleteLater);
    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

};

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QWidget mainWidget;
    QVBoxLayout layout(&mainWidget);

    SmoothListModel *model = new SmoothListModel(&mainWidget);
    SmoothListView *listView = new SmoothListView(&mainWidget);
    listView->setModel(model);

    layout.addWidget(listView);
    mainWidget.show();

    return app.exec();
}

#include "main.moc"

