//
// Created by jeremiah tesfaye on 6/19/25.
//

#include <QDialogButtonBox>
#include <view/SessionForm.h>
#include <capture/PacketCapture.h>
#include <QFormLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>


SessionForm::SessionForm(QWidget *parent)
: QDialog(parent)
, count(0)
, flags(0)
, settings(0)
, start_session_button(nullptr)
{

    auto main_layout = new QVBoxLayout(this);

    auto offline_box = setup_offline_form();
    auto online_box = setup_online_form();

    connect(offline_box, &QGroupBox::toggled, online_box, [=](bool toggled) {

        if (toggled)
            online_box->setChecked(false);
            is_online = false;

    });

    connect(online_box, &QGroupBox::toggled, offline_box, [=](bool toggled) {

        if(toggled)
            offline_box->setChecked(false);
            is_online = true;

    });

    main_layout->addWidget(offline_box);
    main_layout->addWidget(online_box);

    auto btn_widget = new QWidget(this);
    auto btn_layout = new QHBoxLayout(this);

    start_session_button = new QPushButton("Start", this);

    btn_layout->addStretch();
    btn_layout->addWidget(start_session_button);
    btn_widget->setLayout(btn_layout);

    main_layout->addWidget(btn_widget);

    setLayout(main_layout);

}

QPushButton *SessionForm::get_start_session_button() const {
    return start_session_button;
}

QGroupBox *SessionForm::setup_offline_form() {

    file_path = new QLineEdit(this);
    file_path->setReadOnly(true);

    auto file_select_btn = new QPushButton("&Select .pcap",this);

    connect(file_select_btn, &QPushButton::clicked, this, [=]() {

        QString path = QFileDialog::getOpenFileName(this,
            "Select a file",
            "",
            "PCAP Files (*.pcap)");

        if (!path.isEmpty()) {
            file_path->setText(path);
        }
    });

    auto layout = new QHBoxLayout(this);

    layout->addWidget(file_path);
    layout->addWidget(file_select_btn);

    auto offline_group_box = new QGroupBox("offline capture");
    offline_group_box->setCheckable(true);
    offline_group_box->setChecked(false);

    offline_group_box->setLayout(layout);

    return offline_group_box;

}


QGroupBox *SessionForm::setup_online_form() {

    auto *layout = new QFormLayout(this);

    layout->addRow(setup_device_list_box());

    layout->addRow(setup_packet_count_box());

    auto toggle_button = setup_toggle_button();

    layout->addRow(toggle_button);

    auto adv_settings_widget = setup_more_settings_section();

    layout->addRow(adv_settings_widget);

    //toggle show/hide button
    connect(toggle_button, &QToolButton::toggled, [=](bool checked) {

        if (checked)
            adv_settings_widget->setVisible(true);
        else
            adv_settings_widget->setVisible(false);

        toggle_button->setText(checked ? "Hide more settings ▲" : "Show more settings ▼");
    });

    auto box = new QGroupBox("Online capture");
    box->setCheckable(true);
    box->setLayout(layout);
    box->setChecked(false);

    return box;
}


QWidget* SessionForm::setup_packet_count_box() {

    auto container = new QWidget(this);

    auto label = new QLabel("Number of packets",container);

    packet_count_box = new QSpinBox(container);

    auto layout = new QHBoxLayout(container);

    packet_count_box->setMinimum(1);

    packet_count_box->setMaximum(1000000);

    layout->addWidget(label);

    layout->addWidget(packet_count_box);

    container->setLayout(layout);

    return container;

}

QWidget *SessionForm::setup_device_list_box() {

    auto container = new QWidget(this);

    auto label = new QLabel("Select a device: ", container);

    auto layout = new QHBoxLayout(container);

    layout->addWidget(label);

    device_list_combo = new QComboBox(container);

    populate_device_list();

    layout->addWidget(device_list_combo);

    container->setLayout(layout);

    return container;

}


QToolButton* SessionForm::setup_toggle_button() {

    QToolButton* toggle_button = new QToolButton(this);

    toggle_button->setText("Show Advanced ▼");

    toggle_button->setCheckable(true);

    toggle_button->setChecked(false);

    return toggle_button;

}


QWidget *SessionForm::setup_more_settings_section() {

    auto* settings_widget = new QWidget(this);

    settings_widget->setVisible(false);

    auto settings_layout = new QFormLayout(settings_widget);

    auto cap_size_widget = new QWidget(settings_widget);
    auto cap_size_label = new QLabel("Amount of bytes per packet");
    auto cap_size_layout = new QHBoxLayout(cap_size_widget);

    capture_size_spin_box = new QSpinBox();

    capture_size_spin_box->setMaximum(65535);

    capture_size_spin_box->setMinimum(1);

    cap_size_layout->addWidget(cap_size_label);

    cap_size_layout->addWidget(capture_size_spin_box);

    cap_size_widget->setLayout(cap_size_layout);

    promisc_mode = new QCheckBox("Promiscuous mode", this);

    immediate_mode = new QCheckBox("Immediate mode", this);

    monitor_mode = new QCheckBox("Monitor mode", this);

    high_prec_time = new QCheckBox("High precision timestamp", this);

    low_buffer_timeout = new QCheckBox("Slow mode", this);

    settings_layout->addWidget(cap_size_widget);
    settings_layout->addWidget(promisc_mode);
    settings_layout->addWidget(immediate_mode);
    settings_layout->addWidget(monitor_mode);
    settings_layout->addWidget(high_prec_time);
    settings_layout->addWidget(low_buffer_timeout);

   settings_widget->setLayout(settings_layout);

    return settings_widget;


}

void
SessionForm::populate_device_list() const {

    using namespace capture;

    std::vector<std::string> names = PacketCapture::get_devices();

    for (auto n : names) {

        device_list_combo->addItem(n.data());

    }

}

int
SessionForm::get_packet_count() const {

    return packet_count_box->value();

}


std::string
SessionForm::device_selected() const {

    return device_list_combo->currentText().toStdString();

}

int
SessionForm::get_capture_size() const {

    return capture_size_spin_box->value();

}

u_int8_t
SessionForm::get_settings() {

    using namespace capture;

    if (promisc_mode->isChecked()) {

        settings ^= PROMISC;
    }

    if (immediate_mode->isChecked()) {
        settings ^= IMMEDIATE;
    }

    if (monitor_mode->isChecked()) {
        settings ^= MONITOR;
    }

    if (high_prec_time->isChecked()) {

        settings ^= PRECISION;
    }


    if (!low_buffer_timeout->isChecked()) {

        settings ^= HIGH_TRAFF;

    }

    return settings;

}

u_int8_t
SessionForm::get_flags() const {

    return flags;

}


std::string SessionForm::get_file_path() const {

    if (file_path->text().isEmpty()) {
        throw std::runtime_error("No file selected");
    }

    return file_path->text().toStdString();

}






