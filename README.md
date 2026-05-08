# MediCore Hospital Management System

## Requirements

| Component | Minimum version |
|-----------|----------------|
| g++ | C++17 (`-std=c++17`) |
| SFML *(GUI mode only)* | 3.x |

---

## Compile & Run

### GUI mode (SFML required)

**Compile:**
```bash
g++ -std=c++17 -DHMS_ENABLE_SFML -Iinclude \
  src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Doctor.cpp \
  src/FileHandler.cpp src/FileNotFoundException.cpp src/HospitalException.cpp \
  src/InsufficientFundsException.cpp src/InvalidInputException.cpp \
  src/Patient.cpp src/Person.cpp src/Prescription.cpp \
  src/SlotUnavailableException.cpp src/Validator.cpp \
  src/HospitalSystem.cpp src/UITheme.cpp src/UIWidgets.cpp \
  src/LoginUI.cpp src/PatientUI.cpp src/DoctorUI.cpp src/AdminUI.cpp \
  src/SFMLUI.cpp src/main.cpp \
  -lsfml-graphics -lsfml-window -lsfml-system \
  -o hospital_sfml.exe
```

**Run:**
```bash
./hospital_sfml.exe
```

Opens a **900 × 600** interactive window. Navigate with mouse clicks or keyboard shortcuts (`1`–`4` on the main menu, `Tab` between fields, `Enter` to confirm, `Esc` to go back).

---

### Console mode (no SFML needed)

**Compile:**
```bash
g++ -std=c++17 -Iinclude \
  src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Doctor.cpp \
  src/FileHandler.cpp src/FileNotFoundException.cpp src/HospitalException.cpp \
  src/InsufficientFundsException.cpp src/InvalidInputException.cpp \
  src/Patient.cpp src/Person.cpp src/Prescription.cpp \
  src/SlotUnavailableException.cpp src/Validator.cpp \
  src/HospitalSystem.cpp src/UITheme.cpp src/UIWidgets.cpp \
  src/LoginUI.cpp src/PatientUI.cpp src/DoctorUI.cpp src/AdminUI.cpp \
  src/SFMLUI.cpp src/main.cpp \
  -o hospital_console.exe
```

**Run:**
```bash
./hospital_console.exe
```

All core functionality runs through the terminal. No SFML installation required — simply omit the `-DHMS_ENABLE_SFML` flag and SFML linker flags.
