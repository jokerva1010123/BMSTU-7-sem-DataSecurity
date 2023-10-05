def fast_pow(base: int, power: int, divider: int) -> int:
    result = 1

    while power != 0:
        if power % 2:
            result = result * base % divider
        
        power //= 2
        base = base * base % divider

    return result


def encipher_data(data: list, public_key: dict) -> list:
    cipher_data = []

    for number in data:
        cipher_data.append(fast_pow(number, public_key['key'],
                                    public_key['module']))

    return cipher_data


def decipher_data(data: list, private_key: dict) -> list:
    plain_data = []

    for number in data:
        plain_data.append(fast_pow(number, private_key['key'],
                                   private_key['module']))

    return plain_data
