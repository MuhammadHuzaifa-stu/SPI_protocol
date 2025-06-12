#include <systemc.h>
#include <spi_master.h>

    
// -- SCLK generator
void spi_master::clock_divider() {
    if (!arst_n.read()) {
        clk_div.write(0);
        clk_gen.write(0);
        clk_gen_cmpl.write(0);
    } else {
        if ((current_state.read() == MASTER_ENABLE) || (current_state.read() == MASTER_DATA  ) ||
            ((clk_div.read() > 0 && clk_div.read() < 4) && current_state.read() == MASTER_WAITE_SCK)) {
            
            if (clk_div.read() == 4) {
                clk_div.write(0);
                clk_gen.write(!clk_gen.read());
            }
            else {
                clk_div.write(clk_div.read() + 1);
                clk_gen.write(clk_gen.read());
            }
            clk_gen_cmpl.write(0);
        } else {
            clk_div.write(0);
            clk_gen.write(0);  // Return to CPOL value
            clk_gen_cmpl.write(1);
        }
    }
}
    
// -- INPUT data register
void spi_master::input_data_register() {
    if (!arst_n.read()) {
        data_save.write(0);
    }
    else if (en.read() == 1) {
        if (ext_command_in.read() == 0x00) {
            sc_uint<64> temp = (ext_command_in.read(), ext_address_in.read(), ext_data_in.read());
            data_save.write(temp);
        }
        else {
            sc_uint<64> temp = (ext_command_in.read(), ext_address_in.read(), 0x00000000);
            data_save.write(temp);
        }
    }
    else {
        data_save.write(data_save.read());
    }
}
    
// -- MOSI data handling
void spi_master::mosi_data_handling() {
    if (en.read()) {
        data_tx.write(data_save.read());
        data_count.write(0);
        data_end.write(0);
    }
    else if (current_state.read() == MASTER_DATA) {
        // Shift data
        if (data_count.read() == 0) {
            data_tx.write(data_tx.read());
        } else {
            data_tx.write((data_tx.read().range(62, 0), 0));
        }
        
        if (data_count.read() == 63) {
            data_count.write(0);
            data_end.write(1);
        }
        else {
            data_count.write(data_count.read() + 1);
            data_end.write(0);
        }
    }
    else {
        data_tx.write(data_save.read());
        data_count.write(0);
        data_end.write(0);
    }
}
    
// -- MISO data handling
void spi_master::miso_data_handling() {
    if (!arst_n.read()) {
        data_in.write(0);
    }
    else if (current_state.read() == MASTER_DATA) {
        if ((data_count.read() >= 32) && (data_count.read() <= 63)) {
            data_in.write((data_in.read().range(30, 0), miso.read()));
        }
        else {
            data_in.write(data_in.read());
        }
    }
    else {
        data_in.write(0);
    }
}
    
// -- FSM
void spi_master::fsm_state_update() {
    if (!arst_n.read()) {
        current_state.write(MASTER_IDLE);
    }
    else {
        current_state.write(next_state.read());
    }
}
    
void spi_master::fsm_next_state_logic() {
    switch(current_state.read()) {
        case MASTER_IDLE:
            if (en.read() == 1) {
                next_state.write(MASTER_ENABLE);
            }
            else {
                next_state.write(MASTER_IDLE);
            }
            break;
        case MASTER_ENABLE:
            next_state.write(MASTER_DATA);
            break;
        case MASTER_DATA:
            if (data_end.read() == 1) {
                next_state.write(MASTER_WAITE_SCK);
            }
            else {
                next_state.write(MASTER_DATA);
            }
            break;
        case MASTER_WAITE_SCK:
            if (clk_gen_cmpl.read() == 1) {
                next_state.write(MASTER_IDLE);
            }
            else {
                next_state.write(MASTER_WAITE_SCK);
            }
            break;
        default:
            next_state.write(MASTER_IDLE);
    }
}
    
void spi_master::output_logic() {
    switch(current_state.read()) {
        case MASTER_IDLE:
            cs.write(1);
            sck.write(0);
            mosi.write(0);
            break;
        case MASTER_ENABLE:
            cs.write(0);
            sck.write(clk_gen.read());
            mosi.write(0);
            break;
        case MASTER_DATA:
            cs.write(0);
            sck.write(clk_gen.read());
            mosi.write(data_tx.read()[63]);
            break;
        
        case MASTER_WAITE_SCK:
            cs.write(0);
            sck.write(clk_gen.read());
            mosi.write(data_tx.read()[63]);
            break;
        default:
            cs.write(1);
            sck.write(0);
            mosi.write(0);
    }
}
    
void spi_master::assign_outputs() {
    ext_data_out.write(data_in.read());
    bsy.write((current_state.read() != MASTER_IDLE) ? 0 : 1);
}