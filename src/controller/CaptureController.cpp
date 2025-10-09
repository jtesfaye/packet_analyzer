

#include <controller/CaptureController.h>
#include <iostream>
#include <print>
#include <QMenuBar>
#include <view/SessionForm.h>
#include <util/PacketObserver.h>

CaptureController::CaptureController() :
m_window_view(new StartupWindow()),
consumer_thread()
{

    consumer_thread = nullptr;

    init_menubar();

    init_start_btn(m_window_view->get_start_session_btn());

    connect_session_form_to_main();

    m_window_view->show();

}

void CaptureController::init_menubar() {

    QAction* start = m_window_view->get_session_start_action();

    connect(start,
        &QAction::triggered,
        this,
        &CaptureController::start_session
        );

    m_window_view->get_session_menu()->addAction(start);

}

void CaptureController::init_start_btn(QPushButton *btn) {

    connect(btn, &QPushButton::clicked, this, [=]() {

        m_window_view->get_form()->show();

    });

}


void CaptureController::connect_session_form_to_main() {

    const QPushButton* form = m_window_view->get_form()->get_start_session_button();

    connect(form, &QPushButton::clicked, this, &CaptureController::start_session);

}

void CaptureController::start_session() {

    auto form = m_window_view->get_form();

    if (form->isOnline()) {
        start_online_capture(form->device_selected(),
            form->get_packet_count(),
            form->get_capture_size(),
            form->get_settings(),
            0x1111
        );
    } else if (!form->isOnline()) {

        start_offline_capture(form->get_file_path());
    }
}

void CaptureController::start_online_capture(
    std::string device_name,
    int count,
    int size,
    u_int8_t settings,
    u_int8_t flags) {

    using namespace capture;

    flags = 0xff;
    capture = PacketCapture::createOnlineCapture(
        device_name
        , count
        , size
        , settings
        , flags);

    PacketRefBuffer* buffer_ref = capture->get_buffer().get();
    PacketObserver* observer = capture->get_observer().get();

    DisplayModel* model = new DisplayModel{*buffer_ref, this};

    m_window_view->get_table_view()->setModel(model);

    consumer_thread = new QThread;

    observer->moveToThread(consumer_thread);

    connect(consumer_thread, &QThread::started, observer, &PacketObserver::wait_for_next);

    connect(
        observer,
        &PacketObserver::emit_packets_ready,
        model,
        &DisplayModel::add_data,
        Qt::QueuedConnection);

    consumer_thread->start();

    m_window_view->get_table_view()->setModel(model);

    m_window_view->show();

    std::thread capture_thread([&] {

        capture->start_capture();

        while (true) {

        }

    });

    capture_thread.detach();

}

void CaptureController::start_offline_capture(std::string pcap_file) {


    capture = PacketCapture::createOfflineCapture(pcap_file);

    PacketRefBuffer* buffer_ref = capture->get_buffer().get();
    PacketObserver* observer = capture->get_observer().get();

    DisplayModel* model = new DisplayModel{*buffer_ref, this};

    m_window_view->get_table_view()->setModel(model);

    consumer_thread = new QThread;

    observer->moveToThread(consumer_thread);

    connect(consumer_thread, &QThread::started, observer, &PacketObserver::wait_for_next);

    connect(
        observer,
        &PacketObserver::emit_packets_ready,
        model,
        &DisplayModel::add_data,
        Qt::QueuedConnection);

    consumer_thread->start();

    m_window_view->get_table_view()->setModel(model);

    m_window_view->show();

    std::thread capture_thread([&] {

        capture->start_capture();

        while (true) {

        }

    });

    capture_thread.detach();

}










