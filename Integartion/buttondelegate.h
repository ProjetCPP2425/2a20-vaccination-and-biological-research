#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>

class ButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ButtonDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override {
        QWidget *widget = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);

        QPushButton *btnUpdate = new QPushButton("✏️", widget);
        QPushButton *btnDelete = new QPushButton("🗑", widget);
        btnUpdate->setFixedSize(30, 30);
        btnDelete->setFixedSize(30, 30);

        layout->addWidget(btnUpdate);
        layout->addWidget(btnDelete);
        widget->setLayout(layout);


        return widget;
    }

signals:
    void updateRequested();
    void deleteRequested();
};

#endif // BUTTONDELEGATE_H
