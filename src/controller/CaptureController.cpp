

#include <controller/CaptureController.h>
#include <iostream>
#include <QMenuBar>
#include <view/SessionForm.h>
#include <util/PacketGetter.h>
#include <util/DetailsReader.h>
#include <packet/LayerWrappers.h>
#include <util/PacketObserver.h>

CaptureController::CaptureController() :
m_window_view(new MainWindow()),
m_model(new DisplayModel(this))
{

    consumer_thread = nullptr;

    qRegisterMetaType<proccessed_packet>("proccessed_packet");

    init_menubar();

    m_window_view->get_table_view()->setModel(m_model);

    //when a new packet(s) comes in, the controller will notify the model
    connect(this,
        &CaptureController::update_model,
        m_model,
        &DisplayModel::add_data
        );


    //signal for when a row on the table is selected
    connect(
        m_window_view->get_table_view(),
        &QTableView::clicked,
        m_window_view,
        [this](const QModelIndex &index) {

            emit m_window_view->packetSelected(index.row());

        });


    //after a row is selected, controller get the packet parsed.
    connect(
        m_window_view,
        &MainWindow::packetSelected,
        this,
        &CaptureController::get_packet_details
        );


    /*  The view has a sidebar that displays details of a packet, when the controller gets data to display,
     *  this signal passes that string to the view
     */
    connect(this,
        &CaptureController::display_details,
        m_window_view,
        &MainWindow::displayPacketDetails
        );

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


void CaptureController::start_session() {

    if (SessionFormOnline form(m_window_view); form.exec() == QDialog::Accepted) {
        const std::string device_name = form.device_selected();
        const int count = form.get_packet_count();
        const int cap_size = form.get_capture_size();
        const u_int8_t settings = form.get_settings();
        const u_int8_t flags = form.get_flags();

        start_online_capture(device_name, count, cap_size, settings, flags);
    }

}


void CaptureController::send_to_model(const proccessed_packet& pkt) {

    emit update_model(pkt);

}


void
CaptureController::start_online_capture
(std::string device_name, int count, int size, u_int8_t settings, u_int8_t flags) {

    using namespace capture;

    capture = PacketCapture::createOnlineCapture(
        device_name
        , count
        , size
        , settings
        , flags);

    PacketRefBuffer* buffer_ref = capture->get_buffer().get();



}


DisplayModel *CaptureController::get_model() const {

    return m_model;

}


void CaptureController::get_packet_details(const int row) {

    DetailsReader str_repr;
    auto&[layer2_details, layer_3_details, layer2_str] = m_model->get_details(row);
    link_layer& layer2 = *layer2_details;

    layer2_str = std::visit([&](auto&& arg) {

        return str_repr(arg);

    }, layer2);

    //then get a string of said string

    std::string full_details {layer2_str + "\n"};

    emit display_details(full_details);

}






