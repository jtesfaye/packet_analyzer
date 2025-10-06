//
// Created by jeremiah tesfaye on 6/15/25.
//

#include <view/StartupWindow.h>
#include <controller/CaptureController.h>
#include <QSplitter>
#include <QMenuBar>
#include <QStackedWidget>
#include <QIcon>


StartupWindow::StartupWindow(QWidget* parent)
: QMainWindow(parent)
, session_form(new SessionForm(this))
, m_table_view(new QTableView(this))
, m_sidebar(new Sidebar(this))
, stacked_widget(new QStackedWidget(this))
, m_start_session_btn(new QPushButton( this))
, m_stop_session_btn(new QPushButton(this)) {

    auto main_widget = new QWidget(this);
    auto main_layout = new QVBoxLayout(main_widget);


    auto btn_widget = new QWidget(this);
    auto btn_layout = new QHBoxLayout(btn_widget);

    auto start_icon = QIcon("../../../assets/play.png");
    auto stop_icon = QIcon("../../../assets/stop.png");

    m_start_session_btn->setIcon(start_icon);
    m_start_session_btn->setIconSize(QSize(48, 48));
    m_start_session_btn->setToolTip("Start a new capture session");

    m_stop_session_btn->setIcon(stop_icon);
    m_stop_session_btn->setIconSize(QSize(48,48));
    m_stop_session_btn->setToolTip("End capture session");

    btn_layout->addWidget(m_start_session_btn);
    btn_layout->addWidget(m_stop_session_btn);
    btn_layout->addStretch();

    btn_widget->setLayout(btn_layout);

    auto splitter = new QSplitter(Qt::Vertical, this);

    m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_session_menu = menuBar()->addMenu("&Session");

    m_session_start = new QAction("&Start capture", this);

    stacked_widget->addWidget(m_table_view);

    stacked_widget->setCurrentIndex(0);

    splitter->addWidget(stacked_widget);

    main_layout->addWidget(btn_widget);
    main_layout->addWidget(splitter);

    main_widget->setLayout(main_layout);

    setCentralWidget(main_widget);

}

void StartupWindow::switch_view() {

    if (stacked_widget->currentIndex() == 1)
        stacked_widget->setCurrentIndex(0);

    else if (stacked_widget->currentIndex() == 0)
        stacked_widget->setCurrentIndex(1);

}

SessionForm *StartupWindow::get_form() {
    return session_form;
}





