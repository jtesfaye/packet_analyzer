//
// Created by jeremiah tesfaye on 6/15/25.
//

#include <view/StartupWindow.h>
#include <controller/CaptureController.h>
#include <QSplitter>
#include <QMenuBar>
#include <QStackedWidget>
#include <QIcon>
#include <QLineEdit>


StartupWindow::StartupWindow(QWidget* parent)
: QMainWindow(parent)
, session_form(new SessionForm(this))
, m_table_view(new QTableView(this))
, m_start_session_btn(new QPushButton( this))
, m_stop_session_btn(new QPushButton(this))
, m_save_capture_btn(new QPushButton(this))
, m_filter_box(new QLineEdit(this))
, m_tree_view(new QTreeView(this)) {

    configure_table_view();
    setCentralWidget(setup_central_view());
    resize(600,600);

}

QWidget *StartupWindow::setup_central_view() {

    auto main_widget = new QWidget(this);
    auto main_layout = new QVBoxLayout(main_widget);
    auto splitter = new QSplitter(Qt::Orientation::Vertical, this);

    auto btn_widget = setup_buttons();

    main_layout->addWidget(btn_widget);

    m_filter_box->setReadOnly(false);
    main_layout->addWidget(m_filter_box);

    m_table_view->setSelectionMode(QTableView::SingleSelection);

    splitter->addWidget(m_table_view);
    splitter->addWidget(m_tree_view);

    main_layout->addWidget(splitter);

    main_widget->setLayout(main_layout);

    return main_widget;
}


QWidget *StartupWindow::setup_buttons() {

    auto btn_widget = new QWidget(this);
    auto btn_layout = new QHBoxLayout(btn_widget);

    auto start_icon = QIcon(":/assets/play.png");
    auto stop_icon = QIcon(":/assets/stop.png");
    auto save_icon = QIcon(":/assets/save.png");

    m_start_session_btn->setIcon(start_icon);
    m_start_session_btn->setIconSize(QSize(48, 48));
    m_start_session_btn->setToolTip("Start a new capture session");

    m_stop_session_btn->setIcon(stop_icon);
    m_stop_session_btn->setIconSize(QSize(48,48));
    m_stop_session_btn->setToolTip("End capture session");

    m_save_capture_btn->setIcon(save_icon);
    m_save_capture_btn->setIconSize(QSize(48,48));
    m_save_capture_btn->setToolTip("Save capture");

    btn_layout->addWidget(m_start_session_btn);
    btn_layout->addWidget(m_stop_session_btn);
    btn_layout->addWidget(m_save_capture_btn);
    btn_layout->addStretch();

    btn_widget->setLayout(btn_layout);

    return btn_widget;

}

void StartupWindow::configure_table_view() {

    m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

}

SessionForm *StartupWindow::get_form() {
    return session_form;
}





