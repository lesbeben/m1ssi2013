#! /usr/bin/python

from PyQt5.QtWidgets import QWidget, QApplication, QVBoxLayout, QLabel
from PyQt5.QtWidgets import QHBoxLayout, QPushButton, QLineEdit, QComboBox
from PyQt5.QtWidgets import QInputDialog, QSpinBox


import totp
import sys

class QOtpToken(QWidget):
    def __init__(self):
        super(QOtpToken, self).__init__()
        self.set_view()
        self.generator = totp.Totp_Generator('')
        self.generator.quantum = self.quantum.value()
        self.generator.length = self.length.value()
        self.connect_components()
    
    def set_view(self):
        self.setWindowTitle("QOtpToken")

        vbox = QVBoxLayout(self)
        hbox = QHBoxLayout()
        hbox.addWidget(QLabel("Secret: "))
        self.secret = QLineEdit()
        hbox.addWidget(self.secret)
        vbox.addLayout(hbox)

        hbox = QHBoxLayout()
        self.quantum = QSpinBox()
        self.quantum.setMinimum(1)
        self.length = QSpinBox()
        self.length.setValue(6)
        self.length.setMinimum(6)
        self.length.setMaximum(8)
        self.generate = QPushButton("Générer")
        hbox.addWidget(QLabel("Quantum: "))
        hbox.addWidget(self.quantum)
        hbox.addWidget(QLabel("Taille: "))
        hbox.addWidget(self.length)
        hbox.addWidget(self.generate)
        vbox.addLayout(hbox)

        hbox = QHBoxLayout()
        hbox.addWidget(QLabel("Mot de passe jetable: "))
        self.display = QLineEdit("")
        self.display.setReadOnly(True)
        hbox.addWidget(self.display)
        vbox.addLayout(hbox)
        self.setLayout(vbox)

    def connect_components(self):
        self.generate.clicked.connect(self.generate_totp_clicked)
        self.length.valueChanged.connect(self.update_length)
        self.quantum.valueChanged.connect(self.update_quantum)

    def generate_totp_clicked(self):
        if (self.secret.isModified()):
            self.secret.setModified(False)
            self.generator.secret = self.secret.text()
        otp = str(self.generator.get_otp())
        self.display.setText('0' * (self.length.value() - len(otp)) + otp)


    def update_length(self):
        self.generator.length = self.length.value()

    def update_quantum(self):
        self.generator.quantum = self.quantum.value()

if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = QOtpToken()
    window.show()

    sys.exit(app.exec_())
