rem Compile the app to an EXE file
rem pyinstaller -F --icon=.\resources\Virtual_Cathode_Setup\blue_laser.ico -n "VC_Setup" mainApp.py

pyinstaller --onefile -n "llrf_daq_test" test.py
