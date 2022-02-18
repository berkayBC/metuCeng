public class CengHashRow {

    // GUI-Based Methods
    // These methods are required by GUI to work properly.
    public CengBucket bucket;
    // public int globalD;
    public String hashPrefix;

    public CengHashRow(String hashPrefix) {
        this.bucket = new CengBucket();
        // this.globalD = 0;
        this.hashPrefix = hashPrefix;
    }

    public String hashPrefix() {
        return hashPrefix;
    }

    public CengBucket getBucket() {
        return bucket;
    }

    public void setBucket(CengBucket bucket) {
        this.bucket = bucket;
    }

    public boolean isVisited() {
        return false;
    }

    // Own Methods
}
