mod decycling;
mod uhs;
fn main() {
    println!("Hello, world!");
    let k = 8;
    let L = 100;
    let mut uhs = uhs::UHS::new(k);
    let decycling_path = format!("decyc{}.uhs", k);
    let additional_path = format!("add{}{}.uhs", k, L);
    println!("Built a complete de Bruijn graph of order {}", k);
    let decycling_size = decycling::compute_decyc_set(&mut uhs, &decycling_path);
    println!("Computed decycling set of size {}", decycling_size);
    let additional_size = uhs.find_additional_set(L, &additional_path, 1);
    println!("Computed additional set of size {}", additional_size);
}
