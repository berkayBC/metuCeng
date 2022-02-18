import java.util.ArrayList;

public class CengHashTable {

    public static int globalD;
    public ArrayList<CengHashRow> hashRows;
    public int numberOfEmptyBuckets;

    public CengHashTable() {
        this.hashRows = new ArrayList<>();
        globalD = 0;
        CengHashRow hashRow = new CengHashRow("0");
        hashRow.bucket = new CengBucket(0);
        hashRows.add(hashRow);
        this.numberOfEmptyBuckets = 0;
    }

    public String BinaryHash(CengPoke poke) {
        int hashValue = poke.pokeKey() % CengPokeKeeper.getHashMod();
        String binaryHash = Integer.toBinaryString(hashValue);
        if ((int) (Math.log(CengPokeKeeper.getHashMod()) / Math.log(2)) != binaryHash.length()) {
            for (int i = 0; i < (int) (Math.log(CengPokeKeeper.getHashMod()) / Math.log(2)) - binaryHash.length() + 1; i++) {
                binaryHash = "0" + binaryHash;
            }
        }
        return binaryHash;
    }

    public void deletePoke(Integer pokeKey) {
        int hashIndex = pokeKey % CengPokeKeeper.getHashMod();
        String binaryHash = Integer.toBinaryString(hashIndex);
        if ((int) (Math.log(CengPokeKeeper.getHashMod()) / Math.log(2)) != binaryHash.length()) {
            for (int i = 0; i < (int) (Math.log(CengPokeKeeper.getHashMod()) / Math.log(2)) - binaryHash.length() + 1; i++) {
                binaryHash = "0" + binaryHash;
            }
        }
        String realHash = binaryHash.substring(0, globalD);

        int rowIndex = Integer.parseInt(realHash, 2);

        boolean found = false;
        for (int i = 0; i < hashRows.size(); i++) {
            if (hashRows.get(i).bucket.len != 0) {
                for (int j = 0; j < hashRows.get(i).bucket.len; j++) {
                    if (hashRows.get(i).bucket.pokes.get(j).pokeKey().equals(pokeKey)) {
                        found = true;
                    }
                }
            }
        }
        if (found) {
            hashRows.get(rowIndex).bucket.delete(pokeKey);
            if (hashRows.get(rowIndex).bucket.len == 0) {
                this.numberOfEmptyBuckets++;
            }
            System.out.println("\"delete\": {");
            System.out.println("\t\"emptyBucketNum\": " + this.numberOfEmptyBuckets);
            System.out.println("}");
        }
    }

    public void addPoke(CengPoke poke) {
        if (globalD == 0) {
            if (!hashRows.get(0).bucket.isFull()) {
                hashRows.get(0).bucket.insert(poke);
            } else { //globali 1 yap
                duplicateHashRow();
                addPoke(poke);
            }
        } else {
            String newHash = BinaryHash(poke);
            String realHash = newHash.substring(0, globalD);

            int hashIndex = Integer.parseInt(realHash, 2);
            if (!hashRows.get(hashIndex).bucket.isFull()) {
                hashRows.get(hashIndex).bucket.insert(poke);
            } else {
                if (hashRows.get(hashIndex).bucket.localD < globalD) {
                    if (realHash.substring(realHash.length() - 1).equals("1")) {
                        duplicateBucket(hashRows.get(hashIndex - 1), hashIndex - 1);
                    } else {
                        duplicateBucket(hashRows.get(hashIndex), hashIndex);
                    }
                    addPoke(poke);
                } else {
                    duplicateHashRow();
                    addPoke(poke);
                }
            }
        }
    }

    public void searchPoke(Integer pokeKey) {
        int hashIndex = pokeKey % CengPokeKeeper.getHashMod();
        String binaryHash = Integer.toBinaryString(hashIndex);
        if ((int) (Math.log(CengPokeKeeper.getHashMod()) / Math.log(2)) != binaryHash.length()) {
            for (int i = 0; i < (int) (Math.log(CengPokeKeeper.getHashMod()) / Math.log(2)) - binaryHash.length() + 1; i++) {
                binaryHash = "0" + binaryHash;
            }
        }
        String realHash = binaryHash.substring(0, globalD);
        int rowIndex = Integer.parseInt(realHash, 2);
        CengBucket searchBucket = hashRows.get(rowIndex).bucket;

        boolean found = false;

        for (int i = 0; i < searchBucket.len; i++) {
            if (searchBucket.pokes.get(i).pokeKey().equals(pokeKey)) {
                found = true;
                break;
            }
        }

        if (found) {
            ArrayList<CengHashRow> searchRows = new ArrayList<>();
            for (int i = 0; i < hashRows.size(); i++) {
                for (int j = 0; j < hashRows.get(i).bucket.pokes.size(); j++) {
                    if (hashRows.get(i).bucket.pokes.get(j).pokeKey().equals(pokeKey)) {
                        searchRows.add(hashRows.get(i));
                    }
                }
            }
            System.out.println("\"search\": {");
            for (int i = 0; i < searchRows.size(); i++) {
                System.out.println("\t\"row\": {\n\t\t\"hashPref\": "
                        + searchRows.get(i).hashPrefix
                        + ",\n\t\t\"bucket\": {"
                        + "\n\t\t\t\"hashLength\": " + searchRows.get(i).bucket.localD + ","
                        + "\n\t\t\t\"pokes\": [");
                for (int j = 0; j < searchRows.get(i).bucket.pokes.size(); j++) {
                    System.out.println("\t\t\t\t\"poke\": {"
                            + "\n\t\t\t\t\t\"hash\": " + BinaryHash(searchRows.get(i).bucket.pokes.get(j)) + ","
                            + "\n\t\t\t\t\t\"pokeKey\": " + searchRows.get(i).bucket.pokes.get(j).pokeKey() + ","
                            + "\n\t\t\t\t\t\"pokeName\": " + searchRows.get(i).bucket.pokes.get(j).pokeName() + ","
                            + "\n\t\t\t\t\t\"pokePower\": " + searchRows.get(i).bucket.pokes.get(j).pokePower() + ","
                            + "\n\t\t\t\t\t\"pokeType\": " + searchRows.get(i).bucket.pokes.get(j).pokeType()
                    );
                    if (j == searchRows.get(i).bucket.pokes.size() - 1) {
                        System.out.println("\t\t\t\t}");
                    } else {
                        System.out.println("\t\t\t\t},");
                    }
                }
                if (i == searchRows.size() - 1) {
                    System.out.println("\t\t\t]");
                    System.out.println("\t\t}\n\t}\n}");
                } else {
                    System.out.println("\t\t\t]");
                    System.out.println("\t\t}\n\t},");
                }
            }
        } else {
            System.out.println("\"search\": {\n}");
        }
    }

