mod decycling;
mod uhs;
fn main() {
    println!("Hello, world!");
    let k = 9;
    let mut uhs = uhs::UHS::new(k);
    let decycling_path = format!("decyc{}.uhs", k);
    println!("Built a complete de Bruijn graph of order {}", k);
    let decycling_size = decycling::compute_decyc_set(&mut uhs, &decycling_path);
    println!("Computed decycling set of size {}", decycling_size);
}
