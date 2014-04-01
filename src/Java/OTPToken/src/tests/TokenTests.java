package tests;

import org.junit.Test;

import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;
import com.java.utils.HOTP;
import com.java.utils.IOTP;
import com.java.utils.ISecret;
import com.java.utils.Secret;
import com.java.utils.TOTP;

import junit.framework.TestCase;

/**
 * Une classe de test pour la classe Token.
 * 
 * @author celtic
 */
public class TokenTests extends TestCase {
	
	/*
	 * Test du constructeur en condition normales.
	 */
	@Test
	public void testTokenStringMethodOTPGen() {
		String nom = "test"; //Le nome
		OTPMethodType methodType = OTPMethodType.HOTP; //La méthode
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF"); //Un secret pour le générateur d'otp
		IOTP otpGen = new HOTP(secret); //Le générateur
		Token token = new Token(nom, methodType, otpGen); //Le token a tester
		//Cohérence des nom donnés et stockés
		assertEquals("Name is incorect.", token.getNom(), "test");
		//Cohérence des méthodes donnés et stockés
		assertEquals(
				"OTP method is incorect.", 
				token.getMethodType(), 
				OTPMethodType.HOTP
		);
		//Cohérence des générateurs donnés et stockés
		assertEquals("Generator is incorect.", token.getIotpGen(), otpGen);
	}
	
	/*
	 * Test du constructeur dans des cas dégénérés.
	 */
	@SuppressWarnings("unused")
	@Test
	public void testTokenBad() {
		//Création des paramètres normaux
		int count = 0;
		OTPMethodType methodType = OTPMethodType.HOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		IOTP otpGen = new HOTP(secret);
		Token token;
		//Nom à null
		try {
			token = new Token(null, methodType, otpGen);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//methodType à null
		try {
			token = new Token("test", null, otpGen);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//otpGen à null
		try {
			token = new Token("test", methodType, null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//incoherence entre le type de méthode et le générateur
		try {
			token = new Token("test", OTPMethodType.TOTP, otpGen);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//On vérifie que les quatres cas se sont terminés avec des IAr
		assertEquals(
				"Bad cases not correctly handled" + (4 - count) + "out of 4", 
				4, count
		);
	}
	
	/*
	 * Test de la génération de mots de passe jetables pour HOTP
	 */
	@Test
	public void testGenerateHOTP() {
		int count = (int) (Math.random() * 1000); //Un compteur aléatoire.
		OTPMethodType methodType = OTPMethodType.HOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		/*
		 * Initialisation d'un premier générateur pour l'initialisation du token
		 */
		IOTP otpGen = new HOTP(count, secret, 6);
		/*
		 * Initialisation d'un deuxième générateur qui servira de témoin
		 */
		IOTP otpGen2 = new HOTP(count, secret, 6);
		//Création du token
		Token token = new Token("test", methodType, otpGen);
		//On test la cohérence témoin / token sur 100 valeurs successives.
		for (int i = 0; i < 100; i++) {	
			assertEquals(
					"Wrong value for" + i + "th OTP.", 
					otpGen2.generer(), 
					token.generate()
			);
		}
	}
	
	@Test
	public void testGenerateTOTP() {
		
		OTPMethodType methodType = OTPMethodType.TOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		//Initialisation d'un premier générateur pour l'initialisation du token.
		IOTP otpGen = new TOTP(secret, 6, 1);
		//Initialisation d'un second générateur témoin.
		IOTP otpGen2 = new TOTP(secret, 6, 1);
		Token token = new Token("test", methodType, otpGen);
		//Cohérence témoin / token de la première valeur
		assertEquals(
			"Wrong value for OTP.", 
			otpGen2.generer(), 
			token.generate()
		);
		//On attend le temps d'un quantum de temps (ici fixé à 1)
		try {
			Thread.sleep(1500);
		} catch (InterruptedException e) {
			System.out.println("Fatal ; interruption while in sleep.");
			return;
		}
		//Cohérence témoin / token de la première valeur
		assertEquals(
			"Wrong value for OTP.", 
			otpGen2.generer(), 
			token.generate()
		);
	}
	
	@Test
	public void testToken() {
		int count = 0;
		try {
			Token token = new Token();
			int otp = token.generate();
		} catch (IllegalStateException e) {
			count++;
		}
		assertEquals("Bad cases not correctly handled.", 1, count);
	}
}
