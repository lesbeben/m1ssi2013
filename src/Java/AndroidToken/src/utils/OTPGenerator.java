package utils;
/**
 * Une classe abstraite pour les générateurs d'OTP.
 *
 * @author celtic
 */
public abstract class OTPGenerator implements IOTP {

    /**
     * Les nombres minimum et maximum de chiffres composant les OTP générés.
     */
    private static final int MIN_DIGITS = 6;
    private static final int MAX_DIGITS = 8;

    /**
     * Le nombre de chiffre composant les OTP générés.
     */
    protected int digits;

    /**
     * Le constructeur OTPGenerator.
     * @param digits le nombre de chiffres composant les OTP générés
     */
    public OTPGenerator(int digits) {
        if ((digits < MIN_DIGITS) || (digits > MAX_DIGITS)) {
            throw new IllegalArgumentException("digits");
        }
        this.digits = digits;
    }

    /**
     * Le constructeur par défaut pour un OTP à 6 chiffres.
     */
    public OTPGenerator() {
        this.digits = MIN_DIGITS;
    }

    @Override
	public int generer() {
        byte[] hs = Utils.hmacSha1(getKey(), getCount());
        byte[] sBits = Utils.truncate(hs);
        int sNum = Utils.convert(sBits);
        int d = sNum % (10 ^ getDigits());
    	return d;
    }

    /**
     * Retourne le nombre de chiffres composant les OTP générés.
     * @return digits Le nombre de chiffres composants les OTP générés.
     */
    protected int getDigits() {
        return digits;
    }

	/**
	 * Retourne la valeur du compteur actuel du générateur d'OTP. 
	 * @return La valeur du compteur.
	 */
	protected abstract long getCount();
	
	/**
	 * Fixe la valeur du compteur du générateur d'OTP.
	 */
	protected abstract void increaseCount();

    /**
     * Retourne la valeur de la clé secrète du générateur.
     * @return la clé secrète
     */
    protected abstract ISecret getKey();

	
}
