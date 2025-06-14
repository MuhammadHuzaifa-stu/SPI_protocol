#include <systemc.h>
#include <spi_slave.h>


void spi_slave::slave_output_block() {
    if (!arst_n.read()) {

        recieved_data.write(0);
        recieved_addr.write(0);
        recieved_command.write(0);

    } else {

        recieved_data.write(save_data);
        recieved_addr.write(save_addr);
        recieved_command.write(save_command);

    }
}

void spi_slave::bit_counter() {

    if (current_state.read() == SLAVE_IDLE) {

        bit_cnt.write(0);

    } else if (current_state.read() == SLAVE_DATA || 
               current_state.read() == SLAVE_ENABLE) {

        bit_cnt.write(bit_cnt.read() + 1);

    } else {

        bit_cnt.write(bit_cnt.read());

    }

}

void spi_slave::mosi_data_handling() {

    if (cs.read()) {

        save_addr.write(0);
        save_command.write(0);
        save_data.write(0);
    
    } else {

        if (next_state.read() == SLAVE_DATA || 
            next_state.read() == SLAVE_ENABLE) {
            
            if (bit_cnt.read() <= 6) {

                save_command.write(((save_command.read() << 1) | mosi.read()));
            
            } else if (bit_cnt.read() <= 30) {
            
                save_addr.write(((save_addr.read() << 1) | mosi.read()));
            
            } else if (bit_cnt.read() <= 62) {
            
                save_data.write(((save_data.read() << 1) | mosi.read()));
            
            } else {
            
                save_addr.write(save_addr.read());
                save_command.write(save_command.read());
                save_data.write(save_data.read());
            
            }
        
        }

    }
}

// -- FSM
void spi_slave::fsm_state_update() {

    if (!arst_n.read()) {

        current_state.write(SLAVE_IDLE);

    } else {

        current_state.write(next_state.read());

    }
}
    
void spi_slave::fsm_next_state_logic() {

    switch(current_state.read()) {
        case SLAVE_IDLE:
            if (!cs.read() && mosi.read()) {

                next_state.write(SLAVE_ENABLE);
            } else {

                next_state.write(SLAVE_IDLE);

            }
            break;
        case SLAVE_ENABLE:
            if (cs.read()) {

                next_state.write(SLAVE_IDLE);
            
            } else {
    
                next_state.write(SLAVE_DATA);

            }
            break;
        case SLAVE_DATA:
            if (cs.read()) {

                next_state.write(SLAVE_IDLE);

            } else if (bit_cnt.read() == 63) {

                next_state.write(SLAVE_DATA_VAL);

            } else {

                next_state.write(SLAVE_DATA);

            }
            break;
        case SLAVE_DATA_VAL:
            if (cs.read()) {

                next_state.write(SLAVE_IDLE);
            
            } else {
    
                next_state.write(SLAVE_IDLE);

            }

            break;
        default:
            next_state.write(SLAVE_IDLE);
    }
}
    
void spi_slave::output_logic() {
    switch(current_state.read()) {
        case SLAVE_IDLE:
            recieved_data_val.write(0);
            break;
        case SLAVE_ENABLE:
            recieved_data_val.write(0);
            break;
        case SLAVE_DATA:
            recieved_data_val.write(0);
            break;
        case SLAVE_DATA_VAL:
            recieved_data_val.write(1);
            break;
        default:
            recieved_data_val.write(0);
    }
}
