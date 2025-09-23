//
// Created by jeremiah tesfaye on 6/15/25.
//

#include <view/StartupWindow.h>
#include <controller/CaptureController.h>
#include <QSplitter>
#include <QMenuBar>
#include <QStackedWidget>
StartupWindow::StartupWindow(QWidget* parent)
: QMainWindow(parent)
, session_form(new SessionFormOnline(this))
, m_table_view(new QTableView(this))
, m_sidebar(new Sidebar(this))
, stacked_widget(new QStackedWidget(this)){

    auto splitter = new QSplitter(Qt::Horizontal, this);

    m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_session_menu = menuBar()->addMenu("&Session");

    m_session_start = new QAction("&Start capture", this);

    stacked_widget->addWidget(m_table_view);
    stacked_widget->addWidget(session_form);

    splitter->addWidget(stacked_widget);

    splitter->addWidget(m_sidebar);

    setCentralWidget(stacked_widget);

}

void StartupWindow::switch_view() {

    if (stacked_widget->currentIndex() == 1)
        stacked_widget->setCurrentIndex(0);

    else if (stacked_widget->currentIndex() == 0)
        stacked_widget->setCurrentIndex(1);

}

SessionFormOnline *StartupWindow::get_form() {
    return session_form;
}





