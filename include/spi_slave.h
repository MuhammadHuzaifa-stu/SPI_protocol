#ifndef SPI_SLAVE
#define SPI_SLAVE

#include <systemc.h>

enum state_slave_t {
    SLAVE_IDLE     = 0,
    SLAVE_ENABLE   = 1,
    SLAVE_DATA     = 2,
    SLAVE_DATA_VAL = 3
};

SC_MODULE(spi_slave) {

    // -- Ports
    sc_in_clk           clk;
    sc_in<bool>         arst_n;

    // master <-> slave
    sc_in<bool>         sck;
    sc_in<bool>         cs;
    sc_in<bool>         mosi;

    // recieved data
    sc_out<sc_uint<8>>  recieved_command;
    sc_out<sc_uint<24>> recieved_addr;
    sc_out<sc_uint<32>> recieved_data;
    sc_out<bool>        recieved_data_val;

    // ---------- Internal signals ----------

    sc_signal<state_slave_t> current_state;
    sc_signal<state_slave_t> next_state;

    sc_signal<sc_uint<8>>    save_command;
    sc_signal<sc_uint<24>>   save_addr;
    sc_signal<sc_uint<32>>   save_data;

    sc_signal<sc_uint<7>>    bit_cnt;


    void slave_output_block();    
    
    void mosi_data_handling();

    void bit_counter();

    // -- FSM
    void fsm_state_update();
    void fsm_next_state_logic();
    void output_logic();


    SC_CTOR(spi_slave) {
        SC_METHOD(slave_output_block);
        sensitive << clk.pos() << arst_n.neg();

        SC_METHOD(mosi_data_handling);
        sensitive << sck.pos() << cs;
                
        SC_METHOD(bit_counter);
        sensitive << sck.pos();

        // FSM state update
        SC_METHOD(fsm_state_update);
        sensitive << clk.pos() << arst_n.neg();
        
        // FSM next state logic
        SC_METHOD(fsm_next_state_logic);
        sensitive << current_state << cs << mosi << bit_cnt;   
        
        // Output logic
        SC_METHOD(output_logic);
        sensitive << current_state;
    }

};

#endif // SPI_SLAVE