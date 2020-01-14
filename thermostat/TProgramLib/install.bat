@echo off
echo -----------------------------------------   Generating keywords

rem see https://github.com/r89m/arduino-keywords

python d:\Programs\Python37-32\Scripts\arduino-keywords

echo -----------------------------------------   Installing lib...
copy *.* D:\Programs\Arduino\libraries\TProgramLib