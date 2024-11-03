import os
import platform

DEBUG_FILE = os.path.abspath('./temp/debug.txt')
CLEAR_SCREEN = 'clear'
if (platform.system() == 'Windows'):
    DEBUG_FILE = 'temp\\debug.txt'
    CLEAR_SCREEN = 'cls'

running = True

def stop():
    global running
    running = False

text = ''
base_text = ''

if __name__ == '__main__':
    while running:
        if text != base_text:
            os.system(CLEAR_SCREEN)
            print(text)
            base_text = text

        if os.path.isfile(DEBUG_FILE):
            with open(DEBUG_FILE, 'r') as f:
                text = f.read()