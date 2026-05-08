# MediCore Hospital Management System

## Requirements

| Component | Minimum version |
|-----------|----------------|
| g++ | C++17 (`-std=c++17`) |
| SFML | 3.x |

> **Note:** Both GUI and console builds require SFML to be installed, as the UI source files include SFML headers unconditionally.

---

## Dependency Setup — Windows (MSYS2 MinGW-w64)

Open the **MSYS2 MinGW x64** shell and run:

```bash
pacman -S mingw-w64-x86_64-sfml
```

---

## Compile & Run

> **Important:** Run all commands from the project root directory and use the **VS Code terminal** or any terminal where `where.exe g++` returns `C:\msys64\mingw64\bin\g++.exe`.
> If it doesn't, prepend MSYS2 to your PATH first:
> ```powershell
> $env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
> ```

---

### GUI mode (SFML window)

**Compile:**
```powershell
g++ -std=c++17 -DHMS_ENABLE_SFML -Iinclude src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Doctor.cpp src/FileHandler.cpp src/FileNotFoundException.cpp src/HospitalException.cpp src/InsufficientFundsException.cpp src/InvalidInputException.cpp src/Patient.cpp src/Person.cpp src/Prescription.cpp src/SlotUnavailableException.cpp src/Validator.cpp src/HospitalSystem.cpp src/UITheme.cpp src/UIWidgets.cpp src/LoginUI.cpp src/PatientUI.cpp src/DoctorUI.cpp src/AdminUI.cpp src/SFMLUI.cpp src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o hospital_sfml.exe
```

**Run:**
```powershell
./hospital_sfml.exe
```

Opens a **900 × 600** interactive window. Navigate with mouse clicks or keyboard shortcuts (`1`–`4` on the main menu, `Tab` between fields, `Enter` to confirm, `Esc` to go back).

---

### Console mode

**Compile:**
```powershell
g++ -std=c++17 -Iinclude src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Doctor.cpp src/FileHandler.cpp src/FileNotFoundException.cpp src/HospitalException.cpp src/InsufficientFundsException.cpp src/InvalidInputException.cpp src/Patient.cpp src/Person.cpp src/Prescription.cpp src/SlotUnavailableException.cpp src/Validator.cpp src/HospitalSystem.cpp src/UITheme.cpp src/UIWidgets.cpp src/LoginUI.cpp src/PatientUI.cpp src/DoctorUI.cpp src/AdminUI.cpp src/SFMLUI.cpp src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o hospital_console.exe
```

**Run:**
```powershell
./hospital_console.exe
```

All core functionality runs through the terminal. The `-DHMS_ENABLE_SFML` flag is omitted to disable the GUI, but SFML linker flags are still required.
