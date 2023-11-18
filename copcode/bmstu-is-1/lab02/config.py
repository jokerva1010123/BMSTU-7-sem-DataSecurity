import string

alphabet = list(string.printable
        + "АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя"
        + "№")
alphabet.remove('\r')

bin_alphabet = bytes([x for x in range(256)])

if __name__ == "__main__":
    print(alphabet)
    print(bin_alphabet)
