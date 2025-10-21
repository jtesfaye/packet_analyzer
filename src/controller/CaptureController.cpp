
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <view/SessionForm.h>
#include <util/PacketObserver.h>
#include <capture/CaptureSession.h>
#include <controller/CaptureController.h>


CaptureController::CaptureController() :
m_window_view(new StartupWindow()),
consumer_thread()
{

    consumer_thread = nullptr;

    init_start_btn(m_window_view->get_start_session_btn());

    connect_session_form_to_main();

    m_window_view->show();

}


void CaptureController::init_start_btn(QPushButton *btn) {

    connect(btn, &QPushButton::clicked, this, [=]() {

        if (current_session) {
            prompt_save();
            current_session->send_command(SessionCommand::end());
            current_session.reset();
        }

        m_window_view->get_form()->show();

    });
}

void CaptureController::init_stop_btn(QPushButton *btn, const std::shared_ptr<CaptureSession>& session) {

    connect(btn, &QPushButton::clicked, this, [=]() {

        this->stop_online_capture(session);

    });

}

void CaptureController::connect_session_form_to_main() {

    const QPushButton* form = m_window_view->get_form()->get_start_session_button();

    connect(form, &QPushButton::clicked, this, &CaptureController::start_session);

}

void CaptureController::start_session() {

    auto form = m_window_view->get_form();
    start_capture(form->get_config());
    form->hide();
}


void CaptureController::stop_online_capture(const std::shared_ptr<CaptureSession>& session) {
    session->send_command(SessionCommand::stop());
}


void CaptureController::start_capture(const CaptureConfig& config) {

    using namespace capture;

    current_session = std::make_shared<CaptureSession>(config);

    if (config.mode == CaptureMode::Online) {
        init_stop_btn(m_window_view->get_stop_session_btn(), current_session);
    }

    connect_observer_to_model(*current_session);

    std::thread session_thread([this] {

        current_session->start_session();

    });

    session_thread.detach();

    current_session->send_command(SessionCommand::start());

}

void CaptureController::connect_observer_to_model(const CaptureSession& session) {

    //This removes any pre-existing models from a previous session.
    if (auto m = m_window_view->get_table_view(); m->model()) {
        delete m->model();
    }

    const std::shared_ptr<PacketRefBuffer> buffer_ref = session.get_buffer();
    const std::shared_ptr<PacketObserver> observer = session.get_observer();

    DisplayModel* model = new DisplayModel{buffer_ref, this};

    m_window_view->get_table_view()->setModel(model);

    consumer_thread = new QThread;

    observer->moveToThread(consumer_thread);

    connect(consumer_thread, &QThread::started, observer.get(), &PacketObserver::wait_for_next);

    connect(
        observer.get(),
        &PacketObserver::emit_packets_ready,
        model,
        &DisplayModel::add_data,
        Qt::QueuedConnection);

    consumer_thread->start();

}

void CaptureController::prompt_save() const {

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(nullptr,
        "End session",
        "Do you want to save the current capture before starting a new one?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {

        QString path = QFileDialog::getSaveFileName(nullptr,
        "Create a file",
        "",
        "PCAP Files (*.pcap)"
        );

        current_session->send_command(SessionCommand::save(path.toStdString()));
    }
}











