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

### GUI mode (SFML window + Audio)

**Compile:**
```powershell
g++ -std=c++17 -DHMS_ENABLE_SFML -Iinclude src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Doctor.cpp src/FileHandler.cpp src/FileNotFoundException.cpp src/HospitalException.cpp src/InsufficientFundsException.cpp src/InvalidInputException.cpp src/Patient.cpp src/Person.cpp src/Prescription.cpp src/SlotUnavailableException.cpp src/Validator.cpp src/HospitalSystem.cpp src/UITheme.cpp src/UIWidgets.cpp src/LoginUI.cpp src/PatientUI.cpp src/DoctorUI.cpp src/AdminUI.cpp src/AudioManager.cpp src/SFMLUI.cpp src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o hospital_sfml.exe
```

**Run:**
```powershell
./hospital_sfml.exe
```

Opens a **900 × 600** resizable interactive window. Press **F11** to toggle fullscreen.

| Control | Action |
|---------|--------|
| Mouse click | Navigate buttons and fields |
| `1` – `4` | Role selection on main menu |
| `Tab` | Switch between input fields |
| `Enter` | Confirm / submit |
| `Esc` | Go back / close overlay |
| `F11` | Toggle fullscreen |
| `* Settings` button (top-right) | Open audio settings |

---

### Console mode

**Compile:**
```powershell
g++ -std=c++17 -Iinclude src/Admin.cpp src/Appointment.cpp src/Bill.cpp src/Doctor.cpp src/FileHandler.cpp src/FileNotFoundException.cpp src/HospitalException.cpp src/InsufficientFundsException.cpp src/InvalidInputException.cpp src/Patient.cpp src/Person.cpp src/Prescription.cpp src/SlotUnavailableException.cpp src/Validator.cpp src/HospitalSystem.cpp src/UITheme.cpp src/UIWidgets.cpp src/LoginUI.cpp src/PatientUI.cpp src/DoctorUI.cpp src/AdminUI.cpp src/AudioManager.cpp src/SFMLUI.cpp src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o hospital_console.exe
```

**Run:**
```powershell
./hospital_console.exe
```

All core functionality runs through the terminal. The `-DHMS_ENABLE_SFML` flag is omitted to disable the GUI. `-lsfml-audio` is not required for the console build.

---

## Assets

The following asset folders must be present in the project root for the GUI build:

```
assets/
  fonts/
    Cinzel/
      Cinzel-VariableFont_wght.ttf     ← display font (titles, buttons)
    Raleway/
      Raleway-VariableFont_wght.ttf    ← body font (labels, tables)
  sounds/
    music1.mp3    ← background track 1
    music2.mp3    ← background track 2
    click1.mp3    ← button click sound 1
    click2.mp3    ← button click sound 2
    error1.mp3    ← error sound 1
    error2.mp3    ← error sound 2
```

If fonts are missing the app falls back to Windows system fonts automatically.

---

## Data Files

All data is stored as CSV in the `data/` folder. The folder must exist before running:

```
data/
  patients.txt
  doctors.txt
  admin.txt
  appointments.txt
  bills.txt
  prescriptions.txt
  security_log.txt
  discharged.txt
```

**Sample admin credentials:** ID `1`, Password `admin123`
