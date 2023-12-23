import java.util.logging.Logger;

public class RBTree<K extends Comparable<K>> {
    private RBNode<K> root;

    public RBNode<K> root() {
        return root;
    }

    private static boolean isRed(RBNode<?> p) {
        return p == null ? false : p.color == Color.RED;
    }

    private void rotateLeft(RBNode<K> p) {
        RBNode<K> r = p.right;
        p.right = r.left;

        if (r.left != null) {
            r.left.parent = p;
        }

        r.parent = p.parent;
        if (p.parent == null) {
            root = r;
        } else if (p.parent.left == p) {
            p.parent.left = r;
        } else {
            p.parent.right = r;
        }

        r.left = p;
        p.parent = r;
    }

    private void rotateRight(RBNode<K> p) {
        RBNode<K> l = p.left;
        p.left = l.right;

        if (l.right != null) {
            l.right.parent = p;
        }
        l.parent = p.parent;

        if (p.parent == null) {
            root = l;
        } else if (p.parent.right == p) {
            p.parent.right = l;
        } else {
            p.parent.left = l;
        }

        l.right = p;
        p.parent = l;
    }

    public boolean add(K key) {
        RBNode<K> t = root;
        RBNode<K> parent = null;
        int cmp = 0;
        while (t != null) {
            parent = t;
            cmp = key.compareTo(t.key);
            if (cmp < 0) {
                t = t.left;
            } else if (cmp > 0) {
                t = t.right;
            } else {
                return false;
            }
        }

        RBNode<K> z = new RBNode<>(key, parent);
        if (cmp < 0) {
            parent.left = z;
        } else if (cmp > 0) {
            parent.right = z;
        } else {
            root = z;
        }
        addFixUp(z);
        return true;
    }

    private final static Logger logger = Logger.getLogger(RBTree.class.getName());

    private void addFixUp(RBNode<K> z) {
        while (isRed(z.parent)) {
            var p = z.parent;
            if (p == p.parent.left) {
                if (isRed(p.parent.right)) {
                    logger.fine("Case 1");
                    p.color = Color.BLACK;
                    p.parent.color = Color.RED;
                    p.parent.right.color = Color.BLACK;
                    z = p.parent;
                    continue;
                }

                if (z == p.right) {
                    logger.fine("Case 2");
                    rotateLeft(p);
                    p = p.parent;
                }
                logger.fine("Case 3");
                p.color = Color.BLACK;
                p.parent.color = Color.RED;
                rotateRight(p.parent);
                break;
            } else {
                if (isRed(p.parent.left)) {
                    logger.fine("Case 4");
                    p.color = Color.BLACK;
                    p.parent.color = Color.RED;
                    p.parent.left.color = Color.BLACK;
                    z = p.parent;
                    continue;
                }

                if (z == p.left) {
                    logger.fine("Case 5");
                    rotateRight(p);
                    p = p.parent;
                }
                logger.fine("Case 6");
                p.color = Color.BLACK;
                p.parent.color = Color.RED;
                rotateLeft(p.parent);
                break;
            }
        }
        root.color = Color.BLACK;
    }
}
