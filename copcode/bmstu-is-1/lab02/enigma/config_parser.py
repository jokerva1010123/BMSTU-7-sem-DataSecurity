def readRotor(line):
    order, position = line.split()
    order = list(map(int, order.split(',')))
    position = int(position)

    return order, position


def readReflector(line):
    return list(map(int, line.split()))


def LoadConfig(filename):
    rotorsConfig = []
    reflectorConfig = {}

    curReadFunc = None
    with open(filename, "r") as file:
        for i, line in enumerate(file):
            if line == "rotor\n":
                curReadFunc = readRotor

            elif line == "reflector\n":
                curReadFunc = readReflector

            elif curReadFunc is not None:
                result = curReadFunc(line)
                if curReadFunc == readRotor:
                    rotorsConfig.append(result)
                else:
                    reflectorConfig[result[0]] = result[1]
                    reflectorConfig[result[1]] = result[0]

            else:
                print("Неверный формат настроек")

    return rotorsConfig, reflectorConfig


def SaveConfig(config, filename):
    rotorsConfig = config[0]
    reflectorConfig = config[1]

    stringRotorsConf = []

    for rConf in rotorsConfig:
        order = ""
        for i, num in enumerate(rConf[0]):
            order += (',' if i else '') + str(num)

        position = rConf[1]
        stringRotorsConf.append((order, position))


    stringReflectorConf = ""
    keys = []
    for i, key in enumerate(reflectorConfig):
        if reflectorConfig[key] not in keys:
            keyChar, valueChar = key, reflectorConfig[key]

            stringReflectorConf += str(keyChar) + " " + str(valueChar) + '\n'

            keys.append(key)

    with open(filename, "w") as file:
        for conf in stringRotorsConf:
            file.write("rotor\n")
            file.write(conf[0])
            file.write(" ")
            file.write(str(conf[1]))
            file.write("\n")

        file.write("reflector\n")
        file.write(stringReflectorConf)
