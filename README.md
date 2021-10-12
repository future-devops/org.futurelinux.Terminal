# org.futurelinux.Terminal

  The terminal is the VTE virtual terminal emulator that is added on a window designed using GTK-3.0.

  It was designed to be very light and to meet the terminal needs without straining the system.

# Screenshot:
  
  ![Alt text](https://github.com/future-devops/org.futurelinux.Terminal/blob/main/terminal.png "Terminal Screenshot")

# Features:

  * Drag drop
  * Keyboard shortcuts
  * Quick action buttons (new, copy, paste)

# Keyboard Shortcuts:

  |  Combination      | Description                          |
  |  ---              | ---                                  |
  |  Ctrl + Shift + T | Open Terminal                        |
  |  Ctrl + Shift + C | Copy                                 |
  |  Ctrl + Shift + V | Paste                                |
  |  Ctrl + Shift + Q | Close Terminal                       |
  |  Ctrl + Shift + R | Complete From Past Commands (Search) |
  
# Requirements:  

  * gtk3-devel
  * vte291-devel

# Build:

  `git clone https://github.com/future-devops/org.futurelinux.Terminal.git`
  
  `cd org.futurelinux.Terminal/`
  
  `make`
  
  `sudo make install`

# Knowledge:

  Terminal is free software distributed under the MIT License.
