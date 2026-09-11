//
// Created by jeremiah tesfaye on 3/10/26.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <session/SessionCommand.h>
#include <sniffer_api.h>
#include <capture/CaptureConfig.h>
#include <session/CaptureSession.h>

namespace py = pybind11;

class CaptureSessionWrapper
{
public:
  CaptureSessionWrapper() : session_(sink_) {}

  void send_command(SessionCommand cmd)
  {
    session_.send_command(std::move(cmd));
  }

  PythonSink::EventMsg poll()
  {
    return sink_.poll();
  }

private:
  CaptureSession session_;
  PythonSink sink_;
};


PYBIND11_MODULE(sniffer_py, m)
{
  // CaptureMode
  py::enum_<CaptureMode>(m, "CaptureMode")
      .value("Online", CaptureMode::Online)
      .value("Offline", CaptureMode::Offline)
      .export_values();

  py::enum_<PythonSink::EventMsgType>(m, "EventMessageType")
    .value("Empty", PythonSink::EventMsgType::Empty)
    .value("Packet", PythonSink::EventMsgType::Packet)
    .value("Detail", PythonSink::EventMsgType::Detail)
    .value("Stat", PythonSink::EventMsgType::Stat);

  // CaptureConfig
  py::class_<CaptureConfig>(m, "CaptureConfig")
    .def(py::init<>())
    .def_readwrite("mode", &CaptureConfig::mode)
    .def_readwrite("source", &CaptureConfig::source)
    .def_readwrite("packet_count", &CaptureConfig::packet_count)
    .def_readwrite("capture_size", &CaptureConfig::capture_size)
    .def_readwrite("settings", &CaptureConfig::settings)
    .def_readwrite("flags", &CaptureConfig::flags)
    .def_readwrite("filter", &CaptureConfig::filter);

  // CommandType
  py::enum_<CommandType>(m, "CommandType")
      .value("Start", CommandType::Start)
      .value("Stop", CommandType::Stop)
      .value("Save", CommandType::Save)
      .value("End", CommandType::End)
      .export_values();

  // SessionCommand
  py::class_<SessionCommand>(m, "SessionCommand")
    .def_static("start", &SessionCommand::start, py::arg("config"))
    .def_static("stop", &SessionCommand::stop)
    .def_static("save", &SessionCommand::save, py::arg("path"))
    .def_static("end", &SessionCommand::end)
    .def_static("get_details", &SessionCommand::get_details, py::arg("pkt_id"))
    .def_readonly("type", &SessionCommand::type)
    .def_readonly("arg", &SessionCommand::arg);

  // CaptureSessionWrapper
  py::class_<CaptureSessionWrapper>(m, "CaptureSession")
      .def(py::init<>())
      .def("send_command", &CaptureSessionWrapper::send_command, py::arg("cmd"))
      .def("poll", &CaptureSessionWrapper::poll);

  //EventMessage
  py::class_<PythonSink::EventMsg>(m, "EventMessage")
    .def(py::init<>())
    .def_readwrite("type", &PythonSink::EventMsg::type)
    .def_readwrite("message", &PythonSink::EventMsg::msg);
}
