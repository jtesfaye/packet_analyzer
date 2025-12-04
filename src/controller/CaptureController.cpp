
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <view/SessionForm.h>
#include <util/PacketObserver.h>
#include <controller/CaptureController.h>
#include <print>

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

}

void CaptureController::start_capture(const CaptureConfig& config, ViewComponents& comp) {

    using namespace capture;
    m_row_model = std::make_shared<RowModel>(this);
    m_detail_model = std::make_shared<DetailModel>(this);

    Models m(m_row_model.get(), m_detail_model.get(), comp.chart_view);
    current_session = std::make_shared<CaptureSession>(config, m);

    connect(comp.row_view, &QTableView::clicked, this, [this] (const QModelIndex& index) {
        current_session->send_command(CommandType::GetDetails(index.row()));
    });

    comp.row_view->setModel(m_row_model.get());
    comp.detail_view->setModel(m_detail_model.get());

    std::thread session_thread([this] {

        current_session->start_session();

    });

    session_thread.detach();

    current_session->send_command(SessionCommand::start());
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











