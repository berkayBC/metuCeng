public class CengPoke {

    private Integer pokeKey;
    private String pokeName;
    private String pokePower;
    private String pokeType;

    public CengPoke(Integer pokeKey, String pokeName, String pokePower, String pokeType) {
        this.pokeKey = pokeKey;
        this.pokeName = pokeName;
        this.pokePower = pokePower;
        this.pokeType = pokeType;
    }

    // Getters

    public Integer pokeKey() {
        return pokeKey;
    }

    public String pokeName() {
        return pokeName;
    }

    public String pokePower() {
        return pokePower;
    }

    public String pokeType() {
        return pokeType;
    }

    // Setter
    public void setPokeKey(int pokeKey) {
        this.pokeKey = pokeKey;
    }

    public void setPokeName(String pokeName) {
        this.pokeName = pokeName;
    }

    public void setPokePower(String pokePower) {
        this.pokePower = pokePower;
    }

    public void setPokeType(String pokeType) {
        this.pokeType = pokeType;
    }

    // GUI method - do not modify
    public String fullName() {
        return "" + pokeKey() + "\t" + pokeName() + "\t" + pokePower() + "\t" + pokeType;
    }
}
