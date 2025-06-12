
---

```markdown
# 🧩 SPI Master-Slave Communication in SystemC

This repository contains a SystemC-based implementation of an SPI (Serial Peripheral Interface) protocol. The project is currently in progress and aims to model a complete SPI controller—including both **Master** and **Slave** components—with eventual integration and testing using **UVM**-style verification.

---

## 📌 Project Status

- ✅ `spi_master` module implemented and **tested successfully**
- 📄 Design documentation ongoing — refer to `doc/tn15_spi_interface_specification.pdf`
- 🚧 `spi_slave` currently **under development**
- 🔜 Once `spi_slave` is complete, the full **SPI controller/top module** will be built and tested
- 🧪 Planned: UVM-based verification of the entire SPI system

---

## 🗂️ Directory Structure

```

.
├── include/                 # Header files (spi\_master.h, etc.)
├── src/                     # Source files (spi\_master.cpp, etc.)
├── verif/                   # Testbench files (tb\_spi\_master.cpp)
├── doc/                     # Documentation and design specs
│   └── specification.pdf    # SPI protocol design reference
├── build/                   # Compiled output target (created during build)
├── Makefile                 # Build system
└── README.md                # Project overview

````

---

## 🔧 Build Instructions

Make sure [SystemC](https://accellera.org/downloads/standards/systemc) is installed on your system and the `SYSTEMC_HOME` path in the `Makefile` is correctly set.

### 🛠 Compile

```bash
make
````

### ▶️ Run Simulation

```bash
make run
```

### 🧹 Clean Build Files

```bash
make clean
```

> 🔁 Modify `SYSTEMC_HOME`, `lib-linux64`, or `lib` inside the `Makefile` depending on your platform and installation path.

---

## 📦 Current Features

### ✅ Completed:

* `spi_master`:

  * Clock generation
  * Configurable CPOL/CPHA    #Pending
  * MOSI transmission logic
  * SCLK toggling and control
  * Verified via testbench in `verif/tb_spi_master.cpp`

### 🔄 In Progress:

* `spi_slave`:

  * MISO reception logic
  * Response behavior to master's clock and control
  * Testing interface hooks

### 🧩 Upcoming:

* Integration into a **top-level SPI controller module**
* **Documentation** finalization with waveform diagrams, signal timing, and FSM transitions
* **UVM-style testbench** for complete SPI system verification

---

## 📄 Documentation

You can find the design reference document being followed in:

```
doc/tn15_spi_interface_specification.pdf
```

This document outlines the protocol's structure, signal definitions, timing diagrams, and step-by-step design milestones.
However, there are some modifications also, which will soon be shared in a separate doc and diagrams. 

---

## 🙋‍♂️ Author

**\[Muhammad Huzaifa]** – Electrical Engineer | Python + C/Cpp + SystemC + Digital Design Enthusiast
For questions or collaborations, feel free to open an issue or reach out.
