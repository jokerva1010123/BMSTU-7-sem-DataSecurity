import argparse
from Crypto import Random
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA3_256
from Crypto.Signature import pkcs1_15

def keygen():
    key_pair = RSA.generate(2048)
    return key_pair.public_key().export_key('PEM'), key_pair.export_key('PEM')

def sign(msg: bytes, prtkey: bytes) -> bytes:
    hash = SHA3_256.new(msg)
    signature = pkcs1_15.new(RSA.import_key(prtkey)).sign(hash)
    return signature

def read(filename: str) -> bytes:
    with open(filename, 'rb') as file:
        content = file.read()
    return content

def write(filename: str, content: bytes) -> None:
    with open(filename, 'wb') as file:
        file.write(content)

def check(msg: bytes, pubkey: bytes, signature: bytes) -> bool:
    hash = SHA3_256.new(msg)
    try:
        pkcs1_15.new(RSA.import_key(pubkey)).verify(hash, signature)
        return True
    except ValueError:
        return False
        
def parse_args():
    parser = argparse.ArgumentParser(prog='Digital Signature')
    subparsers = parser.add_subparsers(dest='mode', required=True, help='sub-command help')

    parser_keygen = subparsers.add_parser('keygen')
    parser_keygen.add_argument('pubkey_filename')
    parser_keygen.add_argument('prtkey_filename')

    parser_sign = subparsers.add_parser('sign')
    parser_sign.add_argument('input_filename')
    parser_sign.add_argument('prtkey_filename')
    parser_sign.add_argument('signature_filename')

    parser_check = subparsers.add_parser('check')
    parser_check.add_argument('input_filename')
    parser_check.add_argument('pubkey_filename')
    parser_check.add_argument('signature_filename')
    parser_check.add_argument('-q', '--quiet', action='store_true')

    return parser.parse_args()

def main():
    args = vars(parse_args())

    match args['mode']:
        case 'keygen':
            pubkey, prtkey = keygen()
            write(args['pubkey_filename'], pubkey)
            write(args['prtkey_filename'], prtkey)

        case 'sign':
            msg = read(args['input_filename'])
            prtkey = read(args['prtkey_filename'])
            signature = sign(msg, prtkey)
            write(args['signature_filename'], signature)

        case 'check':
            msg = read(args['input_filename'])
            pubkey = read(args['pubkey_filename'])
            signature = read(args['signature_filename'])

            if check(msg, pubkey, signature):
                if not args['quiet']:
                    print('Valid signature')
            else:
                if not args['quiet']:
                    print('Invalid signature')
                exit(1)

if __name__ == "__main__":
    main()
