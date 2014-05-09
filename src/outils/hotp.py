import hashlib
import hmac

class Hotp_Generator():
    def __init__(self, secret):
        self.__secret = bytes.fromhex(secret)

    def get_otp(self):
        # Création d'un objet permettant de calculer hmac_sha1
        hmac_gen = hmac.new(self.__secret, self.__count.to_bytes(8, byteorder='big'), hashlib.sha1)
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
    def count(self):
        return self.__count

    @count.setter
    def count(self, value):
        assert value >= 0
        self.__count = value

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
        self.__secret = bytes.fromhex(value)

if __name__ == "__main__":
    gen = Hotp_Generator("12345678901234567890")
    print(gen.get_otp(1))
