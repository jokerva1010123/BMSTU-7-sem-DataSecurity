startfileTemplate = '''
def program():
    print("Ответ на вопрос Вселенной: 42")

def getMachineId():
    return open('/var/lib/dbus/machine-id', 'r').read()[:-1]

def answerIs():
    if (getMachineId() == "{0}"):
        program()
    else:
        print("Ошибка: У Вас нет доступа")

answerIs()
'''

def getMachineId():
    return open('/var/lib/dbus/machine-id', 'r').read()[:-1]

def createStartfile(key: str):
    open('./answerIs.py', 'w').write(startfileTemplate.format(key))

def install():
    createStartfile(getMachineId())
    print("Установка выполнена успешно")

install()
