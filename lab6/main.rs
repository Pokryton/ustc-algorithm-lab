mod huffman;

use std::collections::BTreeMap;
use std::fs::File;
use std::io::{self, Write, BufWriter};

fn main() -> io::Result<()> {
    let mut freqs = BTreeMap::new();

    let f = File::open("orignal.txt")?;
    io::read_to_string(f)?
        .chars()
        .filter(|c| c.is_ascii_graphic())
        .for_each(|c| *freqs.entry(c).or_insert(0) += 1);

    let mut f = BufWriter::new(File::create("table.txt")?);

    let table = huffman::generate_codebook(&freqs);

    let len_before = (freqs.len() as f64).log2().ceil() as usize;
    let mut size_before: usize = 0;
    let mut size_after: usize = 0;

    writeln!(f, "  char |   freq       |     code")?;
    writeln!(f, "-------+--------------+-------------------")?;
    for (c, freq) in freqs {
        let code = table.get(&c).unwrap();

        write!(f, "   {c}   |  {freq:<10}  |  ")?;
        for &bit in code {
            write!(f, "{}", bit as i32)?;
        }
        writeln!(f)?;

        size_before += freq * len_before;
        size_after += freq * code.len();
    }

    let rate = size_after as f64 / size_before as f64;

    println!("fixed-length encoding: {}b", size_before);
    println!("huffman encoding: {}b", size_after);
    println!("compression rate: {:.5}%", rate * 100.0);

    Ok(())
}
