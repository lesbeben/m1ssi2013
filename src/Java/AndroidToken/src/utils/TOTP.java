package utils;

/**
 * Une classe pour l'implémentation de la génération des générateurs de mots de
 * passes jetables pour la méthode TOTP.
 * 
 * @author celtic
 */
public class TOTP extends OTPGenerator {
	
	/**
	 * La valeur du quantum de temps. A savoir le temps durant lequel un otp
	 * est valide.
	 */
	public static final int TIME_QUANTUM = 30;
	
	/**
	 * La valeur du compteur pour ce générateur.
	 */
	private long count;
	/**
	 * La valeur de la clef pour ce générateur.
	 */
	private ISecret key;
	
	/**
	 * Instancie un générateur d'OTP TOTP.
	 * @param key La clef secrete pour la generation.
     * @param digits Le nombre de chiffres composant les OTP générés.
	 */
	public TOTP(ISecret key, int digits) {
		super(digits);
        if (key == null) {
            throw new IllegalArgumentException("key");
        }
        this.key = key;
	}

    /**
     * Le constructeur par défaut, avec OTP de longueur minimale.
     * @param key La clef secrete pour la generation.
     */
    public TOTP(ISecret key) {
        super();
        if (key == null) {
            throw new IllegalArgumentException("key");
        }
        this.key = key;
    }

	@Override
	/**
	 * Renvoie la valeur du compteur de temps.
	 * @return time / TIME_QUANTUM
	 */
	protected long getCount() {
        
		return 0;
	}

	@Override
	/**
	 * Le compteur est fixé sur le temps. Cette méthode n'execute rien.
	 */
	protected void increaseCount() {
		// Rien
	}

    @Override
    protected ISecret getKey() {
        return key;
    }
}
