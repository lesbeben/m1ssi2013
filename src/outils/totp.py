import hashlib
import hmac
import time

class Totp_Generator():
    def __init__(self, secret):
        self.__secret = secret.encode()

    def get_otp(self):
        # Obtention du compteur
        count = int(time.mktime(time.gmtime()) / self.quantum)
        # Création d'un objet permettant de calculer hmac_sha1
        hmac_gen = hmac.new(self.__secret, count.to_bytes(8, byteorder='big'), hashlib.sha1)
        # HMAC_SHA1(secret, count)
        full_otp = hmac_gen.digest()
        # Calcul du décalage pour obtenir l'otp
        offset = full_otp[19] & 0x0F 
        # Extraction des octets pour réaliser un hotp
        sliced_otp = full_otp[offset:offset + 4]
        # Conversion de l'otp en entier
        otp = int.from_bytes(sliced_otp, byteorder='big') & 0x7FFFFFFF
        return otp % 10 ** self.__length

    @property
    def quantum(self):
        return self.__quantum
    
    @quantum.setter
    def quantum(self, value):
        assert value >= 0
        self.__quantum = value

    @property
    def length(self):
        return self.__length

    @length.setter
    def length(self, value):
        assert value >= 6
        assert value <= 8
        self.__length = value

    @property
    def secret(self):
        return self.__secret.decode()

    @secret.setter
    def secret(self, value):
        self.__secret = value.encode()

if __name__ == "__main__":
    gen = Hotp_Generator("12345678901234567890")
    gen.length = 6
    gen.quantum = 30
    print(gen.get_otp())