    public void print() {
        System.out.println("\"table\": {");
        for (int i = 0; i < this.hashRows.size(); i++) {
            System.out.println("\t\"row\": {\n\t\t\"hashPref\": "
                    + this.hashRows.get(i).hashPrefix
                    + ",\n\t\t\"bucket\": {"
                    + "\n\t\t\t\"hashLength\": " + this.hashRows.get(i).bucket.localD + ","
                    + "\n\t\t\t\"pokes\": [");
            for (int j = 0; j < this.hashRows.get(i).bucket.len; j++) {
                //print pokes
                if (j == this.hashRows.get(i).bucket.len - 1) {
                    System.out.println("\t\t\t\t\"poke\": {"
                            + "\n\t\t\t\t\t\"hash\": " + BinaryHash(this.hashRows.get(i).bucket.pokes.get(j)) + ","
                            + "\n\t\t\t\t\t\"pokeKey\": " + this.hashRows.get(i).bucket.pokes.get(j).pokeKey() + ","
                            + "\n\t\t\t\t\t\"pokeName\": " + this.hashRows.get(i).bucket.pokes.get(j).pokeName() + ","
                            + "\n\t\t\t\t\t\"pokePower\": " + this.hashRows.get(i).bucket.pokes.get(j).pokePower() + ","
                            + "\n\t\t\t\t\t\"pokeType\": " + this.hashRows.get(i).bucket.pokes.get(j).pokeType() + "\n\t\t\t\t}"
                    );
                } else {
                    System.out.println("\t\t\t\t\"poke\": {"
                            + "\n\t\t\t\t\t\"hash\": " + BinaryHash(this.hashRows.get(i).bucket.pokes.get(j)) + ","
                            + "\n\t\t\t\t\t\"pokeKey\": " + this.hashRows.get(i).bucket.pokes.get(j).pokeKey() + ","
                            + "\n\t\t\t\t\t\"pokeName\": " + this.hashRows.get(i).bucket.pokes.get(j).pokeName() + ","
                            + "\n\t\t\t\t\t\"pokePower\": " + this.hashRows.get(i).bucket.pokes.get(j).pokePower() + ","
                            + "\n\t\t\t\t\t\"pokeType\": " + this.hashRows.get(i).bucket.pokes.get(j).pokeType() + "\n\t\t\t\t},"
                    );
                }
            }
            if (i == this.hashRows.size() - 1) {
                System.out.println("\t\t\t]");
                System.out.println("\t\t}\n\t}\n}");
            } else {
                System.out.println("\t\t\t]");
                System.out.println("\t\t}\n\t},");
            }
        }
    }

    // GUI-Based Methods
    // These methods are required by GUI to work properly.

    public int prefixBitCount() {
        return globalD;
    }

    public int rowCount() {
        return hashRows.size();
    }

    public CengHashRow rowAtIndex(int index) {
        return hashRows.get(index);
    }

    // Own Methods

    public void duplicateHashRow() {
        if (globalD == 0) {
            globalD++;
            hashRows.get(0).hashPrefix = "0";
            hashRows.add(new CengHashRow("1"));
            hashRows.get(1).setBucket(hashRows.get(0).getBucket());
        } else {
            globalD++;
            for (int i = 0; i < hashRows.size(); i++) {
                if (hashRows.get(i).hashPrefix.length() < globalD) {
                    String a = hashRows.get(i).hashPrefix;
                    a += "1";
                    hashRows.add(i + 1, new CengHashRow(a));
                    hashRows.get(i + 1).setBucket(hashRows.get(i).getBucket());
                    hashRows.get(i).hashPrefix += "0";
                }
            }
        }
    }

    public void duplicateBucket(CengHashRow row, int rowIndex) {
        CengBucket newBucket = new CengBucket(row.getBucket().localD + 1);
        hashRows.get(rowIndex + 1).setBucket(newBucket);
        row.getBucket().localD += 1;
        ArrayList<CengPoke> tempList = new ArrayList<>(row.bucket.pokes);
        row.bucket.deleteAll();
        for (int i = 0; i < tempList.size(); i++) {
            addPoke(tempList.get(i));
        }
    }
}

