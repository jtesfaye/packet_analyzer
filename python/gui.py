import sys
import threading
import time
import json
from datetime import datetime
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout,
    QPushButton, QTableWidget, QTableWidgetItem,
    QDialog, QFormLayout, QLineEdit, QDialogButtonBox, QComboBox, QSpinBox,
    QSplitter, QTreeWidget, QTreeWidgetItem
)
from PyQt6.QtCore import pyqtSignal, QObject
sys.path.append("/Users/jt/projects/workspace/personal_projects/sniffer/build")
import sniffer_py

class ConfigDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Capture Configuration")

        layout = QFormLayout(self)

        self.source_input = QLineEdit("en0")
        self.filter_input = QLineEdit("")
        self.capture_size_input = QSpinBox()
        self.capture_size_input.setRange(64, 65535)
        self.capture_size_input.setValue(250)

        self.packet_count_input = QSpinBox()
        self.packet_count_input.setRange(0, 1000000)
        self.packet_count_input.setValue(0)

        self.mode_input = QComboBox()
        self.mode_input.addItems(["Online", "Offline"])

        layout.addRow("Interface:", self.source_input)
        layout.addRow("Filter:", self.filter_input)
        layout.addRow("Capture Size:", self.capture_size_input)
        layout.addRow("Packet Count (0=inf):", self.packet_count_input)
        layout.addRow("Mode:", self.mode_input)

        self.buttons = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok |
            QDialogButtonBox.StandardButton.Cancel
        )
        self.buttons.accepted.connect(self.accept)
        self.buttons.rejected.connect(self.reject)

        layout.addWidget(self.buttons)

    def get_config(self):
        config = sniffer_py.CaptureConfig()
        config.source = self.source_input.text()
        config.filter = self.filter_input.text()
        config.capture_size = self.capture_size_input.value()
        config.packet_count = self.packet_count_input.value()
        config.flags = 1
        config.settings = 0xff

        if self.mode_input.currentText() == "Online":
            config.mode = sniffer_py.CaptureMode.Online
        else:
            config.mode = sniffer_py.CaptureMode.Offline

        return config

class CaptureWorker(QObject):
    packet_received = pyqtSignal(object)
    detail_received = pyqtSignal(object)
    stopped = pyqtSignal()

    def __init__(self, config):
        super().__init__()
        self.config = config
        self.running = False
        self.session = None

    def start(self):
        self.running = True
        self.session = sniffer_py.CaptureSession()
        self.session.send_command(sniffer_py.SessionCommand.start(self.config))

        while self.running:
            msg = self.session.poll()

            if msg.type == sniffer_py.EventMessageType.Packet:
                self.packet_received.emit(msg.message)
            elif msg.type == sniffer_py.EventMessageType.Detail:
                self.detail_received.emit(msg.message)

        self.session.send_command(sniffer_py.SessionCommand.stop())
        self.session.send_command(sniffer_py.SessionCommand.end())
        self.stopped.emit()

    def stop(self):
        self.running = False

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Packet Analyzer")
        self.resize(800, 500)

        # Layout
        central = QWidget()
        self.setCentralWidget(central)
        layout = QVBoxLayout()
        central.setLayout(layout)

        # Buttons
        self.start_btn = QPushButton("Start Capture")
        self.stop_btn = QPushButton("Stop Capture")
        self.stop_btn.setEnabled(False)

        layout.addWidget(self.start_btn)
        layout.addWidget(self.stop_btn)

        # Table
        self.table = QTableWidget(0, 8)
        self.table.setHorizontalHeaderLabels([
            "#", "Time", "Src", "Dest", "Protocol",
            "Length", "Payload", "Description"
        ])

        self.splitter = QSplitter()
        layout.addWidget(self.splitter)

        self.splitter.addWidget(self.table)

        self.detail_tree = QTreeWidget()
        self.detail_tree.setHeaderLabel("Packet Details")
        self.splitter.addWidget(self.detail_tree)

        self.table.cellClicked.connect(self.on_row_clicked)

        # Signals
        self.start_btn.clicked.connect(self.start_capture)
        self.stop_btn.clicked.connect(self.stop_capture)

        # Worker
        self.worker = None
        self.thread = None
        self.packet_index = 0

    def create_config(self):
        config = sniffer_py.CaptureConfig()
        config.capture_size = 250
        config.flags = 1
        config.filter = ""
        config.mode = sniffer_py.CaptureMode.Online
        config.settings = 0xff
        config.packet_count = 0  # continuous
        config.source = "en0"
        return config

    def start_capture(self):
        dialog = ConfigDialog(self)
        if dialog.exec() != QDialog.DialogCode.Accepted:
            return

        config = dialog.get_config()

        self.worker = CaptureWorker(config)

        self.thread = threading.Thread(target=self.worker.start, daemon=True)
        self.worker.packet_received.connect(self.add_packet)
        self.worker.detail_received.connect(self.display_details)

        self.thread.start()

        self.start_btn.setEnabled(False)
        self.stop_btn.setEnabled(True)

    def stop_capture(self):
        if self.worker:
            self.worker.stop()

        self.start_btn.setEnabled(True)
        self.stop_btn.setEnabled(False)

    def add_packet(self, packet):
        try:
            data = json.loads(packet)
        except Exception:
            return  # skip malformed packets

        row = self.table.rowCount()
        self.table.insertRow(row)

        packet_id = data.get("id", row)
        self.table.setItem(row, 0, QTableWidgetItem(str(packet_id)))
        # Convert time
        try:
            ts = datetime.fromtimestamp(
                data.get("time_sec", 0)
            ).strftime("%Y-%m-%d %H:%M:%S")
        except Exception:
            ts = "N/A"

        self.table.setItem(row, 1, QTableWidgetItem(ts))
        self.table.setItem(row, 2, QTableWidgetItem(str(data.get("src", ""))))
        self.table.setItem(row, 3, QTableWidgetItem(str(data.get("dest", ""))))
        self.table.setItem(row, 4, QTableWidgetItem(str(data.get("protocol", ""))))
        self.table.setItem(row, 5, QTableWidgetItem(str(data.get("wire_length", ""))))
        self.table.setItem(row, 6, QTableWidgetItem(str(data.get("payload_length", ""))))
        self.table.setItem(row, 7, QTableWidgetItem(str(data.get("desc", ""))))

    def display_details(self, detail_json):
        try:
            data = json.loads(detail_json)
        except Exception:
            return

        self.detail_tree.clear()

        for layer in data:
            name = layer.get("name", "")
            if not name:
                continue

            parent = QTreeWidgetItem([name])
            self.detail_tree.addTopLevelItem(parent)

            for field in layer.get("fields", []):
                child = QTreeWidgetItem([field])
                parent.addChild(child)

        self.detail_tree.expandAll()

    def on_row_clicked(self, row, column):
        if not self.worker or not self.worker.session:
            return

        try:
            item = self.table.item(row, 0)
            if item is None:
                return

            packet_id = int(item.text())
            cmd = sniffer_py.SessionCommand.get_details(packet_id)
            self.worker.session.send_command(cmd)
        except Exception as e:
            print("Error requesting details:", e)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())