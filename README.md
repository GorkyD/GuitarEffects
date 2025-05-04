# GuitarEffects

### Description
GuitarEffects is a digital audio effects processor for guitar, built with C++ and Qt. The application provides a user-friendly interface for managing and applying various audio effects to guitar input in real-time.

### Features
- Real-time audio processing
- Multiple effects management through a centralized system
- Modern Qt-based user interface

### Requirements
- C++17 or higher
- Qt 6.x
- CMake 3.16 or higher
- Audio interface with ASIO support

### Building the Project
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Usage
1. Connect your guitar to your audio interface
2. Launch the application
3. Select desired effects from the interface
4. Adjust effect parameters in real-time

### Project Structure
- `src/` - Source code
  - `effects/` - Effect implementations
  - `ui/` - Qt interface components
  - `core/` - Core audio processing logic