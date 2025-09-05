//
// Created by jeremiah tesfaye on 6/15/25.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QToolBar>
#include <QTableView>
#include <view/Sidebar.h>


class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

public slots:

    void displayPacketDetails(std::string s) const;

signals:

    void packetSelected(int row);

private:

    void setup_menu_bar();

    void setup_table_view();

    void start_session();

    QTableView *m_table_view;
    Sidebar* m_sidebar;
    QMenu *m_session_menu;
    QAction* m_session_start;

public:

    QTableView* get_table_view() {return m_table_view;}
    Sidebar* get_sidebar() {return m_sidebar;}
    QMenu* get_session_menu() {return m_session_menu;}
    QAction* get_session_start_action() {return m_session_start;}




};



#endif //MAINMENU_H
