use std::cmp::{Ordering, Reverse};
use std::collections::{BTreeMap, BinaryHeap};

#[derive(Debug, PartialEq, Eq)]
enum NodeKind {
    Branch(Box<Node>, Box<Node>),
    Leaf(char),
}

#[derive(Debug, PartialEq, Eq)]
struct Node {
    freq: usize,
    kind: NodeKind,
}

impl PartialOrd for Node {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.freq.partial_cmp(&other.freq)
    }
}

impl Ord for Node {
    fn cmp(&self, other: &Self) -> Ordering {
        self.freq.cmp(&other.freq)
    }
}

#[inline]
fn new_leaf(ch: char, freq: usize) -> Box<Node> {
    Box::new(Node {
        freq,
        kind: NodeKind::Leaf(ch),
    })
}

#[inline]
fn new_branch(left: Box<Node>, right: Box<Node>) -> Box<Node> {
    Box::new(Node {
        freq: left.freq + right.freq,
        kind: NodeKind::Branch(left, right),
    })
}

fn build_tree<'a, T>(freqs: T) -> Option<Box<Node>>
where
    T: IntoIterator<Item = (&'a char, &'a usize)>,
{
    let mut pq = BinaryHeap::new();

    for (&c, &freq) in freqs {
        if freq > 0 {
            let n = new_leaf(c, freq);
            pq.push(Reverse(n));
        }
    }

    while let Some(Reverse(x)) = pq.pop() {
        if let Some(Reverse(y)) = pq.pop() {
            let z = new_branch(x, y);
            pq.push(Reverse(z));
        } else {
            return Some(x);
        }
    }

    None
}

fn walk_tree(node: Box<Node>, codebook: &mut BTreeMap<char, Vec<bool>>, code: &mut Vec<bool>) {
    match node.kind {
        NodeKind::Leaf(c) => {
            codebook.insert(c, code.clone());
        }
        NodeKind::Branch(left, right) => {
            code.push(false);
            walk_tree(left, codebook, code);
            code.pop();

            code.push(true);
            walk_tree(right, codebook, code);
            code.pop();
        }
    }
}

pub fn generate_codebook<'a, T>(freqs: T) -> BTreeMap<char, Vec<bool>>
where
    T: IntoIterator<Item = (&'a char, &'a usize)>,
{
    let mut codebook = BTreeMap::new();
    if let Some(root) = build_tree(freqs) {
        walk_tree(root, &mut codebook, &mut vec![]);
    }

    codebook
}
