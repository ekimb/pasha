use std::collections::HashMap;


const fn array_size(x: usize) -> usize {
    (x/8+(!!(x%8))) as usize
}

pub fn toggle_bit(array: &mut Vec<u8>, index: usize) {
    array[index / 8] ^= 1 << (index % 8);
}

pub fn get_bit(array: &mut Vec<u8>, index: usize) -> u8 {
    return 1 & (array[index / 8] >> (index % 8));
}

#[derive(Debug, Default)]
pub struct UHS {
    D: Vec<Vec<f64>>,
    Fcurr: Vec<f64>,
    Fprev: Vec<f64>,
    hitting_num_array: Vec<f64>,
    delta: f64,
    epsilon: f64,
    total: f64,
    pub k: usize,
    curr: usize,
    l: usize,
    h: usize,
    exit: usize,
    finished: Vec<u8>,
    used: Vec<u8>,
    edge_array: Vec<u8>,
    pick: Vec<u8>,
    topo_sort: Vec<isize>,
    stage_vertices: Vec<usize>,
    vertex_count: usize,
    vertex_exp: usize,
    vertex_exp2: usize,
    vertex_exp3: usize,
    vertex_exp_mask: usize,
    vertex_exp_1: usize,
    edge_count: usize,
    edge_num: usize,
    alphabet_map: HashMap<char, usize>,
    alphabet: String,
}
impl UHS {
    pub fn new(inp_k: usize) -> Self {
        let mut uhs_obj = UHS {k: inp_k, ..Default::default()};
        uhs_obj.edge_num = (4_usize.pow(inp_k as u32)) as usize;
        uhs_obj.edge_array = vec![255; array_size(uhs_obj.edge_num)];
        uhs_obj.edge_count = uhs_obj.edge_num;
        uhs_obj.vertex_count = uhs_obj.edge_num / 4;
        uhs_obj.alphabet = "ACGT".to_string();
        uhs_obj.alphabet_map = HashMap::<char, usize>::new();
        for (i, c) in uhs_obj.alphabet.chars().enumerate() {
            uhs_obj.alphabet_map.insert(c, i);
        }
        return uhs_obj;
    }

    pub fn get_label(&self, mut i: usize) -> String {
        let mut finalString = String::new();
        for _ in 0..self.k {
            finalString = format!("{}{}", self.get_char(i % 4), finalString);
            i = i / 4;
        }
        return finalString;
    }
    pub fn get_char(&self, i: usize) -> char {
        return self.alphabet.chars().nth(i).unwrap();
    }
    pub fn remove_edge(&mut self, i: usize) {
        println!("{}", i);
        if get_bit(&mut self.edge_array, i) == 1 {
                self.edge_count -= 1;
                toggle_bit(&mut self.edge_array, i);
            } 
        }


}