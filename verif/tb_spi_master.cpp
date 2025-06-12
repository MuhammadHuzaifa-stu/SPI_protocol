#include "systemc.h"
#include "spi_master.h"

int sc_main(int argc, char* argv[]) {
    sc_clock               clk("clk", 10, SC_NS);  // 10ns period (100MHz)
    sc_signal<bool>        arst_n;
    
    // Chip Select
    sc_signal<bool>        en;
    
    // Master Out Slave In
    sc_signal<sc_uint<8>>  ext_command_in;
    sc_signal<sc_uint<24>> ext_address_in;
    sc_signal<sc_uint<32>> ext_data_in;
    
    // Master In Slave Out
    sc_signal<sc_uint<32>> ext_data_out;
    sc_signal<bool>        bsy;
    
    // master <-> slave
    sc_signal<bool>        cs;
    sc_signal<bool>        sck;
    sc_signal<bool>        mosi;
    sc_signal<bool>        miso;

    // Connect the DUT
    cout << "Connecting DUT..." << endl;
    spi_master master("SPI_MASTER");
    master.clk           (clk           );
    master.arst_n        (arst_n        );
    master.en            (en            );
    master.ext_command_in(ext_command_in);
    master.ext_address_in(ext_address_in);
    master.ext_data_in   (ext_data_in   );
    master.ext_data_out  (ext_data_out  );
    master.bsy           (bsy           );
    master.cs            (cs            );
    master.sck           (sck           );
    master.mosi          (mosi          );
    master.miso          (miso          );
    cout << "DUT Connected Successfully" << endl;

    // Open VCD file
    cout << "Creating VCD file..." << endl;
    sc_trace_file *wf = sc_create_vcd_trace_file("spi_master");
    // Dump the desired signals
    sc_trace(wf, clk,            "clk"           );
    sc_trace(wf, arst_n,         "arst_n"        );
    sc_trace(wf, en,             "en"            );
    sc_trace(wf, ext_command_in, "ext_command_in");
    sc_trace(wf, ext_address_in, "ext_address_in");
    sc_trace(wf, ext_data_in,    "ext_data_in"   );
    sc_trace(wf, ext_data_out,   "ext_data_out"  );
    sc_trace(wf, bsy,            "bsy"           );
    sc_trace(wf, cs,             "cs"            );
    sc_trace(wf, sck,            "sck"           );
    sc_trace(wf, mosi,           "mosi"          );
    sc_trace(wf, miso,           "miso"          );

    // -- Additional signals for debugging purpose
    // sc_trace(wf, master.current_state, "current_state"    );
    // sc_trace(wf, master.next_state,    "next_state"       );
    // sc_trace(wf, master.data_save,     "data_save"        );
    // sc_trace(wf, master.data_tx,       "data_tx"          );
    // sc_trace(wf, master.data_count,    "data_count"       );
    // sc_trace(wf, master.data_end,      "data_end"         );
    // sc_trace(wf, master.clk_div,       "clk_div"          );
    // sc_trace(wf, master.clk_gen,       "clk_gen"          );
    // sc_trace(wf, master.clk_gen_cmpl,  "clk_gen_cmpl"     );


    // Initialize all variables
    cout << "Initializing Signals..." << endl;
    arst_n         = 0;
    en             = 0;
    ext_command_in = 0x00;
    ext_address_in = 0xA1B2C3;
    ext_data_in    = 0xDEADBEEF;
    miso           = 0;

    // Reset sequence
    cout << "Applying reset..." << endl;
    sc_start(20, SC_NS);
    arst_n = 1;
    sc_start(10, SC_NS);

    // Test sequence
    cout << "Starting test sequence..." << endl;
    en = 1;
    sc_start(10, SC_NS);
    en = 0;

    // Wait for transaction to complete
    cout << "@" << sc_time_stamp() << endl;
    cout << "Waiting for transaction to complete..." << endl;
    sc_start(10, SC_NS);
    while (!bsy.read()) {
        sc_start(10, SC_NS);
    }

    // Add some more clock cycles to observe the end of transaction
    sc_start(50, SC_NS);

    cout << "Simulation complete" << endl;
    sc_close_vcd_trace_file(wf);
    cout << "VCD file closed" << endl;
    
    return 0;
}