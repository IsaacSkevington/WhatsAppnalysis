echo "Commencing permission change operation\n\n"
ICACLS "%~dp0\Files" /inheritance:e
ICACLS "%~dp0\Files" /GRANT *S-1-1-0:F /T
ICACLS "%~dp0\Setup.exe" /GRANT *S-1-1-0:F /T
echo "Permissions changing complete"
@echo "0" > "%~dp0\Files\Data\FirstTime.txt"
TIMEOUT 5