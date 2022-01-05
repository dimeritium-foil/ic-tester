import curses as nc
from curses import wrapper
import serial
import serial.tools.list_ports
import platform

def main(stdscr):
    
    stdscr.clear()
    stdscr.border()
    stdscr.nodelay(True)
    nc.curs_set(0)

    # show logo and get the last y coordinate for the cursor
    logo_y = show_logo(stdscr)
    stdscr.refresh()

    message_window_width = int((nc.COLS - 1) / 1.5)
    message_window_height = 6

    message_window_y = logo_y + 4
    message_window_x = int((nc.COLS - 1 - message_window_width) / 2)

    message_window = nc.newwin(message_window_height, message_window_width, message_window_y, message_window_x)
    message_window.border()

    message_window.addstr(2, 3, "Welcome to the ultimate IC tester!")
    message_window.addstr(3, 3, "Place your IC and press the button to start testing.")
    message_window.refresh()

    # color pair for a successful match
    nc.init_pair(1, nc.COLOR_GREEN, nc.COLOR_BLACK)

    ports = serial.tools.list_ports.comports()
    serial_port = ""

    # hacky way of choosing the UART to USB interface
    if system == "Linux" or system == "Darwin":
        for port in ports:
            if "USB" in port.device:
                serial_port = port.device
    elif system == "Windows":
        for port in ports:
            if "USB" in port.description:
                serial_port = port.device

    if serial_port == "":
        message_window.erase()
        message_window.border()
        message_window.addstr(2, 3, "The IC tester is not connected.")
        message_window.refresh()
        
        stdscr.nodelay(False)
        stdscr.refresh()
        stdscr.getkey()
        exit()

    avr = serial.Serial(port=serial_port, baudrate=9600, timeout=None)

    while True:

        # get the output from the microcontroller and decode it
        test_result = avr.readline().decode("UTF-8")[0:-1]
        test_result = "".join(char for char in test_result if char.isprintable())

        message_window.erase()
        message_window.border()

        if test_result == "null":
            message_window.addstr(2, 3, "No match found.")
        else:
            message_window.addstr(2, 3, "Match found!")
            message_window.addstr(3, 3, test_result, nc.color_pair(1))

        message_window.refresh()

def show_logo(window):

    logo = \
    """
            ██      ██      ██      ██      ██      ██      ██      ██      ██      ██       
            ██      ██      ██      ██      ██      ██      ██      ██      ██      ██       
    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
    ▓████████████████████████████████████████████████████████████████████████████████████████▓
    ▓████████████████████████████████████████████████████████████████████████████████████████▓
    ▓████████  ███      ████          █        ██        █          █        █       ████████▓
    ░░███████  ██  ████  ███████  █████  ███████  ████████████  █████  ███████  ████  ███████▓
    ░░░██████  █  ██████████████  █████  ███████  ████████████  █████  ███████  ████  ███████▓
    ░░░██████  █  ██████████████  █████       ██         █████  █████       ██       ████████▓
    ░░░██████  █  ██████████████  █████  ██████████████  █████  █████  ███████  ███  ████████▓
    ░░███████  ██  ████  ███████  █████  ██████████████  █████  █████  ███████  ████  ███████▓
    ▓████████  ███      ████████  █████        █        ██████  █████        █  █████  ██████▓
    ▓████████████████████████████████████████████████████████████████████████████████████████▓
    ▓████████████████████████████████████████████████████████████████████████████████████████▓
    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
            ██      ██      ██      ██      ██      ██      ██      ██      ██      ██       
            ██      ██      ██      ██      ██      ██      ██      ██      ██      ██       
    """

    logo = logo.splitlines()
    logo_width = max([len(line) for line in logo])

    logo_y = int(nc.LINES / 8)
    logo_x = int((nc.COLS - 1 - logo_width) / 2)

    for row in range(len(logo)):
        window.addstr(logo_y + row, logo_x, logo[row])

    return logo_y + len(logo)

if __name__ == "__main__":
    
    system = platform.system()

    # maximize cmd window on Windows else it throws an error
    if system == "Windows":
        import ctypes

        kernel32 = ctypes.WinDLL('kernel32')
        user32 = ctypes.WinDLL('user32')

        SW_MAXIMIZE = 3

        hWnd = kernel32.GetConsoleWindow()
        user32.ShowWindow(hWnd, SW_MAXIMIZE)
        
    wrapper(main)
