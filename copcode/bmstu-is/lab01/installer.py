import os
import shutil
import subprocess
try:
    import PyInstaller.__main__
except ImportError:
    os.system('python3 -m pip install pyinstaller')

import PyInstaller.__main__

file_template = """
import os
import levenstein as lev


def get_key():
    output = os.popen("cat /var/lib/dbus/machine-id").read()[:-1]
    return output


def main():
    if get_key() == "{0}":
        lev.start()
    else:
        print("Доступ запрещен!")

if __name__ == "__main__":
    main()
"""


def get_key():
    output = os.popen("cat /var/lib/dbus/machine-id").read()[:-1]
    return output


def createStartFile():
    with open("start.py", "w") as file:
        file.write(file_template.format(get_key()))


def install():
    PyInstaller.__main__.run(['--onefile', '--clean', 'start.py'])


def move():
    shutil.move("./dist/start", "./start")


def clean():
    os.remove("./start.py")
    shutil.rmtree("./build")
    shutil.rmtree("./dist")
    os.remove("./start.spec")


def main():
    createStartFile()
    install()
    move()
    clean()


if __name__ == "__main__":
    main()
