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
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>



class SessionForm : public QDialog {
    Q_OBJECT

public:

    explicit SessionForm(QWidget* parent = nullptr);

    ~SessionForm() override = default;

    [[nodiscard]] int get_packet_count() const;

    [[nodiscard]] std::string device_selected() const;

    [[nodiscard]] int get_capture_size() const;

    [[nodiscard]] u_int8_t get_flags() const;

    [[nodiscard]] QPushButton* get_start_session_button() const;

    [[nodiscard]] QString get_file_path() const;

    bool isOnline() {
        return is_online;
    }

    u_int8_t get_settings();

private:

    [[nodiscard]] QGroupBox* setup_online_form();
    [[nodiscard]] QGroupBox* setup_offline_form();

    void populate_device_list() const;
    QWidget* setup_packet_count_box();
    QWidget* setup_device_list_box();
    QToolButton* setup_toggle_button();
    QWidget* setup_more_settings_section();

    QLineEdit* file_path;

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

    bool is_online;

};



#endif //SESSIONFORM_H
