# C Projects Collection 🚀

A curated collection of systems programming, graphics, simulation, and utility projects written in C. This repository serves as a portfolio showcasing various aspects of C programming, from low-level system calls to graphical applications and computational simulations.

## 📑 Table of Contents

- [Overview](#overview)
- [Projects](#projects)
  - [Graphics & Visualization](#graphics--visualization)
  - [System Utilities](#system-utilities)
  - [Simulations](#simulations)
  - [Network & Communication](#network--communication)
  - [Language Implementation](#language-implementation)
- [Technologies Used](#technologies-used)
- [Building Projects](#building-projects)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This repository contains a diverse range of C projects demonstrating:

- **Systems Programming**: Low-level OS interactions, kernel syscalls
- **Graphics Programming**: SDL, GTK3, and custom rendering
- **Network Programming**: UDP/TCP protocols
- **Algorithm Implementation**: Mathematical modeling, simulations
- **Compiler Theory**: Lambda calculus interpreter
- **Hardware Interfacing**: Keyboard, mouse, battery monitoring

Each project is self-contained with its own build system and documentation.

---

## 📂 Projects

### Graphics & Visualization

#### 📊 Function Plotter
**Description**: Mathematical function visualization tool with interactive plotting capabilities.

**Features**:
- Real-time function rendering using SDL2
- Mathematical expression parsing via TinyExpr library
- Support for arbitrary mathematical functions
- Configurable domain and range
- Interactive graph navigation

**Technologies**: SDL2, TinyExpr

**Use Case**: Visualizing mathematical functions, educational tool for calculus and algebra

---

#### 🎨 Paint
**Description**: Clone of the classic Microsoft Paint drawing application.

**Features**:
- Brush and pencil tools
- Color palette selection
- Shape drawing primitives (line, rectangle, circle)
- Canvas save/load functionality
- Undo/redo support

**Technologies**: SDL2 / GTK3 (depending on implementation)

**Use Case**: Simple raster graphics editor, educational graphics programming

---

#### ⌨️ Keyboard Tester
**Description**: Interactive keyboard testing application with visual feedback.

**Features**:
- Real-time key press detection
- Visual keyboard layout (ThinkPad T480 specific)
- Key response time measurement
- Dead key detection
- Customizable layouts

**Technologies**: SDL2 / GTK3, X11/Wayland

**Use Case**: Hardware testing, keyboard diagnostics, debugging input issues

---

#### 🖱️ Mouse Tracker
**Description**: Mouse and touchpad testing utility with precision metrics.

**Features**:
- Real-time cursor position tracking
- Click detection and visualization
- Movement speed analysis
- Touchpad gesture recognition
- Accuracy testing mode

**Technologies**: SDL2, X11/Wayland input APIs

**Use Case**: Input device testing, touchpad calibration, gaming hardware validation

---

### System Utilities

#### 📁 File Splitter
**Description**: Command-line utility for splitting and joining large files.

**Features**:
- Split files into N equal parts
- Rejoin split files into original
- Checksum verification
- Progress indication
- Configurable chunk sizes

**Technologies**: Standard C libraries, POSIX I/O

**Use Case**: Large file transfer, backup management, storage optimization

---

#### ⏱️ Timer
**Description**: Precision countdown timer using kernel-level syscalls.

**Features**:
- Microsecond-precision timing
- Signal-based implementation using `setitimer()`
- Custom signal handlers with `sigaction()`
- Non-blocking operation
- Multiple timer support

**Technologies**: POSIX syscalls, signals

**Use Case**: Performance measurement, task scheduling, real-time applications

---

#### 💻 Terminal Own
**Description**: Lightweight terminal emulator based on suckless st.

**Features**:
- Minimal footprint
- VTE (Virtual Terminal Emulator) support
- GTK3 integration
- Customizable keybindings
- Tab support
- Configuration via header files

**Technologies**: GTK3, VTE, X11/Wayland

**Use Case**: Lightweight terminal alternative, embedded systems, minimal desktop environments

---

#### 🔋 Battery Notifications
**Description**: System tray daemon for battery status monitoring.

**Features**:
- Real-time battery level monitoring
- Configurable notification thresholds
- Low battery warnings
- Charging status detection
- System tray integration

**Technologies**: D-Bus, libnotify, UPower

**Use Case**: Laptop power management, preventing data loss from unexpected shutdowns

---

#### 💾 Benchmark Disk
**Description**: Storage device performance testing utility.

**Features**:
- Sequential read/write benchmarking
- Random I/O performance testing
- IOPS (I/O Operations Per Second) measurement
- Latency analysis
- Multiple block size testing
- Results export to CSV/JSON

**Technologies**: POSIX I/O, direct I/O, `fopen()`, `fread()`, `fwrite()`

**Use Case**: SSD/HDD performance validation, storage comparison, system diagnostics

---

### Simulations

#### 🌊 Spring On Mass Simulation
**Description**: Physics simulation of a spring-mass-damper system.

**Features**:
- Real-time spring dynamics visualization
- Friction/damping coefficient adjustment
- Mass and spring constant configuration
- Energy conservation analysis
- Export simulation data

**Mathematical Model**:
```
F = -kx - cv
m(d²x/dt²) = -kx - c(dx/dt)
```

**Technologies**: SDL2, numerical integration (Euler, RK4)

**Use Case**: Physics education, mechanical system modeling, algorithm testing

---

#### ⚛️ Particles Collision
**Description**: N-body particle collision simulation with physics engine.

**Features**:
- Elastic collision simulation
- Mass-based momentum transfer
- Real-time collision detection (quadtree/spatial hashing)
- Configurable particle properties
- Gravity and electromagnetic forces

**Technologies**: SDL2, computational geometry

**Use Case**: Physics simulation, game engine development, algorithm visualization

---

#### 🍃 Leaf Venation
**Description**: Procedural leaf vein pattern generation using space colonization algorithm.

**Implementation Based On**:
> [Modeling and visualization of leaf venation patterns (Runions et al.)](https://algorithmicbotany.org/papers/venation.sig2005.pdf)

**Features**:
- Biologically-inspired pattern generation
- Space colonization algorithm implementation
- Adjustable attraction/kill distances
- Multiple leaf shape templates
- Export to SVG/PNG

**Technologies**: SDL2, computational biology algorithms

**Use Case**: Procedural generation, botanical modeling, computer graphics research

---

### Network & Communication

#### 📡 UDP Message Sender
**Description**: Lightweight UDP packet transmission utility.

**Features**:
- Send/receive UDP datagrams
- Broadcast and multicast support
- Packet fragmentation handling
- Network statistics
- Raw socket mode

**Technologies**: POSIX sockets, `sendto()`, `recvfrom()`

**Use Case**: Network testing, IoT communication, game networking, service discovery

---

### Language Implementation

#### 🔬 Lamb Language
**Description**: Minimal functional programming language based on pure lambda calculus.

**Features**:
- Pure lambda calculus interpreter
- Church encoding for data structures
- Lexer and parser implementation
- REPL (Read-Eval-Print Loop)
- Reduction strategies (normal order, applicative order)
- De Bruijn index notation support

**Grammar**:
```
expr := var | λvar.expr | (expr expr)
```

**Example Programs**:
```lambda
# Identity function
λx.x

# Church numerals
ZERO  = λf.λx.x
ONE   = λf.λx.f x
TWO   = λf.λx.f (f x)

# Boolean operations
TRUE  = λx.λy.x
FALSE = λx.λy.y
AND   = λp.λq.p q p
```

**Technologies**: Recursive descent parsing, AST evaluation

**Use Case**: Programming language theory, compiler education, functional programming

---

## 🛠️ Technologies Used

### Graphics Libraries
- **SDL2**: Cross-platform graphics and input
- **GTK3**: GUI toolkit for Linux applications
- **VTE**: Virtual Terminal Emulator library

### System Programming
- **POSIX API**: File I/O, processes, signals
- **Linux syscalls**: Low-level kernel interaction
- **D-Bus**: Inter-process communication
- **UPower**: Power management interface

### Networking
- **Berkeley sockets**: TCP/UDP networking
- **Raw sockets**: Low-level packet manipulation

### Mathematical Libraries
- **TinyExpr**: Mathematical expression parsing and evaluation

---

## 📊 Project Statistics

| Category | Count | Primary Technologies |
|----------|-------|---------------------|
| Graphics & Visualization | 4 | SDL2, GTK3 |
| System Utilities | 4 | POSIX, syscalls |
| Simulations | 3 | SDL2, numerical methods |
| Network | 1 | BSD sockets |
| Language Implementation | 1 | Parser, interpreter |
| **Total** | **13** | **C99/C11** |

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

### Adding New Projects
1. Create a new directory with descriptive name
2. Include a project-specific README
3. Add Makefile with standard targets (all, clean, run)
4. Update this main README with project description

### Improving Existing Projects
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request with detailed description

### Code Standards
- Follow K&R or Linux kernel coding style
- Comment complex algorithms
- Use meaningful variable names
- Include error handling
- Write portable code (avoid compiler-specific extensions)

---

## 📝 License

Each project may have its own license. Check individual project directories for specific licensing information.

Unless otherwise specified, projects are available under the MIT License.

---

## 🎓 Learning Objectives

This repository demonstrates proficiency in:

- ✅ Low-level memory management
- ✅ Systems programming and POSIX APIs
- ✅ Graphics programming and rendering
- ✅ Network protocol implementation
- ✅ Algorithm design and optimization
- ✅ Physics and mathematical modeling
- ✅ Compiler/interpreter design
- ✅ Cross-platform development
- ✅ Hardware interfacing
- ✅ Real-time systems

---

## 📚 Resources

### Books
- *The C Programming Language* by Kernighan & Ritchie
- *Advanced Programming in the UNIX Environment* by Stevens & Rago
- *Computer Graphics: Principles and Practice* by Foley et al.

### Documentation
- [SDL2 Wiki](https://wiki.libsdl.org/)
- [GTK Documentation](https://docs.gtk.org/)
- [POSIX Specification](https://pubs.opengroup.org/onlinepubs/9699919799/)

### Papers
- [Space Colonization Algorithm - Runions et al.](https://algorithmicbotany.org/papers/venation.sig2005.pdf)

---

## 🔗 Related Projects

- [suckless.org](https://suckless.org/) - Philosophy of simple, minimal software
- [TinyExpr](https://github.com/codeplea/tinyexpr) - Math expression parser
- [SDL2](https://www.libsdl.org/) - Simple DirectMedia Layer

---

## 👤 Author

Maintained as a personal portfolio of C programming projects.

**Focus Areas**: Systems programming, graphics, simulations, algorithms

---

## ⭐ Showcase Projects

Highlighted projects demonstrating advanced concepts:

1. **Lamb Language** - Complete interpreter implementation
2. **Leaf Venation** - Research paper algorithm implementation  
3. **Particles Collision** - Real-time physics engine
4. **Terminal Own** - Complex systems integration (GTK + VTE)

---

**Last Updated**: January 2026
