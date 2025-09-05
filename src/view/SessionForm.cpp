//
// Created by jeremiah tesfaye on 6/19/25.
//

#include "../../include/view/SessionForm.h"
#include "../../include/capture/PacketCapture.h"
#include <QDialogButtonBox>
#include <QFormLayout>

#include <QLabel>
#include <QHBoxLayout>


SessionFormOnline::SessionFormOnline(QWidget *parent)
: QDialog(parent)
, count(0)
, flags(0)
, settings(0)
{

    auto *layout = new QFormLayout();

    layout->addRow(setup_device_list_box());

    layout->addRow(setup_packet_count_box());

    auto *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addRow(btnBox);

    auto toggle_button = setup_toggle_button();

    layout->addRow(toggle_button);

    auto adv_settings_widget = setup_more_settings_section();

    layout->addRow(adv_settings_widget);

    this->setLayout(layout);

    //toggle show/hide button
    connect(toggle_button, &QToolButton::toggled, [=](bool checked) {

        if (checked)
            adv_settings_widget->setVisible(true);
        else
            adv_settings_widget->setVisible(false);

        toggle_button->setText(checked ? "Hide more settings ▲" : "Show more settings ▼");
    });

}


QWidget* SessionFormOnline::setup_packet_count_box() {

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

QWidget *SessionFormOnline::setup_device_list_box() {

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


QToolButton* SessionFormOnline::setup_toggle_button() {

    QToolButton* toggle_button = new QToolButton(this);

    toggle_button->setText("Show Advanced ▼");

    toggle_button->setCheckable(true);

    toggle_button->setChecked(false);

    return toggle_button;

}


QWidget *SessionFormOnline::setup_more_settings_section() {

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
SessionFormOnline::populate_device_list() const {

    using namespace capture;

    std::vector<std::string> names = PacketCapture::get_devices();

    for (auto n : names) {

        device_list_combo->addItem(n.data());

    }

}



int
SessionFormOnline::get_packet_count() const {

    return packet_count_box->value();

}


std::string
SessionFormOnline::device_selected() const {

    return device_list_combo->currentText().toStdString();

}



int
SessionFormOnline::get_capture_size() const {

    return capture_size_spin_box->value();

}



u_int8_t
SessionFormOnline::get_settings() {

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
SessionFormOnline::get_flags() const {

    return flags;

}






