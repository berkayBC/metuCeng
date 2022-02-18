import java.util.ArrayList;

public class CengBucket {

    // GUI-Based Methods
    // These methods are required by GUI to work properly.

    public ArrayList<CengPoke> pokes;
    public int localD;
    public int bucketSize;
    public int len;

    public CengBucket() {
        this.localD = 0;
        this.bucketSize = CengPokeKeeper.getBucketSize();
        this.pokes = new ArrayList<>();
        this.len = 0;
    }

    public CengBucket(int localD) {
        this.localD = localD;
        this.len = 0;
        this.pokes = new ArrayList<>();
        this.bucketSize = CengPokeKeeper.getBucketSize();
    }

    //Added

    public void insert(CengPoke poke) {
        if (!isFull() && len != bucketSize) {
            pokes.add(poke);
            this.len++;
        }
    }

    public void delete(Integer pokeKey) {
        for (int i = 0; i < len; i++) {
            if (pokes.get(i).pokeKey().equals(pokeKey)) {
                pokes.remove(pokes.get(i));
                len--;
            }
        }
    }

    public boolean isFull() {
        return (pokes.size() == bucketSize);
    }

    public int pokeCount() {
        return pokes.size();
    }

    public CengPoke pokeAtIndex(int index) {
        if (index >= this.pokeCount()) {
            return null;
        } else {
            return this.pokes.get(index);
        }
    }

    public int getHashPrefix() {
        return localD;
    }

    public Boolean isVisited() {
        return false;
    }

    // Own Methods

    public void deleteAll() {
        for (int i = 0; i < len; i++) {
            pokes.remove(0);
        }
        len = 0;
    }
}
