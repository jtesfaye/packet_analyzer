
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <view/SessionForm.h>
#include <util/PacketObserver.h>
#include <controller/CaptureController.h>
#include <print>
#include <util/RowFactory.h>
#include <util/TreeModelFactory.h>

CaptureController::CaptureController() :
consumer_thread()
{
    consumer_thread = nullptr;
}

void CaptureController::stop_capture() {
    if (!current_session) return;

    if (stopped) {
        end_capture();
    }

    current_session->send_command(SessionCommand::stop());
    stopped = true;
}

void CaptureController::end_capture() {

    if (!current_session) return;

    if (stopped) {
        prompt_save();
    } else {
        current_session->send_command(SessionCommand::stop());
        prompt_save();

    }

    current_session->send_command(SessionCommand::end());
    current_session.reset();

}

void CaptureController::connect_session_to_table_view(const QTableView& table) {

    connect(&table, &QTableView::clicked, this, &CaptureController::recieve_row_index);

}

void CaptureController::start_capture(const CaptureConfig& config, QTableView& row_table, QTreeView &tree) {

    using namespace capture;

    current_session = std::make_shared<CaptureSession>(config);

    connect_session_to_table_view(row_table);

    connect_observer_to_this(*current_session, row_table, tree);

    std::thread session_thread([this] {

        current_session->start_session();

    });

    session_thread.detach();

    current_session->send_command(SessionCommand::start());

}

void CaptureController::connect_observer_to_this(const CaptureSession& session, QTableView &table, QTreeView &tree) {

    //This removes any pre-existing models from a previous session.
    if (m_row_model) {
        m_row_model.reset();
    }

    const std::shared_ptr<PacketObserver> observer = session.get_observer();

    m_row_model = std::make_shared<RowModel>(this);
    m_detail_model = std::make_shared<DetailModel> (this);

    table.setModel(m_row_model.get());
    tree.setModel(m_detail_model.get());

    consumer_thread = new QThread;

    observer->moveToThread(consumer_thread);

    connect(
        consumer_thread,
        &QThread::started,
        observer.get(),
        &PacketObserver::start_observer);

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


    connect(this,
        &CaptureController::send_row_to_model,
        m_row_model.get(),
        &RowModel::add_data);
    consumer_thread->start();

}

void CaptureController::receive_details(const std::vector<ProtocolDetails>& details) {

    m_detail_model->set_data(&details);
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











