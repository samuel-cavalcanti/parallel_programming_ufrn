use std::{
    sync::{Arc, Mutex},
    thread::JoinHandle,
};

#[derive(Debug)]
struct Node {
    data: Vec<i32>,
    neighborhoods: Mutex<Vec<JoinHandle<i32>>>,
}

impl Node {
    fn from_value(data: i32) -> Node {
        Node {
            data: vec![data],
            neighborhoods: Mutex::new(vec![]),
        }
    }

    fn compute(self) -> i32 {
        /*
            Em termos de memória essa função desaloca toda memória
            usada pela estrutura Node e retorna um inteiro de 32bits.
            Dado que foi utilizado safe rust e o código compila, logo esse
            código está livre data race e como não referências cíclicas
            também está livre de memory leak.
         */
        let result: i32 = self.data.iter().sum();

        let neighborhoods = self.neighborhoods.into_inner().unwrap();

        let neighborhoods_sum: i32 = neighborhoods
            .into_iter()
            .map(|handle| handle.join().expect("Unable to lock neighborhood"))
            .sum();

        result + neighborhoods_sum
    }
}

fn start_to_compute_node(node: Node) -> JoinHandle<i32> {
    std::thread::spawn(move || {
        let result = node.compute();
        std::thread::sleep(std::time::Duration::from_micros(500));

        result
    })
}

fn receive_value(left: Arc<Node>, right: Arc<Node>) {
    let right = Arc::try_unwrap(right).unwrap();

    let mut left_neighborhoods = left
        .neighborhoods
        .lock()
        .expect("Unable to lock neighborhood");

    left_neighborhoods.push(start_to_compute_node(right))
}

fn create_new_tree_bitwise(mut nodes: Vec<Arc<Node>>) -> Vec<Arc<Node>> {
    let size = nodes.len();

    match size {
        2 => {
            let left = nodes.remove(0);
            let right = nodes.remove(0);

            receive_value(left.clone(), right);

            vec![left]
        }

        3 => {
            let left = nodes.remove(0);
            let middle = nodes.remove(0);
            let right = nodes.remove(0);

            receive_value(left.clone(), middle);
            receive_value(left.clone(), right);

            vec![left]
        }

        _ => {
            let mut new_nodes = vec![];

            let mut size = size;

            if size % 2 != 0 {
                size = size - 1;

                new_nodes.push(nodes.remove(size - 1));
            }

            for i in (0..size).step_by(2) {
                let left = nodes.remove(0);
                let right = nodes.remove(0);
                println!("i: {} left: {:?} right: {:?}", i, left, right);
                receive_value(left.clone(), right);
                new_nodes.push(left);
            }
            println!("Next iteration");

            create_new_tree_bitwise(new_nodes)
        }
    }
}

fn main() {
    let data = vec![8, 19, 7, 15, 7, 13, 12, 14];

    let nodes: Vec<Arc<Node>> = data
        .clone()
        .iter()
        .map(|&v| Node::from_value(v))
        .map(|node| Arc::new(node))
        .collect();

    let root = create_new_tree_bitwise(nodes)[0].clone();

    let root = Arc::try_unwrap(root).unwrap();

    let total = root.compute();

    assert!( total ==  data.iter().sum::<i32>());
    println!(
        "total: {} data sum {} ",
        total,
        data.iter().sum::<i32>()
    );
}

#[test]
fn tree_sum() {
    /*
        Teste com várias entradas
    */
    let data_tests = vec![
        vec![8, 19, 7, 15, 7, 13, 12, 14],
        vec![8, 19, 7, 15, 7, 13, 12],
        vec![8, 19, 7, 15, 7, 13],
        vec![8, 19, 7, 15, 7],
        vec![8, 19, 7, 15],
        vec![8, 19, 7],
        vec![8, 19],
    ];

    for data in data_tests {
        let nodes: Vec<Arc<Node>> = data
            .clone()
            .iter()
            .map(|&v| Node::from_value(v))
            .map(|node| Arc::new(node))
            .collect();

        let root = create_new_tree_bitwise(nodes)[0].clone();

        let root = Arc::try_unwrap(root).unwrap();

        assert_eq!(root.compute(), data.iter().sum::<i32>());
    }
}
