//
// Created by jeremiah tesfaye on 6/15/25.
//

#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QTableView>
#include <view/Sidebar.h>
#include <view/SessionForm.h>


class StartupWindow : public QMainWindow {

    Q_OBJECT

public:

    explicit StartupWindow(QWidget* parent = nullptr);
    ~StartupWindow() override = default;

    SessionFormOnline* get_form();
public slots:

    void switch_view();

private:

    void setup_menu_bar();

    void setup_table_view();

    void start_session();

    void show_table_view();



    QTableView *m_table_view;
    Sidebar* m_sidebar;
    QMenu *m_session_menu;

    QStackedWidget* stacked_widget;

    QAction* m_session_start;

public:

    SessionFormOnline* session_form;
    QTableView* get_table_view() {return m_table_view;}
    Sidebar* get_sidebar() {return m_sidebar;}
    QMenu* get_session_menu() {return m_session_menu;}

    QAction* get_session_start_action() {return m_session_start;}




};



#endif //STARTUPMENU_H
