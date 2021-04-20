/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    decycling.h
    Header file for main operations for the decycling set calculation via Mykkeltveit's algorithm.
    @author Baris Ekim
    @version 1.0 4/15/19
*/
use super::uhs;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufWriter;


pub fn compute_decyc_set(uhs: &mut uhs::UHS, decycling_path: &str) -> usize {
    let f = File::create(decycling_path).unwrap();
    let mut f = BufWriter::new(f);
    let mut size = 0;
    let k = uhs.k;
    let mut a : Vec<isize> = vec![0; k+1];
    let mut i : usize = 1;
    a[0] = -1;
    while i != 0 {
        if k % i == 0 {
            let node = get_edge(&a, i, k);
            let label = uhs.get_label(node);
            &uhs.remove_edge(node);
            size += 1;
            writeln!(&mut f, "{}", label);
        }
        i = uhs.k;
        while i > 0 {
            if a[i] != 3 {break;}
            i -= 1;
        }
        if i != 0 {
            a[i] = a[i] + 1;
            for j in i+1..k+1 {a[j] = a[j-i];}
        }
    }
    f.flush();
    return size;

}

pub fn get_edge(a: &Vec<isize>, i: usize, k: usize) -> usize {
    let u = 3.415926 * 2.0 / (k as f64);
    let mut q;
    if i < k {q = k;}
    else {
        q = 1;
        let mut s = 0.0;
        while s >= 0.0001 {
            for l in 1..k+1 {
                s += a[l] as f64 * (((l - 1 + k - q) % k) as f64 * u).sin();
            }
            q += 1;
        } 
        s = 0.0;
        while s < 0.0001 && q < k+k {
            for l in 1..k+1 {
                s += a[l] as f64 * (((l - 1 + k + k - q) % k) as f64 * u).sin();
            }
            q += 1;
        }
        if q > k {q -= k;}
    }
    let mut code = 0;
    for j in q+1..k+1 {code = 4 * code + a[j];}
    for j in 1..q+1 {code = 4 * code + a[j];}
    return code as usize;

}