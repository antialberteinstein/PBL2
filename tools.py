import platform
import os
from colorama import Fore, Style
import debug
import threading

WINDOWS = 'Windows'
MACOS = 'Darwin'
LINUX = 'Linux'

# Default for Linux
OS = LINUX
RMDIR = 'rm -rf'
MKDIR = 'mkdir -p'
CPDIR = 'cp -r'
CPFILE = 'cp'
RM = 'rm -f'
MV = 'mv'
CMAKE_RESET = 'cmake ..'
CMAKE_BUILD = 'cmake --build .'
EXEC_PATH = './build/'
EXEC_FILE = 'pbl'
CLEAR_SCREEN = 'clear'
WINDOWS_SQLITE3_DLL_FILE = ''

running = True

if platform.system() == 'Windows':
    OS = WINDOWS
    RMDIR = 'rmdir /s /q'
    MKDIR = 'mkdir'
    CPDIR = 'xcopy /s /e /i'
    CPFILE = 'copy'
    RM = 'del'
    MV = 'move'
    EXEC_PATH = '.\\build\\Debug\\'
    CLEAR_SCREEN = 'cls'
    WINDOWS_SQLITE3_DLL_FILE = 'Libraries\\sqlite3\\sqlite3.dll'
elif platform.system() == 'Darwin':
    OS = MACOS
    # Same as Linux


def reset_project():
    print(Fore.CYAN + 'Resetting project...' + Style.RESET_ALL)
    os.system(f'{RMDIR} build')
    os.system(f'{MKDIR} build')
    os.chdir('build')
    MSG = Fore.GREEN + 'Done.'
    if os.system(f'{CMAKE_RESET}'):
        MSG = Fore.RED + 'Failed to reset project.'
    os.chdir('..')
    print(MSG + Style.RESET_ALL)

def build_project():
    print(Fore.CYAN + 'Building project...' + Style.RESET_ALL)
    os.chdir('build')
    MSG = Fore.GREEN + 'Done.'
    if os.system(f'{CMAKE_BUILD}'):
        MSG = Fore.RED + 'Failed to build project.'
    os.chdir('..')
    print(MSG + Style.RESET_ALL)

def run_project():
    print(Fore.CYAN + 'Running project...' + Style.RESET_ALL)
    if (OS == WINDOWS):
        START_CMD = 'start'
    else:
        START_CMD = './start'
        os.system(f'chmod +x {EXEC_PATH}{EXEC_FILE}')


    MSG = Fore.GREEN + 'Done.'
    if (os.system(f'{START_CMD} {EXEC_PATH}{EXEC_FILE}')):
        MSG = Fore.RED + 'Failed to run project.'
    

    print(MSG + Style.RESET_ALL)

def enter_to_continue():
    input('Press Enter to continue...')

def exit_program():
    global running
    running = False

def copy_nessesary_files():
    os.system(f'{CPDIR} res {EXEC_PATH}/res')
    if (OS == WINDOWS):
        # In Windows, we need to copy sqlite3.dll to build folder
        os.system(f'{CPFILE} {WINDOWS_SQLITE3_DLL_FILE} {EXEC_PATH}')



if __name__ == '__main__':
    while running:
        os.system(CLEAR_SCREEN)
        print('Chương trình quản lý project.')
        print('Version 1.0.0')
        print('Author: Eins\n')

        print('1. Reset project.')
        print('2. Biên dịch chương trình.')
        print('3. Chạy chương trình.')
        print('4. Thoát.')

        option = int(input('Chọn: '))

        match option:
            case 1:
                reset_project()
            case 2:
                build_project()
                if (OS == WINDOWS):
                    # In Windows, we need to do it twice
                    #   there is a bug "pwsh.exe" not found in the first time.
                    build_project()
            case 3:
                run_project()
            case 4:
                exit_program()
            case _:
                print('Chức năng không tồn tại.')

        enter_to_continue()
