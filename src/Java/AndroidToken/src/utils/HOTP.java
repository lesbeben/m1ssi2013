package utils;

/**
 * Une classe pour l'implémentation de la génération des générateurs de mots de
 * passes jetables pour la méthode HOTP.
 * 
 * @author Gaëtan Ferry
 */
public class HOTP extends OTPGenerator {
	
	/**
	 * La valeur du compteur pour ce générateur.
	 */
	private long count;
	/**
	 * La valeur de la clef pour ce générateur.
	 */
	private ISecret key;
	
	/**
	 * Construit un générateur d'OTP HOTP.
	 * 
	 * @param count La valeur du compteur pour le générateur.
	 * @param key La clef secrète pour la génération.
     * @param digits Le nombre de chiffres composant les OTP générés.
	 */
	public HOTP(long count, ISecret key, int digits) {
        super(digits);
        if (key == null) {
            throw new IllegalArgumentException("key");
        }
		this.count = count;
        this.key = key;
	}

    /**
     * Le constructeur par défaut avec compteur à 0 et les OTP de longueur 
     * minimale.
     * @param key La clef secrète pour la génération.
     */
    public HOTP(ISecret key) {
        super();
        if (key == null) {
            throw new IllegalArgumentException("key");
        }
        this.count = 0;
        this.key = key;
    }
	
	@Override
	public long getCount() {
		return count;
	}

	@Override
	public void increaseCount() {
		if (count == Long.MAX_VALUE) {
            throw new IllegalStateException(
            		"Le compteur a atteint son maximum"
            );
        }
		count = count + 1;
	}
	
	
    @Override
	public ISecret getKey() {
        return key;
    }
}
