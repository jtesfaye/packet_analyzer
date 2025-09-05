
#include <include/capture/Offline.h>
#include <sys/_types/_u_int8_t.h>
#include <iostream>


namespace capture {

  Offline::Offline
  (
    std::string&& path_name,
    const int count
  ) 
  : m_file_path(std::move(path_name))
  {

    initialize_handle();

    set_data_link(pcap_datalink(handle()));

    if (!handle()) {

      std::cerr << "error with handle" << errbuf << "\n";

    }

    std::cout << "Finished construction\n";
  }

  void
  Offline::initialize_handle() {

    std::cout << "Starting initalize_handle\n";

    if (pcap_t* pcap_handle = pcap_open_offline(m_file_path.c_str(), errbuf)) {

      _handle = std::unique_ptr<pcap_t, decltype(&close_handle)> (
        pcap_handle,
        pcap_close
      );

    } else {

      throw std::runtime_error(errbuf);

    }

    std::cout << "Finished with initalize_handle\n";

  }

}

