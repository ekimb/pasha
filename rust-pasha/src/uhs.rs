use std::collections::HashMap;
use rand::Rng;
use std::io::{Write,BufWriter};
use std::fs::File;


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
    F_curr: Vec<f64>,
    F_prev: Vec<f64>,
    hitting_num_array: Vec<f64>,
    delta: f64,
    epsilon: f64,
    total: f64,
    pub k: usize,
    curr: usize,
    l: usize,
    h: usize,
    exit: isize,
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

    pub fn get_label(&mut self, mut i: usize) -> String {
        let mut finalString = String::new();
        for _ in 0..self.k {
            finalString = format!("{}{}", self.get_char(i % 4), finalString);
            i = i / 4;
        }
        return finalString;
    }

    pub fn get_char(&mut self, i: usize) -> char {
        return self.alphabet.chars().nth(i).unwrap();
    }

    pub fn remove_edge(&mut self, i: usize) {
        if get_bit(&mut self.edge_array, i) == 1 {
            self.edge_count -= 1;
            toggle_bit(&mut self.edge_array, i);
        } 
    }

    pub fn find_additional_set(&mut self, L: usize, additional_path: &str, threads: usize) -> usize {
        let f = File::create(additional_path).unwrap();
        let mut f = BufWriter::new(f);
        let mut rng = rand::thread_rng();
        self.vertex_exp = 4_usize.pow(self.k as u32 - 1) as usize;
        let mut hitting_count = 0;
        self.l = L - self.k + 1;
        self.delta = 1.0 / self.l as f64;
        self.epsilon = (1.0 - 8.0*self.delta)/4.0;
        let alpha = 1.0 - 4.0*self.delta - 2.0*self.epsilon;
        println!("Alpha: {}\nDelta: {}\nEpsilon: {}", 1.0/alpha, self.delta, self.epsilon);
        self.hitting_num_array = vec![0.0; self.edge_num];
        self.topo_sort = vec![0; self.vertex_exp];
        self.D = vec![vec![0.0; self.vertex_exp]; self.l+1];
        self.D[0] = vec![1.4e-45_f64; self.vertex_exp];
        self.F_curr = vec![0.0; self.vertex_exp];
        self.F_prev = vec![1.4e-45_f64; self.vertex_exp];
        self.pick = vec![0; array_size(self.edge_num)];
        self.used = vec![0; array_size(self.vertex_exp)];
        self.finished = vec![0; array_size(self.vertex_exp)];
        println!("Initialized arrays.");
        self.topological_sort();
        println!("Finished topological sort.");
        println!("Length of longest remaining path: {}", self.max_length());
        self.calculate_paths(threads);
        let mut stage_vertices = Vec::<usize>::new();
        let mut max_index = self.find_max(&mut stage_vertices, threads, false);
        self.h = self.hitting_num_array[max_index].log((1.0 + self.epsilon)) as usize;
        let prob = self.delta / self.l as f64;
        while self.h > 0 {
            let mut stage_vertices = Vec::<usize>::new();
            self.total = 0.0;
            let mut hitting_count_stage = 0.0;
            let mut path_count_stage = 0.0;
            self.calculate_paths(threads);
            max_index = self.find_max(&mut stage_vertices, threads, true);
            if stage_vertices.len() == 0 {self.h -= 1;continue;}
            if self.exit == -1 {break;}
            for v in stage_vertices.iter() {
                if get_bit(&mut self.pick, *v) == 0 && (self.hitting_num_array[*v] > (self.delta.powf(3.0) * self.total)) {
                    toggle_bit(&mut self.pick, *v);
                    hitting_count_stage += 1.0;
                    path_count_stage += self.hitting_num_array[*v];
                }
            }
            for u in stage_vertices.iter() {
                for v in stage_vertices.iter() {
                    if get_bit(&mut self.pick, *u) == 0 {
                        if rng.gen::<f64>() <= prob {
                            toggle_bit(&mut self.pick, *u);
                            hitting_count_stage += 1.0;
                            path_count_stage += self.hitting_num_array[*u];
                        }
                        if get_bit(&mut self.pick, *v) == 0 {
                            if rng.gen::<f64>() <= prob {
                                toggle_bit(&mut self.pick, *v);
                                hitting_count_stage += 1.0;
                                path_count_stage += self.hitting_num_array[*v];
                            }
                        }
                    }
                }
                for v in stage_vertices.iter() {
                    if get_bit(&mut self.pick, *v) == 1 {
                        self.remove_edge(*v);
                        let label = self.get_label(*v);
                        writeln!(&mut f, "{}", label); 
                        hitting_count += 1;
                    }
                }
                self.h -= 1;
            }
        }
        f.flush();
        self.topological_sort();
        println!("Length of longest remaining path: {}", self.max_length());
        return hitting_count;
    }

    pub fn find_max(&mut self, stage_vertices: &mut Vec<usize>, threads: usize, in_stage: bool) -> usize {
        let mut max_val = 0.0;
        let mut max_idx = 0;
        let mut count = 0;
        for i in 0..self.edge_num {
            if get_bit(&mut self.edge_array, i) == 1 && self.hitting_num_array[i] > max_val {
                max_val = self.hitting_num_array[i];
                max_idx = i;
            }
            if in_stage && (self.hitting_num_array[i] >= (1.0+self.epsilon).powf(self.h as f64-1.0)) && (self.hitting_num_array[i] <= (1.0+self.epsilon).powf(self.h as f64)) {
                stage_vertices.push(i);
                self.total += self.hitting_num_array[i];
                count += 1;
            }
        }
        return max_idx;
    } 

    pub fn calculate_paths(&mut self, threads: usize) -> usize {
        self.curr = 1;
        self.vertex_exp2 = self.vertex_exp * 2;
        self.vertex_exp3 = self.vertex_exp * 3;
        self.vertex_exp_mask = self.vertex_exp - 1;
        self.vertex_exp_1 = 4_u32.pow((self.k-2) as u32) as usize;
        for j in 1..self.l {
            for i in 0..self.vertex_exp {
                self.D[j][i] = get_bit(&mut self.edge_array, i) as f64*self.D[j-1][(i >> 2)] + get_bit(&mut self.edge_array, i + self.vertex_exp) as f64*self.D[j-1][((i + self.vertex_exp) >> 2)] + get_bit(&mut self.edge_array, i + self.vertex_exp2) as f64*self.D[j-1][((i + self.vertex_exp2) >> 2)] + get_bit(&mut self.edge_array, i + self.vertex_exp3) as f64*self.D[j-1][((i + self.vertex_exp3) >> 2)];
            }
        }
        while self.curr <= self.l {
            for i in 0..self.vertex_exp {
                let index = i * 4;
                self.F_curr[i] = get_bit(&mut self.edge_array, index) as f64*self.F_prev[index & self.vertex_exp_mask] + get_bit(&mut self.edge_array, index + 1) as f64*self.F_prev[(index + 1) & self.vertex_exp_mask] + get_bit(&mut self.edge_array, index + 2) as f64*self.F_prev[(index + 2) & self.vertex_exp_mask] + get_bit(&mut self.edge_array, index + 3) as f64*self.F_prev[(index + 3) & self.vertex_exp_mask];
            }
            for i in 0..self.edge_num {
                self.hitting_num_array[i] += self.F_prev[i % self.vertex_exp]/1.4e-45_f64 * self.D[(self.l-self.curr)][i / 4] / 1.4e-45_f64;
                if get_bit(&mut self.edge_array, i) == 0 {self.hitting_num_array[i] = 0.0;}
            }
            for i in 0..self.vertex_exp {self.F_prev[i] = self.F_curr[i];}
            self.curr += 1;
        }
        return 1;
    }

    pub fn get_adjacent(&mut self, v: usize) -> Vec<usize> {
        let mut adj_vertex = Vec::<usize>::new();
        for i in 0..4 {
            let index = v + i * self.vertex_exp;
            if get_bit(&mut self.edge_array, index) == 1 {adj_vertex.push(index / 4);}
        }
        return adj_vertex;
    }

    pub fn depth_first_search(&mut self, mut index: isize, u: usize) -> isize {
        toggle_bit(&mut self.used, u);
        let mut cycle : bool = false;
        for v in self.get_adjacent(u).iter() {
            //println!("{} to {}", u, v);
            if get_bit(&mut self.used, *v) == 1 && get_bit(&mut self.finished, *v) == 0 {
                cycle = true;
            }
            if get_bit(&mut self.used, *v) == 0 {
                index = self.depth_first_search(index, *v);
                cycle = cycle || (index == -1);
            }
        }
        toggle_bit(&mut self.finished, u);
        if index != -1 {self.topo_sort[index as usize] = u as isize};
        if cycle {return -1;}
        else {return index + 1;}
    }

    pub fn topological_sort(&mut self) {
        let mut index : isize = 0;
        println!("{} edges will be sorted.", self.edge_count);
        for i in 0..self.vertex_exp {
            if get_bit(&mut self.used, i) == 0 {
                index = self.depth_first_search(index, i);
                if (index == -1) {self.topo_sort = Vec::<isize>::new(); return;}
            }
        }
    }

    pub fn max_length(&mut self) -> usize {
        let mut depth : Vec<isize> = vec![0; self.vertex_exp];
        let mut max_depth : isize = -1;
        for i in 0..self.vertex_exp {
            let mut max_vert_depth = -1;
            for j in 0..4 {
                let edge_index = self.topo_sort[i] as usize + j * self.vertex_exp;
                let vertex_index = edge_index / 4;
                if depth[vertex_index as usize] > max_vert_depth && get_bit(&mut self.edge_array, edge_index as usize) == 1 {max_vert_depth = depth[vertex_index as usize];}
            }
            depth[self.topo_sort[i] as usize] = max_vert_depth + 1;
            if depth[self.topo_sort[i] as usize] > max_depth {max_depth = depth[self.topo_sort[i] as usize];}
        }
        return max_depth as usize;
    }





}