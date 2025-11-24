//
// Created by jeremiah tesfaye on 6/19/25.
//

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <model/RowModel.h>

class Sidebar : public QWidget {

Q_OBJECT

public:
    Sidebar(QWidget* parent = nullptr);
    ~Sidebar() override = default;

    void setText(std::string) const;

private:

    QPlainTextEdit* textView;

};



#endif //SIDEBAR_H
