package utils;

/**
 * Une classe pour l'implémentation de la génération des générateurs de mots de
 * passes jetables pour la méthode HOTP.
 * 
 * @author celtic
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
	 * 
	 * @param count La valeur du compteur pour le generateur.
	 * @param key La clef secrete pour la generation.
	 */
	public HOTP(long count, ISecret key) {
		//TODO Constructeur
	}
	
	@Override
	protected long getCount() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	protected void increaseCount() {
		// TODO Auto-generated method stub
		
	}	
}
