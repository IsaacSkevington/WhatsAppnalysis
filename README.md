# WhatsAppnalysis
This app allows you to analyse WhatsApp messages from a chat

# Getting the program
Install the program using either the msi file or executable installer located in [Install WhatsAppnalysis/Release](https://github.com/IsaacSkevington/WhatsAppnalysis/tree/main/Install%20WhatsAppnalysis/Release). On first open, the program will need to change write permissions in its working directory to allow it to run, so needs to be started twice.

# Compiling from source
Download the repository and open WhatsAppnalysis.sln, then run the build.  
NOTE: [boost libraries](https://www.boost.org/) are needed to compile the source code.   
In the WhatsAppnalysis project settings:  
  - The top level directory must be added to 'Additional Includes'
  - The stage/lib directory needs to be added to the linker 'Additional Library Directories'  

Visual Studio Microsoft Installer extension is also required to open and compile the installer project.
