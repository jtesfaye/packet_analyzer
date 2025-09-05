//
// Created by jeremiah tesfaye on 6/19/25.
//

#include "../../include/view/Sidebar.h"


Sidebar::Sidebar(QWidget *parent) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    textView = new QPlainTextEdit(this);

    textView->setReadOnly(true);

    layout->addWidget(textView);

    setLayout(layout);


}


void Sidebar::setText(std::string s) const {

    textView->setPlainText(s.data());

}

