public class RBNode<K> {
    final K key;
    Color color;
    RBNode<K> left;
    RBNode<K> right;
    RBNode<K> parent;

    public RBNode(K key, RBNode<K> parent) {
        this.key = key;
        this.color = Color.RED;
        this.left = null;
        this.right = null;
        this.parent = parent;
    }

    @Override
    public String toString() {
        return key + ", " + color;
    }
}
