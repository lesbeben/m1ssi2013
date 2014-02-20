package utils;

/**
 * Une classe abstraite pour les générateurs d'OTP.
 * Cette classe implémente le type ISecret défini dans l'interface IOTP.java.
 * La taille des OTP calculés par cette classe doit être comprise entre 6 et 8
 * caractères.
 * 
 * @post MIN_DIGITS <= |generer()| <= MAX_DIGITS
 * @author Gaëtan Ferry
 */
public abstract class OTPGenerator implements IOTP {

    /**
     * Les nombres minimum et maximum de chiffres composant les OTP générés.
     */
    public static final int MIN_DIGITS = 6;
    public static final int MAX_DIGITS = 8;

    /**
     * Le nombre de chiffre composant les OTP générés.
     */
    private int digits;

    /**
     * Construit un générateur d'OTP.
     * 
     * @param digits le nombre de chiffres composant les OTP générés
     */
    public OTPGenerator(int digits) {
        if ((digits < MIN_DIGITS) || (digits > MAX_DIGITS)) {
            throw new IllegalArgumentException("digits");
        }
        this.digits = digits;
    }

    /**
     * Le constructeur par défaut.
     * Fixe la taille des OTP générés à 6.
     */
    public OTPGenerator() {
        this.digits = MIN_DIGITS;
    }

    @Override
	public int generer() {
        byte[] hs = Utils.hmacSha1(getKey(), getCount());
        byte[] sBits = Utils.truncate(hs);
        int sNum = Utils.convert(sBits);
        int d = sNum % (int) (Math.pow(10, getDigits()));
        increaseCount();
    	return d;
    }

    /**
     * Retourne le nombre de chiffres composant les OTP générés.
     * 
     * @return digits Le nombre de chiffres composants les OTP générés.
     */
    protected int getDigits() {
        return digits;
    }

	/**
	 * Retourne la valeur du compteur actuel du générateur d'OTP. 
	 * 
	 * @return La valeur du compteur.
	 */
	public abstract long getCount();
	
	/**
	 * Augmente la valeur du compteur du générateur d'un quantum.
	 */
	public abstract void increaseCount();

    /**
     * Retourne la valeur de la clé secrète du générateur.
     * 
     * @return la clé secrète
     */
    public abstract ISecret getKey();

	
}
