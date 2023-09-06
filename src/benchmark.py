import subprocess
import time

import pyautogui


def main():
    # Start the executable
    proc = subprocess.Popen(
        ["C:\\Users\\gk646\\Documents\\CodingProjects\\C++\\MageQuest\\cmake-build-release\\MageQuest.exe"])

    # Wait for 5 seconds for the game to start
    time.sleep(2)

    # Get screen dimensions
    screen_width, screen_height = pyautogui.size()

    # Press mouse 1 in the middle of the screen
    #pyautogui.click(x=screen_width / 2, y=420, button='left', clicks=10, interval=0.1)

    # Hold down some keys (for example, 'w', 'a', 's', 'd')
    pyautogui.keyDown('d')
    time.sleep(3)
    pyautogui.keyUp('d')

    pyautogui.keyDown('s')
    time.sleep(3)
    pyautogui.keyUp('s')

    pyautogui.keyDown('h')
    pyautogui.keyDown('2')
    time.sleep(10)

    pyautogui.keyUp('h')
    pyautogui.keyUp('2')
    # Finish
    print("Benchmark completed.")

    proc.terminate()


if __name__ == '__main__':
    main()
