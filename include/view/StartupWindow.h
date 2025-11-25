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
#include <QTreeView>
#include <QPushButton>


class StartupWindow : public QMainWindow {
    Q_OBJECT

public:

    explicit StartupWindow(QWidget* parent = nullptr);
    ~StartupWindow() override = default;

    SessionForm* get_form();

private:

    QWidget* setup_buttons();
    QWidget* setup_central_view();
    void configure_table_view();

    QTableView *m_table_view;
    QTreeView* m_tree_view;
    QLineEdit* m_filter_box;
    QPushButton* m_start_session_btn;
    QPushButton* m_stop_session_btn;
    QPushButton* m_save_capture_btn;

public:

    SessionForm* session_form;

    QTableView* get_table_view() {return m_table_view;}

    QTreeView* get_tree_view() {return m_tree_view;}

    QPushButton* get_start_session_btn() {return m_start_session_btn;}

    QPushButton* get_stop_session_btn() {return m_stop_session_btn;}

};



#endif //STARTUPMENU_H
