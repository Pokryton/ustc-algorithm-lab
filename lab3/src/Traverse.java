import java.util.Deque;
import java.util.ArrayDeque;
import java.util.Iterator;

public class Traverse {
    private final RBNode<?> root;

    public Traverse(RBTree<?> tree) {
        this.root = tree.root();
    }

    private static class InOrderIterator implements Iterator<RBNode<?>> {
        private RBNode<?> node;

        public InOrderIterator(RBNode<?> root) {
            node = root;
            while (node.left != null) {
                node = node.left;
            }
        }

        @Override
        public boolean hasNext() {
            return node != null;
        }

        @Override
        public RBNode<?> next() {
            RBNode<?> z = node;
            if (node.right != null) {
                node = node.right;
                while (node.left != null) {
                    node = node.left;
                }
            } else {
                while (node.parent != null && node == node.parent.right) {
                    node = node.parent;
                }
                node = node.parent;
            }

            return z;
        }
    }

    public Iterable<RBNode<?>> inOrder() {
        return () -> new InOrderIterator(root);
    }

    private static class PreOrderIterator implements Iterator<RBNode<?>> {
        private RBNode<?> node;

        public PreOrderIterator(RBNode<?> root) {
            node = root;
        }

        @Override
        public boolean hasNext() {
            return node != null;
        }

        @Override
        public RBNode<?> next() {
            RBNode<?> z = node;

            if (node.left != null) {
                node = node.left;
            } else if (node.right != null) {
                node = node.right;
            } else {
                while (node != null) {
                    RBNode<?> parent = node.parent;
                    if (parent != null && parent.right != null && parent.right != node) {
                        node = parent.right;
                        break;
                    }
                    node = parent;
                }
            }

            return z;
        }
    }


    public Iterable<RBNode<?>> preOrder() {
        return () -> new PreOrderIterator(root);
    }

    private static class LevelOrderIterator implements Iterator<RBNode<?>> {
        private final Deque<RBNode<?>> queue;

        LevelOrderIterator(RBNode<?> root) {
            queue = new ArrayDeque<>();
            queue.add(root);
        }

        @Override
        public boolean hasNext() {
            return !queue.isEmpty();
        }

        @Override
        public RBNode<?> next() {
            RBNode<?> z = queue.remove();
            if (z.left != null) {
                queue.add(z.left);
            }
            if (z.right != null) {
                queue.add(z.right);
            }
            return z;
        }
    }

    public Iterable<RBNode<?>> levelOrder() {
        return () -> new LevelOrderIterator(root);
    }
}
