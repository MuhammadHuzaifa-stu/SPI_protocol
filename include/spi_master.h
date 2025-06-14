#ifndef SPI_MASTER
#define SPI_MASTER

#include <systemc.h>

enum state_master_t {
    MASTER_IDLE      = 0,
    MASTER_ENABLE    = 1,
    MASTER_DATA      = 2,
    MASTER_WAITE_SCK = 3
};

SC_MODULE(spi_master) {
    
    // -- Ports
    sc_in_clk           clk;
    sc_in<bool>         arst_n;
    
    // Chip Select
    sc_in<bool>         en;
    
    // Master Out Slave In
    sc_in<sc_uint<8>>   ext_command_in;
    sc_in<sc_uint<24>>  ext_address_in;
    sc_in<sc_uint<32>>  ext_data_in;
    
    // Master In Slave Out
    sc_out<sc_uint<32>> ext_data_out;
    sc_out<bool>        bsy;
    
    // master <-> slave
    sc_out<bool>        cs;
    sc_out<bool>        sck;
    sc_out<bool>        mosi;
    sc_in<bool>         miso;
    
    // ---------- Internal signals ----------

    sc_signal<state_master_t> current_state;
    sc_signal<state_master_t> next_state;
    
    sc_signal<sc_uint<4>>     clk_div;
    sc_signal<bool>           clk_gen;
    
    sc_signal<sc_uint<64>>    data_save;
    sc_signal<sc_uint<6>>     data_count;
    sc_signal<bool>           data_end;
    
    sc_signal<sc_uint<64>>    data_tx;
    sc_signal<sc_uint<32>>    data_in;

    sc_signal<bool>           clk_gen_cmpl;
    
    // -- SCLK generator
    void clock_divider();
    
    // -- INPUT data register
    void input_data_register();
    
    // -- MOSI data handling
    void mosi_data_handling();
    
    // -- MISO data handling
    void miso_data_handling();
    
    // -- FSM
    void fsm_state_update();
    void fsm_next_state_logic();
    void output_logic();
    void assign_outputs();
    
    SC_CTOR(spi_master) {
        // Clock divider process
        SC_METHOD(clock_divider);
        sensitive << clk.pos() << arst_n.neg();
        
        // Input data register
        SC_METHOD(input_data_register);
        sensitive << clk.pos() << arst_n.neg();
        
        // MOSI data handling (triggered on sck posedge)
        SC_METHOD(mosi_data_handling);
        sensitive << sck.pos() << en.neg();
        
        // MISO data handling (triggered on sck negedge)
        SC_METHOD(miso_data_handling);
        sensitive << sck.neg() << arst_n.neg();
        
        // FSM state update
        SC_METHOD(fsm_state_update);
        sensitive << clk.pos() << arst_n.neg();
        
        // FSM next state logic
        SC_METHOD(fsm_next_state_logic);
        sensitive << current_state << en << data_end << clk_gen_cmpl;
        
        // Output logic
        SC_METHOD(output_logic);
        sensitive << current_state << clk_gen << data_tx;
        
        // Continuous assignments
        SC_METHOD(assign_outputs);
        sensitive << current_state << data_in;
    }
};

#endif // SPI_MASTER