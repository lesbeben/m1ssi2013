package utils;
/**
 * Une classe abstraite pour les générateurs d'OTP.
 *
 * @post c1 = getCount(), o1 = generer(), c2 = getCount(), o2 = generer 
 * 				c1 != c2 => o1 != o2 
 * @author celtic
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
    /**
     * Deux valeurs successives doivent être différentes.
     */
	public int generer() {
        byte[] hs = Utils.hmacSha1(getKey(), getCount());
        byte[] sBits = Utils.truncate(hs);
        int sNum = Utils.convert(sBits);
        int d = sNum % (10 ^ getDigits());
        increaseCount();
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
	public abstract long getCount();
	
	/**
	 * Fixe la valeur du compteur du générateur d'OTP.
	 */
	public abstract void increaseCount();

    /**
     * Retourne la valeur de la clé secrète du générateur.
     * @return la clé secrète
     */
    public abstract ISecret getKey();

	
}
