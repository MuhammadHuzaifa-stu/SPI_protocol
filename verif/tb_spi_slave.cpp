#include "systemc.h"
#include "spi_slave.h"

int sc_main(int argc, char* argv[]) {
    // Clock and reset
    sc_clock               clk("clk", 10, SC_NS);  // 100MHz
    sc_signal<bool>        arst_n;
    
    // master <-> slave
    sc_signal<bool>        cs;
    sc_signal<bool>        sck;
    sc_signal<bool>        mosi;
    
    // Received data
    sc_signal<sc_uint<8>>  received_command;
    sc_signal<sc_uint<24>> received_addr;
    sc_signal<sc_uint<32>> received_data;
    sc_signal<bool>        received_data_val;

    // Instantiate DUT
    spi_slave slave("SPI_SLAVE");
    slave.clk              (clk              );
    slave.arst_n           (arst_n           );
    slave.cs               (cs               );
    slave.sck              (sck              );
    slave.mosi             (mosi             );
    slave.recieved_command (received_command );
    slave.recieved_addr    (received_addr    );
    slave.recieved_data    (received_data    );
    slave.recieved_data_val(received_data_val);

    // Trace file
    sc_trace_file *wf = sc_create_vcd_trace_file("spi_slave");
    sc_trace(wf, clk,               "clk"              );
    sc_trace(wf, arst_n,            "arst_n"           );
    sc_trace(wf, cs,                "cs"               );
    sc_trace(wf, sck,               "sck"              );
    sc_trace(wf, mosi,              "mosi"             );
    sc_trace(wf, received_command,  "received_command" );
    sc_trace(wf, received_addr,     "received_addr"    );
    sc_trace(wf, received_data,     "received_data"    );
    sc_trace(wf, received_data_val, "received_data_val");
    sc_trace(wf, slave.bit_cnt,     "bit_cnt"          );

    // additional signals for debugging
    // sc_trace(wf, slave.save_command, "save_command");
    // sc_trace(wf, slave.save_addr,    "save_addr"   );
    // sc_trace(wf, slave.save_data,    "save_data"   );

    // Test sequence
    cout << "===== SPI Slave Sanity Test =====\n";
    
    // Reset sequence
    cout << "[1] Applying reset...\n";
    arst_n = 0;
    cs = 1;
    mosi = 0;
    sc_start(100, SC_NS);
    arst_n = 1;
    sc_start(50, SC_NS);

    // command test
    cout << "[2] Testing command reception...\n";
    cs   = 0;
    mosi = 1;
    sc_start(10, SC_NS);
    
    // Send command 0xA5 (8 bits)
    for(int i = 7; i >= 0; i--) {
        mosi = (0xA5 >> i) & 1;
        sck = 1; sc_start(5, SC_NS);
        sck = 0; sc_start(5, SC_NS);
    }
    
    // Address + data test
    cout << "[3] Testing address + data reception...\n";
    // Send address 0x123456 (24 bits)
    for(int i = 23; i >= 0; i--) {
        mosi = (0x123456 >> i) & 1;
        sck = 1; sc_start(5, SC_NS);
        sck = 0; sc_start(5, SC_NS);
    }
    
    // Send data 0xDEADBEEF (32 bits)
    for(int i = 31; i >= 0; i--) {
        mosi = (0xDEADBEEF >> i) & 1;
        sck = 1; sc_start(5, SC_NS);
        sck = 0; sc_start(5, SC_NS);
    }
    
    cs = 1;
    sc_start(50, SC_NS);
    
    cout << "===== Test Complete =====\n";
    cout << "Check waveform for detailed signal inspection\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}