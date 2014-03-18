#! /usr/bin/python

from PyQt5.QtWidgets import QWidget, QApplication, QVBoxLayout, QLabel
from PyQt5.QtWidgets import QHBoxLayout, QPushButton, QLineEdit, QComboBox
from PyQt5.QtWidgets import QInputDialog, QSpinBox


import hotp
import sys
import secretstorage

class QOtpToken(QWidget):
    def __init__(self):
        super(QOtpToken, self).__init__()
        self.set_view()
        self.generator = hotp.Hotp_Generator('')
        self.generator.count = self.counter.value()
        self.generator.length = self.length.value()
        self.connect_components()
    
    def set_view(self):
        self.setWindowTitle("QOtpToken")

        vbox = QVBoxLayout(self)
        hbox = QHBoxLayout()
        hbox.addWidget(QLabel("Secret: "))
        self.secret = QLineEdit()
        self.secret.setEchoMode(QLineEdit.Password)
        hbox.addWidget(self.secret)
        vbox.addLayout(hbox)

        hbox = QHBoxLayout()
        self.counter = QSpinBox()
        self.counter.setMinimum(0)
        self.length = QSpinBox()
        self.length.setValue(6)
        self.length.setMinimum(6)
        self.length.setMaximum(8)
        self.generate = QPushButton("Générer")
        hbox.addWidget(QLabel("Compteur: "))
        hbox.addWidget(self.counter)
        hbox.addWidget(QLabel("Taille: "))
        hbox.addWidget(self.length)
        hbox.addWidget(self.generate)
        vbox.addLayout(hbox)

        hbox = QHBoxLayout()
        hbox.addWidget(QLabel("Mot de passe jetable: "))
        self.display = QLineEdit("Test")
        self.display.setReadOnly(True)
        hbox.addWidget(self.display)
        vbox.addLayout(hbox)
        self.setLayout(vbox)
        pass

    def connect_components(self):
        self.generate.clicked.connect(self.generate_hotp_clicked)
        self.length.valueChanged.connect(self.update_length)
        self.counter.valueChanged.connect(self.update_count)
        pass

    def generate_hotp_clicked(self):
        if (self.secret.isModified()):
            self.secret.setModified(False)
            self.generator.secret = self.secret.text()
        otp = str(self.generator.get_otp())
        self.display.setText('0' * (self.length.value() - len(otp)) + otp)
        pass


    def update_length(self):
        self.generator.length = self.length.value()

    def update_count(self):
        self.generator.count = self.counter.value()

if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = QOtpToken()
    window.show()

    sys.exit(app.exec_())
