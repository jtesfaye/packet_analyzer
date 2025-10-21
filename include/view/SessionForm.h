//
// Created by jeremiah tesfaye on 6/19/25.
//

#ifndef SESSIONFORM_H
#define SESSIONFORM_H

#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QDialog>
#include <QGroupBox>
#include <capture/CaptureConfig.h>
#include <QToolButton>


class SessionForm : public QDialog {
    Q_OBJECT

public:

    explicit SessionForm(QWidget* parent = nullptr);

    ~SessionForm() override = default;

    bool isOnline() {
        return is_online;
    }

    QPushButton* get_start_session_button() const;

    CaptureConfig get_config();

private:

    int get_packet_count() const;
    std::string get_device_selected() const;
    int get_capture_size() const;
    u_int8_t get_flags() const;

    std::string get_file_path() const;
    std::string get_filter() const;
    u_int8_t get_settings();

    [[nodiscard]] QGroupBox* setup_online_form();
    [[nodiscard]] QGroupBox* setup_offline_form();

    void populate_device_list() const;
    QWidget* setup_packet_count_box();
    QWidget* setup_device_list_box();
    QToolButton* setup_toggle_button();
    QWidget* setup_more_settings_section();

    QLineEdit* file_path;
    QLineEdit* filter;

    QComboBox* device_list_combo;
    QSpinBox* packet_count_box;
    QSpinBox* capture_size_spin_box;
    QCheckBox* promisc_mode;
    QCheckBox* high_prec_time;

    QPushButton* start_session_button;

    std::vector<std::string> device_list;
    int count;
    u_int8_t settings;
    u_int8_t flags;

    bool is_online;

};



#endif //SESSIONFORM_H
