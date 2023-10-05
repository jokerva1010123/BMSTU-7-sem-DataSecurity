# Распечатать шкалу прогресса выполнения
def printProgressBar (iteration, total, prefix = '', suffix = '', 
        decimals = 2, length = 100, fill = '█', printEnd = "\r"):
    """
    Вызывается в цикле для создания шкалы выполнения
    @params:
        iteration   - Необоходимо : текущая итерация (Int)
        total       - Необоходимо : всего итераций (Int)
        prefix      - Опционально : текст перед шкалой (Str)
        suffix      - Опционально : текст после шкалы (Str)
        decimals    - Опционально : положительное количество знаков
                                    после запятой в процентах (Int)
        length      - Опционально : длина шкалы (Int)
        fill        - Опционально : символ заполнения шкалы (Str)
        printEnd    - Опционально : крайний символ (e.g. "\r", "\r\n") (Str)
    """

    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))

    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end = printEnd)

    # Пустая строка, когда закончилось
    if iteration == total: 
        print()
