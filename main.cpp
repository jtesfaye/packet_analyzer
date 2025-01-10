#include <iostream>
#include <pcap/pcap.h>

int main(int argc, char* argv[]) {

    pcap_t* handle;
    const char* device;
    pcap_if_t* device_list;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (argc > 2) {
        std::cout << "Usage: <device name>\n";
        return 0;
    } else {
        device = argc == 1 ? "any" : argv[1];
    }

    //pcap_init used to initialize the packet capture library
    //implicitly handled so in some situations you dont need to call it
    if (pcap_init(PCAP_CHAR_ENC_UTF_8, errbuf) != 0) {
        std::cout << "Error with init: " << errbuf << '\n';
    }

    //pcap_create returns a handle, takes deivce name and errorbuf, 2 char arrays
    handle = pcap_create(device, errbuf);
    if (handle == nullptr) {
        std::cout << "pcap_create failed: " << errbuf << '\n';
        return -1;
    }


    pcap_set_snaplen(handle, 65535);

    //handle needs to be activated with pcap_activate
    if (int act = pcap_activate(handle) != 0) {
        
        if (act < 0) {
            std::cout << "pcap_activate error: " << pcap_geterr(handle);
        } else {
            std::cout << "pcap_activate warning" << pcap_geterr(handle);
        }

        pcap_close(handle);
        return 1;
        
    }

    std::cout << "List of all devices that can be opened for capture\n";
    if (pcap_findalldevs(&device_list, errbuf) != 0) {
        std::cout << "Error with finding device list: " << errbuf << '\n';
        pcap_freealldevs(device_list);
    }

    //print out items in list
    pcap_if_t* it;
    for (it = device_list; it != nullptr; it = it->next) {

        std::cout << it->name;
        if (it->description != nullptr) {
            std::cout << " " << it->description;
        }
        std::cout << "\n";
    }

    pcap_loop(handle, 10, callback, NULL);

    pcap_freealldevs(device_list);
    pcap_close(handle);

    return 0;



}

/*
pkthdr members, 
ts - a struct timeval containing the packet was captured
caplen - a bpf_u_int32 giving the number of bytes available from capture
len - a bpf_u_int32 giving the length of the packet, in bytes (which might be more than bytes available)
*/
void callback(u_char *data, const struct pcap_pkthdr* h, const u_char *bytes) {

}