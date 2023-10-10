from Cryptodome.Hash import SHA256
from Cryptodome.PublicKey import RSA
from Cryptodome import Random
from Cryptodome.Signature import pkcs1_15

def main():
	action = input("Chose action (sign -- s, check -- c): ")
	if (action != "s" and action != "c"):
		print("Stop")
		return

	if (action == "s"):
		filename = input("Input filename: ")
		random_generator = Random.new().read
		key = RSA.generate(2048, random_generator)
		try:
			_hash = SHA256.new()
			with open(filename, "rb") as sign_file:
				for chunk in iter(lambda: sign_file.read(4096), b""):
					_hash.update(chunk)
		except FileNotFoundError:
			print("Cannot open file")
			exit(0)
		signature = pkcs1_15.new(key).sign(_hash)

		with open(filename + '.key.pem', "wb") as pkey:
			pkey.write(key.publickey().export_key())

		with open(filename + '.signature', 'wb') as signed_file:
			signed_file.write(signature)
		print("Digital sign was successfully created")

	elif (action == "c"):
		key_file = input("Input key filename: ")
		sign_file = input("Input signature filename: ")
		check_file = input("Input filename to check: ")
		try:
			with open(key_file, "rb") as pkey:
				key = RSA.import_key(pkey.read())
			print("Read key from " + key_file + ".key.pem")
		except FileNotFoundError:
			print("Key file not found")
			exit(0)

		try:
			with open(sign_file, "rb") as signed_file:
				signature = signed_file.read()
			print("Read signature")
		except FileNotFoundError:
			print("Signature file not found")
			exit(0)
		
		print("Hashing " + check_file + "...")
		try:
			# Получаем хэш файла
			_hash = SHA256.new()
			with open(check_file, "rb") as checked_file:
				for chunk in iter(lambda: checked_file.read(4096), b""):
					_hash.update(chunk)
		except(FileNotFoundError):
			print("Check file not found")
			exit(0)
		
		try:       
			pkcs1_15.new(key.publickey()).verify(_hash, signature)
			print("Signature CONFIRMED. ")
		except(ValueError):
			print("Signature NOT CONFIRMED")
	else:
		print("Unknown action")		

if __name__ == "__main__":
	main()
