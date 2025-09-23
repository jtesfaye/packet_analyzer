//
// Created by jeremiah tesfaye on 6/19/25.
//

#ifndef SESSIONFORM_H
#define SESSIONFORM_H

#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QToolButton>
#include <QPushButton>



class SessionFormOnline : public QWidget {
    Q_OBJECT

public:

    explicit SessionFormOnline(QWidget* parent = nullptr);

    ~SessionFormOnline() override = default;

    int get_packet_count() const;

    std::string device_selected() const;

    int get_capture_size() const;

    u_int8_t get_settings();

    u_int8_t get_flags() const;

    QPushButton* get_start_session_button() const;

private:

    void populate_device_list() const;
    QWidget* setup_packet_count_box();
    QWidget* setup_device_list_box();
    QToolButton* setup_toggle_button();
    QWidget* setup_more_settings_section();

    QComboBox* device_list_combo;
    QSpinBox* packet_count_box;
    QSpinBox* capture_size_spin_box;
    QCheckBox* promisc_mode;
    QCheckBox* immediate_mode;
    QCheckBox* monitor_mode;
    QCheckBox* high_prec_time;
    QCheckBox* low_buffer_timeout;

    QPushButton* start_session_button;

    std::vector<std::string> device_list;
    int count;
    u_int8_t settings;
    u_int8_t flags;


};



#endif //SESSIONFORM_H
