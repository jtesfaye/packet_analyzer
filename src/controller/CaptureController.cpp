
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <view/SessionForm.h>
#include <util/PacketObserver.h>
#include <capture/CaptureSession.h>
#include <controller/CaptureController.h>
#include <print>
#include <util/RowFactory.h>
#include <util/TreeModelFactory.h>

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

void CaptureController::connect_session_to_table_view(const QTableView& table) {

    connect(&table, &QTableView::clicked, this, &CaptureController::recieve_row_index);

}

void CaptureController::start_capture(const CaptureConfig& config) {

    using namespace capture;

    current_session = std::make_shared<CaptureSession>(config);

    if (config.mode == CaptureMode::Online) {
        init_stop_btn(m_window_view->get_stop_session_btn(), current_session);
    }

    connect_session_to_table_view(*m_window_view->get_table_view());

    connect_observer_to_this(*current_session);

    std::thread session_thread([this] {

        current_session->start_session();

    });

    session_thread.detach();

    current_session->send_command(SessionCommand::start());

}

void CaptureController::connect_observer_to_this(const CaptureSession& session) {

    //This removes any pre-existing models from a previous session.
    if (m_model) {
        m_model.reset();
    }

    const std::shared_ptr<PacketObserver> observer = session.get_observer();

    m_model = std::make_shared<DisplayModel>(this);

    m_window_view->get_table_view()->setModel(m_model.get());

    consumer_thread = new QThread;

    observer->moveToThread(consumer_thread);

    connect(consumer_thread, &QThread::started, observer.get(), &PacketObserver::start_observer);

    //When packets enter the internal buffer, Observer notifies controller of ready packets
    connect(
        observer.get(),
        &PacketObserver::emit_packets_ready,
        this,
        &CaptureController::recieve_row,
        Qt::QueuedConnection);

    //When controller requests the details of a packets, observer returns with emit_pkt_details
    connect(
        observer.get(),
        &PacketObserver::emit_pkt_details,
        this,
        &CaptureController::receive_details,
        Qt::QueuedConnection);

    //When a click signal from a QTableView is recieved, the controller forwards that to the observer
    connect(
        this,
        &CaptureController::forward_detail_request,
        observer.get(),
        &PacketObserver::receive_detail_request,
        Qt::QueuedConnection);


    connect(this, &CaptureController::send_row_to_model, m_model.get(), &DisplayModel::add_data);
    consumer_thread->start();

}

void CaptureController::receive_details(const std::vector<ProtocolDetails>& details) {

    //later cache the old models instead
    if (const auto old_model = m_window_view->get_tree_view()->model()) {
        delete old_model;
    }

    m_window_view->get_tree_view()->setModel(TreeModelFactory::make_model(details));

}


void CaptureController::recieve_row(std::deque<packet_ref>::iterator first, std::deque<packet_ref>::iterator last) const {

    emit send_row_to_model(first, last);
}

void CaptureController::recieve_row_index(const QModelIndex& index) {

    size_t row = index.row();
    emit forward_detail_request(row);

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











