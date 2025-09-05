//
// Created by jeremiah tesfaye on 6/15/25.
//

#include <view/MainWindow.h>
#include <controller/CaptureController.h>
#include <QSplitter>
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_table_view(new QTableView(this))
, m_sidebar(new Sidebar(this)) {

    auto splitter = new QSplitter(Qt::Horizontal, this);

    //connect(m_main_view, &MainView::packetSelected, controller, &CaptureController::get_packet_details);

    //setup_menu_bar();

    m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_session_menu = menuBar()->addMenu("&Session");

    m_session_start = new QAction("&Start capture", this);

    splitter->addWidget(m_table_view);

    splitter->addWidget(m_sidebar);

    setCentralWidget(splitter);

}


void MainWindow::displayPacketDetails(std::string s) const {

    m_sidebar->setText(std::move(s));

}


