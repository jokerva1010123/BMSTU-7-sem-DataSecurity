import os

def compareFilesPerByte(filePath1: str, filePath2: str):
    try:
        file1 = open(filePath1, "rb")
    except FileNotFoundError:
        print("Ошибка: файла \'", filePath1,"\' не существует", sep="")
        return

    try:
        file2 = open(filePath2, "rb")
    except FileNotFoundError:
        print("Ошибка: файла \'", filePath2,"\' не существует", sep="")
        return

    ind = 0

    fileSize = file1.seek(0, os.SEEK_END) # Получить размер файла в байтах
    file1.seek(0, 0) # Установить указатель файла на начало

    while True:
        b1 = file1.read(1)
        b2 = file2.read(1)

        if (not b1) and (not b2):
            print("\nКонец обоих файлов. Файлы побайтово идентичны")
            break
        elif (not b1):
            print("\nКонец Файла 1. Имеются лишние символы, начиная с: \
                \n[,", ind,"] = ", b2)
            break
        elif (not b2):
            print("\nКонец Файла 2. Имеются лишние символы, начиная с: \
                \n[,", ind,"] = ", b1)
            break

        if (b1 != b2):
            print("\nСимволы не совпали! [", ind, "] b1 = ", b1, "; b2 = ", b2)
            break

        ind += 1

        if ((ind % 500) == 0):
            print("Проверено:", ind, "символов из", fileSize)

    file1.close()
    file2.close()


def main():
    print("""
        Великий Сравнитель Файлов

    Программа побайтово сравнивает два файла и выводит:
    1. 'Конец обоих файлов. Файлы побайтово идентичны' -- если файлы одинаковы побайтово
    2. 'Конец Файла X. Имеются лишние символы, начиная с: [номер] = символ' -- если файл содержит лишние символы
    3. 'Символы не совпали! [номер] b1 = символ1; b2 = символ2' -- если символы не совпали
    """)

    file1 = input("Введите путь до первого файла: ")
    file2 = input("Введите путь до второго файла: ")
    print("=" * 70)

    compareFilesPerByte(file1, file2)

    print("=" * 70)


if __name__ == "__main__":
    main()
